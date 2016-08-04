#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([archtop], [0.0.1], [mattdbartlett@yahoo.com])
AC_CONFIG_SRCDIR([src/main.cpp])
AC_CONFIG_HEADERS([config.h])

AC_CONFIG_MACRO_DIRS([m4])

AM_INIT_AUTOMAKE

# Checks for programs.
AC_PROG_CXX

# Checks for libraries.
PKG_PROG_PKG_CONFIG
PKG_CHECK_MODULES([DXFLIB], [dxflib])
AC_SUBST([DXFLIB_CFLAGS])
AC_SUBST([DXFLIB_LIBS])


# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile src/Makefile])

LT_INIT

AC_OUTPUT
