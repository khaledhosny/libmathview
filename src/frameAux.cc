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

#include <algorithm>

#include <assert.h>

#include "FormattingContext.hh"
#include "MathMLContainerElement.hh"
#include "MathMLFrame.hh"
#include "MathMLRowElement.hh"
#include "MathMLTextNode.hh"
#include "MathMLTokenElement.hh"
#include "frameAux.hh"

const BoundingBox&
getFrameBoundingBox(const SmartPtr<MathMLFrame>& frame)
{
  assert(false);
#if 0
  assert(frame != SmartPtr<MathMLFrame>(0));
  return frame->GetBoundingBox();
#endif
}

static SmartPtr<MathMLTextNode>
getLeftSibling(const SmartPtr<MathMLTextNode>& node)
{
  assert(node);
  assert(node->GetParent());
  assert(is_a<MathMLTokenElement>(node->GetParent()));

  SmartPtr<MathMLTokenElement> token = smart_cast<MathMLTokenElement>(node->GetParent());
  assert(token);
  std::vector< SmartPtr<MathMLTextNode> >::const_iterator p = std::find(token->GetContent().begin(),
								   token->GetContent().end(),
								   node);
  assert(p != token->GetContent().end());
  if (p != token->GetContent().begin()) return *(p - 1);
  else return 0;
}

static SmartPtr<MathMLElement>
getLeftSibling(const SmartPtr<MathMLElement>& elem)
{
  assert(elem);
  assert(elem->GetParent());
  assert(is_a<MathMLRowElement>(elem->GetParent()));
  SmartPtr<MathMLRowElement> row = smart_cast<MathMLRowElement>(elem->GetParent());
  assert(row);
  std::vector< SmartPtr<MathMLElement> >::const_iterator p = std::find(row->GetContent().begin(),
								  row->GetContent().end(),
								  elem);
  assert(p != row->GetContent().end());
  if (p != row->GetContent().begin()) return *(p - 1);
  else return 0;
}

SmartPtr<MathMLFrame>
getLeftSibling(const SmartPtr<MathMLFrame>& frame)
{
  assert(frame);
  if (is_a<MathMLTextNode>(frame)) return getLeftSibling(smart_cast<MathMLTextNode>(frame));
  else if (is_a<MathMLElement>(frame)) return getLeftSibling(smart_cast<MathMLElement>(frame));
  else
    {
      assert(0);
      return 0;
    }
}

static SmartPtr<MathMLTextNode>
getRightSibling(const SmartPtr<MathMLTextNode>& node)
{
  assert(node);
  assert(node->GetParent());
  assert(is_a<MathMLTokenElement>(node->GetParent()));

  SmartPtr<MathMLTokenElement> token = smart_cast<MathMLTokenElement>(node->GetParent());
  assert(token);
  std::vector< SmartPtr<MathMLTextNode> >::const_iterator p = std::find(token->GetContent().begin(),
								   token->GetContent().end(),
								   node);
  assert(p != token->GetContent().end());
  if (p != token->GetContent().end() - 1) return *(p + 1);
  else return 0;
}

static SmartPtr<MathMLElement>
getRightSibling(const SmartPtr<MathMLElement>& elem)
{
  assert(elem);
  assert(elem->GetParent());
  assert(is_a<MathMLRowElement>(elem->GetParent()));
  SmartPtr<MathMLRowElement> row = smart_cast<MathMLRowElement>(elem->GetParent());
  assert(row);
  std::vector< SmartPtr<MathMLElement> >::const_iterator p = std::find(row->GetContent().begin(),
								  row->GetContent().end(),
								  elem);
  assert(p != row->GetContent().end());
  if (p != row->GetContent().end() - 1) return *(p + 1);
  else return 0;
}

SmartPtr<MathMLFrame>
getRightSibling(const SmartPtr<MathMLFrame>& frame)
{
  assert(frame);
  if (is_a<MathMLTextNode>(frame)) return getRightSibling(smart_cast<MathMLTextNode>(frame));
  else if (is_a<MathMLElement>(frame)) return getRightSibling(smart_cast<MathMLElement>(frame));
  else
    {
      assert(0);
      return 0;
    }
}

