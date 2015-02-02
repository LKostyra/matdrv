#ifndef _MATDRV_UT_COMMON_H_
#define _MATDRV_UT_COMMON_H_

#include <gtest/gtest.h>

#include <linux/matdrv.h>

#include <memory>
#include <string>

// constants needed in tests
extern std::string MATDRV_DEVICE_PATH;
extern std::string TEST_MESSAGE_SHORT;

// unique_ptr wrapper for file descriptor
void FDPtrDelete(int* fd);
typedef std::unique_ptr<int, std::function<void(int*)>> FDPtr;

#endif // _MATDRV_UT_COMMON_H_
