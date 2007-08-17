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

#include "TFMManager.hh"

#define DECLARE_TABLE(n) extern void n(TFM::Font*&, TFM::Dimension*&, TFM::Character*&);

DECLARE_TABLE(cmr10_tables)
DECLARE_TABLE(cmb10_tables)
DECLARE_TABLE(cmbxti10_tables)
DECLARE_TABLE(cmti10_tables)
DECLARE_TABLE(cmss10_tables)
DECLARE_TABLE(cmssi10_tables)
DECLARE_TABLE(cmssbx10_tables)
DECLARE_TABLE(cmtt10_tables)
DECLARE_TABLE(cmsy10_tables)
DECLARE_TABLE(cmbsy10_tables)
DECLARE_TABLE(cmmi10_tables)
DECLARE_TABLE(cmmib10_tables)
DECLARE_TABLE(cmex10_tables)
DECLARE_TABLE(msam10_tables)
DECLARE_TABLE(msbm10_tables)

TFMManager::TFMManager()
{
  static TFMTable table[] = {
    { "cmr10", cmr10_tables },
    { "cmb10", cmb10_tables },
    { "cmbxti10", cmbxti10_tables },
    { "cmti10", cmti10_tables },
    { "cmss10", cmss10_tables },
    { "cmssi10", cmssi10_tables },
    { "cmssbx10", cmssbx10_tables },
    { "cmtt10", cmtt10_tables },
    { "cmsy10", cmsy10_tables },
    { "cmbsy10", cmbsy10_tables },
    { "cmmi10", cmmi10_tables },
    { "cmmib10", cmmib10_tables },
    { "cmex10", cmex10_tables },
    { "msam10", msam10_tables },
    { "msbm10", msbm10_tables },
    { 0, 0 }
  };

  for (unsigned i = 0; table[i].name != 0; i++)
    {
      TFM::Font* font;
      TFM::Dimension* dimension;
      TFM::Character* character;
      (table[i].tables)(font, dimension, character);
      tfmCache[table[i].name] = TFM::create(table[i].name, font, dimension, character);
    }
}

TFMManager::~TFMManager()
{ }

SmartPtr<TFM>
TFMManager::getTFM(const String& name) const
{
  const TFMCache::const_iterator p = tfmCache.find(name);
  if (p != tfmCache.end())
    return p->second;
  else
    return 0;
}
