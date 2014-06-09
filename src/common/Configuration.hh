// Copyright (C) 2000-2007, Luca Padovani <padovani@sti.uniurb.it>.
//
// This file is part of GtkMathView, a flexible, high-quality rendering
// engine for MathML documents.
// 
// GtkMathView is free software; you can redistribute it and/or modify it
// either under the terms of the GNU Lesser General Public License version
// 3 as published by the Free Software Foundation (the "LGPL") or, at your
// option, under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation (the "GPL").  If you do not
// alter this notice, a recipient may use your version of this file under
// either the GPL or the LGPL.
//
// GtkMathView is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the LGPL or
// the GPL for more details.
// 
// You should have received a copy of the LGPL and of the GPL along with
// this program in the files COPYING-LGPL-3 and COPYING-GPL-2; if not, see
// <http://www.gnu.org/licenses/>.

#ifndef __Configuration_hh__
#define __Configuration_hh__

#include <vector>

#include "defs.h"
#include "String.hh"
#include "StringHash.hh"
#include "RGBColor.hh"
#include "Object.hh"
#include "SmartPtr.hh"
#include <unordered_map>
#include "Length.hh"

class Configuration : public Object
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
  typedef std::unordered_map<String, SmartPtr<Entry>, StringHash, StringEq> Map;
  Map map;
};

#endif // __Configuration_hh__
