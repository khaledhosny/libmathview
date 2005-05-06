/*
 * Copyright (C) 2000-2005, Luca Padovani <luca.padovani@cs.unibo.it>.
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
 * http: *helm.cs.unibo.it/mml-widget/, or send a mail to
 * <lpadovan@cs.unibo.it>
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
