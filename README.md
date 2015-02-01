MatDrv Project v 0.0.0
======================

Driver for delegating matrix calculations to external devices.

Driver is under development, so right now it is recommended not to use it.

Directory structure
-------------------

* **gtest** - Google Test Framework sources
* **src** - MatDrv source files
    * **kernel** - kernelspace part of MatDrv
    * **tests** - tests for MatDrv kernel module
* **util** - MatDrv utility scripts

Building driver on VM
---------------------

In order to properly build the driver using util/build.sh script, the following conditions must be met:
* Forwarded SSH port (22 on VM => 3022 on host)
* Host visible inside VM from IP 10.0.2.2
* Installed dependencies on VM:
    * **git** - for cloning repository onto VM
    * **gcc**, **make**, **kernel-devel** and **kernel-headers** - for kernel module building
    * **gcc-c++**, **cmake** - for additional tools building
* Keep both **kernel-devel** and **kernel-headers** up to date with current version of your Linux kernel
