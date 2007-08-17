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

#ifndef __gtkmathview_decorators_common_h__
#define __gtkmathview_decorators_common_h__

#include "gtkmathview_common.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

  typedef struct _GtkMathViewDefaultCursorDecorator GtkMathViewDefaultCursorDecorator;

  GtkMathViewDefaultCursorDecorator* GTKMATHVIEW_METHOD_NAME(decor_default_cursor_new)(GtkMathView*);
  void GTKMATHVIEW_METHOD_NAME(decor_default_cursor_free)(GtkMathViewDefaultCursorDecorator*);
  void GTKMATHVIEW_METHOD_NAME(decor_default_cursor_set)(GtkMathViewDefaultCursorDecorator*,
							 gboolean, GtkMathViewModelId, gboolean,
							 gint, gboolean);
  void GTKMATHVIEW_METHOD_NAME(decor_default_cursor_get)(GtkMathViewDefaultCursorDecorator*,
							 gboolean*, GtkMathViewModelId*, gboolean*,
							 gint*, gboolean*);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __gtkmathview_decorators_common_h__ */
