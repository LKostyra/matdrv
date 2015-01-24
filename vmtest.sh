#!/bin/bash

# @file   vmrun.sh
# @author LKostyra (costyrra.xl@gmail.com)
# @brief  Utility script for testing the driver on VM

# perform basic test - add module, remove and print dmesg

SSH="ssh -tq -p 3022 root@localhost"

echo "==== INSERT/REMOVE TEST ===="
echo "Clearing dmesg"
$SSH 'dmesg -C'
echo "Inserting module"
$SSH 'insmod matdrv_build/bin/matdrv.ko'
echo "lsmod info:"
$SSH 'lsmod | grep matdrv'
echo -n "Checking if matdrv device is present... "
if $SSH stat /dev/matdrv \> /dev/null 2\>\&1; then
    echo "YES"
else
    echo "NO"
    echo "Removing module"
    $SSH 'rmmod matdrv'
    echo "Exiting"
    exit
fi
echo "Removing module"
$SSH 'rmmod matdrv'
echo "Printing dmesg"
$SSH 'dmesg'

echo "==== ECHO TEST ===="
echo "Clearing dmesg"
$SSH 'dmesg -C'
echo "Inserting module"
$SSH 'insmod matdrv_build/bin/matdrv.ko'
echo "Sending info"
$SSH 'echo TestText > /dev/matdrv'
echo "Receiving text:"
$SSH 'cat /dev/matdrv'
echo "Removing module"
$SSH 'rmmod matdrv'
echo "Printing dmesg"
$SSH 'dmesg'
