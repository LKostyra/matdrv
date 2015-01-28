#!/bin/bash

# @file   build.sh
# @author LKostyra (costyrra.xl@gmail.com)
# @brief  Script for remote building (ex. on a VM).

# Script performing build on external system

SSH="ssh -tq -p 3022 root@localhost"

# Check if VM system is available
$SSH 'echo "VM is available"'
if [ "$?" -ne "0" ]; then
    echo "Could not connect to VM."
    echo "Please start VM, make sure sshd is running and ssh port (22) is available from host under 3022 port."
    exit
fi

# move to root directory
pushd . > /dev/null
DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
cd ${DIR}/..
echo -n "Current directory is "; pwd

# copy sources to virtual machine
echo "Creating build directory in VM"
$SSH 'mkdir -p ~/matdrv_build'

echo "Copying sources to VM:"
echo "    Project sources"
scp -P 3022 -r src root@localhost:matdrv_build/
echo "    gtest"
scp -P 3022 -r gtest root@localhost:matdrv_build/
echo "    Additional data"
scp -P 3022 Makefile root@localhost:matdrv_build/
scp -P 3022 70-matdrv.conf root@localhost:matdrv_build/

# run make
echo "Building project on VM:"
echo "    Kernel module"
$SSH 'cd ~/matdrv_build/ && make'
echo "    gtest"
$SSH 'cd ~/matdrv_build/gtest && cmake . && make'
echo "    Module tests"
$SSH 'cd ~/matdrv_build/src/tests && cmake . && make'

echo "Installing"
$SSH 'cd ~/matdrv_build/ && make install'

echo "Reloading udev"
$SSH 'udevadm control --reload-rules'

popd > /dev/null
