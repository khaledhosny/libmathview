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

#include "ReplacementContext.hh"

ReplacementContext::ReplacementContext()
{ }

ReplacementContext::ReplacementContext(const AreaId& id, const AreaRef& area)
{
  subst.push_back(std::make_pair(id, area));
}

ReplacementContext::ReplacementContext(const ReplacementContext& context, unsigned head)
{
  for (std::list< std::pair<AreaId,AreaRef> >::const_iterator p = context.subst.begin();
       p != context.subst.end();
       p++)
    if (!(*p).first.empty() && (*p).first.head() == head)
      subst.push_back(*p);
}

ReplacementContext::~ReplacementContext()
{ }

void
ReplacementContext::add(const AreaId& id, const AreaRef& area)
{
  for (std::list< std::pair<AreaId,AreaRef> >::const_iterator p = subst.begin();
       p != subst.end();
       p++)
    if ((*p).first == id)
      throw DuplicateSubstitution();
  subst.push_back(std::make_pair(id, area));
}

AreaRef
ReplacementContext::get() const
{
  for (std::list< std::pair<AreaId,AreaRef> >::const_iterator p = subst.begin();
       p != subst.end();
       p++)
    if ((*p).first.empty()) return (*p).second;

  return 0;
}

