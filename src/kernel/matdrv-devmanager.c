/**
 * @file   matdrv-devmanager.c
 * @author LKostyra (costyrra.xl@gmail.com)
 * @brief  Management of MatDrv device node
 */

#include "matdrv-devmanager.h"
#include "matdrv-backend.h"
#include "matdrv-log.h"

#include <linux/types.h>
#include <linux/fs.h>

#define MATDRV_DEV_CLASS_NAME "matdrv_class"
#define MATDRV_DEV_MINOR_COUNT 1

struct cdev* gMatDevCdev = NULL;

dev_t matDev = 0;
static struct class* matDevClass = NULL;

ssize_t matDevWrite(struct file* filp, const char __user *buf, size_t count, loff_t* off)
{
    return 0;
}

ssize_t matDevRead(struct file* filp, char __user *buf, size_t count, loff_t* off)
{
    return 0;
}

static int matDevOpen(struct inode* inode, struct file* file)
{
    LOGI("Opening device.");
    return 0;
}

static int matDevRelease(struct inode* inode, struct file* file)
{
    LOGI("Releasing device.");
    return 0;
}

long matDevIoctl(struct file* filp, unsigned int cmd, unsigned long arg)
{
    long ret;
    matdrv_matrix_t mat;

    switch (cmd)
    {
    case MATDRV_IOCTL_SET_OP:
        if (arg < (unsigned long)MATOP_COUNT)
        {
            return matSetOp((enum matOps)arg);
        }
        else
        {
            return -ENOENT;
        }

    case MATDRV_IOCTL_SEND_MATRIX:
        ret = copy_from_user(&mat, (matdrv_matrix_t*)arg, sizeof(matdrv_matrix_t));
        if (ret)
        {
            LOGE("Failed to copy matrix from user: %ld", ret);
            return ret;
        }
        return matSendMatrix(&mat);

    case MATDRV_IOCTL_GET_RESULT:
        // get from user space where we will copy result data
        ret = copy_from_user(&mat, (matdrv_matrix_t*)arg, sizeof(matdrv_matrix_t));
        if (ret)
        {
            LOGE("Failed to copy matrix from user: %ld", ret);
            return ret;
        }
        ret = matGetResultMatrix(&mat);
        if (ret)
        {
            LOGE("Failed to get result matrix: %ld", ret);
            return ret;
        }
        return 0;

    default:
        return -ENOTTY;
    }
}

struct file_operations fops =
{
    .owner = THIS_MODULE,
    .read = matDevRead,
    .write = matDevWrite,
    .open = matDevOpen,
    .release = matDevRelease,
    .unlocked_ioctl = matDevIoctl,
};

int matDevCreate(void)
{
    int ret = 0;

    LOGI("Registering char device in system.");

    ret = alloc_chrdev_region(&matDev, 0, MATDRV_DEV_MINOR_COUNT, MATDRV_DEV_NAME);
    if (ret)
    {
        LOGE("Failed to allocate device number.");
        goto out;
    }

    matDevClass = class_create(THIS_MODULE, MATDRV_DEV_CLASS_NAME);
    if (IS_ERR(matDevClass))
    {
        LOGE("Failed to create class " MATDRV_DEV_CLASS_NAME);
        ret = PTR_ERR(matDevClass);
        goto out;
    }

    gMatDevCdev = cdev_alloc();
    if (!gMatDevCdev)
    {
        LOGE("Failed to allocate cdev for name %s", MATDRV_DEV_NAME);
        ret = -ENODEV;
        goto out;
    }

    gMatDevCdev->ops = &fops;
    gMatDevCdev->owner = THIS_MODULE;

    ret = cdev_add(gMatDevCdev, matDev, 1);
    if (ret)
    {
        LOGE("Failed to register device number for %s", MATDRV_DEV_NAME);
        goto out;
    }

    device_create(matDevClass, NULL, matDev, NULL, MATDRV_DEV_NAME);
    LOGI("Allocated %s device number %d:%d", MATDRV_DEV_NAME, MAJOR(matDev), MINOR(matDev));

    ret = 0;
out:
    return ret;
}

void matDevCleanup(void)
{
    LOGI("Releasing char device from system.");

    if (matDev && matDevClass)
    {
        LOGD("Destroying device.");
        device_destroy(matDevClass, matDev);
    }

    if (gMatDevCdev)
    {
        LOGD("Deleting cdev.");
        cdev_del(gMatDevCdev);
        gMatDevCdev = NULL;
    }

    if (matDevClass)
    {
        LOGD("Destroying device class.");
        class_destroy(matDevClass);
        matDevClass = NULL;
    }

    if (matDev)
    {
        LOGD("Unregistering char device.");
        unregister_chrdev_region(matDev, MATDRV_DEV_MINOR_COUNT);
        matDev = 0;
    }

    LOGI("Cleanup finished.");
}
