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

#include "Attribute.hh"
#include "AbstractLogger.hh"
#include "MathMLOperatorDictionary.hh"
#include "AttributeList.hh"

MathMLOperatorDictionary::MathMLOperatorDictionary()
{ }

MathMLOperatorDictionary::~MathMLOperatorDictionary()
{ unload(); }

void
MathMLOperatorDictionary::add(const AbstractLogger& logger,
			      const String& opName, const String& form,
			      const SmartPtr<AttributeList>& defaults)
{
  FormDefaults& formDefaults = items[opName];
  if (form == "prefix")
    if (formDefaults.prefix)
      logger.out(LOG_WARNING, "duplicate `prefix' form for operator `%s' in dictionary (ignored)",
		 opName.c_str());
    else
      formDefaults.prefix = defaults;
  else if (form == "infix")
    if (formDefaults.prefix)
      logger.out(LOG_WARNING, "duplicate `infix' form for operator `%s' in dictionary (ignored)",
		 opName.c_str());
    else
      formDefaults.infix = defaults;
  else if (form == "postfix")
    if (formDefaults.prefix)
      logger.out(LOG_WARNING, "duplicate `postfix' form for operator `%s' in dictionary (ignored)",
		 opName.c_str());
    else
      formDefaults.postfix = defaults;
  else
    logger.out(LOG_WARNING, 
	       "invalid `form' attribute for entry `%s' in operator dictionary (ignored)",
	       opName.c_str());
}

void
MathMLOperatorDictionary::unload()
{ }

void
MathMLOperatorDictionary::search(const String& opName,
				 SmartPtr<AttributeList>& prefix,
				 SmartPtr<AttributeList>& infix,
				 SmartPtr<AttributeList>& postfix) const
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

