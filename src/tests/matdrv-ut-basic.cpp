#include <gtest/gtest.h>
#include <fcntl.h>
#include <unistd.h>

const char* TEST_MESSAGE_SHORT = "Some test message";

class BasicTest: public ::testing::Test
{
public:
    int devFD;
};

// basic open and close device test
TEST_F(BasicTest, OpenCloseDevice)
{
    devFD = open("/dev/matdrv", O_RDWR | O_NONBLOCK);
    ASSERT_GT(devFD, -1);

    devFD = close(devFD);
    ASSERT_GT(devFD, -1);
}

TEST_F(BasicTest, WriteRead)
{
    devFD = open("/dev/matdrv", O_RDWR | O_NONBLOCK);
    ASSERT_GT(devFD, -1);

    int msgLen = strlen(TEST_MESSAGE_SHORT);
    ASSERT_EQ(write(devFD, TEST_MESSAGE_SHORT, msgLen), msgLen);

    const char* buf[256];
    ASSERT_EQ(read(devFD, buf, msgLen), msgLen);

    devFD = close(devFD);
    ASSERT_GT(devFD, -1);
}
