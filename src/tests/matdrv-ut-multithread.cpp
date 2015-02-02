#include "matdrv-ut-common.hpp"

#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include <iostream>
#include <thread>

class MultithreadTest: public ::testing::Test
{
};

void ThreadSimple()
{
    FDPtr devFD(new int, FDPtrDelete);
    *devFD = open(MATDRV_DEVICE_PATH.c_str(), O_RDWR | O_NONBLOCK);
    ASSERT_GT(*devFD, -1);

    // perform simple IOCTL calls
    ASSERT_EQ(ioctl(*devFD, MATDRV_IOCTL_SET_OP, MATOP_ADD), 0);
    ASSERT_EQ(ioctl(*devFD, MATDRV_IOCTL_SET_OP, MATOP_SUBTRACT), 0);
    ASSERT_EQ(ioctl(*devFD, MATDRV_IOCTL_SET_OP, MATOP_MULTIPLY), 0);
}

// similar to MultiplyMatrix, however a bit more demanding
void ThreadCalculation()
{
    FDPtr devFD(new int, FDPtrDelete);
    *devFD = open(MATDRV_DEVICE_PATH.c_str(), O_RDWR | O_NONBLOCK);
    ASSERT_GT(*devFD, -1);

    // create a test matrix for us
    matdrv_matrix_t mat;
    mat.sizex = 300;
    mat.sizey = 300;
    mat.matrix = new int[mat.sizex * mat.sizey];

    for (int i=0; i<mat.sizex*mat.sizey; ++i)
        mat.matrix[i] = i;

    // some random number should fail
    ASSERT_EQ(ioctl(*devFD, MATDRV_IOCTL_SEND_MATRIX, &mat), 0);

    // create a test matrix number two
    matdrv_matrix_t mat2;
    mat2.sizex = 300;
    mat2.sizey = 300;
    mat2.matrix = new int[mat2.sizex * mat2.sizey];

    for (int i=0; i<mat2.sizex*mat2.sizey; ++i)
        mat2.matrix[i] = (i + 43) * 2;

    // some random number should fail
    ASSERT_EQ(ioctl(*devFD, MATDRV_IOCTL_SEND_MATRIX, &mat2), 0);
    ASSERT_EQ(ioctl(*devFD, MATDRV_IOCTL_SET_OP, MATOP_MULTIPLY), 0);

    // create result matrix
    matdrv_matrix_t mat3;
    mat3.sizex = 300;
    mat3.sizey = 300;
    mat3.matrix = new int[mat3.sizex * mat3.sizey];
    ASSERT_EQ(ioctl(*devFD, MATDRV_IOCTL_GET_RESULT, &mat3), 0);
}

TEST_F(MultithreadTest, ThreadSimpleAccess)
{
    std::thread t1(ThreadSimple);
    std::thread t2(ThreadSimple);
    std::thread t3(ThreadSimple);

    t1.join();
    t2.join();
    t3.join();
}

TEST_F(MultithreadTest, ThreadCalculationAccess)
{
    std::thread t1(ThreadCalculation);
    std::thread t2(ThreadCalculation);
    std::thread t3(ThreadCalculation);

    t1.join();
    t2.join();
    t3.join();
}

TEST_F(MultithreadTest, ThreadMixedAccess)
{
    std::thread t1(ThreadSimple);
    std::thread t2(ThreadCalculation);
    std::thread t3(ThreadSimple);
    std::thread t4(ThreadCalculation);

    t1.join();
    t2.join();
    t3.join();
    t4.join();
}
