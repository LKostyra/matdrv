# @file   Makefile
# @author LKostyra (costyrra.xl@gmail.com)
# @brief  Main Makefile for MatDrv

# Subdirectories to visit
SUBDIRS = src/kernel

# Global variable used to copy compilation results
MATDRV_ROOTDIR := $(shell pwd)
export MATDRV_ROOTDIR

default:
	for dir in $(SUBDIRS); do $(MAKE) -C $$dir $@; done

install:
	for dir in $(SUBDIRS); do $(MAKE) -C $$dir $@; done
	install -m 644 70-matdrv.conf /etc/udev/rules.d/

clean:
	for dir in $(SUBDIRS); do $(MAKE) -C $$dir $@; done
