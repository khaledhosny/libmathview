dnl Process this file with autoconf to produce a configure script.

AC_INIT
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
	pipe,
	[  --enable-pipe[=ARG]     enable the -pipe option in the GCC compiler [default=no]],
	enable_pipe=$enableval,
	enable_pipe=no
)

AC_ARG_ENABLE(
	profile,
	[  --enable-profile[=ARG]  include profiling information [default=no]],
	enable_profile=$enableval,
	enable_profile=no
)

AC_ARG_ENABLE(
	debug,
	[  --enable-debug[=ARG]    include debugging debug [default=yes]],
	enable_debug=$enableval,
	enable_debug=no
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
	tfm,
	[  --enable-tfm[=ARG]  enable support level for TeX Font Metrics (0,1,2,3) [disabled=0,default=2]],
	enable_tfm=$enableval,
	enable_tfm="2"
)
if test $enable_tfm = "yes"; then
  enable_tfm="2"
elif test $enable_tfm = "no"; then
  enable_tfm="0"
fi

AC_ARG_ENABLE(
	gtk,
	[  --enable-gtk[=ARG]  enable GTK backend [default=auto]],
	enable_gtk=$enableval,
	enable_gtk="auto"
)

AC_ARG_ENABLE(
	svg,
	[  --enable-svg[=ARG]  enable SVG backend [default=yes]],
	enable_svg=$enableval,
	enable_svg="yes"
)

AC_ARG_ENABLE(
	ps,
	[  --enable-ps[=ARG]  enable PostScript backend [default=yes]],
	enable_ps=$enableval,
	enable_ps="yes"
)

AC_ARG_ENABLE(
	gcc-pch,
	[  --enable-gcc-pch=[yes/no/auto]       use gcc4 pch support (default=auto)],
	enable_gcc_pch=$enableval,
	enable_gcc_pch="auto"
)

AC_ARG_WITH(
	t1lib,
	[  --with-t1lib[=ARG]      compile with t1lib 1.x library [default=auto]],
	with_t1lib=$withval,
	with_t1lib=auto
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
        popt-prefix,
        [  --with-popt-prefix=PFX prefix dir where popt is installed],
        [
                with_popt_prefix="yes"
                POPT_PREFIX=$withval
        ],
        [
                with_popt_prefix="no"
        ]
)

AC_ARG_WITH(
	popt,
	[  --with-popt[=ARG] use popt - required for mathmlsvg and viewer (default=auto)],
	with_popt=$withval,
	with_popt="auto"
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
#define GMV_ENABLE_TFM (GMV_TFM_LEVEL != 0)

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

AC_CHECK_HEADERS(hash_map,
	[
		AC_DEFINE(GMV_HAVE_HASH_MAP,1,[Define if <hash_map> is provided])
		GMV_HAVE_HASH_MAP_CFLAGS=-DGMV_HAVE_HASH_MAP=1
	],
	[
		GMV_HAVE_HASH_MAP_CFLAGS=
	]
)
AC_SUBST(GMV_HAVE_HASH_MAP_CFLAGS)
AC_CHECK_HEADERS(ext/hash_map,
	[
		AC_DEFINE(GMV_HAVE_EXT_HASH_MAP,1,[Define if <ext/hash_map> is provided])
		GMV_HAVE_EXT_HASH_MAP_CFLAGS=-DGMV_HAVE_EXT_HASH_MAP=1
	],
	[
		GMV_HAVE_EXT_HASH_MAP_CFLAGS=
	]
)
AC_SUBST(GMV_HAVE_EXT_HASH_MAP_CFLAGS)

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

have_popt="no"
if test $with_popt = "auto"; then
	AC_CHECK_HEADERS(popt.h,[
		AC_CHECK_LIB(popt, poptGetContext, [have_popt="yes"])
	])
else
	have_popt=$with_popt
fi

if test $have_popt = "yes"; then
	AC_DEFINE(HAVE_POPT_H,1,[Define to 1 if popt.h is available])

        if test $with_popt_prefix = "yes"; then
                POPT_CFLAGS="-I$POPT_PREFIX/include"
		POPT_LIBS="-L$POPT_PREFIX/lib -lpopt"
	else
		POPT_CFLAGS=""
		POPT_LIBS="-lpopt"
        fi
else
	POPT_CFLAGS=""
	POPT_LIBS=""
fi
AM_CONDITIONAL([COND_HAVE_POPT], [test "$have_popt" = "yes"])
AC_SUBST(POPT_CFLAGS)
AC_SUBST(POPT_LIBS)

AC_LANG([C])

PKG_CHECK_MODULES(GLIB, [glib-2.0 >= 2.2.1],,[AC_MSG_ERROR(could not find GLIB)])
AC_SUBST(GLIB_CFLAGS)
AC_SUBST(GLIB_LIBS)

have_gtk="no"
have_pango="no"
if test "$enable_gtk" = "auto" -o "$enable_gtk" = "yes"; then
  PKG_CHECK_MODULES(GTK, [gtk+-2.0 >= 2.2.1],
    [AC_DEFINE(HAVE_GTK,1,[Define to 1 if GTK+ is installed])
     have_gtk="yes"],
    [AC_MSG_WARN([could not find GTK+])])
  AC_SUBST(GTK_CFLAGS)
  AC_SUBST(GTK_LIBS)
  PKG_CHECK_MODULES(PANGOX, pangox,
    [AC_DEFINE(HAVE_PANGO,1,[Define to 1 if Pango is installed])
     have_pango="yes"],
    [AC_MSG_WARN([could not find Pango])])
  AC_SUBST(PANGOX_CFLAGS)
  AC_SUBST(PANGOX_LIBS)
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

have_t1lib="no"
if test $with_t1lib = "auto"; then
	AC_CHECK_HEADERS(t1lib.h,[
		AC_CHECK_LIB(t1, T1_InitLib, [have_t1lib="yes"],,[-lm])
	])
else
	have_t1lib=$with_t1lib
fi

if test $have_t1lib = "yes"; then
	AC_DEFINE(HAVE_LIBT1,1,[Define to 1 to compile with t1lib for Type 1 fonts])

        if test $with_t1lib_prefix = "yes"; then
                T1_CFLAGS="-I$T1LIB_PREFIX/include"
		T1_LIBS="-L$T1LIB_PREFIX/lib -lt1"
		T1_LIBS_X="-L$T1LIB_PREFIX/lib -lt1x"
	else
		T1_CFLAGS=""
		T1_LIBS="-lt1"
		T1_LIBS_X="-lt1x"
        fi
else
	T1_CFLAGS=""
	T1_LIBS=""
	T1_LIBS_X=""
fi
AM_CONDITIONAL([COND_T1LIB], [test $have_t1lib = "yes"])
AC_SUBST(T1_CFLAGS)
AC_SUBST(T1_LIBS)
AC_SUBST(T1_LIBS_X)

CFLAGS="$CFLAGS -W -Wall"
CXXFLAGS="$CXXFLAGS -W -Wall"

if test $enable_debug = yes; then
	CFLAGS="$CFLAGS -O0"
	CXXFLAGS="$CXXFLAGS -O0"
	AC_DEFINE(ENABLE_DEBUG,1,[Define to 1 if you want to enable validity checks while running])
fi

if test $enable_profile = yes; then
	CFLAGS="$CFLAGS -pg"
	CXXFLAGS="$CXXFLAGS -pg"
	AC_DEFINE(ENABLE_PROFILE,1,[Define to 1 to let the widget collect some information for profiling purposes])
fi
AM_CONDITIONAL([COND_PROFILE], [test "$enable_profile" = "yes"])

if test $enable_pipe = yes; then
	CFLAGS="$CFLAGS -pipe"
	CXXFLAGS="$CXXFLAGS -pipe"
fi

if test $enable_builder_cache = yes; then
    AC_DEFINE(ENABLE_BUILDER_CACHE,1,[Define to 1 to enable caching of MathML Text nodes (slower but saves memory)])	
fi

AC_DEFINE_UNQUOTED(GMV_TFM_LEVEL, $enable_tfm, [Define to 0, 1, 2, or 3 depending on the TFM support level you want])
AM_CONDITIONAL([COND_TFM], [test "$enable_tfm" != "0"])
AM_CONDITIONAL([COND_TFM_LEVEL_1], [test "$enable_tfm" = "1"])
AM_CONDITIONAL([COND_TFM_LEVEL_2], [test "$enable_tfm" = "2"])
AM_CONDITIONAL([COND_TFM_LEVEL_3], [test "$enable_tfm" = "3"])

if test "$enable_tfm" = "0" -a "$enable_svg" = "yes"; then
	AC_MSG_WARN([SVG support requires TFM support])
	enable_svg="no"
fi
	
if test "$enable_svg" = "yes"; then
	AC_DEFINE(ENABLE_SVG,1,[Define to 1 if you want to enable the SVG backend])
fi
AM_CONDITIONAL([COND_SVG], [test "$enable_svg" = "yes"])

if test "$enable_ps" = "yes"; then
	AC_DEFINE(ENABLE_PS,1,[Define to 1 if you want to enable the PostScript backend])
fi
AM_CONDITIONAL([COND_PS], [test "$enable_ps" = "yes"])

AM_BINRELOC
AM_CONDITIONAL(WITH_BINRELOC, test "x$br_cv_binreloc" = "xyes")

AC_MSG_CHECKING([whether to use GCC precompiled headers with $CXX])
if test "x$GCC" = "xyes" ; then
	_gmv_gcc_version=`$CXX --version | grep GCC | head -1 | cut -f 3 -d " "`
        _gmv_gcc_major=`echo $_gmv_gcc_version | cut -f 1 -d "."`

	if [[ "$_gmv_gcc_major" -ge "4" ]] ; then
		if test "x$enable_gcc_pch" = "xyes" ; then
			AC_MSG_RESULT(yes)
			gmv_gcc_pch="yes"
		elif test "x$enable_gcc_pch" = "xauto" ; then
			AC_MSG_RESULT(yes)
			gmv_gcc_pch="yes"
		else
			AC_MSG_RESULT(no)
			gmv_gcc_pch="no"
		fi
	else
		AC_MSG_RESULT(no)
		gmv_gcc_pch="no"
	fi
else
	gmv_gcc_pch="no"
fi
AM_CONDITIONAL(USE_GCC_PCH, test "x$gmv_gcc_pch" = "xyes")

AC_CONFIG_FILES([
 Makefile
 gtkmathview.spec
 config/Makefile
 config/gtkmathview.conf.xml
 scripts/Makefile
 auto/Makefile
 auto/Char.hh.in
 autopackage/Makefile
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
 src/backend/common/tfm/Makefile
 src/backend/gtk/Makefile
 src/backend/svg/Makefile
 src/backend/ps/Makefile
 src/view/Makefile
 src/widget/Makefile
 viewer/Makefile
 mathmlsvg/Makefile
 mathmlps/Makefile
 doc/Makefile
 mathview-core.pc
 mathview-frontend-custom-reader.pc
 mathview-frontend-libxml2-reader.pc
 mathview-frontend-libxml2.pc
 mathview-frontend-gmetadom.pc
 mathview-backend-ps.pc
 mathview-backend-svg.pc
 mathview-backend-gtk.pc
 gtkmathview-custom-reader.pc
 gtkmathview-libxml2-reader.pc
 gtkmathview-libxml2.pc
 gtkmathview-gmetadom.pc
 autopackage/abimath.apspec
])
AC_OUTPUT

cat <<EOF

GtkMathView ${MATHVIEW_VERSION}

Compilation

  GCC pipe            ${enable_pipe}
  Profiling           ${enable_profile}
  Debugging           ${enable_debug}
  xsltproc            ${HAVE_XSLTPROC}
  popt                ${have_popt}

Engine

  MathML              yes
  BoxML               ${enable_boxml}

Frontend

  GMetaDOM            ${enable_gmetadom}
  libxml2             ${enable_libxml2}
  libxml2 reader      ${enable_libxml2_reader}
  custom reader       ${enable_custom_reader}

Backend

  GTK+                ${have_gtk}
  Type1 fonts (t1lib) ${have_t1lib}
  TFM support level   ${enable_tfm}
  SVG                 ${enable_svg}
  PostScript          ${enable_ps}

Performance

  Builder cache       ${enable_builder_cache}

EOF
