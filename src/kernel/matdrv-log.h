/**
 * @file   matdrv-log.h
 * @author LKostyra (costyrra.xl@gmail.com)
 * @brief  Basic logging utilities for MatDrv
 */

#ifndef _MATDRV_LOG_H_
#define _MATDRV_LOG_H_

#include "matdrv-common.h"

#define LOG(kern_level, level, format, ...)                             \
    do {                                                                \
        printk(kern_level "MatDrv [" #level "] %s@%d: " format "\n",    \
               __func__, __LINE__, ##__VA_ARGS__);                      \
    } while(0)

// TODO: LOGD should show up only when driver is compiled in DEBUG
#ifdef DEBUG
#define LOGD(fmt, ...) LOG(KERN_DEBUG, DBG, fmt, ##__VA_ARGS__)
#else
#define LOGD(fmt, ...) do { } while(0)
#endif

// macros independent from build compilation
#define LOGI(fmt, ...) LOG(KERN_INFO, INF, fmt, ##__VA_ARGS__)
#define LOGW(fmt, ...) LOG(KERN_WARNING, WRN, fmt, ##__VA_ARGS__)
#define LOGE(fmt, ...) LOG(KERN_ERR, ERR, fmt, ##__VA_ARGS__)

#endif // _MATDRV_LOG_H_
