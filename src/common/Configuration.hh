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

#ifndef __Configuration_hh__
#define __Configuration_hh__

#include <vector>

#include "defs.h"
#include "String.hh"
#include "StringHash.hh"
#include "RGBColor.hh"
#include "Object.hh"
#include "SmartPtr.hh"
#include "HashMap.hh"
#include "Length.hh"

class GMV_EXPORT Configuration : public Object
{
protected:
  Configuration(void);
  virtual ~Configuration();

public:
  static SmartPtr<Configuration> create(void)
  { return new Configuration(); }

  class Entry : public Object
  {
  protected:
    Entry(const String&, const SmartPtr<Entry>&);
    virtual ~Entry();

  public:
    static SmartPtr<Entry> create(const String&, const SmartPtr<Entry>&);

    String getValue(void) const { return value; }
    String asString(void) const { return value; }
    bool asInt(int&) const;
    bool asBool(bool&) const;
    bool asRGBColor(RGBColor&) const;
    SmartPtr<Entry> getNext(void) const;

  private:
    String value;
    SmartPtr<Entry> next;
  };

  static const char* getBinaryVersion(void);
  static void addConfigurationPath(const String&);
  static const std::vector<String>& getConfigurationPaths(void);

  bool has(const String&) const;
  void add(const String&, const String&);
  SmartPtr<Entry> get(const String&) const;
  std::vector<SmartPtr<Entry> > getAll(const String&) const;

  String getString(const SmartPtr<class AbstractLogger>&, const String&, const String&) const;
  std::vector<String> getStringList(const String&) const;
  int getInt(const SmartPtr<class AbstractLogger>&, const String&, int) const;
  bool getBool(const SmartPtr<class AbstractLogger>&, const String&, bool) const;
  RGBColor getRGBColor(const SmartPtr<class AbstractLogger>&, const String&, const RGBColor&) const;
  Length getLength(const SmartPtr<class AbstractLogger>&, const String&, const Length&) const;

private:
  static std::vector<String> configurationPaths;
  typedef HASH_MAP_NS::hash_map<String, SmartPtr<Entry>, StringHash, StringEq> Map;
  Map map;
};

#endif // __Configuration_hh__
