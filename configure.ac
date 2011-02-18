#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.65])
AC_INIT([Sokoban], [0.2], [kangirigungi@gmail.com])
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([src/main.cpp])
AC_CONFIG_HEADERS([config.h])

AC_ARG_ENABLE([debug], [  --enable-debug   Compile with debugging enabled.],
	[debug=${enableval}], [debug=no])

# Set default compiler flags
if test -z $CXXFLAGS; then
	if test x${debug} = xyes; then
		CXXFLAGS='-O0 -g'
	else
		CXXFLAGS='-O2'
	fi
fi

if test x${debug} != xyes; then
	AC_MSG_NOTICE(Debugging is NOT enabled.)
	CPPFLAGS="$CPPFLAGS -DNDEBUG"
else
	AC_MSG_NOTICE(Debugging is enabled.)
fi

# Checks for programs.
AC_PROG_CXX
AC_PROG_INSTALL

AC_LANG([C++])

# Checks for header files.

AC_LANG_PUSH([C])
#
AC_LANG_POP([C])
# Boost headers
AC_CHECK_HEADERS([boost/shared_ptr.hpp boost/scoped_ptr.hpp boost/format.hpp boost/format.hpp], [],
	[AC_MSG_ERROR(You need the Boost libraries.)])
# STL headers
AC_CHECK_HEADERS([algorithm vector deque memory iostream], [],
	[AC_MSG_ERROR(You need the STL libraries.)])

# check boost_program_options library
SAVED_LDFLAGS=$LDFLAGS
LDFLAGS="$LDFLAGS -lboost_program_options"
AC_LINK_IFELSE(
  [AC_LANG_PROGRAM([#include <boost/program_options.hpp>],
    [boost::program_options::options_description d;])],
  [SAVED_LDFLAGS="$SAVED_LDFLAGS -lboost_program_options"
  AC_MSG_NOTICE(boost_program_options library found.)],
  [AC_MSG_ERROR([You need the boost::program_options library.])])
LDFLAGS=$SAVED_LDFLAGS


# Configuration parameters

AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT
