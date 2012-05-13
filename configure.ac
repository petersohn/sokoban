#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.65])
AC_INIT([Sokoban], [0.2], [kangirigungi@gmail.com])
AM_INIT_AUTOMAKE


m4_include([ax_boost_base.m4])
m4_include([ax_boost_program_options.m4])
m4_include([ax_boost_thread.m4])
m4_include([ax_boost_system.m4])
m4_include([ax_boost_date_time.m4])


AC_CONFIG_SRCDIR([src/main.cpp])
AC_CONFIG_HEADERS([config.h])

# Set default compiler flags
if test -z $CXXFLAGS; then
	CXXFLAGS='-O2'
fi

# Checks for programs.
AC_PROG_CXX
AC_PROG_INSTALL

AC_LANG([C++])

# Checks for header files.

AC_LANG_PUSH([C])
#
AC_LANG_POP([C])

LIBS="$LIBS -lboost_program_options -lboost_system -lboost_thread -lboost_date_time"

# Boost headers
AX_BOOST_BASE([1.42], [],
	[AC_MSG_ERROR(You need the Boost libraries.)])
AX_BOOST_PROGRAM_OPTIONS([1.42])
AX_BOOST_SYSTEM([1.42])
AX_BOOST_THREAD([1.42])
AX_BOOST_DATE_TIME([1.42])


# STL headers
AC_CHECK_HEADERS([algorithm vector deque memory iostream], [],
	[AC_MSG_ERROR(You need the STL libraries.)])

# check boost_program_options library
#if test x${use_program_options} != xno; then
#	AC_CHECK_HEADERS([boost/program_options.hpp], [use_program_options=yes],
#		[AC_MSG_WARN(boost::program_options headers not found.)
#		use_program_options=no])
#fi
#if test x${use_program_options} != xno; then
#	SAVED_LDFLAGS=$LDFLAGS
#	LDFLAGS="$LDFLAGS -lboost_program_options"
#	AC_LINK_IFELSE(
#	  [AC_LANG_PROGRAM([#include <boost/program_options.hpp>],
#	    [boost::program_options::options_description d;])],
#	  [SAVED_LDFLAGS="$SAVED_LDFLAGS -lboost_program_options"
#	  AC_MSG_NOTICE(boost_program_options library found.)
#	  use_program_options=yes],
#	  [AC_MSG_WARN([boost::program_options library not found.])
#	    use_program_options=no])
#	LDFLAGS=$SAVED_LDFLAGS
#fi

if test x${use_program_options} = xyes; then
	AC_DEFINE([USE_PROGRAM_OPTIONS], [], [Use the boost::program_options library] )
fi


# Configuration parameters
AC_CONFIG_FILES([Makefile
                 src/Makefile])


AC_OUTPUT
