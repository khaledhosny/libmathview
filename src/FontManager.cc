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
#include <string.h>
#include <stdio.h>

#include "AFont.hh"
#include "FontManager.hh"

FontManager::FontManager()
{
}

FontManager::~FontManager()
{
}

FontManager::Bucket*
FontManager::SearchFont(const FontAttributes& fa, const class ExtraFontAttributes* efa) const
{
  for (std::vector<Bucket*>::const_iterator i = content.begin();
       i != content.end();
       i++)
    {
      assert(*i);
      if ((*i)->fontAttributes.Equals(fa) && (*i)->extraFontAttributes == efa) return *i;
    }

  return NULL;
}

FontManager::Bucket*
FontManager::SearchFont(const class AFont* font) const
{
  assert(font != NULL);
  for (std::vector<Bucket*>::const_iterator i = content.begin();
       i != content.end();
       i++)
    {
      assert(*i != NULL);
      if ((*i)->font == font) return *i;
    }

  return NULL;
}

const AFont*
FontManager::GetFont(const FontAttributes& fa, const class ExtraFontAttributes* efa) const
{
  Bucket* bucket = SearchFont(fa, efa);
  if (bucket != NULL) return bucket->font;

  const AFont* font = SearchNativeFont(fa, efa);
  // by caching even NULL fonts we prevent subsequent search
  // for that native font, which may be time-consuming since usually
  // implies I/O. In general, the caching mechanism works well if
  // font configuration is static during the widget executions (so no
  // run-time extensions)

  bucket = new Bucket;
  bucket->fontAttributes = fa;
  bucket->extraFontAttributes = efa;
  bucket->font = font;
  bucket->used = false;

  content.push_back(bucket);

  return bucket->font;
}

void
FontManager::ResetUsedFonts() const
{
  for (std::vector<Bucket*>::const_iterator i = content.begin();
       i != content.end();
       i++)
    {
      assert(*i);
      (*i)->used = false;
    }
}

void
FontManager::MarkAsUsed(const class AFont* font) const
{
  assert(font != NULL);

  Bucket* bucket = SearchFont(font);
  assert(bucket != NULL);

  bucket->used = true;
}
