#include "matdrv-ut-common.hpp"
#include <fcntl.h>
#include <unistd.h>

std::string MATDRV_DEVICE_PATH = "/dev/matdrv";
std::string TEST_MESSAGE_SHORT = "Some test message";

void FDPtrDelete(int* fd)
{
    close(*fd);
    *fd = 0;
}
