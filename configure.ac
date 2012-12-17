dnl Process this file with autoconf to produce a configure script.

AC_INIT
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([src/common/Clock.hh])

MATHVIEW_MAJOR_VERSION=0
MATHVIEW_MINOR_VERSION=8
MATHVIEW_MICRO_VERSION=0
MATHVIEW_VERSION=$MATHVIEW_MAJOR_VERSION.$MATHVIEW_MINOR_VERSION.$MATHVIEW_MICRO_VERSION
MATHVIEW_VERSION_INFO=`expr $MATHVIEW_MAJOR_VERSION + $MATHVIEW_MINOR_VERSION`:$MATHVIEW_MICRO_VERSION:$MATHVIEW_MINOR_VERSION

MAJOR_VERSION=$MATHVIEW_MAJOR_VERSION
MINOR_VERSION=$MATHVIEW_MINOR_VERSION
VERSION=$MATHVIEW_VERSION

AC_SUBST(MAJOR_VERSION)
AC_SUBST(MINOR_VERSION)
AC_SUBST(MATHVIEW_VERSION)
AC_SUBST(MATHVIEW_VERSION_INFO)

AREAMODEL_VERSION_INFO=$MATHVIEW_VERSION_INFO
AC_SUBST(AREAMODEL_VERSION_INFO)

AC_ARG_ENABLE(
	builder-cache,
	[  --enable-builder-cache[=ARG] enable the cache for MathML text nodes [default=no]],
	enable_builder_cache=$enableval,
	enable_builder_cache=no
)

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
	AC_DEFINE(GMV_ENABLE_BOXML,1,[Define to 1 if you want support for BoxML markup])
	GMV_ENABLE_BOXML_CFLAGS=-DGMV_ENABLE_BOXML=1
else
	GMV_ENABLE_BOXML_CFLAGS=
fi
AC_SUBST(GMV_ENABLE_BOXML_CFLAGS)

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

AC_ARG_ENABLE(
	gtk,
	[  --enable-gtk[=ARG]  enable GTK backend [default=auto]],
	enable_gtk=$enableval,
	enable_gtk="auto"
)

dnl Automake configuration
AM_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE(gtkmathview, $MATHVIEW_VERSION)
AC_LIBTOOL_WIN32_DLL
AM_PROG_LIBTOOL

dnl config.h top and bottom
AH_TOP([
/* Copyright (C) 2000-2007, Luca Padovani <padovani@sti.uniurb.it>.
 *
 * This file is part of GtkMathView, a flexible, high-quality rendering
 * engine for MathML documents.
 * 
 * GtkMathView is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 * 
 * GtkMathView is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
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

dnl Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS(unistd.h)

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

AC_LANG_PUSH(C++)

AC_MSG_CHECKING([whether the C++ compiler supports the standard character traits])
AC_TRY_LINK(
	[#include <string>],
	[
		std::basic_string<char> s1;
		std::basic_string<wchar_t> s2;
		std::basic_string<wchar_t> s3;
		s2 = s3;
	],
	[
		AC_DEFINE(STD_TRAITS,1,[Define to 1 if standard traits for char and wchar_t are provided])
		AC_MSG_RESULT(yes)
	],
	[
		AC_MSG_RESULT(no)
	]
)

AC_LANG_POP(C++)

AC_CHECK_PROG(HAVE_XSLTPROC, xsltproc, yes, no)
if test $HAVE_XSLTPROC = "no"; then
	AC_MSG_WARN(xsltproc not found, the compilation may fail)
fi
AM_CONDITIONAL([COND_XSLTPROC], [test "$HAVE_XSLTPROC" = "yes"])

AC_LANG([C])

PKG_CHECK_MODULES(GLIB, [glib-2.0 >= 2.2.1],,[AC_MSG_ERROR(could not find GLIB)])
AC_SUBST(GLIB_CFLAGS)
AC_SUBST(GLIB_LIBS)

GLIB_GENMARSHAL=`pkg-config --variable=glib_genmarshal glib-2.0`
AC_SUBST(GLIB_GENMARSHAL)

have_gtk="no"
have_pango="no"
if test "$enable_gtk" = "auto" -o "$enable_gtk" = "yes"; then
  PKG_CHECK_MODULES(GTK, [gtk+-2.0 >= 2.2.1],
    [AC_DEFINE(HAVE_GTK,1,[Define to 1 if GTK+ is installed])
     have_gtk="yes"],
    [AC_MSG_WARN([could not find GTK+])])
  AC_SUBST(GTK_CFLAGS)
  AC_SUBST(GTK_LIBS)
  PKG_CHECK_MODULES(PANGO, pango,
    [AC_DEFINE(HAVE_PANGO,1,[Define to 1 if Pango is installed])
     have_pango="yes"],
    [AC_MSG_WARN([could not find Pango])])
  AC_SUBST(PANGO_CFLAGS)
  AC_SUBST(PANGO_LIBS)
fi

AM_CONDITIONAL([COND_GTK], [test "$enable_gtk" = "yes" -o \( "$enable_gtk" = "auto" -a \( "$have_gtk" = "yes" -a "$have_pango" = "yes" \) \) ])

AM_CONDITIONAL([COND_CUSTOM_READER], [test "$enable_custom_reader" = "yes" -o "$enable_custom_reader" = "auto"])

have_libxml2="no"
PKG_CHECK_MODULES(XML, [libxml-2.0 >= 2.6.7],
  [AC_DEFINE(HAVE_LIBXML,1,[Define to 1 if libxml2 is installed])
   have_libxml2="yes"],
  [AC_MSG_WARN([could not find libxml2])])
AC_SUBST(XML_CFLAGS)
AC_SUBST(XML_LIBS)
AM_CONDITIONAL([COND_LIBXML2], [test "$enable_libxml2" = "yes" -o \( "$enable_libxml2" = "auto" -a "$have_libxml2" = "yes" \)]) 
AM_CONDITIONAL([COND_LIBXML2_READER], [test "$enable_libxml2_reader" = "yes" -o \( "$enable_libxml2_reader" = "auto" -a "$have_libxml2" = "yes" \)]) 

have_gmetadom="no"
if test "$enable_gmetadom" = "yes" -o "$enable_gmetadom" = "auto"; then
  PKG_CHECK_MODULES(DOM, [gdome2-cpp-smart >= 0.1.8],
    [AC_DEFINE(HAVE_GMETADOM,1,[Define to 1 if gmetadom is installed])
     have_gmetadom="yes"],
    [AC_MSG_WARN([could not find GMetaDOM])]
    [enable_gmetadom="no"])
  AC_SUBST(DOM_CFLAGS)
  AC_SUBST(DOM_LIBS)
fi
AM_CONDITIONAL([COND_GMETADOM], [test "$enable_gmetadom" = "yes" -o \( "$enable_gmetadom" = "auto" -a "$have_gmetadom" = "yes" \)])

CFLAGS="$CFLAGS -W -Wall"
CXXFLAGS="$CXXFLAGS -W -Wall"

if test $enable_builder_cache = yes; then
    AC_DEFINE(ENABLE_BUILDER_CACHE,1,[Define to 1 to enable caching of MathML Text nodes (slower but saves memory)])	
fi

AM_SILENT_RULES([yes])

AC_CONFIG_FILES([
 Makefile
 gtkmathview.spec
 config/Makefile
 config/gtkmathview.conf.xml
 scripts/Makefile
 auto/Makefile
 auto/Char.hh.in
 src/Makefile
 src/common/Makefile
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
 src/backend/Makefile
 src/backend/common/Makefile
 src/backend/gtk/Makefile
 src/view/Makefile
 src/widget/Makefile
 viewer/Makefile
 doc/Makefile
 mathview-core.pc
 mathview-frontend-custom-reader.pc
 mathview-frontend-libxml2-reader.pc
 mathview-frontend-libxml2.pc
 mathview-frontend-gmetadom.pc
 mathview-backend-gtk.pc
 gtkmathview-custom-reader.pc
 gtkmathview-libxml2-reader.pc
 gtkmathview-libxml2.pc
 gtkmathview-gmetadom.pc
])
AC_OUTPUT

cat <<EOF

GtkMathView ${MATHVIEW_VERSION}

Compilation:
  xsltproc            ${HAVE_XSLTPROC}

Engine:
  MathML              yes
  BoxML               ${enable_boxml}

Frontend:
  GMetaDOM            ${enable_gmetadom}
  libxml2             ${enable_libxml2}
  libxml2 reader      ${enable_libxml2_reader}
  custom reader       ${enable_custom_reader}

Backend:
  GTK+                ${have_gtk}

Performance:
  Builder cache       ${enable_builder_cache}

EOF
