lib_LTLIBRARIES = libccontainer.la
libccontainer_la_SOURCES = chash.c clist.c cdynstorage.c circularbuf.c circularbuf.h cdynstorage.h clist.h chash.h clist_common.h
libccontainer_la_LDFLAGS = ${LDFLAGS} -Wl,--export-dynamic -rdynamic -version-info 0:1:0
libccontainer_la_CFLAGS = ${CFLAGS} -D_GNU_SOURCE=1 -D_REENTRANT -I./ -Wall
