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

#ifndef MathMLPaddedElement_hh
#define MathMLPaddedElement_hh

#if defined(HAVE_GMETADOM)
#include "gmetadom.hh"
#endif

#include "MathMLEmbellishment.hh"
#include "MathMLNormalizingContainerElement.hh"

class MathMLPaddedElement
  : public MathMLNormalizingContainerElement, public MathMLEmbellishment
{
protected:
  MathMLPaddedElement(void);
#if defined(HAVE_GMETADOM)
  MathMLPaddedElement(const DOM::Element&);
#endif
  virtual ~MathMLPaddedElement();

public:
  static Ptr<MathMLElement> create(void)
  { return Ptr<MathMLElement>(new MathMLPaddedElement()); }
#if defined(HAVE_GMETADOM)
  static Ptr<MathMLElement> create(const DOM::Element& el)
  { return Ptr<MathMLElement>(new MathMLPaddedElement(el)); }
#endif

  virtual const AttributeSignature* GetAttributeSignature(AttributeId) const;
  //virtual void Normalize(const Ptr<MathMLDocument>&);
  virtual void Setup(RenderingEnvironment&);
  virtual void DoLayout(const class FormattingContext&);
  virtual void SetPosition(scaled, scaled);
  //virtual void SetDirty(const Rectangle* = NULL);
  virtual Ptr<class MathMLOperatorElement> GetCoreOperator(void);

private:
  struct LengthDimension {
    bool      valid;        // true if this attribute was set
    int       sign;         // -1 => -, 0 => no sign specified, +1 => +
    float     number;       // the (unsigned) number specified
    bool      percentage;   // true if % was set (=> pseudo == true)
    bool      pseudo;       // true if pseudo unit used
    KeywordId pseudoUnitId; // pseudo-unit (valid if pseudo == true)
    scaled    unit;         // standard unit (valid if pseudo == false)
  };

  void   ParseLengthDimension(RenderingEnvironment&, const Value*, LengthDimension&, KeywordId);
  scaled EvalLengthDimension(scaled, const LengthDimension&, const BoundingBox&) const;

  LengthDimension width;
  LengthDimension lSpace;
  LengthDimension height;
  LengthDimension depth;

  scaled lSpaceE; // evaluated
};

#endif // MathMLPaddedElement_hh
