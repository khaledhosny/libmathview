// Copyright (C) 2000-2007, Luca Padovani <padovani@sti.uniurb.it>.
// 
// This file is part of GtkMathView, a flexible, high-quality rendering
// engine for MathML documents.
// 
// GtkMathView is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
// 
// GtkMathView is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <config.h>
#include <cassert>

#include "StringAux.hh"
#include "Attribute.hh"
#include "AbstractLogger.hh"
#include "MathMLOperatorDictionary.hh"
#include "AttributeSet.hh"

MathMLOperatorDictionary::MathMLOperatorDictionary()
{ }

MathMLOperatorDictionary::~MathMLOperatorDictionary()
{ unload(); }

void
MathMLOperatorDictionary::add(const AbstractLogger& logger,
			      const String& opName, const String& form,
			      const SmartPtr<AttributeSet>& defaults)
{
  FormDefaults& formDefaults = items[opName];
  if (form == "prefix")
    formDefaults.prefix = defaults;
  else if (form == "infix")
    formDefaults.infix = defaults;
  else if (form == "postfix")
    formDefaults.postfix = defaults;
  else
    logger.out(LOG_WARNING, 
	       "invalid `form' attribute for entry `%s' in operator dictionary (ignored)",
	       escape(UCS4StringOfString(opName)).c_str());
}

void
MathMLOperatorDictionary::unload()
{ }

void
MathMLOperatorDictionary::search(const String& opName,
				 SmartPtr<AttributeSet>& prefix,
				 SmartPtr<AttributeSet>& infix,
				 SmartPtr<AttributeSet>& postfix) const
{
  prefix = infix = postfix = 0;

  Dictionary::const_iterator p = items.find(opName);
  if (p != items.end())
    {
      prefix = (*p).second.prefix;
      infix = (*p).second.infix;
      postfix = (*p).second.postfix;
    }
}

