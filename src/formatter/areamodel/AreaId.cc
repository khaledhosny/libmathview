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

#include <config.h>

#include "AreaId.hh"

AreaId::AreaId()
{
}

AreaId::AreaId(unsigned head)
{
  if (head > 0) path.push_back(head - 1);
}

AreaId::AreaId(unsigned head, const AreaId& tail)
{
  if (head == 0)
    throw InvalidAreaId();
  else
    {
      path.reserve(tail.path.size() + 1);
      path.push_back(head - 1);
      path.insert(path.end(), tail.path.begin(), tail.path.end());
    }
}

AreaId::AreaId(const_iterator begin, const_iterator end) : path(begin, end)
{
}

unsigned
AreaId::head() const
{
  if (path.empty())
    throw InvalidAreaId();
  else
    return path[0] + 1;
}

AreaId
AreaId::tail() const
{
  if (path.empty())
    throw InvalidAreaId();
  else
    return AreaId(path.begin() + 1, path.end());
}
