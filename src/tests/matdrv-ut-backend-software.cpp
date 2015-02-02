#include "matdrv-ut-common.hpp"

#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include <iostream>

class BackendSoftwareTest: public ::testing::Test
{
};

TEST_F(BackendSoftwareTest, AddMatrix)
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
    ASSERT_EQ(ioctl(*devFD, MATDRV_IOCTL_SET_OP, MATOP_ADD), 0);

    // create result matrix
    matdrv_matrix_t mat3;
    mat3.sizex = 2;
    mat3.sizey = 3;
    mat3.matrix = new int[mat3.sizex * mat3.sizey];
    ASSERT_EQ(ioctl(*devFD, MATDRV_IOCTL_GET_RESULT, &mat3), 0);

    // verify result
    for (unsigned int i=0; i<mat3.sizex*mat3.sizey; ++i)
        ASSERT_EQ(mat3.matrix[i], mat.matrix[i]+mat2.matrix[i]);
}

TEST_F(BackendSoftwareTest, SubtractMatrix)
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

    for (int i=0; i<mat.sizex*mat.sizey; ++i)
        mat2.matrix[i] = (i + 43) * 2;

    // some random number should fail
    ASSERT_EQ(ioctl(*devFD, MATDRV_IOCTL_SEND_MATRIX, &mat2), 0);
    ASSERT_EQ(ioctl(*devFD, MATDRV_IOCTL_SET_OP, MATOP_SUBTRACT), 0);

    // create result matrix
    matdrv_matrix_t mat3;
    mat3.sizex = 2;
    mat3.sizey = 3;
    mat3.matrix = new int[mat3.sizex * mat3.sizey];
    ASSERT_EQ(ioctl(*devFD, MATDRV_IOCTL_GET_RESULT, &mat3), 0);

    // verify result
    for (unsigned int i=0; i<mat3.sizex*mat3.sizey; ++i)
        ASSERT_EQ(mat3.matrix[i], mat.matrix[i]-mat2.matrix[i]);
}

TEST_F(BackendSoftwareTest, AddIncorrectXSizeMatrix)
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
    mat2.sizex = 4;
    mat2.sizey = 3;
    mat2.matrix = new int[mat2.sizex * mat2.sizey];

    for (int i=0; i<mat.sizex*mat.sizey; ++i)
        mat2.matrix[i] = (i + 43) * 2;

    // some random number should fail
    ASSERT_EQ(ioctl(*devFD, MATDRV_IOCTL_SEND_MATRIX, &mat2), 0);
    ASSERT_EQ(ioctl(*devFD, MATDRV_IOCTL_SET_OP, MATOP_ADD), 0);

    // create result stub matrix - ioctl should return error anyway
    matdrv_matrix_t mat3;
    ASSERT_NE(ioctl(*devFD, MATDRV_IOCTL_GET_RESULT, &mat3), 0);
    ASSERT_EQ(errno, EINVAL);
}

TEST_F(BackendSoftwareTest, AddIncorrectYSizeMatrix)
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
    mat2.sizey = 6;
    mat2.matrix = new int[mat2.sizex * mat2.sizey];

    for (int i=0; i<mat.sizex*mat.sizey; ++i)
        mat2.matrix[i] = (i + 43) * 2;

    // some random number should fail
    ASSERT_EQ(ioctl(*devFD, MATDRV_IOCTL_SEND_MATRIX, &mat2), 0);
    ASSERT_EQ(ioctl(*devFD, MATDRV_IOCTL_SET_OP, MATOP_ADD), 0);

    // create result stub matrix - ioctl should return error anyway
    matdrv_matrix_t mat3;
    ASSERT_NE(ioctl(*devFD, MATDRV_IOCTL_GET_RESULT, &mat3), 0);
    ASSERT_EQ(errno, EINVAL);
}

TEST_F(BackendSoftwareTest, AddIncorrectResultMatrix)
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

    for (int i=0; i<mat.sizex*mat.sizey; ++i)
        mat2.matrix[i] = (i + 43) * 2;

    // some random number should fail
    ASSERT_EQ(ioctl(*devFD, MATDRV_IOCTL_SEND_MATRIX, &mat2), 0);
    ASSERT_EQ(ioctl(*devFD, MATDRV_IOCTL_SET_OP, MATOP_ADD), 0);

    // create result stub matrix - ioctl should return error anyway
    matdrv_matrix_t mat3;
    mat3.sizex = 5;
    mat3.sizey = 1;
    mat3.matrix = new int[mat3.sizex * mat3.sizey];
    ASSERT_NE(ioctl(*devFD, MATDRV_IOCTL_GET_RESULT, &mat3), 0);
    ASSERT_EQ(errno, EINVAL);
}
