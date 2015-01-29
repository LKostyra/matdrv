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

if ssh -p 3022 root@localhost '[ ! -d "matdrv" ]'; then
    echo "Copying sources to VM:"
    echo "Repo root dir is $(pwd)"
    CLONE_CMD="git clone lkostyra@10.0.2.2:$(pwd)"
    $SSH $CLONE_CMD
fi

BRANCH_NAME=$(git rev-parse --abbrev-ref HEAD)
echo "Current branch is ${BRANCH_NAME} - resetting to there"
RESET_CMD="cd ~/matdrv; git fetch origin -p; git reset --hard origin/${BRANCH_NAME}"
$SSH $RESET_CMD

# run make
echo "Building project on VM:"
echo "    Kernel module"
$SSH 'cd ~/matdrv/ && make'
echo "    gtest"
$SSH 'cd ~/matdrv/gtest && cmake . && make'
echo "    Module tests"
$SSH 'cd ~/matdrv/src/tests && cmake . && make'

echo "Installing"
$SSH 'cd ~/matdrv/ && make install'

echo "Reloading udev"
$SSH 'udevadm control --reload-rules'

popd > /dev/null
