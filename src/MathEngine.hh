// Copyright (C) 2000, Luca Padovani <luca.padovani@cs.unibo.it>.
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
// http://cs.unibo.it/~lpadovan/mml-widget/, or send a mail to
// <luca.padovani@cs.unibo.it>

#ifndef MathEngine_hh
#define MathEngine_hh

#include <gdome.h>

#include "scaled.hh"
#include "Logger.hh"
#include "RGBValue.hh"
#include "Configuration.hh"
#include "OperatorDictionary.hh"

class MathEngine {
public:
  MathEngine(void);
  ~MathEngine();

  void  Init(class DrawingArea*, class FontManager*);

  bool  Load(const char*);
  bool  Load(GdomeDocument*);
  void  Unload(void);

  void  Setup(void);

  void  MinMaxLayout(void);
  void  Layout(void);
  void  SetDirty(const struct Rectangle* = NULL);
  void  Render(void);
  void  Render(const struct Rectangle*);
  void  Update(const struct Rectangle* = NULL);

  bool  IsOnLink(scaled, scaled) const;

  class MathMLElement* GetRoot(void) const { return root; }
  class MathMLElement* GetElementAt(scaled, scaled) const;

  void                 SetSelected(class MathMLElement*);
  class MathMLElement* GetSelected(void) const { return selected; }

  void GetDocumentBoundingBox(class BoundingBox&) const;
  void GetDocumentRectangle(class Rectangle&) const;

  void     SetDefaultFontSize(unsigned);
  unsigned GetDefaultFontSize(void) const { return defaultFontSize; }

  void  SetVerbosity(int);
  int   GetVerbosity(void) const;

  // T1 specific methods (they produce a warning when used with a
  // GTK drawing area)
  void SetAntiAliasing(bool);
  bool GetAntiAliasing(void) const;
  void SetKerning(bool);
  bool GetKerning(void) const;
  void SetTransparency(bool);
  bool GetTransparency(void) const;

  static void InitGlobalData(const char*);

#if 0
  static EntitiesTable      entitiesTable;
#endif
  static OperatorDictionary dictionary;
  static Configuration      configuration;
  static Logger             logger;

private:
  unsigned defaultFontSize;

  class MathMLDocument* document;
  class MathMLElement*  root;
  class MathMLElement*  selected;

  class DrawingArea* area;
  class FontManager* fontManager;
  class CharMapper*  charMapper;
};

#endif // MathEngine_hh
