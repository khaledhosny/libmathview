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

#ifndef MathMLMultiScriptsElement_hh
#define MathMLMultiScriptsElement_hh

#include <vector>

#if defined(HAVE_GMETADOM)
#include "gmetadom.hh"
#endif

#include "MathMLEmbellishment.hh"
#include "MathMLContainerElement.hh"
#include "MathMLScriptCommonElement.hh"

class MathMLMultiScriptsElement
  : public MathMLContainerElement, public MathMLScriptCommonElement, public MathMLEmbellishment
{
protected:
  MathMLMultiScriptsElement(void);
#if defined(HAVE_GMETADOM)
  MathMLMultiScriptsElement(const DOM::Element&);
#endif
  virtual ~MathMLMultiScriptsElement();

public:
  unsigned GetScriptsSize(void) const { return subScript.size(); }
  void     SetScriptsSize(unsigned);
  unsigned GetPreScriptsSize(void) const { return preSubScript.size(); }
  void     SetPreScriptsSize(unsigned);
  void     SetBase(const Ptr<MathMLElement>&);
  void     SetSubScript(unsigned, const Ptr<MathMLElement>&);
  void     SetSuperScript(unsigned, const Ptr<MathMLElement>&);
  void     SetPreSubScript(unsigned, const Ptr<MathMLElement>&);
  void     SetPreSuperScript(unsigned, const Ptr<MathMLElement>&);
  Ptr<MathMLElement> GetBase(void) const { return base; }
  Ptr<MathMLElement> GetSubScript(unsigned) const;
  Ptr<MathMLElement> GetSuperScript(unsigned) const;
  Ptr<MathMLElement> GetPreSubScript(unsigned) const;
  Ptr<MathMLElement> GetPreSuperScript(unsigned) const;

  static Ptr<MathMLElement> create(void)
  { return Ptr<MathMLElement>(new MathMLMultiScriptsElement()); }
#if defined(HAVE_GMETADOM)
  static Ptr<MathMLElement> create(const DOM::Element& el)
  { return Ptr<MathMLElement>(new MathMLMultiScriptsElement(el)); }
#endif

  virtual void Normalize(const Ptr<class MathMLDocument>&);
  virtual void Setup(class RenderingEnvironment&);
  virtual void DoLayout(const class FormattingContext&);
  virtual void SetPosition(scaled, scaled);
  virtual void Render(const class DrawingArea&);
  virtual void ReleaseGCs(void);

#if 0
  virtual void SetDirty(const Rectangle* = 0);
  virtual void SetDirtyLayout(bool = false);
#endif
  virtual void SetFlagDown(Flags);
  virtual void ResetFlagDown(Flags);
  virtual scaled GetLeftEdge(void) const;
  virtual scaled GetRightEdge(void) const;
  virtual Ptr<class MathMLOperatorElement> GetCoreOperator(void);
  virtual Ptr<MathMLElement> Inside(scaled, scaled);
  virtual void Replace(const Ptr<MathMLElement>&, const Ptr<MathMLElement>&);

private:
  std::vector< Ptr<MathMLElement> > subScript;
  std::vector< Ptr<MathMLElement> > superScript;
  std::vector< Ptr<MathMLElement> > preSubScript;
  std::vector< Ptr<MathMLElement> > preSuperScript;

  scaled subShiftX;
  scaled subShiftY;

  scaled superShiftX;
  scaled superShiftY;
};

#endif // MathMLMultiScriptsElement_hh
