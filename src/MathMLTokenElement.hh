// Copyright (C) 2000-2003, Luca Padovani <luca.padovani@cs.unibo.it>.
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
// http://helm.cs.unibo.it/mml-widget, or send a mail to
// <luca.padovani@cs.unibo.it>

#ifndef MathMLTokenElement_hh
#define MathMLTokenElement_hh

#include <vector>

#include "RGBValue.hh"
#include "FontAttributes.hh"
#include "MathMLContainerElement.hh"

// base class for token element. Token elemens can contain character data
// and a very limited set of other MathML elements (e.g. <malignmark>)
class MathMLTokenElement : public MathMLElement
{
protected:
  MathMLTokenElement(const SmartPtr<class MathMLView>&);
  virtual ~MathMLTokenElement();

public:
  unsigned       GetSize(void) const { return content.size(); }
  void           SetSize(unsigned);
  SmartPtr<class MathMLTextNode> GetChild(unsigned) const;
  void           SetChild(unsigned, const SmartPtr<MathMLTextNode>&);
  void           RemoveChild(unsigned);
  void           InsertChild(unsigned, const SmartPtr<MathMLTextNode>&);
  void           AppendChild(const SmartPtr<class MathMLTextNode>&);
  void           Append(const String&);
  void           SwapChildren(std::vector< SmartPtr<MathMLTextNode> >&);

  virtual void   construct(void);
  virtual void   refine(class AbstractRefinementContext&);
  virtual void 	 Setup(class RenderingEnvironment&);
  virtual void 	 DoLayout(const class FormattingContext&);
  virtual void   SetPosition(const scaled&, const scaled&);
  virtual void 	 Render(const class DrawingArea&);

  bool           IsNonMarking(void) const;

  virtual scaled GetLeftEdge(void) const;
  virtual scaled GetRightEdge(void) const;
  scaled         GetDecimalPointEdge(void) const;

  RGBValue       GetColor(void) const { return color; }

  //virtual SmartPtr<class MathMLCharNode> GetCharNode(void) const;
  const std::vector< SmartPtr<class MathMLTextNode> >& GetContent(void) const { return content; }
  String         GetRawContent(void) const;
  unsigned       GetLogicalContentLength(void) const;

private:
  std::vector< SmartPtr<class MathMLTextNode> > content;

protected:
  void SetContentPosition(const scaled&, const scaled&);

  static SmartPtr<class MathMLTextNode> SubstituteMGlyphElement(const DOM::Element&);
  static SmartPtr<class MathMLTextNode> SubstituteAlignMarkElement(const DOM::Element&);
  
  void AddItalicCorrection(void);

  scaled   sppm;
  RGBValue color;
};

#endif // MathMLTokenElement_hh
