libccontainer
=============

Various data structures in plain c

Setup
=====

    cd libccontainer
    bash ./bootstrap
    ./configure --prefix=/usr # if you dont like autotools default (/usr/local)
    make; make install
    
Examples
========

Usage examples: http://uprojects.org/libccontainer-usage/

Whats inside
============

Well tested:
  * Linked list implementation
  * Hash table implementation

Not so well tested:
  * Dynamic storage (malloc/free inside statically allocated buffer)
  * Circular buffer
