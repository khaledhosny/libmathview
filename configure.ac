dnl Process this file with autoconf to produce a configure script.
AC_INIT
AC_CONFIG_SRCDIR([src/MathMLObject.cc])

MATHVIEW_MAJOR_VERSION=0
MATHVIEW_MINOR_VERSION=4
MATHVIEW_MICRO_VERSION=2
MATHVIEW_VERSION=$MATHVIEW_MAJOR_VERSION.$MATHVIEW_MINOR_VERSION.$MATHVIEW_MICRO_VERSION
MATHVIEW_VERSION_INFO=`expr $MATHVIEW_MAJOR_VERSION + $MATHVIEW_MINOR_VERSION`:$MATHVIEW_MICRO_VERSION:$MATHVIEW_MINOR_VERSION

MAJOR_VERSION=$MATHVIEW_MAJOR_VERSION
VERSION=$MATHVIEW_VERSION

AC_SUBST(MAJOR_VERSION)
AC_SUBST(MATHVIEW_VERSION)
AC_SUBST(MATHVIEW_VERSION_INFO)

AC_ARG_ENABLE(
	profile,
	[  --enable-profile[=ARG]  include profiling information [default=no]],
	profile=$enableval,
	profile=no
)

AC_ARG_ENABLE(
	debug,
	[  --enable-debug[=ARG]    include debugging debug [default=yes]],
	enable_debug=$enableval,
	enable_debug=yes
)

if test "x$enable_debug" = "xyes"; then
   AC_DEFINE(ENABLE_DEBUG,,[Define to 1 if you want to enable validity checks while running])
fi

AC_ARG_WITH(
	scaled-type,
	[  --with-scaled-type[=TYPE] define the type for scaled points [default=float]],
	SCALED_TYPE=$withval,
	SCALED_TYPE="float"
)
AC_SUBST(SCALED_TYPE)

AC_ARG_ENABLE(
	texish,
	[  --enable-texish[=ARG]   enable TeX like layout when possible [default=no]],
	texish_mathml=$enableval,
	texish_mathml=no
)

if test "x$texish_mathml" = "xyes"; then
   AC_DEFINE(TEXISH_MATHML,,[Define to 1 if you want some formatting to be done using TeX layout algorithm])
fi

AC_ARG_ENABLE(
	extensions,
	[  --enable-extensions[=ARG] enable MathML extensions [default=yes]],
	enable_extensions=$enableval,
	enable_extensions=no
)

if test "x$enable_extension" = "xyes"; then
   AC_DEFINE(ENABLE_EXTENSIONS,,[Define to 1 to allow some extensions to MathML (new attributes)])
fi

AC_ARG_WITH(
	gmetadom,
	[  --with-gmetadom[=ARG]   compile with gmetadom library [default=auto]],
	gmetadom=$withval,
	gmetadom=auto
)

AC_ARG_WITH(
	minidom,
	[  --with-minidom[=ARG]    compile with minidom library [default=auto]],
	minidom=$withval,
	minidom=auto
)
	
AC_ARG_WITH(
	t1lib,
	[  --with-t1lib[=ARG]      compile with t1lib 1.x library [default=auto]],
	t1lib=$withval,
	t1lib=auto
)

AC_ARG_WITH(
        t1lib-prefix,
        [  --with-t1lib-prefix=PFX prefix dir where t1lib is installed],
        [
                with_t1lib_prefix=yes
                T1LIB_PREFIX=$withval
        ],
        [
                with_t1lib_prefix=no
        ]
)

AC_ARG_WITH(
	glib-config,
	[  --with-glib-config[=ARG] use glib-config to determine glib configuration],
	with_glib_config=$withval,
	with_glib_config=no
)

AC_ARG_ENABLE(
	export-PS,
	[  --enable-export-PS[=ARG] enable support for exporting PostScript [default=auto]],
	export_PS=$enableval,
	export_PS=auto
)

dnl Automake configuration
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE(gtkmathview, $MATHVIEW_VERSION)
dnl AC_DISABLE_SHARED
AM_PROG_LIBTOOL

dnl config.h top and bottom
AH_TOP([
/* Copyright (C) 2000-2003, Luca Padovani <luca.padovani@cs.unibo.it>.
 *
 * This file is part of GtkMathView, a Gtk widget for MathML.
 * 
 * GtkMathView is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * GtkMathView is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GtkMathView; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 * 
 * For details, see the GtkMathView World-Wide-Web page,
 * http://helm.cs.unibo.it/mml-widget, or send a mail to
 * <luca.padovani@cs.unibo.it>
 */

#ifndef config_h
#define config_h
])

AH_BOTTOM([
#endif /* config_h */
])

dnl Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AC_PROG_LN_S
AC_ISC_POSIX
AC_CHECK_PROG(HAVE_ICONV, iconv, yes, no)
AC_CHECK_PROG(HAVE_GLIB_CONFIG, glib-config, yes, no)

dnl Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS(wctype.h,,AC_MSG_WARN(could not find required header file wctype.h))
AC_CHECK_HEADERS(wchar.h,,AC_MSG_WARN(could not find required header file wchar.h))
AC_CHECK_HEADERS(unistd.h)
AC_CHECK_HEADER(iconv.h,,AC_MSG_ERROR(could not find required header file iconv.h))
AC_CHECK_HEADERS(getopt.h)
AC_MSG_CHECKING(for hidden getopt.h)
AC_TRY_COMPILE(
	[#include <getopt.h>],
	[],
	[
		AC_DEFINE(HAVE_HIDDEN_GETOPT,,[Define to 1 if #include <getopt.h> works even when HAVE_GETOPT_H is false])
		AC_MSG_RESULT(yes)
	],
	[
		AC_MSG_RESULT(no)
	]
)

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_CHAR_UNSIGNED
AC_C_BIGENDIAN

dnl Check for iconv with const pointer as second argument
AC_MSG_CHECKING(whether iconv has a const argument)
AC_LANG_PUSH(C++)
AC_TRY_COMPILE(
	[#include <iconv.h>],
	[
		const char* inbuf;
		iconv_t     cd;
		iconv(cd, &inbuf, 0, 0, 0);
	],
	[
		AC_DEFINE(ICONV_CONST,,[Define to 1 if iconv has a const second argument])
		AC_MSG_RESULT(yes)
	],
	[
		AC_MSG_RESULT(no)
	]
)

AC_CHECK_HEADERS(hash_map)
AC_CHECK_HEADERS(ext/hash_map)

AC_LANG_POP(C++)

AC_CHECK_PROG(HAVE_ICONV, iconv, yes, no)
if test $HAVE_ICONV = "yes"; then
	if iconv -t UTF-8 -f UTF-8 </dev/null >/dev/null 2>&1; then
		ICONV_UTF8="UTF-8"
	else
		AC_MSG_WARN(UTF-8 to UTF-8 is not supported. This may result in failed assertions)
		ICONV_UTF8="UTF-8"
	fi

	if iconv -t UCS-4BE -f UCS-4BE </dev/null >/dev/null 2>&1; then
		ICONV_UCS4BE="UCS-4BE"
	elif iconv -t UCS-4 -f UCS-4 </dev/null >/dev/null 2>&1; then
		AC_MSG_WARN(assuming that iconv UCS-4 is bigendian)
		ICONV_UCS4BE="UCS-4"
	else
		AC_MSG_WARN(UCS-4 to UCS-4 is not supported. This may result in failed assertions)
		ICONV_UCS4BE="UCS-4"
	fi

	if iconv -t UCS-4LE -f UCS-4LE </dev/null >/dev/null 2>&1; then
		ICONV_UCS4LE="UCS-4LE"
	else
		ICONV_UCS4LE="no"
	fi
else
	AC_MSG_WARN(iconv not found. Assuming that UTF-8 and UCS-4 exist. This may result in failed assertions)
	ICONV_UTF8="UTF-8"
	ICONV_UCS4BE="UCS-4"
	ICONV_UCS4LE="no"
fi
AC_DEFINE_UNQUOTED(ICONV_UTF8, "$ICONV_UTF8", iconv UTF-8 encoding)
AC_DEFINE_UNQUOTED(ICONV_UCS4BE, "$ICONV_UCS4BE", iconv UCS-4 encoding big endian)
if test $ICONV_UCS4LE != "no"; then
	AC_DEFINE_UNQUOTED(ICONV_UCS4LE, "$ICONV_UCS4LE", iconv UCS-4 encoding little endian)
fi

dnl Checks for libraries
AC_CHECK_LIB(iberty, getopt_long,
	LIBS="$LIBS -liberty",
	,
	-liberty
)
AC_CHECK_FUNC(getopt_long, AC_DEFINE(HAVE_GETOPT_LONG,,[Define to 1 if you have getopt_long]))

AC_LANG([C])
if test $with_glib_config = "no"; then
	AM_PATH_GLIB(1.2.0,,AC_MSG_ERROR(could not find glib))
else
	if test $HAVE_GLIB_CONFIG = "yes"; then
		GLIB_CFLAGS=`glib-config --cflags`
		GLIB_LIBS=`glib-config --libs`
	else
		AC_MSG_ERROR(could not find glib)
	fi
fi

AM_PATH_GTK(1.2.0, gtklib="yes", gtklib="no")

if test $gtklib = "no"; then
	AC_MSG_WARN(GTK is needed in order to use the widget)
fi

if test $gmetadom = "no" -a $minidom = "no"; then
	AC_MSG_ERROR(you must specify at least one DOM library to be used)
fi

if test $gmetadom = "yes" -a $minidom = "yes"; then
	AC_MSG_ERROR(you must specify at most one DOM library to be used)
fi

if test $minidom = "yes"; then
	gmetadom="no"
fi

if test $gmetadom = "yes"; then
	minidom="no"
fi

AC_CHECK_PROG(HAVE_GMETADOM_CONFIG, gmetadom-config, yes, no)
if test $HAVE_GMETADOM_CONFIG = "no"; then
	gmetadom="no"
fi

if test $gmetadom = "auto"; then
	GMETADOM_MIN_VERSION=0.1.5
	AC_MSG_CHECKING(for gmetadom >= $GMETADOM_MIN_VERSION)
	vers=`gmetadom-config --module=gdome_cpp_smart --version | awk -F. '{ printf "%d", ($1 * 1000 + $2) * 1000 + $3;}'`
	minvers=`echo $GMETADOM_MIN_VERSION | awk -F. '{ printf "%d", ($1 * 1000 + $2) * 1000 + $3;}'`

	if test "$vers" -lt "$minvers"; then
		gmetadom="no"
	else
		gmetadom="yes"
	fi

	if test $gmetadom = "yes"; then
		AC_MSG_RESULT(found)
	else
		AC_MSG_RESULT(not found)
	fi
fi

AC_CHECK_PROG(HAVE_MINIDOM_CONFIG, minidom-config, yes, no)
if test $HAVE_MINIDOM_CONFIG = "no"; then
	minidom="no"
fi

if test $minidom = "auto"; then
	MINIDOM_MIN_VERSION=0.1.3
	AC_MSG_CHECKING(for minidom >= $MINIDOM_MIN_VERSION)
	vers=`minidom-config --version | awk -F. '{ printf "%d", ($1 * 1000 + $2) * 1000 + $3;}'`
	minvers=`echo $MINIDOM_MIN_VERSION | awk -F. '{ printf "%d", ($1 * 1000 + $2) * 1000 + $3;}'`

	if test "$vers" -lt "$minvers"; then
		minidom="no"
	else
		minidom="yes"
	fi

	if test $minidom = "yes"; then
		AC_MSG_RESULT(found)
	else
		AC_MSG_RESULT(not found)
	fi
fi

if test $gmetadom = "no" -a $minidom = "no"; then
	AC_MSG_ERROR(no DOM library was found, this is a fatal error)
fi

if test $gmetadom = "yes" -a $minidom = "yes"; then
	AC_MSG_WARN(both gmetadom and minidom were found, using gmetadom)
	minidom="no"
fi

if test $gmetadom = "yes"; then
	DOM_CFLAGS=`gmetadom-config --module=gdome_cpp_smart --cflags`
	DOM_LIBS=`gmetadom-config --module=gdome_cpp_smart --libs`
	DOM_IMPLEMENTATION="GMETADOM"
	AC_DEFINE(HAVE_GMETADOM,1,[Define to 1 to compile with GMetaDOM (recommended)])
else
	DOM_CFLAGS="$GLIB_CFLAGS `minidom-config --cflags`"
	DOM_LIBS="$GLIB_LIBS `minidom-config --libs`"
	DOM_IMPLEMENTATION="MINIDOM"
	AC_DEFINE(HAVE_MINIDOM,1,[Define to 1 to compile with minidom (deprecated)])
fi
AC_SUBST(DOM_CFLAGS)
AC_SUBST(DOM_LIBS)
AC_SUBST(DOM_IMPLEMENTATION)

if test $t1lib = "auto"; then
	AC_CHECK_LIB(t1, T1_InitLib, t1lib="yes", t1lib="no", -lm)
	AC_CHECK_HEADER(t1lib.h,, t1lib="no")
fi

if test $t1lib = "yes"; then
	AC_DEFINE(HAVE_LIBT1,1,[Define to 1 to compile with t1lib for Type 1 fonts])

        if test $with_t1lib_prefix = "yes"; then
                T1_CFLAGS="-I$T1LIB_PREFIX/include"
	else
		T1_CFLAGS=""
        fi

	T1_LIBS="-lt1x -lt1"
else
	T1_CFLAGS=""
	T1_LIBS=""
fi
AC_SUBST(T1_CFLAGS)
AC_SUBST(T1_LIBS)

if test "(" $t1lib = "no" ")" -a "(" $export_PS = "yes" ")"; then
	AC_MSG_WARN(cannot export to PostScript if t1lib is not available)
	export_PS=no
fi

if test $export_PS = "auto"; then
	export_PS=yes
fi

CFLAGS="$CFLAGS -W -Wall -O0"
CXXFLAGS="$CXXFLAGS -W -Wall -O0"

if test $profile = yes; then
	CFLAFS="$CFLAGS -p"
	CXXFLAGS="$CXXFLAGS -p"
	AC_DEFINE(ENABLE_PROFILE,,[Define to 1 to let the widget collect some information for profiling purposes])
fi

AC_CONFIG_FILES([
Makefile
gtkmathview.spec
config/Makefile
config/math-engine-configuration.xml
scripts/Makefile
src/Makefile
src/scaled.hh
auto/Makefile
auto/config.dirs
doc/Makefile
viewer/Makefile
randomath/Makefile
mathml2ps/Makefile
])
AC_OUTPUT

dnl getopt/Makefile
