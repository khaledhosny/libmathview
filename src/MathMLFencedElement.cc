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

#include <config.h>
#include <assert.h>
#include <stddef.h>

#include "stringAux.hh"
#include "allocTextNode.hh"
#include "StringUnicode.hh"
#include "MathMLCharNode.hh"
#include "MathMLRowElement.hh"
#include "MathMLStringNode.hh"
#include "MathMLFencedElement.hh"
#include "MathMLOperatorElement.hh"

#if defined(HAVE_MINIDOM)
MathMLFencedElement::MathMLFencedElement(mDOMNodeRef node)
#elif defined(HAVE_GMETADOM)
MathMLFencedElement::MathMLFencedElement(const GMetaDOM::Element& node)
#endif
  : MathMLNormalizingContainerElement(node, TAG_MFENCED)
{
  normalized = false;
  openFence = closeFence = separators = NULL;
}

MathMLFencedElement::~MathMLFencedElement()
{
  delete openFence;
  delete closeFence;
  delete separators;
}

const AttributeSignature*
MathMLFencedElement::GetAttributeSignature(AttributeId id) const
{
  static AttributeSignature sig[] = {
    { ATTR_OPEN,       fenceParser,      new StringC("("), NULL },
    { ATTR_CLOSE,      fenceParser,      new StringC(")"), NULL },
    { ATTR_SEPARATORS, separatorsParser, new StringC(","), NULL },
    { ATTR_NOTVALID,   NULL,             NULL,             NULL }
  };

  const AttributeSignature* signature = GetAttributeSignatureAux(id, sig);
  if (signature == NULL) signature = MathMLNormalizingContainerElement::GetAttributeSignature(id);

  return signature;
}

void
MathMLFencedElement::Setup(RenderingEnvironment* env)
{
  assert(env != NULL);

  const Value* value = NULL;

  value = GetAttributeValue(ATTR_OPEN, env);
  if (value != NULL && value->ToString() != NULL) openFence = value->ToString()->Clone();
  else openFence = NULL;
  delete value;

  value = GetAttributeValue(ATTR_CLOSE, env);
  if (value != NULL && value->ToString() != NULL) closeFence = value->ToString()->Clone();
  else closeFence = NULL;
  delete value;

  value = GetAttributeValue(ATTR_SEPARATORS, env);
  if (value != NULL && value->ToString() != NULL) separators = value->ToString()->Clone();
  else separators = NULL;
  delete value;

  if (!normalized) {
    NormalizeFencedElement();
    normalized = true;
  }

  MathMLContainerElement::Setup(env);
}

void
MathMLFencedElement::Normalize()
{
  //MathMLContainerElement::Normalize();
}

void
MathMLFencedElement::NormalizeFencedElement()
{
  MathMLRowElement* mainRow = new MathMLRowElement(NULL);
  MathMLRowElement* mrow = NULL;
  MathMLOperatorElement* fence = NULL;

  if (openFence != NULL && openFence->GetLength() > 0) {
    fence = new MathMLOperatorElement(NULL);
    fence->Append(openFence);
    fence->SetFence();
    fence->SetParent(mainRow);
    mainRow->content.Append(fence);
  }

  bool moreArguments = content.GetSize() > 1;

  if (moreArguments) mrow = new MathMLRowElement(NULL);
  else mrow = mainRow;

  unsigned i = 0;
  while (content.GetSize() > 0) {
    MathMLElement* arg = content.RemoveFirst();
    assert(arg != NULL);

    arg->SetParent(mrow);
    mrow->content.Append(arg);

    if (separators != NULL
	&& separators->GetLength() > 0
	&& content.GetSize() > 0) {
      unsigned offset = (i < separators->GetLength()) ? i : separators->GetLength() - 1;
      const String* sep = allocString(*separators, offset, 1);
      assert(sep != NULL);

      MathMLOperatorElement* separator = new MathMLOperatorElement(NULL);
      separator->SetSeparator();
      separator->Append(sep);
      separator->SetParent(mrow);
      mrow->content.Append(separator);
    }

    i++;
  }

  if (moreArguments) {
    mrow->SetParent(mainRow);
    mainRow->content.Append(mrow);
  }

  if (closeFence != NULL && closeFence->GetLength() > 0) {
    fence = new MathMLOperatorElement(NULL);
    fence->Append(closeFence);
    fence->SetFence();
    fence->SetParent(mainRow);
    mainRow->content.Append(fence);
  }

  mainRow->Normalize();

  assert(content.GetSize() == 0);
  mainRow->SetParent(this);
  content.Append(mainRow);
}

bool
MathMLFencedElement::IsBreakable() const
{
  return true;
}
