dnl Process this file with autoconf to produce a configure script.
AC_INIT
AC_CONFIG_SRCDIR([src/common/Clock.hh])

MATHVIEW_MAJOR_VERSION=0
MATHVIEW_MINOR_VERSION=6
MATHVIEW_MICRO_VERSION=4
MATHVIEW_VERSION=$MATHVIEW_MAJOR_VERSION.$MATHVIEW_MINOR_VERSION.$MATHVIEW_MICRO_VERSION
MATHVIEW_VERSION_INFO=`expr $MATHVIEW_MAJOR_VERSION + $MATHVIEW_MINOR_VERSION`:$MATHVIEW_MICRO_VERSION:$MATHVIEW_MINOR_VERSION

MAJOR_VERSION=$MATHVIEW_MAJOR_VERSION
VERSION=$MATHVIEW_VERSION

AC_SUBST(MAJOR_VERSION)
AC_SUBST(MATHVIEW_VERSION)
AC_SUBST(MATHVIEW_VERSION_INFO)

AREAMODEL_VERSION_INFO=$MATHVIEW_VERSION_INFO
AC_SUBST(AREAMODEL_VERSION_INFO)

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
   AC_DEFINE(ENABLE_DEBUG,1,[Define to 1 if you want to enable validity checks while running])
fi

AC_ARG_ENABLE(
	breaks,
	[  --enable-breaks[=ARG]   enable linebreaking with mspace elements [default=no]],
	enable_breaks=$enableval,
	enable_breaks=no
)

if test "x$enable_breaks" = "xyes"; then
	AC_DEFINE(ENABLE_BREAKS,1,[Define to 1 if you want to enable linebreaking using mspace elements])
fi

AC_ARG_ENABLE(
	boxml,
	[  --enable-boxml[=ARG]    enable support for BoxML markup [default=yes]],
	enable_boxml=$enableval,
	enable_boxml=yes
)
AM_CONDITIONAL([COND_BOXML], [test "$enable_boxml" = "yes"])
if test "x$enable_boxml" = "xyes"; then
	AC_DEFINE(ENABLE_BOXML,1,[Define to 1 if you want support for BoxML markup])
fi

AC_ARG_ENABLE(
	gmetadom,
	[  --enable-gmetadom[=ARG]  enable the GMetaDOM frontend [default=auto]],
	enable_gmetadom=$enableval,
	enable_gmetadom="auto"
)

AC_ARG_ENABLE(
	libxml2,
	[  --enable-libxml2[=ARG]   enable the libxml2 frontend [default=auto]],
	enable_libxml2=$enableval,
	enable_libxml2="auto"
)

AC_ARG_ENABLE(
	libxml2-reader,
	[  --enable-libxml2-reader[=ARG] enable the libxml2 reader frontend [default=auto]],
	enable_libxml2_reader=$enableval,
	enable_libxml2_reader="auto"
)

AC_ARG_ENABLE(
	custom-reader,
	[  --enable-custom-reader[=ARG] enable the custom reader frontend [default=yes]],
	enable_custom_reader=$enableval,
	enable_custom_reader="yes"
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

dnl Automake configuration
AM_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE(gtkmathview, $MATHVIEW_VERSION)
dnl AC_DISABLE_SHARED
AM_PROG_LIBTOOL

dnl config.h top and bottom
AH_TOP([
/* Copyright (C) 2000-2004, Luca Padovani <luca.padovani@cs.unibo.it>.
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
		AC_DEFINE(HAVE_HIDDEN_GETOPT,1,[Define to 1 if #include <getopt.h> works even when HAVE_GETOPT_H is false])
		AC_MSG_RESULT(yes)
	],
	[
		AC_MSG_RESULT(no)
	]
)

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_BIGENDIAN
AC_C_CHAR_UNSIGNED
AC_CHECK_SIZEOF(unsigned char, 1)
AC_CHECK_SIZEOF(unsigned short, 2)
AC_CHECK_SIZEOF(unsigned int, 4)
AC_CHECK_SIZEOF(unsigned long, 4)
AC_CHECK_SIZEOF(unsigned long long, 8)
AC_CHECK_SIZEOF(wchar_t, 8)
if test "x$ac_cv_c_char_unsigned" = "xyes"; then
   AC_SUBST(CHAR_UNSIGNED, "1")
else
   AC_SUBST(CHAR_UNSIGNED, "0")
fi
AC_SUBST(GTKMATHVIEW_SIZEOF_CHAR, "$ac_cv_sizeof_unsigned_char")
AC_SUBST(GTKMATHVIEW_SIZEOF_SHORT, "$ac_cv_sizeof_unsigned_short")
AC_SUBST(GTKMATHVIEW_SIZEOF_INT, "$ac_cv_sizeof_unsigned_int")
AC_SUBST(GTKMATHVIEW_SIZEOF_LONG, "$ac_cv_sizeof_unsigned_long")
AC_SUBST(GTKMATHVIEW_SIZEOF_LONG_LONG, "$ac_cv_sizeof_unsigned_long_long")
AC_SUBST(GTKMATHVIEW_SIZEOF_WCHAR_T, "$ac_cv_sizeof_wchar_t")


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
		AC_DEFINE(ICONV_CONST,1,[Define to 1 if iconv has a const second argument])
		AC_MSG_RESULT(yes)
	],
	[
		AC_MSG_RESULT(no)
	]
)

AC_CHECK_HEADERS(hash_map)
AC_CHECK_HEADERS(ext/hash_map)
AC_CHECK_HEADERS(rope)
AC_CHECK_HEADERS(ext/rope)

AC_MSG_CHECKING([whether the C++ compiler supports the standard character traits])
AC_TRY_COMPILE(
	[#include <string>],
	[
		std::basic_string<char> s1;
		std::basic_string<wchar_t> s2;
	],
	[
		AC_MSG_RESULT(yes)
		STD_TRAITS="yes"
	],
	[
		AC_MSG_RESULT(no)
		STD_TRAITS="no"
	]
)
AC_SUBST(STD_TRAITS)

AC_MSG_CHECKING([whether the C++ compiler supports non-standard character traits])
AC_TRY_COMPILE(
	[#include <string>],
	[
		std::basic_string<unsigned short> s1;
		std::basic_string<unsigned int> s2;
		std::basic_string<unsigned long> s3;
	],
	[
		AC_MSG_RESULT(yes)
		ALL_TRAITS="1"
	],
	[
		AC_MSG_RESULT(no)
		ALL_TRAITS="0"
	]
)
AC_SUBST(ALL_TRAITS)

AC_MSG_CHECKING([whether the C++ compiler supports template specialization])
AC_TRY_COMPILE(
	[
		#include <string>

		namespace std
		{
			template<> struct char_traits<unsigned short> { };
		}
	],
	[ ],
	[
		AC_MSG_RESULT(yes)
		TRAIT_SPEC="1"
	],
	[
		AC_MSG_RESULT(no)
		TRAIT_SPEC="0"
	]
)
AC_SUBST(TRAIT_SPEC)

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
AC_CHECK_FUNC(getopt_long, AC_DEFINE(HAVE_GETOPT_LONG,1,[Define to 1 if you have getopt_long]))

AC_LANG([C])

PKG_CHECK_MODULES(GLIB, glib-2.0 >= 2.2.0,, AC_MSG_ERROR(could not fint GLIB))
AC_SUBST(GLIB_CFLAGS)
AC_SUBST(GLIB_LIBS)

PKG_CHECK_MODULES(GTK, gtk+-2.0 >= 2.2.0,, AC_MSG_ERROR(could not find GTK))
AC_SUBST(GTK_CFLAGS)
AC_SUBST(GTK_LIBS)

PKG_CHECK_MODULES(PANGOX, pangox)
AC_SUBST(PANGOX_CFLAGS)
AC_SUBST(PANGOX_LIBS)

PKG_CHECK_MODULES(XFT, xft)
AC_SUBST(XFT_CFLAGS)
AC_SUBST(XFT_LIBS)

AM_CONDITIONAL([COND_CUSTOM_READER], [test "$enable_custom_reader" = "yes" -o "$enable_custom_reader" = "auto"])

have_libxml2="no"
PKG_CHECK_MODULES(XML, libxml-2.0 >= 2.0.0,
  AC_DEFINE(HAVE_LIBXML,1,[Define to 1 is libxml2 is installed])
  have_libxml2="yes",
  AC_MSG_WARN([could not find libxml2]))
AC_SUBST(XML_CFLAGS)
AC_SUBST(XML_LIBS)
AM_CONDITIONAL([COND_LIBXML2], [test "$enable_libxml2" = "yes" -o \( "$enable_libxml2" = "auto" -a "$have_libxml2" = "yes" \)]) 
AM_CONDITIONAL([COND_LIBXML2_READER], [test "$enable_libxml2_reader" = "yes" -o \( "$enable_libxml2_reader" = "auto" -a "$have_libxml2" = "yes" \)]) 

have_gmetadom="no"
PKG_CHECK_MODULES(DOM, gdome2-cpp-smart >= 0.1.8,
  AC_DEFINE(HAVE_GMETADOM,1,[Define to 1 if gmetadom is installed])
  have_gmetadom="yes",
  AC_MSG_WARN([could not find GMetaDOM]))
AC_SUBST(DOM_CFLAGS)
AC_SUBST(DOM_LIBS)
AM_CONDITIONAL([COND_GMETADOM], [test "$enable_gmetadom" = "yes" -o \( "$enable_gmetadom" = "auto" -a "$have_gmetadom" = "yes" \)])

if test $t1lib = "auto"; then
	AC_CHECK_LIB(t1, T1_InitLib, t1lib="yes", t1lib="no", -lm)
	AC_CHECK_HEADER(t1lib.h,, t1lib="no")
fi

if test $t1lib = "yes"; then
	AC_DEFINE(HAVE_LIBT1,1,[Define to 1 to compile with t1lib for Type 1 fonts])

        if test $with_t1lib_prefix = "yes"; then
                T1_CFLAGS="-I$T1LIB_PREFIX/include"
		T1_LIBS="-L$T1LIB_PREFIX/lib -lt1x -lt1"
	else
		T1_CFLAGS=""
		T1_LIBS="-lt1x -lt1"
        fi
else
	T1_CFLAGS=""
	T1_LIBS=""
fi
AC_SUBST(T1_CFLAGS)
AC_SUBST(T1_LIBS)

if test "(" $t1lib = "no" ")" -a "(" "x$export_PS" = "xyes" ")"; then
	AC_MSG_WARN(cannot export to PostScript if t1lib is not available)
	export_PS=no
fi

if test "x$export_PS" = "xauto"; then
	export_PS=yes
fi

CFLAGS="$CFLAGS -W -Wall"
CXXFLAGS="$CXXFLAGS -W -Wall"

if test $profile = yes; then
	CFLAFS="$CFLAGS -O0 -pg"
	CXXFLAGS="$CXXFLAGS -O0 -pg"
	AC_DEFINE(ENABLE_PROFILE,1,[Define to 1 to let the widget collect some information for profiling purposes])
fi

AC_CONFIG_FILES([
 Makefile
 gtkmathview.spec
 config/Makefile
 config/math-engine-configuration.xml
 scripts/Makefile
 auto/Makefile
 auto/config.dirs
 src/Makefile
 src/common/Makefile
 src/common/Char.hh
 src/common/mathvariants/Makefile
 src/common/mathvariants/xml/Makefile
 src/frontend/Makefile
 src/frontend/common/Makefile
 src/frontend/custom_reader/Makefile
 src/frontend/libxml2_reader/Makefile
 src/frontend/libxml2/Makefile
 src/frontend/gmetadom/Makefile
 src/engine/Makefile
 src/engine/common/Makefile
 src/engine/boxml/Makefile
 src/engine/mathml/Makefile
 src/engine/adapters/Makefile
 src/formatter/Makefile
 src/formatter/common/Makefile
 src/formatter/areamodel/Makefile
 src/formatter/gtk_areamodel/Makefile
 src/formatter/mathml_graphic_device/Makefile
 src/formatter/gtk_mathml_graphic_device/Makefile
 src/formatter/boxml_graphic_device/Makefile
 src/formatter/gtk_boxml_graphic_device/Makefile
 src/view/Makefile
 src/widget/Makefile
 viewer/Makefile
 doc/Makefile
 mathview-libxml2.pc
 gtkmathview-custom.pc
 gtkmathview-libxml2.pc
 gtkmathview-gmetadom.pc
])
AC_OUTPUT

