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
// http://cs.unibo.it/~lpadovan/mml-widget, or send a mail to
// <luca.padovani@cs.unibo.it>

#ifndef MathMLTokenElement_hh
#define MathMLTokenElement_hh

#if defined(HAVE_GMETADOM)
#include "gmetadom.hh"
#endif

#include <vector>

#include "RGBValue.hh"
#include "FontAttributes.hh"
#include "MathMLContainerElement.hh"

// base class for token element. Token elemens can contain character data
// and a very limited set of other MathML elements (e.g. <malignmark>)
class MathMLTokenElement : public MathMLElement
{
protected:
  MathMLTokenElement(void);
#if defined(HAVE_GMETADOM)
  MathMLTokenElement(const DOM::Element&);
#endif
  virtual ~MathMLTokenElement();

public:
  static Ptr<MathMLElement> create(void)
  { return Ptr<MathMLElement>(new MathMLTokenElement()); }
#if defined(HAVE_GMETADOM)
  static Ptr<MathMLElement> create(const DOM::Element& el)
  { return Ptr<MathMLElement>(new MathMLTokenElement(el)); }
#endif

  unsigned       GetSize(void) const { return content.size(); }
  void           SetSize(unsigned);
  Ptr<class MathMLTextNode> GetChild(unsigned) const;
  void           SetChild(unsigned, const Ptr<MathMLTextNode>&);
  void           RemoveChild(unsigned);
  void           InsertChild(unsigned, const Ptr<MathMLTextNode>&);
  void           AppendChild(const Ptr<class MathMLTextNode>&);
  void           Append(const String*);
  void           SwapChildren(std::vector< Ptr<MathMLTextNode> >&);

  virtual const AttributeSignature* GetAttributeSignature(AttributeId) const;
  virtual void   Normalize(const Ptr<class MathMLDocument>&);
  virtual void 	 Setup(class RenderingEnvironment&);
  virtual void 	 DoLayout(const class FormattingContext&);
  virtual void   SetPosition(scaled, scaled);
  virtual void 	 Render(const class DrawingArea&);

  bool           IsNonMarking(void) const;

  virtual scaled GetLeftEdge(void) const;
  virtual scaled GetRightEdge(void) const;
  scaled         GetDecimalPointEdge(void) const;

  RGBValue       GetColor(void) const { return color; }

  virtual Ptr<class MathMLCharNode> GetCharNode(void) const;
  const std::vector< Ptr<class MathMLTextNode> >& GetContent(void) const { return content; }
  String*        GetRawContent(void) const;
  unsigned       GetLogicalContentLength(void) const;

private:
  std::vector< Ptr<class MathMLTextNode> > content;

protected:
  void SetContentPosition(scaled, scaled);

  static Ptr<class MathMLTextNode> SubstituteMGlyphElement(const DOM::Element&);
  static Ptr<class MathMLTextNode> SubstituteAlignMarkElement(const DOM::Element&);
  
  void AddItalicCorrection(void);

  scaled   sppm;
  RGBValue color;
};

#endif // MathMLTokenElement_hh
