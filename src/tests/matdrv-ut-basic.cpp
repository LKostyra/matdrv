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
