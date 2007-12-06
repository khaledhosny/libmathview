// Copyright (C) 2000-2007, Luca Padovani <padovani@sti.uniurb.it>.
//
// This file is part of GtkMathView, a flexible, high-quality rendering
// engine for MathML documents.
// 
// GtkMathView is free software; you can redistribute it and/or modify it
// either under the terms of the GNU Lesser General Public License version
// 3 as published by the Free Software Foundation (the "LGPL") or, at your
// option, under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation (the "GPL").  If you do not
// alter this notice, a recipient may use your version of this file under
// either the GPL or the LGPL.
//
// GtkMathView is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the LGPL or
// the GPL for more details.
// 
// You should have received a copy of the LGPL and of the GPL along with
// this program in the files COPYING-LGPL-3 and COPYING-GPL-2; if not, see
// <http://www.gnu.org/licenses/>.

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
      subst.push_back(std::make_pair((*p).first.tail(), (*p).second));
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

