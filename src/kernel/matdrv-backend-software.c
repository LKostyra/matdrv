#include "matdrv-backend-software.h"
#include "matdrv-common.h"
#include "matdrv-log.h"

// prototypes
int matSoftwareInit(void);
void matSoftwareRelease(void);
int matSoftwareSetOp(enum matOps op);
int matSoftwareSendMatrix(matdrv_matrix_t* mat);
int matSoftwareGetResultMatrix(matdrv_matrix_t* mat);

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
int olderMat = 0;
matdrv_matrix_t matrix[2];
matdrv_matrix_t result;

// Init and Release do not need to perform any job in this backend. Below functions are just stubs.
int matSoftwareInit(void)
{
    LOGI("Software backend for MatDrv - initialized successfully");
    return 0;
}
void matSoftwareRelease(void)
{
    if (matrix[0].matrix)
        kfree(matrix[0].matrix);
    if (matrix[0].matrix)
        kfree(matrix[1].matrix);
    if (result.matrix)
        kfree(result.matrix);
    LOGI("Software backend for MatDrv - freed successfully");
}

int matSoftwareSetOp(enum matOps op)
{
    LOGI("Setting operation %d", (int)op);
    selectedOp = op;
    return 0;
}

int matSoftwareSendMatrix(matdrv_matrix_t* mat)
{
    int i = 0;
    long ret;

    if (matrix[olderMat].matrix)
        kfree(matrix[olderMat].matrix);

    // copy matrix from user
    matrix[olderMat].sizex = mat->sizex;
    matrix[olderMat].sizey = mat->sizey;
    matrix[olderMat].matrix = (int*)kmalloc(sizeof(int)*mat->sizex*mat->sizey, GFP_KERNEL);

    // matrix still resides on userspace, thus we need to copy it
    ret = copy_from_user(matrix[olderMat].matrix, mat->matrix, sizeof(int)*mat->sizex*mat->sizey);
    if (ret)
    {
        LOGE("Failed to copy matrix from user: %ld", ret);
        return ret;
    }

    LOGI("Recevied %dx%d matrix", matrix[olderMat].sizex, matrix[olderMat].sizey);
    LOGI("Elements:");
    for (i=0; i<matrix[olderMat].sizex*matrix[olderMat].sizey; ++i)
    {
        LOGI("Element %d = %d", i, matrix[olderMat].matrix[i]);
    }

    olderMat++;
    if (olderMat > 1)
    {
        olderMat = 0;
    }

    return 0;
}

int matSoftwareGetResultMatrix(matdrv_matrix_t* mat)
{
    // TODO calculate matrix

    // TODO copy result to user
    return 0;
}
