/**
 * @file   matdrv-version.h
 * @author LKostyra (costyrra.xl@gmail.com)
 * @brief  MatDrv version information
 */

#ifndef _MATDRV_VERSION_H_
#define _MATDRV_VERSION_H_

#define __MATDRV_VER_MAJOR       0
#define __MATDRV_VER_MINOR       0
#define __MATDRV_VER_MAINTENANCE 0

#define __str(s) #s
#define __MATDRV_VER(maj, min, maint) __str(maj) "." __str(min) "." __str(maint)

// Macro with driver version definition
#define MATDRV_VERSION __MATDRV_VER(__MATDRV_VER_MAJOR, __MATDRV_VER_MINOR, __MATDRV_VER_MAINTENANCE)

#endif //_MATDRV_VERSION_H_
