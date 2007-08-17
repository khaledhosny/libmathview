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

#include <limits>
#include <sstream>

#include "StringAux.hh"

std::string
escape(const UCS4String& s)
{
  std::ostringstream os;
  UCS4String::const_iterator i = s.begin();
  while (i != s.end())
    {
      if (*i >= std::numeric_limits<char>::min() && *i <= std::numeric_limits<char>::max())
	os << static_cast<char>(*i);
      else
	{
	  os << "&#x" << std::hex;
	  os.width(4);
	  os.fill('0');
	  os << static_cast<int>(*i) << std::dec;
	  os.width(0);
	  os << ";";
	}
      i++;
    }

  return os.str();
}

std::ostream&
operator<<(std::ostream& os, const UCS4String& s)
{ return os << escape(s); }

