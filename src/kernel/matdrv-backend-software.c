#include "matdrv-backend-software.h"
#include "matdrv-common.h"
#include "matdrv-log.h"

// prototypes
long matSoftwareInit(void);
void matSoftwareRelease(void);
long matSoftwareSetOp(enum matOps op);
long matSoftwareSendMatrix(matdrv_matrix_t* mat);
long matSoftwareGetResultMatrix(matdrv_matrix_t* mat);

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
long matSoftwareAddBackend(void)
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
long matSoftwareInit(void)
{
    matrix[0].matrix = NULL;
    matrix[1].matrix = NULL;
    result.matrix = NULL;

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

long matSoftwareSetOp(enum matOps op)
{
    LOGI("Setting operation %d", (int)op);
    selectedOp = op;
    return 0;
}

long matSoftwareSendMatrix(matdrv_matrix_t* mat)
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
    LOGD("Elements:");
    for (i=0; i<matrix[olderMat].sizex*matrix[olderMat].sizey; ++i)
    {
        LOGD("Element %d = %d", i, matrix[olderMat].matrix[i]);
    }

    olderMat++;
    if (olderMat > 1)
    {
        olderMat = 0;
    }

    return 0;
}

/**
 * Calculate result matrix dimensions.
 *
 * Function might return an error if matrix sizes are not compatible according to standard algebra
 * calculation rules.
 *
 * @return 0 on success, -EINVAL if matrix sizes are incorrect.
 */
long matSoftwareCalculateMatrixDim(void)
{
    if (selectedOp == MATOP_NONE)
    {
        // we must have knowledge about what matrix to create
        LOGE("No operation selected! Please select operation with MATDRV_IOCTL_SET_OP");
        return -EINVAL;
    }

    if (selectedOp == MATOP_ADD || selectedOp == MATOP_SUBTRACT)
    {
        if (matrix[0].sizex != matrix[1].sizex)
        {
            LOGE("X size mismatch (%d vs %d).",
                 matrix[0].sizex, matrix[1].sizex);
            return -EINVAL;
        }
        else
        {
            // matrix sizes are equal - assume one of them as result
            result.sizex = matrix[0].sizex;
        }

        if (matrix[0].sizey != matrix[1].sizey)
        {
            LOGE("Y size mismatch (%d vs %d).", matrix[0].sizey, matrix[1].sizey);
            return -EINVAL;
        }
        else
        {
            // matrix sizes are equal - assume one of them as result
            result.sizey = matrix[0].sizey;
        }

        return 0;
    }
    else if (selectedOp == MATOP_MULTIPLY)
    {
        // matrix multiplication condition: mat0 sizey == mat1 sizex
        if (matrix[0].sizey != matrix[1].sizex)
        {
            LOGE("matrix0 X does not equal matrix1 Y size (%d vs %d)!",
                 matrix[0].sizey, matrix[1].sizex);
            return -EINVAL;
        }

        // if above is met, result matrix has sizex equal mat0 sizex and sizey equal mat1 sizey
        result.sizex = matrix[0].sizex;
        result.sizey = matrix[1].sizey;
    }

    return 0;
}

long matSoftwareAdd(void)
{
    unsigned int i;

    for (i=0; i<result.sizex*result.sizey; ++i)
    {
        // calculation
        result.matrix[i] = matrix[0].matrix[i] + matrix[1].matrix[i];
    }

    // result matrix calculated successfully, return to GetResultMatrix
    return 0;
}

long matSoftwareSubtract(void)
{
    unsigned int i;

    for (i=0; i<result.sizex*result.sizey; ++i)
    {
        // calculation
        result.matrix[i] = matrix[0].matrix[i] - matrix[1].matrix[i];
    }

    // result matrix calculated successfully, return to GetResultMatrix
    return 0;
}

long matSoftwareMultiply(void)
{
    unsigned int i, j, k; // loop iterators

    // NOTE This might be optimized by using some sophisticated divide-and-conquer algorithm
    //      Right now, this is calculated by using the straightforward old-fashioned way.
    for (i=0; i<result.sizey; ++i)
    {
        for (j=0; j<result.sizex; ++j)
        {
            result.matrix[(i*result.sizex)+j] = 0;
            for (k=0; k<matrix[0].sizey; ++k)
            {
                result.matrix[(i*result.sizex)+j] +=
                        matrix[0].matrix[i*matrix[0].sizex + k] *
                        matrix[1].matrix[k*matrix[1].sizex + j];
            }
        }
    }

    // result matrix calculated successfully, return to GetResultMatrix
    return 0;
}

long matSoftwareGetResultMatrix(matdrv_matrix_t* mat)
{
    long ret;

    ret = matSoftwareCalculateMatrixDim();
    if (ret)
    {
        LOGE("Failed to calculate result matrix dimensions!");
        return ret;
    }

    // check if user provided correct matrix with enough space
    // we must assume, that sizex*sizey is indeed size of users matrix
    // other situations are probably bad programming from user's side
    if (!mat->matrix)
    {
        LOGE("User did not allocare space for the result!");
        return -EINVAL;
    }
    LOGI("Received result matrix %dx%d", mat->sizex, mat->sizey);
    if (mat->sizex != result.sizex || mat->sizey != result.sizey)
    {
        LOGE("Incorrect result matrix provided!");
        return -EINVAL;
    }

    // allocate space
    if (result.matrix)
        kfree(result.matrix);

    result.matrix = (int*)kmalloc(sizeof(int)*result.sizex*result.sizey, GFP_KERNEL);
    if (!result.matrix)
    {
        LOGE("Failed to allocate memory for result matrix!");
        return -ENOMEM;
    }

    // do the math
    switch (selectedOp)
    {
    case MATOP_NONE: // just exit, there is not much we can do
        LOGW("Asked to perform no operation. Was this intentional?");
        return 0;
    case MATOP_ADD:
        ret = matSoftwareAdd();
        break;
    case MATOP_SUBTRACT:
        ret = matSoftwareSubtract();
        break;
    case MATOP_MULTIPLY:
        ret = matSoftwareMultiply();
        break;
    default: // theoretically this should not be reached, but just in case...
        LOGE("Incorrect operation selected!");
        return -ENOENT;
    };
    if (ret)
    {
        LOGE("Failed to calculate matrix: %ld", ret);
        return ret;
    }

    // copy data to user
    ret = copy_to_user(mat->matrix, result.matrix, sizeof(int)*mat->sizex*mat->sizey);
    if (ret)
    {
        LOGE("Failed to copy result to user.");
        return ret;
    }

    return 0;
}
