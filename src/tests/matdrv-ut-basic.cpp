#include "matdrv-ut-common.hpp"

#include <fcntl.h>
#include <unistd.h>

#include <iostream>

class BasicTest: public ::testing::Test
{
};

// basic open and close device test
TEST_F(BasicTest, OpenCloseDevice)
{
    FDPtr devFD(new int, FDPtrDelete);
    *devFD = open(MATDRV_DEVICE_PATH.c_str(), O_RDWR | O_NONBLOCK);
    ASSERT_GT(*devFD, -1);
    // close() will be called by std::unique_ptr deleter
}

// write some example test to device, then try to read it
TEST_F(BasicTest, WriteRead)
{
    FDPtr devFD(new int, FDPtrDelete);
    *devFD = open(MATDRV_DEVICE_PATH.c_str(), O_RDWR | O_NONBLOCK);
    ASSERT_GT(*devFD, -1);
    ASSERT_EQ(write(*devFD, TEST_MESSAGE_SHORT.c_str(), TEST_MESSAGE_SHORT.size()),
              TEST_MESSAGE_SHORT.size());

    char buf[256];
    ASSERT_EQ(read(*devFD, buf, TEST_MESSAGE_SHORT.size()),
              TEST_MESSAGE_SHORT.size());
    std::cout << "TEST: " << TEST_MESSAGE_SHORT.c_str() << ", buf: " << buf << std::endl;
    ASSERT_EQ(strncmp(TEST_MESSAGE_SHORT.c_str(), buf, TEST_MESSAGE_SHORT.size()), 0);
}
