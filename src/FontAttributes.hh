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

#ifndef FontAttributes_hh
#define FontAttributes_hh

#include <string>
#include <vector>

#include "token.hh"
#include "Length.hh"

struct FontAttributes {
  FontAttributes(void);
  FontAttributes(const FontAttributes&, const FontAttributes&);

  bool HasFamily(void) const { return (family != ""); }
  bool HasStyle(void) const { return (style != T__NOTVALID); }
  bool HasWeight(void) const { return (weight != T__NOTVALID); }
  bool HasSize(void) const { return size.defined(); }
  bool Equals(const FontAttributes&) const;
  unsigned Compare(const FontAttributes&) const;
  void Dump(void) const;
  void Null(void);

  bool DownGrade(void);

  std::string  family;
  Length size; // cannot be < 0
  TokenId weight;
  TokenId style;
};

class ExtraFontAttributes {
public:
  std::string GetProperty(const std::string&) const;
  void AddProperty(const std::string&, const std::string&);
  void Dump(void) const;

  struct ExtraFontAttribute {
    std::string name;
    std::string value;
  };

private:
  std::vector<ExtraFontAttribute*> content;
};

#endif // FontAttributes.hh
