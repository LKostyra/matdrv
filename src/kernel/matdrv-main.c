/**
 * @file   matdrv-main.c
 * @author LKostyra (costyrra.xl@gmail.com)
 * @brief  Main entry point for MatDrv
 */

#include "matdrv-log.h"
#include "matdrv-version.h"
#include "matdrv-devmanager.h"

static int matInit(void)
{
    int ret = 0;
    LOGI("MatDrv, version %s", MATDRV_VERSION);

    if ((ret = matDevCreate()) != 0)
    {
        LOGE("Failed to craete dev node.");
        goto out;
    }

    LOGI("Initialized successfully.");

out:
    return ret;
}

static void matExit(void)
{
    LOGI("Shutting down driver.");

    matDevCleanup();

    LOGI("Shutdown successful.");
}

module_init(matInit);
module_exit(matExit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("LKostyra <costyrra.xl@gmail.com>");
MODULE_DESCRIPTION("MatDrv driver for matrix calculations");
