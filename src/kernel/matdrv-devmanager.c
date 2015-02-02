/**
 * @file   matdrv-devmanager.c
 * @author LKostyra (costyrra.xl@gmail.com)
 * @brief  Management of MatDrv device node
 */

#include "matdrv-devmanager.h"
#include "matdrv-log.h"

#include <linux/types.h>
#include <linux/fs.h>
#include <asm/uaccess.h> // used with copy from/to user

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

struct file_operations fops =
{
    .owner = THIS_MODULE,
    .read = matDevRead,
    .write = matDevWrite,
    .open = matDevOpen,
    .release = matDevRelease
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
