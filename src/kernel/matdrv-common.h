/**
 * @file   matdrv-common.h
 * @author LKostyra (costyrra.xl@gmail.com)
 * @brief  Common includes for entire MatDrv project
 */

#ifndef _MATDRV_COMMON_H_
#define _MATDRV_COMMON_H_

#include <linux/init.h>
#include <linux/module.h>

/**
 * Matrix operations handled by MatDrv.
 */
enum matOps
{
    MATOP_NONE = 0, // stub no operation, for initialization purposes
    MATOP_ADD,
    MATOP_SUBTRACT,
    MATOP_MULTIPLY
};

#endif // _MATDRV_COMMON_H_
