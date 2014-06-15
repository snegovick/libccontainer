ACLOCAL_AMFLAGS = -I m4

if BUILD_TESTS
EXTRA=tests examples
else
EXTRA=
endif

SUBDIRS = src $(EXTRA)

TESTS = tests/clisttest tests/chashtest tests/cdstest tests/circbuftest

include_HEADERS = src/chash.h src/clist.h src/clist_common.h src/cdynstorage.h src/circularbuf.h

pkgconfiglibdir = $(libdir)/pkgconfig
pkgconfiglib_DATA = libccontainer.pc
