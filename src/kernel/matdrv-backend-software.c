#include "matdrv-backend-software.h"
#include "matdrv-common.h"
#include "matdrv-log.h"

// prototypes
int matSoftwareInit(void);
void matSoftwareRelease(void);
void matSoftwareSetOp(enum matOps op);
void matSoftwareSendMatrix(int* mat);
void matSoftwareGetResultMatrix(int* mat);

// backend struct
struct matBackendFunc matSoftwareBackend =
{
    .init = matSoftwareInit,
    .release = matSoftwareRelease,
    .setOp = matSoftwareSetOp,
    .sendMatrix = matSoftwareSendMatrix,
    .getResultMatrix = matSoftwareGetResultMatrix
};

// globally visible function - adds current backend to pool
// should be caled by init function of this module
int matSoftwareAddBackend(void)
{
    return matBackendAdd(&matSoftwareBackend);
}

////////////////////////////
// BACKEND IMPLEMENTATION //
////////////////////////////
int selectedOp = MATOP_NONE;

// Init and Release do not need to perform any job in this backend. Below functions are just stubs.
int matSoftwareInit(void)
{
    LOGI("Software backend for MatDrv - initialized successfully");
    return 0;
}
void matSoftwareRelease(void)
{
    LOGI("Software backend for MatDrv - freed successfully");
}

void matSoftwareSetOp(enum matOps op)
{
}

void matSoftwareSendMatrix(int* mat)
{
}

void matSoftwareGetResultMatrix(int* mat)
{
}
