/* Copyright (C) 2000-2007, Luca Padovani <padovani@sti.uniurb.it>.
 *
 * This file is part of GtkMathView, a flexible, high-quality rendering
 * engine for MathML documents.
 * 
 * GtkMathView is free software; you can redistribute it and/or modify it
 * either under the terms of the GNU Lesser General Public License version
 * 3 as published by the Free Software Foundation (the "LGPL") or, at your
 * option, under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation (the "GPL").  If you do not
 * alter this notice, a recipient may use your version of this file under
 * either the GPL or the LGPL.
 *
 * GtkMathView is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the LGPL or
 * the GPL for more details.
 * 
 * You should have received a copy of the LGPL and of the GPL along with
 * this program in the files COPYING-LGPL-3 and COPYING-GPL-2; if not, see
 * <http://www.gnu.org/licenses/>.
 */

#ifndef Gtk_Window_h
#define Gtk_Window_h

#include <glib.h>

#ifdef __cplusplus
extern "C" {
#endif

  /* initGUI: some initialization stuff, creates the main window, sets it with a title */
  void GUI_init(int*, char***, char*, gint);
  void GUI_uninit(void);

  int  GUI_load_document(const char*);
  void GUI_unload_document(void);
  void GUI_dump_entities(void);

  /* main: this is the main event loop, to be called when the program is ready to run */
  void GUI_run(void);

#ifdef __cplusplus
}
#endif

#endif /* Gtk_Window_h */
