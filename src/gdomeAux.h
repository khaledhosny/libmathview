// Copyright (C) 2000-2001, Luca Padovani <luca.padovani@cs.unibo.it>.
//
// This file is part of GtkMathView, a Gtk widget for MathML.
// 
// GtkMathView is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// GtkMathView is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GtkMathView; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
// 
// For details, see the GtkMathView World-Wide-Web page,
// http://www.cs.unibo.it/helm/mml-widget, or send a mail to
// <luca.padovani@cs.unibo.it>

#ifndef gdomeAux_h
#define gdomeAux_h

#include <glib.h>
#include <gdome.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

const gchar*    gdome_str_c(GdomeDOMString*);
gboolean        gdome_str_equal_c(GdomeDOMString*, const gchar*);
gboolean        gdome_str_isEmpty(GdomeDOMString*);

gboolean        gdome_n_name_is(GdomeNode*, const gchar*);
GdomeNode*      gdome_n_nextSibling_unref(GdomeNode*);
GdomeDOMString* gdome_n_content(GdomeNode*);
gboolean        gdome_n_isBlank(GdomeNode*);
gboolean        gdome_n_isFirst(GdomeNode*);
gboolean        gdome_n_isLast(GdomeNode*);
void            gdome_n_setUserData(GdomeNode*, gpointer);
gpointer        gdome_n_getUserData(GdomeNode*);

gboolean        gdome_el_hasAttribute_c(GdomeElement*, const gchar*);
GdomeDOMString* gdome_el_getAttribute_c(GdomeElement*, const gchar*);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // gdomeAux_h
