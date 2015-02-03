/**
 * @file   matdrv-devmanager.h
 * @author LKostyra (costyrra.xl@gmail.com)
 * @brief  Management of MatDrv device node
 */

#ifndef _MATDRV_DEVMANAGER_H_
#define _MATDRV_DEVMANAGER_H_

#include "matdrv-common.h"
#include <linux/cdev.h>
#include <linux/device.h>

#define MATDRV_DEV_NAME "matdrv"
extern struct cdev* gMatDevCdev;
extern struct mutex devLockMutex; // visible in order to initialize in matInit

int matDevCreate(void);
void matDevCleanup(void);

#endif
