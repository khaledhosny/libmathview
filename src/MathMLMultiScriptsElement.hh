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

#ifndef MathMLMultiScriptsElement_hh
#define MathMLMultiScriptsElement_hh

#include <vector>

#include "MathMLEmbellishment.hh"
#include "MathMLContainerElement.hh"
#include "MathMLScriptCommonElement.hh"

class MathMLMultiScriptsElement
  : public MathMLContainerElement, public MathMLScriptCommonElement, public MathMLEmbellishment
{
protected:
  MathMLMultiScriptsElement(const SmartPtr<class MathMLView>&);
  virtual ~MathMLMultiScriptsElement();

public:
  static SmartPtr<MathMLMultiScriptsElement> create(const SmartPtr<class MathMLView>& view)
  { return new MathMLMultiScriptsElement(view); }

  unsigned GetScriptsSize(void) const { return subScript.size(); }
  void     SetScriptsSize(unsigned);
  unsigned GetPreScriptsSize(void) const { return preSubScript.size(); }
  void     SetPreScriptsSize(unsigned);
  void     SetBase(const SmartPtr<MathMLElement>&);
  void     SetSubScript(unsigned, const SmartPtr<MathMLElement>&);
  void     SetSuperScript(unsigned, const SmartPtr<MathMLElement>&);
  void     SetPreSubScript(unsigned, const SmartPtr<MathMLElement>&);
  void     SetPreSuperScript(unsigned, const SmartPtr<MathMLElement>&);
  SmartPtr<MathMLElement> GetBase(void) const { return base; }
  SmartPtr<MathMLElement> GetSubScript(unsigned) const;
  SmartPtr<MathMLElement> GetSuperScript(unsigned) const;
  SmartPtr<MathMLElement> GetPreSubScript(unsigned) const;
  SmartPtr<MathMLElement> GetPreSuperScript(unsigned) const;

  virtual void construct(void);
#if 0
  virtual void Setup(class RenderingEnvironment&);
  virtual void DoLayout(const class FormattingContext&);
  virtual void SetPosition(const scaled&, const scaled&);
  virtual void Render(const class DrawingArea&);
  virtual void ReleaseGCs(void);
#endif

  virtual void SetFlagDown(Flags);
  virtual void ResetFlagDown(Flags);
#if 0
  virtual scaled GetLeftEdge(void) const;
  virtual scaled GetRightEdge(void) const;
#endif
  virtual SmartPtr<class MathMLOperatorElement> GetCoreOperator(void);
#if 0
  virtual SmartPtr<MathMLElement> Inside(const scaled&, const scaled&);
#endif
  virtual void Replace(const SmartPtr<MathMLElement>&, const SmartPtr<MathMLElement>&);

private:
  std::vector< SmartPtr<MathMLElement> > subScript;
  std::vector< SmartPtr<MathMLElement> > superScript;
  std::vector< SmartPtr<MathMLElement> > preSubScript;
  std::vector< SmartPtr<MathMLElement> > preSuperScript;

  scaled subShiftX;
  scaled subShiftY;

  scaled superShiftX;
  scaled superShiftY;
};

#endif // MathMLMultiScriptsElement_hh
