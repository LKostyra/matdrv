/**
 * @file
 * @author LKostyra (costyrra.xl@gmail.com)
 * @brief Software backend for MatDrv.
 *
 * Provides basic matrix calculation functionality. All functions are performed on CPU.
 *
 * This backend, due to its soft nature, should be always available as a 'last resort', if all other
 * backends fail to initialize. Thus, failed initialization *MUST* cause an error when initializing
 * the driver module.
 */

#ifndef _MATDRV_BACKEND_SOFTWARE_H_
#define _MATDRV_BACKEND_SOFTWARE_H_

#include "matdrv-backend.h"

/**
 * Adds this backend to MatDrv backend pool.
 *
 * @return result of matBackendAdd. See matBackendAdd for more details.
 */
long matSoftwareAddBackend(void);

#endif // _MATDRV_BACKEND_SOFTWARE_H_
