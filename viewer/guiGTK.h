/*
 * Copyright (C) 2000, Luca Padovani <luca.padovani@cs.unibo.it>.
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
 * http://cs.unibo.it/~lpadovan/mml-widget, or send a mail to
 * <luca.padovani@cs.unibo.it>
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
