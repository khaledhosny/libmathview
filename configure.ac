dnl Process this file with autoconf to produce a configure script.

dnl Automake configuration
AC_INIT([gtkmathview], [1.0.0])
AC_CONFIG_SRCDIR([src/common/Clock.hh])
AC_CONFIG_MACRO_DIR([m4])
AM_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE([subdir-objects foreign])
AM_SILENT_RULES([yes])

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

AM_MISSING_PROG([XSLTPROC], [xsltproc])
AM_MISSING_PROG([PYTHON], [python2])

dnl =============================================================================
dnl Qt support
dnl =============================================================================

AC_ARG_ENABLE(
  qt,
  [  --enable-qt[=ARG]  enable Qt backend [default=auto]],
  enable_qt=$enableval,
  enable_qt="auto"
)
have_qt=false
if test "$enable_qt" = "auto" -o "$enable_qt" = "yes"; then
  PKG_CHECK_MODULES([QT], [Qt5Core >= 5 Qt5Widgets Qt5Gui], [
    QT_PATH="$(eval $PKG_CONFIG --variable=exec_prefix Qt5Core)"
    AC_PATH_PROGS(MOC, [moc-qt5 moc], moc, [${QT_PATH}/bin ${CONTRIB_DIR}/bin])
    AC_PATH_PROG(RCC, [rcc-qt5 rcc], rcc, [${QT_PATH}/bin ${CONTRIB_DIR}/bin])
    AC_PATH_PROGS(UIC, [uic-qt5 uic], uic, [${QT_PATH}/bin ${CONTRIB_DIR}/bin])
    have_qt=true
  ], :)
fi
if test "$enable_qt" = "yes" -a "$have_qt" != "true"; then
  AC_MSG_ERROR([Qt support requested but Qt could not found])
fi
if $have_qt; then
  AC_DEFINE(HAVE_QT, 1, [Define to 1 if Qt is installed])
fi
AM_CONDITIONAL(HAVE_QT, $have_qt)

dnl =============================================================================
dnl Cairo support
dnl =============================================================================

AC_ARG_ENABLE(
  cairo,
  [  --enable-cairo[=ARG]  enable Cairo backend [default=auto]],
  enable_cairo=$enableval,
  enable_cairo="auto"
)
have_cairo=false
if test "$enable_cairo" = "auto" -o "$enable_cairo" = "yes"; then
  PKG_CHECK_MODULES(CAIRO, cairo-fc, have_cairo=true, :)
fi
if test "$enable_cairo" = "yes" -a "$have_cairo" != "true"; then
  AC_MSG_ERROR([Cairo support requested but cairo could not found])
fi
if $have_cairo; then
  AC_DEFINE(HAVE_CAIRO, 1, [Define to 1 if Cairo is installed])
fi
AM_CONDITIONAL(HAVE_CAIRO, $have_cairo)

dnl =============================================================================
dnl GTK+ support
dnl =============================================================================

AC_ARG_ENABLE(
  gtk,
  [  --enable-gtk[=ARG]  enable GTK widget and viewer [default=auto]],
  enable_gtk=$enableval,
  enable_gtk="auto"
)
have_gtk=false
if test "$enable_gtk" = "auto" -o "$enable_gtk" = "yes"; then
  PKG_CHECK_MODULES(GTK, [gtk+-3.0], have_gtk=true, :)
  GLIB_GENMARSHAL=`pkg-config --variable=glib_genmarshal glib-2.0`
fi
if test "$enable_gtk" = "yes" -a "$have_gtk" != "true"; then
  AC_MSG_ERROR([GTK+ support requested but gtk+-3.0 could not be found])
fi
if $have_gtk; then
  AC_DEFINE(HAVE_GTK, 1, [Define to 1 if GTK+ is installed])
fi
AM_CONDITIONAL(HAVE_GTK, $have_gtk)
AC_SUBST(GLIB_GENMARSHAL)

AC_ARG_ENABLE(
  glib,
  [  --enable-glib[=ARG]  enable GLIB widget and viewer [default=auto]],
  enable_glib=$enableval,
  enable_glib="auto"
)
have_glib=false
if test "$enable_glib" = "auto" -o "$enable_glib" = "yes"; then
  PKG_CHECK_MODULES(GLIB, [glib-2.0 >= 2.2.1], have_glib=true, :)
fi
if test "$enable_glib" = "yes" -a "$have_glib" != "true"; then
  AC_MSG_ERROR([GLib support requested but glib-2.0 could not be found])
fi
if $have_glib; then
  AC_DEFINE(HAVE_GLIB, 1, [Define to 1 if GLib is installed])
fi
AM_CONDITIONAL(HAVE_GLIB, $have_glib)

dnl =============================================================================
dnl Viewers
dnl =============================================================================

have_cairo_viewer=false

if $have_glib; then
  if $have_cairo; then
    have_cairo_viewer=true
  fi
fi

dnl =============================================================================
dnl libxml2 support
dnl =============================================================================

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
have_libxml2=false
if test "$enable_libxml2" = "auto" -o "$enable_libxml2" = "yes"; then
  PKG_CHECK_MODULES(XML, [libxml-2.0 >= 2.6.7], have_libxml2=true, :)
  if test "$have_libxml2" != "true"; then
    AC_CHECK_TOOL(XML2_CONFIG, xml2-config, no)
    AC_MSG_CHECKING([for libxml2 by using xml2-config fallback])
    if test "$XML2_CONFIG" != "no" && "$XML2_CONFIG" --version >/dev/null; then
      have_libxml2=true
      XML_CFLAGS=`$XML2_CONFIG --cflags`
      XML_LIBS=`$XML2_CONFIG --libs`
      AC_MSG_RESULT([yes])
    else
      AC_MSG_ERROR(could not find libxml2)
    fi
  fi
fi
AC_SUBST(XML_CFLAGS)
AC_SUBST(XML_LIBS)
if test "$enable_libxml2" = "yes" -a "$have_libxml2" != "true"; then
  AC_MSG_ERROR([libxml2 support requested but libxml-2.0 could not be found])
fi
if $have_libxml2; then
  AC_DEFINE(HAVE_LIBXML, 1, [Define to 1 if libxml2 is installed])
fi
AM_CONDITIONAL(HAVE_LIBXML2, $have_libxml2)

have_libxml2_reader=false
if test "$enable_libxml2_reader" = "auto" -o "$enable_libxml2_reader" = "yes"; then
  if $have_libxml2; then
    have_libxml2_reader=true
  fi
fi
if test "$enable_libxml2_reader" = "yes" -a "$have_libxml2_reader" != "true"; then
  AC_MSG_ERROR([libxml2 reader support requested but libxml-2.0 could not be found])
fi
if $have_libxml2_reader; then
  AC_DEFINE(HAVE_LIBXML2_READER, 1, [Define to 1 if libxml2 is installed])
fi
AM_CONDITIONAL(HAVE_LIBXML2_READER, $have_libxml2_reader)

dnl =============================================================================
dnl Custom XML reader support
dnl =============================================================================

AC_ARG_ENABLE(
  custom-reader,
  [  --enable-custom-reader[=ARG] enable the custom reader frontend [default=yes]],
  enable_custom_reader=$enableval,
  enable_custom_reader="yes"
)
AM_CONDITIONAL(HAVE_CUSTOM_READER, [test "$enable_custom_reader" = "yes"])

dnl =============================================================================

CFLAGS="$CFLAGS -W -Wall -Wno-unused-parameter"
CXXFLAGS="$CXXFLAGS -W -Wall -Wno-unused-parameter -std=c++11"

AC_CONFIG_FILES([
 Makefile
 config/Makefile
 config/gtkmathview.conf.xml
 scripts/Makefile
 auto/Makefile
 src/Makefile
 viewer/Makefile
 doc/Makefile
 mathview-core.pc
 mathview-frontend-custom-reader.pc
 mathview-frontend-libxml2-reader.pc
 mathview-frontend-libxml2.pc
 mathview-backend-cairo.pc
 mathview-backend-qt.pc
 gtkmathview-custom-reader.pc
 gtkmathview-libxml2-reader.pc
 gtkmathview-libxml2.pc
])
AC_OUTPUT

cat <<EOF

GtkMathView ${PACKAGE_VERSION}

Frontend:
  libxml2             ${have_libxml2}
  libxml2 reader      ${have_libxml2_reader}
  custom reader       ${enable_custom_reader}

Backend:
  Cairo               ${have_cairo}
  Qt                  ${have_qt}

Viewer:
  Cairo               ${have_cairo_viewer}
  GTK+                ${have_gtk}
  Qt                  ${have_qt}

EOF
