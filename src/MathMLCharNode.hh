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

#ifndef MathMLCharNode_hh
#define MathMLCharNode_hh

#include "CharMap.hh"
#include "MathMLTextNode.hh"

class MathMLCharNode: public MathMLTextNode
{
protected:
  MathMLCharNode(Char);
  virtual ~MathMLCharNode();

public:
  static Ptr<MathMLCharNode> create(Char c)
  { return Ptr<MathMLCharNode>(new MathMLCharNode(c)); }

  virtual void 	   Setup(class RenderingEnvironment&);
  virtual void 	   DoLayout(const class FormattingContext&);
  virtual void 	   Render(const DrawingArea&);

  Char             GetChar(void) const { return ch; }
  virtual String*  GetRawContent(void) const;
  virtual unsigned GetLogicalContentLength(void) const;

  virtual bool     IsStretchyChar(void) const;
  virtual bool     HasDecimalPoint(void) const;
  virtual scaled   GetDecimalPointEdge(void) const;

  bool             IsFontified(void) const;
  bool             IsStretchyFontified(void) const;

  virtual void     SetDefaultLargeGlyph(bool);
  virtual void     DoVerticalStretchyLayout(scaled, scaled, scaled, bool);
  virtual void     DoHorizontalStretchyLayout(scaled, bool);
  virtual StretchId GetStretch(void) const;
  const AFont*     GetFont(void) const { return fChar.font; }

  bool             CombineWith(const Ptr<MathMLCharNode>&, scaled&, scaled&) const;

protected:
  void DoVerticalStretchyLayoutAux(scaled, bool);
  void DoHorizontalStretchyLayoutAux(scaled, bool);
  void RenderVerticalStretchyChar(const DrawingArea&, const GraphicsContext*, scaled, scaled);
  void RenderHorizontalStretchyChar(const DrawingArea&, const GraphicsContext*, scaled, scaled);
  void RenderMissingCharacter(const DrawingArea&, const GraphicsContext*);

  FontifiedChar fChar;
  Char          ch;

  BoundingBox   charBox; // this may differ from the final box

  struct StretchyCharLayout {
    FontifiedChar sChar;
    char simple; // if == NULLCHAR then charBox has no simple char
    unsigned n;  // if == 0 then charBox has no multichar
    BoundingBox box[SC_REPEAT + 1];
  };

  StretchyCharLayout* layout;
};

#endif // MathMLCharNode_hh
