/**
 * @file
 * @author LKostyra (costyrra.xl@gmail.com)
 * @brief  Main backend definitions and declarations
 */

#ifndef _MATDRV_BACKEND_H_
#define _MATDRV_BACKEND_H_

#include "matdrv-common.h"


/////////////////
// BACKEND API //
/////////////////

/**
 * A set of functions for each backend, used to communicate with the device
 *
 * Each device type must specify these.
 */
struct matBackendFunc
{
    /**
     * Initializes the backend.
     *
     * @return 0 when backend is inintailzed successfully.
     */
    long (*init)(void);

    /**
     * Cleans after the backend
     */
    void (*release)(void);

    /**
     * Sets which operation should be performed by driver. See enum matOp for more info.
     *
     * @param op Operation to perform by the device.
     */
    long (*setOp)(enum matOps op);

    /**
     * Sends matrix to driver.
     *
     * The device by default keeps two matrices in memory. After this call, older one is replaced by
     * the one provided as a param to this function.
     */
    long (*sendMatrix)(matdrv_matrix_t* mat);

    /**
     * Get result of calculation
     *
     * This function triggers the device to calculate selected operation on matrices kept in memory.
     * Then, result of the calculations is retrieved and returned to user.
     *
     * It is caller's duty to allocate space required for result matrix.
     */
    long (*getResultMatrix)(matdrv_matrix_t* mat);
};

extern struct matBackendFunc** gBackends;

/**
 * Backend initializer - captures defined backend and adds it to the list.
 *
 * It is recommended to sort matBackendAdd calls according to some criteria (eg. slowest backends
 * first) - backend manager will assume that last added working backend is a valid one.
 *
 * @return 0 on success, -ENODEV if backend cannot be initialized.
 */
long matBackendAdd(struct matBackendFunc* backend);

/**
 * Backend activator - switches to backend provided in backendNum
 *
 * @return 0 on success, -ENOENT if backendNum is invalid.
 */
long matBackendActivate(unsigned int backendNum);

/**
 * Backend cleaner - calls release() on all backends and frees backend array.
 */
void matBackendCleanup(void);


///////////////////////////////
// BACKEND GENERAL FUNCTIONS //
///////////////////////////////

// Regular functions visible for the rest of the driver.
// During backends init, driver checks which device is available and chooses appropriate one.
long matSetOp(enum matOps op);
long matSendMatrix(matdrv_matrix_t* mat);
long matGetResultMatrix(matdrv_matrix_t* mat);

#endif // _MATDRV_BACKEND_H_
