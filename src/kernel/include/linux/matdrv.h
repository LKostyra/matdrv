#ifndef _INCLUDE_LINUX_MATDRV_H_
#define _INCLUDE_LINUX_MATDRV_H_

#include <linux/ioctl.h>

/**
 * Matrix operations handled by MatDrv.
 */
enum matOps
{
    MATOP_NONE = 0, // stub no operation, for initialization purposes
    MATOP_ADD,
    MATOP_SUBTRACT,
    MATOP_MULTIPLY,
    MATOP_COUNT // this should always be last one to check if we don't exceed MATOP_ number
};

/**
 * Basic matrix definition
 *
 * sizex - X size of matrix
 * sizey - Y size of matrix
 * matrix - pointer to matrix allocated in user-space
 */
typedef struct matdrv_matrix
{
    unsigned int sizex;
    unsigned int sizey;
    int* matrix;
} matdrv_matrix_t;

#define MATDRV_IOCTL_MAGIC 0x98

#define MATDRV_IOCTL_SET_OP        _IOW(MATDRV_IOCTL_MAGIC, 1, int)
#define MATDRV_IOCTL_SEND_MATRIX   _IOW(MATDRV_IOCTL_MAGIC, 2, struct matdrv_matrix)
#define MATDRV_IOCTL_GET_RESULT    _IOR(MATDRV_IOCTL_MAGIC, 3, struct matdrv_matrix)

#endif // _INCLUDE_LINUX_MATDRV_H_
