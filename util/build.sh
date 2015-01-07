#!/bin/bash

# @file   build.sh
# @author LKostyra (costyrra.xl@gmail.com)
# @brief  Script for remote building (ex. on a VM).

# Script performing build on external system

# Check if VM system is available
ssh -p 3022 root@localhost 'echo "VM is available"'
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
ssh -p 3022 root@localhost 'mkdir -p ~/matdrv_build'

echo "Copying sources to VM"
scp -P 3022 -r src root@localhost:matdrv_build/
scp -P 3022 Makefile root@localhost:matdrv_build/

# run make
echo "Building project on VM"
ssh -p 3022 root@localhost 'cd ~/matdrv_build/ && make'

popd > /dev/null
