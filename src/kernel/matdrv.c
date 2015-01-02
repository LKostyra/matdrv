/**
 * @file   matdrv.c
 * @author LKostyra (costyrra.xl@gmail.com)
 * @brief  Main entry point for MatDrv
 */

#include "matdrv-log.h"
#include "matdrv-version.h"

static int matdrv_init(void)
{
    LOGI("MatDrv, version " MATDRV_VERSION);

    LOGI("Initialized successfully.");
    return 0;
}

static void matdrv_exit(void)
{
    LOGI("Shutdown successful.");
}

module_init(matdrv_init);
module_exit(matdrv_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("LKostyra <costyrra.xl@gmail.com>");
MODULE_DESCRIPTION("MatDrv driver for matrix calculations");
