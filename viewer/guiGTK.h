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

#ifndef guiGTK_h
#define guiGTK_h

#include <glib.h>

#ifdef __cplusplus
extern "C" {
#endif

  /* initGUI: some initialization stuff, creates the main window, sets it with a title */
  void GUI_init(int*, const char***, char*, guint, guint, gint);
  void GUI_uninit(void);

  int  GUI_load_document(const char*);
  void GUI_unload_document(void);
  void GUI_dump_entities(void);

  /* main: this is the main event loop, to be called when the program is ready to run */
  void GUI_run(void);

#ifdef __cplusplus
}
#endif

#endif /* guiGTK_h */
