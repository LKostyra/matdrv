/**
 * @file   matdrv-devmanager.c
 * @author LKostyra (costyrra.xl@gmail.com)
 * @brief  Management of MatDrv device node
 */

#include "matdrv-devmanager.h"
#include "matdrv-log.h"

#define MATDRV_DEV_CLASS_NAME = "matdrv_class"

struct cdev* gMatDevCdev = NULL;
static struct class* matDevClass = NULL;

int matDevCreate(void)
{
    LOGI("Creating cdev");
    int ret = 0;

    ret = alloc_chrdev_region(&gMatDevCdev, 0, 1, MATDRV_DEV_NAME);
    if (ret)
    {
        LOGE("Failed to allocate device number.");
        goto err;
    }

    matDevClass = class_create(THIS_MODULE, MATDRV_DEV_CLASS_NAME);
    if (IS_ERR(matDevClass))
    {
        LOGE("Failed to create class " MATDRV_DEV_CLASS_NAME);
    }

    // TODO finish

err:
    return ret;
}

void matDevRelease(void)
{
    LOGI("Releasing cdev");

    // TODO release resources
}
