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

#ifndef __LengthAux_hh__
#define __LengthAux_hh__

#include <string>
#include <iostream>

#include "Length.hh"

GMV_MathView_EXPORT std::string toString(Length::Unit);
GMV_MathView_EXPORT bool fromString(const std::string&, Length::Unit&);

GMV_MathView_EXPORT std::ostream& operator<<(std::ostream&, const Length::Unit&);
GMV_MathView_EXPORT std::istream& operator>>(std::istream&, Length::Unit&);

GMV_MathView_EXPORT std::string toString(const Length&);

GMV_MathView_EXPORT std::ostream& operator<<(std::ostream&, const Length&);
GMV_MathView_EXPORT std::istream& operator>>(std::istream&, Length&);

#endif // __LengthAux_hh__
