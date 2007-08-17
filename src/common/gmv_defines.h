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

#ifndef __gmv_defines_h__
#define __gmv_defines_h__

#if defined(WIN32)

/*
src/Makefile.am:lib_LTLIBRARIES = libmathview.la
   SUBLIB: src/backend/common/libbackend_common.la
   SUBLIB: src/backend/common/tfm/libtfm.la
   SUBLIB: src/frontend/common/libfrontend_common.la
   SUBLIB: src/engine/mathml/libmathml.la
   SUBLIB: src/engine/boxml/libboxml.la
   SUBLIB: src/engine/adapters/libadapters.la
   SUBLIB: src/engine/common/libengine_common.la
   SUBLIB: src/common/libcommon.la
   SUBLIB: src/common/mathvariants/libmathvariants.la
*/
#if defined(GMV_MathView_DLL) 
#define GMV_MathView_EXPORT __declspec(dllexport)
#else
#define GMV_MathView_EXPORT __declspec(dllimport)
#endif

/*
src/backend/gtk/Makefile.am:lib_LTLIBRARIES = libmathview_backend_gtk.la
src/backend/svg/Makefile.am:lib_LTLIBRARIES = libmathview_backend_svg.la
*/
#if defined(GMV_BackEnd_DLL) 
#define GMV_BackEnd_EXPORT __declspec(dllexport)
#else
#define GMV_BackEnd_EXPORT __declspec(dllimport)
#endif

/*
src/view/Makefile.am:lib_LTLIBRARIES = $(CUSTOM_READER) $(LIBXML2_READER) $(LIBXML2) $(GMETADOM)
 CUSTOM_READER  = libmathview_frontend_custom_reader.la
?? SUBLIB: src/frontend/libxml2_reader/libfrontend_libxml2_reader.la
   SUBLIB: src/frontend/custom_reader/libfrontend_custom_reader.la
  DEPENDS: libmathview.la
 LIBXML2_READER = libmathview_frontend_libxml2_reader.la
   SUBLIB: src/frontend/libxml2_reader/libfrontend_libxml2_reader.la
  DEPENDS: libmathview.la
 LIBXML2        = libmathview_frontend_libxml2.la
   SUBLIB: src/frontend/libxml2/libfrontend_libxml2.la
  DEPENDS: libmathview.la
 GMETADOM       = libmathview_frontend_gmetadom.la
   SUBLIB: src/frontend/gmetadom/libfrontend_gmetadom.la
  DEPENDS: libmathview.la
*/
#if defined(GMV_FrontEnd_DLL) 
#define GMV_FrontEnd_EXPORT __declspec(dllexport)
#else
#define GMV_FrontEnd_EXPORT __declspec(dllimport)
#endif

/*
src/widget/Makefile.am:lib_LTLIBRARIES = $(CUSTOM_READER) $(LIBXML2_READER) $(LIBXML2) $(GMETADOM)
 CUSTOM_READER  = libgtkmathview_custom_reader.la
  DEPENDS: libmathview_backend_gtk.la
  DEPENDS: libmathview_frontend_custom_reader.la
 LIBXML2_READER = libgtkmathview_libxml2_reader.la
  DEPENDS: libmathview_backend_gtk.la
  DEPENDS: libmathview_frontend_libxml2_reader.la
 LIBXML2        = libgtkmathview_libxml2.la
  DEPENDS: libmathview_backend_gtk.la
  DEPENDS: libmathview_frontend_libxml2.la
 GMETADOM       = libgtkmathview_gmetadom.la
  DEPENDS: libmathview_backend_gtk.la
  DEPENDS: libmathview_frontend_gmetadom.la
*/
#if defined(GMV_Widget_DLL) 
#define GMV_Widget_EXPORT __declspec(dllexport)
#else
#define GMV_Widget_EXPORT __declspec(dllimport)
#endif

#else

#define GMV_MathView_EXPORT
#define GMV_BackEnd_EXPORT
#define GMV_FrontEnd_EXPORT
#define GMV_Widget_EXPORT

#endif /* WIN32 */

#endif /* __gmv_defines_h__ */
