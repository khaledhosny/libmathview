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

#include "String.hh"
#include "unidefs.h"
#include "CharMapper.hh"
#include "allocTextNode.hh"
#include "MathMLTextNode.hh"
#include "MathMLCharNode.hh"
#include "MathMLHorizBarNode.hh"
#include "MathMLCombinedCharNode.hh"
#include "MathMLApplyFunctionNode.hh"
#include "MathMLInvisibleTimesNode.hh"
#include "MathMLStringNode.hh"

MathMLTextNode*
allocCharNode(Char ch)
{
  switch (ch) {
  case U_APPLYFUNCTION:
    return new MathMLApplyFunctionNode();
   
  case U_INVISIBLETIMES:
    return new MathMLInvisibleTimesNode();

  case U_UNDERLINE:
  case U_OVERLINE:
    return new MathMLHorizBarNode(ch);

  default:
    return new MathMLCharNode(ch);
  }
}

MathMLTextNode*
allocCombinedCharNode(Char ch, Char cch)
{
  return new MathMLCombinedCharNode(ch, cch);
}

MathMLTextNode*
allocTextNode(const String** str)
{
  assert(str != NULL);
  assert(*str != NULL);

  const String* s = *str;

  unsigned len = s->GetLength();

  MathMLTextNode* node = NULL;

  if (len == 1) {
    node = allocCharNode(s->GetChar(0));
    delete s;
    *str = NULL;
  } else if (len > 1) node = new MathMLStringNode(s);

  return node;
}
