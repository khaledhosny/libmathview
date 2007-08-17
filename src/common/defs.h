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

#ifndef __defs_h__
#define __defs_h__

#define MATHML_NS_URI "http://www.w3.org/1998/Math/MathML"
#define XLINK_NS_URI "http://www.w3.org/1999/xlink"
#define SVG_NS_URI "http://www.w3.org/2000/svg"
#define BOXML_NS_URI "http://helm.cs.unibo.it/2003/BoxML"
#define GMV_NS_URI "http://helm.cs.unibo.it/2005/GtkMathView"

#define EPSILON 0.01f

#define DEFAULT_FONT_SIZE         12
#define DEFAULT_LINK_FOREGROUND   RGBColor(50, 100, 139)
#define DEFAULT_LINK_BACKGROUND   RGBColor::WHITE()
#define DEFAULT_SELECT_FOREGROUND RGBColor::BLACK()
#define DEFAULT_SELECT_BACKGROUND RGBColor(135, 206, 250)
#define DEFAULT_FOREGROUND        RGBColor::BLACK()
#define DEFAULT_BACKGROUND        RGBColor::WHITE()

#endif /* __defs_h__ */
