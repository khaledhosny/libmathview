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
// http://cs.unibo.it/~lpadovan/mml-widget/, or send a mail to
// <luca.padovani@cs.unibo.it>

#include <config.h>

#include "libxml2_reader_Setup.hh"
#include "libxml2_reader_Model.hh"
#include "TemplateSetup.hh"

bool
libxml2_reader_Setup::loadOperatorDictionary(MathMLOperatorDictionary& dictionary, const String& path)
{
  return TemplateSetup<libxml2_reader_Model>::load<MathMLOperatorDictionary, true>(dictionary,
										   "operator dictionary",
										   "dictionary", path);
}

bool
libxml2_reader_Setup::loadConfiguration(Configuration& conf, const String& path)
{
  return TemplateSetup<libxml2_reader_Model>::load<Configuration, false>(conf,
									 "configuration",
									 "math-engine-configuration", path);
}

