dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)

AC_INIT([libccontainer], [1.0.1], [snegovick@uprojects.org])

AC_CONFIG_AUX_DIR([./config])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([1.9 foreign])

# PKG_CHECK_MODULES(PROTOBUF, protobuf >= 2.4.0)

AC_MSG_CHECKING([whether to build with debug information])
AC_ARG_ENABLE([debug],
    [AS_HELP_STRING([--enable-debug],
        [enable debug data generation (def=no)])],
    [debugit="$enableval"],
    [debugit=no])
AC_MSG_RESULT([$debugit])

if test x"$debugit" = x"yes"; then
    AC_DEFINE([DEBUG],[],[Debug Mode])
    AM_CFLAGS="$AM_CFLAGS -g -Wall -Werror -Wno-uninitialized -O0"
    CFLAGS="-g -O0 -Wall -Werror -Wno-uninitialized $CFLAGS"
else
    AC_DEFINE([NDEBUG],[],[No-debug Mode])
    AM_CFLAGS="$AM_CFLAGS -O3"
fi

AC_MSG_CHECKING([whether to build with tests])
AC_ARG_ENABLE([tests],
    [AS_HELP_STRING([--enable-tests],
        [enable tests (def=no)])],
    [testit="$enableval"],
    [testit=no])
AC_MSG_RESULT([$testit])

AC_CHECK_HEADER([CUnit/CUnit.h],
   [
      CUNIT_CFLAGS="$CFLAGS"
      AC_CHECK_LIB([cunit],
         [CU_initialize_registry],
         [CUNIT_LDFLAGS="$LDFLAGS -lcunit" found_cunit="yes"],
         [found_cunit="no"]
      )
   ],
   [
      found_cunit="no"
   ]
)

if test x"$testit" = x"yes"; then
   # Run the header/link tests
   if test x"found_cunit" = x"yes"; then
      CUNIT_STATUS="enabled"
   else
      CUNIT_LDFLAGS=""
      CUNIT_CFLAGS=""
      CUNIT_STATUS="disabled"
   fi
fi

AC_PROG_CC
AC_PROG_LIBTOOL

if test "x$testit" == "xyes" && test "x$found_cunit" == "xno"; then
   AC_MSG_ERROR([tests are enabled, but CUnit is not found], [-1])
fi

AM_CONDITIONAL([BUILD_TESTS], [test "x$testit" != "xno" && test "x$found_cunit" != "xno"])

AC_CONFIG_FILES([Makefile src/Makefile tests/Makefile examples/Makefile libccontainer.pc])

AC_OUTPUT
