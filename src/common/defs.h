/* Copyright (C) 2000-2005, Luca Padovani <luca.padovani@cs.unibo.it>.
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

#ifndef __defs_h__
#define __defs_h__

#define MATHML_NS_URI "http://www.w3.org/1998/Math/MathML"
#define BOXML_NS_URI "http://helm.cs.unibo.it/2003/BoxML"

#define EPSILON 0.01f

#define DEFAULT_FONT_SIZE         12
#define DEFAULT_LINK_FOREGROUND   RGBColor(50, 100, 139)
#define DEFAULT_LINK_BACKGROUND   RGBColor::WHITE()
#define DEFAULT_SELECT_FOREGROUND RGBColor::BLACK()
#define DEFAULT_SELECT_BACKGROUND RGBColor(135, 206, 250)
#define DEFAULT_FOREGROUND        RGBColor::BLACK()
#define DEFAULT_BACKGROUND        RGBColor::WHITE()

#endif /* __defs_h__ */
