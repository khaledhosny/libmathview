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

#include "RGBValue.hh"
#include "FontAttributes.hh"
#include "MathMLContainerElement.hh"

// base class for token element. Token elemens can contain character data
// and a very limited set of other MathML elements (e.g. <malignmark>)
class MathMLTokenElement : public MathMLElement {
public:
  MathMLTokenElement(mDOMNodeRef, TagId t = TAG_NOTVALID);
  virtual const AttributeSignature* GetAttributeSignature(AttributeId) const;
  virtual void 	 Setup(class RenderingEnvironment*);
  virtual void 	 DoLayout(LayoutId, class Layout&);
  virtual void 	 Freeze(void);
  virtual void 	 Render(const class DrawingArea&);
  virtual ~MathMLTokenElement();

  virtual void   Append(const String*);
  void           Append(class MathMLTextNode*);

  virtual bool   IsLast(void) const;
  virtual bool 	 IsToken(void) const;
  virtual bool 	 IsBreakable(void) const;
  bool           IsNonMarking(void) const;
  virtual void 	 GetLinearBoundingBox(BoundingBox&) const;
  virtual void 	 SetDirty(const Rectangle* = NULL);
  virtual BreakId GetBreakability(void) const;

  virtual scaled GetLeftEdge(void) const;
  virtual scaled GetRightEdge(void) const;
  scaled         GetDecimalPointEdge(void) const;

  RGBValue       GetColor(void) const { return color; }
  virtual const class MathMLCharNode* GetCharNode(void) const;

  const Container<class MathMLTextNode*>& GetContent(void) const { return content; }
  unsigned       GetRawContentLength(void) const { return rawContentLength; }

protected:
  void Free(void);
  void AddItalicCorrection(Layout&);

  // for tokens the content is protected so that users have to
  // use the Append methods. For read-only operations there is
  // the access method GetContent
  Container<class MathMLTextNode*> content;
  unsigned rawContentLength;
  scaled   sppm;
  RGBValue color;
};

typedef MathMLTokenElement* MathMLTokenElementPtr;

#define TO_TOKEN(object) (dynamic_cast<MathMLTokenElement*>(object))
#define TO_CONST_TOKEN(object) (dynamic_cast<const MathMLTokenElement*>(object))

#endif // MathMLTokenElement_hh
