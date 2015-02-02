#include "matdrv-ut-common.hpp"

#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include <iostream>

class IoctlTest: public ::testing::Test
{
};

// set operation test
TEST_F(IoctlTest, SetOp)
{
    FDPtr devFD(new int, FDPtrDelete);
    *devFD = open(MATDRV_DEVICE_PATH.c_str(), O_RDWR | O_NONBLOCK);
    ASSERT_GT(*devFD, -1);

    // these calls should succeed
    ASSERT_EQ(ioctl(*devFD, MATDRV_IOCTL_SET_OP, MATOP_ADD), 0);
    ASSERT_EQ(ioctl(*devFD, MATDRV_IOCTL_SET_OP, MATOP_SUBTRACT), 0);
    ASSERT_EQ(ioctl(*devFD, MATDRV_IOCTL_SET_OP, MATOP_MULTIPLY), 0);

    // some random number should fail
    ASSERT_NE(ioctl(*devFD, MATDRV_IOCTL_SET_OP, 155), 0);
}

// send matrix test
TEST_F(IoctlTest, SendMat)
{
    FDPtr devFD(new int, FDPtrDelete);
    *devFD = open(MATDRV_DEVICE_PATH.c_str(), O_RDWR | O_NONBLOCK);
    ASSERT_GT(*devFD, -1);

    // create a test matrix for us
    matdrv_matrix_t mat;
    mat.sizex = 2;
    mat.sizey = 3;
    mat.matrix = new int[mat.sizex * mat.sizey];

    for (int i=0; i<mat.sizex*mat.sizey; ++i)
        mat.matrix[i] = i;

    // some random number should fail
    ASSERT_EQ(ioctl(*devFD, MATDRV_IOCTL_SEND_MATRIX, &mat), 0);

    // create a test matrix number two
    matdrv_matrix_t mat2;
    mat2.sizex = 2;
    mat2.sizey = 3;
    mat2.matrix = new int[mat2.sizex * mat2.sizey];

    for (int i=0; i<mat2.sizex*mat2.sizey; ++i)
        mat2.matrix[i] = (i + 43) * 2;

    // some random number should fail
    ASSERT_EQ(ioctl(*devFD, MATDRV_IOCTL_SEND_MATRIX, &mat2), 0);
}
