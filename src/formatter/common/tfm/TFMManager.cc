// Copyright (C) 2000-2005, Luca Padovani <luca.padovani@cs.unibo.it>.
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
// http://helm.cs.unibo.it/mml-widget/, or send a mail to
// <lpadovan@cs.unibo.it>

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
    { 0, 0 }
  };

  for (unsigned i = 0; table[i].name != 0; i++)
    {
      TFM::Font* font;
      TFM::Dimension* dimension;
      TFM::Character* character;
      (table[i].tables)(font, dimension, character);
      tfmCache[table[i].name] = TFM::create(font, dimension, character);
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
