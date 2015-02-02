/**
 * @file   matdrv-main.c
 * @author LKostyra (costyrra.xl@gmail.com)
 * @brief  Main entry point for MatDrv
 */

#include "matdrv-log.h"
#include "matdrv-version.h"
#include "matdrv-devmanager.h"
#include "matdrv-backend.h"
#include "matdrv-backend-software.h"

#include <linux/mutex.h>

static int matInit(void)
{
    int ret = 0;
    LOGI("MatDrv, version %s", MATDRV_VERSION);

    // add backends - software
    // this one must success, otherwise our driver is useless
    if ((ret = matSoftwareAddBackend()) != 0)
    {
        LOGE("Failed to provide software backend! Error: %d", ret);
        goto out;
    }

    // add other backends
    // these might fail - just inform the user about failure and continue with initialization

    if ((ret = matDevCreate()) != 0)
    {
        LOGE("Failed to craete dev node. Error: %d", ret);
        goto out;
    }

    mutex_init(&devLockMutex);

    LOGI("Initialized successfully.");

out:
    return ret;
}

static void matExit(void)
{
    LOGI("Shutting down driver.");

    // clean device node
    matDevCleanup();

    // clean backends
    matBackendCleanup();

    LOGI("Shutdown successful.");
}

module_init(matInit);
module_exit(matExit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("LKostyra <costyrra.xl@gmail.com>");
MODULE_DESCRIPTION("MatDrv driver for matrix calculations");
