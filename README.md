libccontainer
=============

Various data structures in plain c

Setup
=====


    
If you want ot use it in usual environment i.e. on your pc, then:

    cd libccontainer
    bash ./bootstrap
    ./configure
    make; make install
    
Examples
========

Usage examples: http://uprojects.org/libccontainer-usage/

Cross-compiling
===============

For example to build this library for use with ChibiOS-RT:

    cd libccontainer
    bash ./bootstrap
    CFLAGS="-mthumb -mcpu=cortex-m4 -fomit-frame-pointer -falign-functions=16 -ffunction-sections -fdata-sections -fno-common -Wall \
    -Wextra-Wstrict-prototypes -DCORTEX_USE_FPU=FALSE  -DTHUMB_PRESENT -mno-thumb-interwork -DTHUMB_NO_INTERWORKING" LDFLAGS="-mcpu=cortex-m4 \
    -fomit-frame-pointer -falign-functions=16 -ffunction-sections -fdata-sections -fno-common -nostartfiles -mthumb" ./configure \
    --prefix=${PREFIX}/usr --disable-tests --host=arm-none-eabi
    make; make install
    
Cross-compiling was tested on Ubuntu 14.04 with arm-none-eabi toolchain from Ubuntu repository.

Whats inside
============

Well tested:
  * Linked list implementation
  * Hash table implementation

Not so well tested:
  * Dynamic storage (malloc/free inside statically allocated buffer)
  * Circular buffer
