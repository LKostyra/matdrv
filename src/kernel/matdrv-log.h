/**
 * @file   matdrv-log.h
 * @author LKostyra (costyrra.xl@gmail.com)
 * @brief  Basic logging utilities for MatDrv
 */

#ifndef _MATDRV_LOG_H_
#define _MATDRV_LOG_H_

#include "matdrv-common.h"

// TODO: LOGD should show up only when driver is compiled in DEBUG
#define LOGD(msg)                                                                   \
    do {                                                                            \
        printk(KERN_DEBUG "MATDRV [DBG] %s@%d: %s\n", __func__, __LINE__, msg);     \
    } while(0)

// macros independent from build compilation

// INFO LOG
#define LOGI(msg)                                                                   \
    do {                                                                            \
        printk(KERN_INFO "MATDRV [INF] %s@%d: %s\n", __func__, __LINE__, msg);      \
    } while (0)

// WARNING LOG
#define LOGW(msg)                                                                   \
    do {                                                                            \
        printk(KERN_WARNING "MATDRV [WRN] %s@%d: %s\n", __func__, __LINE__, msg);   \
    } while (0)

// ERROR LOG
#define LOGE(msg)                                                                   \
    do {                                                                            \
        printk(KERN_ERR "MATDRV [ERR] %s@%d: %s\n", __func__, __LINE__, msg);       \
    } while (0)

#endif // _MATDRV_LOG_H_
