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

#include "scaled.hh"
#include "Logger.hh"
#include "RGBValue.hh"
#include "Configuration.hh"
#include "EntitiesTable.hh"
#include "OperatorDictionary.hh"

class MathEngine {
public:
  MathEngine(void);
  ~MathEngine();

  void  Init(class DrawingArea*, class FontManager*);

  bool  Load(const char*);
  void  Unload(void);

  void  Setup(void);

  void  Layout(void);
  void  SetDirty(const struct Rectangle* = NULL);
  void  Render(void);
  void  Render(const struct Rectangle*);
  void  Update(const struct Rectangle* = NULL);

  bool  IsOnLink(scaled, scaled) const;

  void                 SetElement(class MathMLElement*);
  class MathMLElement* GetElement(void) const { return element; }

  void                 SetSelectionFirst(class MathMLElement*);
  void                 SetSelectionLast(class MathMLElement*);
  class MathMLElement* GetSelectionRoot(void) const { return selectionRoot; }
  void                 ResetSelectionRoot(void);
  void                 SetSelection(class MathMLElement*);
  class MathMLElement* GetSelection(void) const { return selection; }

  class MathMLElement* GetRoot(void) const { return root; }
  class MathMLElement* GetElementAt(scaled, scaled) const;

  void GetDocumentBoundingBox(class BoundingBox&) const;
  void GetDocumentRectangle(class Rectangle&) const;

  void     SetDefaultFontSize(unsigned);
  unsigned GetDefaultFontSize(void) const { return defaultFontSize; }

  void  SetVerbosity(int);
  int   GetVerbosity(void) const;

  void  SetAntiAliasing(bool);
  bool  GetAntiAliasing(void) const { return antiAliasing; }

  void        SetKerning(bool);
  static bool GetKerning(void) { return kerning; }

  static void InitGlobalData(const char*);

  static EntitiesTable      entitiesTable;
  static OperatorDictionary dictionary;
  static Configuration      configuration;
  static Logger             logger;

private:
  static class MathMLElement* SelectMinimumTree(class MathMLElement*, class MathMLElement*);

  unsigned    defaultFontSize;
  bool        antiAliasing;
  static bool kerning;


  class MathMLDocument* document;
  class MathMLElement*  root;

  class MathMLElement*  element; // the element the pointer is on

  class MathMLElement*  selectionFirst;
  class MathMLElement*  selectionRoot;
  class MathMLElement*  selection;

  class DrawingArea* area;
  class FontManager* fontManager;
  class CharMapper*  charMapper;
};

#endif // MathEngine_hh
