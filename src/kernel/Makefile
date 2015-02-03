# @file   Makefile
# @author LKostyra (costyrra.xl@gmail.com)
# @brief  Kernelspace Makefile for MatDrv

KERNELVERSION = $(shell uname -r)
KERNELDIR := /lib/modules/$(KERNELVERSION)/build
PWD := $(shell pwd)

obj-m += matdrv.o
matdrv-objs := matdrv-main.o matdrv-devmanager.o matdrv-backend.o matdrv-backend-software.o

default: build

build:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
	mkdir -p $(MATDRV_ROOTDIR)/bin
	cp -f matdrv.ko $(MATDRV_ROOTDIR)/bin/

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
	rm -rf $(MATDRV_ROOTDIR)/bin

install:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules_install
