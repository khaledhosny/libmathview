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

#include <functional>
#include <algorithm>

#include <assert.h>
#include <stddef.h>
#include <string.h>

#include "Globals.hh"
#include "scaledConv.hh"
#include "FontAttributes.hh"

// local definition of adaptors
struct DumpAttributeAdaptor
  : public std::unary_function<ExtraFontAttributes::ExtraFontAttribute*,void>
{
  void operator()(ExtraFontAttributes::ExtraFontAttribute* attr) const
  {
    assert(attr);
    assert(attr->name != "");
    assert(attr->value != "");
    Globals::logger(LOG_DEBUG, "%s = '%s'", attr->name.c_str(), attr->value.c_str());
  }
};

FontAttributes::FontAttributes()
{
  Null();
}

FontAttributes::FontAttributes(const FontAttributes& source, const FontAttributes& def)
{
  assert(NOT_IMPLEMENTED);
}

void
FontAttributes::Null()
{
  family = "";
  style  = T__NOTVALID;
  weight = T__NOTVALID;
  size.unset();
}

bool
FontAttributes::DownGrade()
{
  bool res = true;

  if (HasWeight()) weight = T__NOTVALID;
  else if (HasStyle()) style = T__NOTVALID;
  else if (HasFamily()) family = "";
  else if (HasSize()) size.unset();
  else res = false;

  return res;
}

bool
FontAttributes::Equals(const FontAttributes& fa) const
{
  if (style != fa.style) return false;
  if (weight != fa.weight) return false;
  if ((family == "" && fa.family != "") ||
      (family != "" && fa.family == "")) return false;
  if (family != "" && family != fa.family) return false;
  if (size.defined() != fa.size.defined()) return false;
  if (size.defined() && ToScaledPoints(size) != ToScaledPoints(fa.size)) return false;

  return true;
}

unsigned
FontAttributes::Compare(const FontAttributes& fa) const
{
  unsigned eval = 0;

  if (HasStyle()) {
    if (fa.HasStyle()) {
      if (style != fa.style) return UINT_MAX;
    } else {
      eval++;
    }
  } else {
    if (fa.HasStyle()) eval++;
  }

  if (HasWeight()) {
    if (fa.HasWeight()) {
      if (weight != fa.weight) return UINT_MAX;
    } else {
      eval++;
    }
  } else {
    if (fa.HasWeight()) eval++;
  }

  if (HasFamily()) {
    if (fa.HasFamily()) {
      if (family != fa.family) return UINT_MAX;
    } else {
      eval++;
    }
  } else {
    if (fa.HasFamily()) eval++;
  }

  if (HasSize()) {
    if (fa.HasSize()) {
      scaled d = abs(ToScaledPoints(size) - ToScaledPoints(fa.size));
      eval += static_cast<int>(sp2pt(d));
    } else {
      eval++;
    }
  } else {
    if (fa.HasSize()) eval++;
  }

  return eval;
}

void
FontAttributes::Dump() const
{
  const char* w[] = { "_", "normal", "bold" };
  const char* s[] = { "_", "normal", "italic" };
  const char* m[] = { "*", "text", "math" };

  Globals::logger(LOG_DEBUG, "font(%dpt,%s,%s,%s)",
		  HasSize() ? static_cast<int>(sp2pt(ToScaledPoints(size))) : -1,
		  HasFamily() ? family.c_str() : "_", w[weight + 1], s[style + 1]);
}

// ExtraFontAttributes

std::string
ExtraFontAttributes::GetProperty(const std::string& name) const
{
  for (std::vector<ExtraFontAttribute*>::const_iterator i = content.begin();
       i != content.end();
       i++)
    {
      assert(*i);
      assert((*i)->name != "");
      assert((*i)->value != "");
      if ((*i)->name == name) return (*i)->value;
    }

  return "";
}

void
ExtraFontAttributes::AddProperty(const std::string& name, const std::string& value)
{
  ExtraFontAttribute* attribute = new ExtraFontAttribute;
  attribute->name = name;
  attribute->value = value;
  content.push_back(attribute);
}

void
ExtraFontAttributes::Dump() const
{
  Globals::logger(LOG_DEBUG, "extra font attributes dump:");
  std::for_each(content.begin(), content.end(), DumpAttributeAdaptor());
}
