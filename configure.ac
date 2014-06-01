dnl Process this file with autoconf to produce a configure script.

dnl Automake configuration
AC_INIT([gtkmathview], [1.0.0])
AC_CONFIG_SRCDIR([src/common/Clock.hh])
AC_CONFIG_MACRO_DIR([m4])
AM_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE([subdir-objects foreign])

m4_define(version_triplet,m4_split(AC_PACKAGE_VERSION,[[.]]))
m4_define(version_major,m4_argn(1,version_triplet))
m4_define(version_minor,m4_argn(2,version_triplet))
m4_define(version_micro,m4_argn(3,version_triplet))
MAJOR_VERSION=version_major
MINOR_VERSION=version_minor
MICRO_VERSION=version_micro
MATHVIEW_VERSION_INFO=`expr $MAJOR_VERSION + $MINOR_VERSION`:$MICRO_VERSION:$MINOR_VERSION
AC_SUBST(MAJOR_VERSION)
AC_SUBST(MINOR_VERSION)
AC_SUBST(MICRO_VERSION)
AC_SUBST(MATHVIEW_VERSION_INFO)

AC_ARG_ENABLE(
	builder-cache,
	[  --enable-builder-cache[=ARG] enable the cache for MathML text nodes [default=no]],
	enable_builder_cache=$enableval,
	enable_builder_cache=no
)

AC_ARG_ENABLE(
	gmetadom,
	[  --enable-gmetadom[=ARG]  enable the GMetaDOM frontend [default=auto]],
	enable_gmetadom=$enableval,
	enable_gmetadom="no"
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
	[  --enable-custom-reader[=ARG] enable the custom reader frontend [default=auto]],
	enable_custom_reader=$enableval,
	enable_custom_reader="auto"
)

AC_ARG_ENABLE(
	gtk,
	[  --enable-gtk[=ARG]  enable GTK backend [default=auto]],
	enable_gtk=$enableval,
	enable_gtk="auto"
)

AC_ARG_ENABLE(
	qt,
	[  --enable-qt[=ARG]  enable Qt backend [default=auto]],
	enable_qt=$enableval,
	enable_qt="auto"
)

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

AC_LANG([C])

PKG_CHECK_MODULES(GLIB, [glib-2.0 >= 2.2.1],,[AC_MSG_ERROR(could not find GLIB)])
AC_SUBST(GLIB_CFLAGS)
AC_SUBST(GLIB_LIBS)

GLIB_GENMARSHAL=`pkg-config --variable=glib_genmarshal glib-2.0`
AC_SUBST(GLIB_GENMARSHAL)

AM_MISSING_PROG([XSLTPROC], [xsltproc])

have_qt="no"
if test "$enable_qt" = "auto" -o "$enable_qt" = "yes"; then
  PKG_CHECK_MODULES([QT], [Qt5Core >= 5 Qt5Widgets Qt5Gui], [
    QT_PATH="$(eval $PKG_CONFIG --variable=exec_prefix Qt5Core)"
    AC_PATH_PROGS(MOC, [moc-qt5 moc], moc, [${QT_PATH}/bin ${CONTRIB_DIR}/bin])
    AC_PATH_PROG(RCC, [rcc-qt5 rcc], rcc, [${QT_PATH}/bin ${CONTRIB_DIR}/bin])
    AC_PATH_PROGS(UIC, [uic-qt5 uic], uic, [${QT_PATH}/bin ${CONTRIB_DIR}/bin])
    AC_DEFINE(HAVE_QT,1,[Define to 1 if Qt is installed])
    have_qt="yes"
  ], [
    AC_MSG_WARN([could not find qt])
  ])
  AC_SUBST(QT_CFLAGS)
  AC_SUBST(QT_LIBS)
fi

AM_CONDITIONAL([HAVE_QT], [test "$enable_qt" = "yes" -o \( "$enable_qt" = "auto" -a "$have_qt" = "yes" \) ])

have_gtk="no"
have_pango="no"
if test "$enable_gtk" = "auto" -o "$enable_gtk" = "yes"; then
  PKG_CHECK_MODULES(GTK, [gtk+-2.0 >= 2.2.1],
    [AC_DEFINE(HAVE_GTK,1,[Define to 1 if GTK+ is installed])
     have_gtk="yes"],
    [AC_MSG_WARN([could not find GTK+])])
  AC_SUBST(GTK_CFLAGS)
  AC_SUBST(GTK_LIBS)

  PKG_CHECK_MODULES(CAIRO, cairo,
    [AC_DEFINE(HAVE_CAIRO,1,[Define to 1 if Cairo is installed])
     have_cairo="yes"],
    [AC_MSG_WARN([could not find Cairo])])
  AC_SUBST(CAIRO_CFLAGS)
  AC_SUBST(CAIRO_LIBS)

  PKG_CHECK_MODULES(PANGO, pangocairo,
    [AC_DEFINE(HAVE_PANGO,1,[Define to 1 if Pango is installed])
     have_pango="yes"],
    [AC_MSG_WARN([could not find Pango])])
  AC_SUBST(PANGO_CFLAGS)
  AC_SUBST(PANGO_LIBS)
fi

AM_CONDITIONAL([HAVE_GTK], [test "$enable_gtk" = "yes" -o \( "$enable_gtk" = "auto" -a \( "$have_gtk" = "yes" -a "$have_pango" = "yes" \) \) ])

have_libxml2="no"
PKG_CHECK_MODULES(XML, [libxml-2.0 >= 2.6.7],
  [AC_DEFINE(HAVE_LIBXML,1,[Define to 1 if libxml2 is installed])
   have_libxml2="yes"],
  [AC_MSG_WARN([could not find libxml2])])
if test "$have_libxml2" != "yes"; then
	AC_CHECK_TOOL(XML2_CONFIG, xml2-config, no)
	AC_MSG_CHECKING([for libxml2 by using xml2-config fallback])
	if test "$XML2_CONFIG" != "no" && "$XML2_CONFIG" --version >/dev/null; then
		XML_CFLAGS=`$XML2_CONFIG --cflags`
		XML_LIBS=`$XML2_CONFIG --libs`
		AC_MSG_RESULT([yes])
	else
		AC_MSG_RESULT([no])
	fi
fi
AC_SUBST(XML_CFLAGS)
AC_SUBST(XML_LIBS)
AM_CONDITIONAL([HAVE_LIBXML2], [test "$enable_libxml2" = "yes" -o \( "$enable_libxml2" = "auto" -a "$have_libxml2" = "yes" \)]) 
AM_CONDITIONAL([HAVE_LIBXML2_READER], [test "$enable_libxml2_reader" = "yes" -o \( "$enable_libxml2_reader" = "auto" -a "$have_libxml2" = "yes" \)]) 
AM_CONDITIONAL([HAVE_CUSTOM_READER], [test "$enable_custom_reader" = "yes" -o \( "$enable_custom_reader" = "auto" -a "$have_libxml2" = "yes" \)])

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
AM_CONDITIONAL([HAVE_GMETADOM], [test "$enable_gmetadom" = "yes" -o \( "$enable_gmetadom" = "auto" -a "$have_gmetadom" = "yes" \)])

CFLAGS="$CFLAGS -W -Wall -Wno-unused-parameter"
CXXFLAGS="$CXXFLAGS -W -Wall -Wno-unused-parameter -std=c++11"

if test $enable_builder_cache = yes; then
    AC_DEFINE(ENABLE_BUILDER_CACHE,1,[Define to 1 to enable caching of MathML Text nodes (slower but saves memory)])	
fi

AM_SILENT_RULES([yes])

AC_CONFIG_FILES([
 Makefile
 config/Makefile
 config/gtkmathview.conf.xml
 scripts/Makefile
 auto/Makefile
 auto/Char.hh.in
 src/Makefile
 viewer/Makefile
 doc/Makefile
 mathview-core.pc
 mathview-frontend-custom-reader.pc
 mathview-frontend-libxml2-reader.pc
 mathview-frontend-libxml2.pc
 mathview-frontend-gmetadom.pc
 mathview-backend-cairo.pc
 mathview-backend-qt.pc
 gtkmathview-custom-reader.pc
 gtkmathview-libxml2-reader.pc
 gtkmathview-libxml2.pc
 gtkmathview-gmetadom.pc
])
AC_OUTPUT

cat <<EOF

GtkMathView ${PACKAGE_VERSION}

Frontend:
  GMetaDOM            ${enable_gmetadom}
  libxml2             ${enable_libxml2}
  libxml2 reader      ${enable_libxml2_reader}
  custom reader       ${enable_custom_reader}

Backend:
  GTK+                ${have_gtk}
  Qt                  ${have_qt}

Performance:
  Builder cache       ${enable_builder_cache}

EOF
