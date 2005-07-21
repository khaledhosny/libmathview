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

#include "AbstractLogger.hh"
#include "Configuration.hh"
#include "TemplateStringParsers.hh"

std::vector<String> Configuration::configurationPaths;

Configuration::Configuration()
{ }

Configuration::~Configuration()
{ }

void
Configuration::addConfigurationPath(const String& path)
{ configurationPaths.push_back(path); }

const std::vector<String>&
Configuration::getConfigurationPaths()
{ return configurationPaths; }

void
Configuration::set(const String& key, const String& value)
{
  Map::iterator p = map.find(key);
  if (p != map.end())
    p->second = Entry::create(value, p->second);
  else
    map[key] = Entry::create(value, 0);
}

bool
Configuration::has(const String& key) const
{ return map.find(key) != map.end(); }

const SmartPtr<Configuration::Entry>
Configuration::get(const String& key) const
{
  const Map::const_iterator p = map.find(key);
  if (p != map.end())
    return p->second;
  else
    return 0;
}

String
Configuration::getString(const SmartPtr<AbstractLogger>& logger, const String& key, const String& def) const
{
  if (SmartPtr<Entry> entry = get(key))
    return entry->getValue();
  else
    {
      logger->out(LOG_INFO, "missing `%s' key in configuration", key.c_str());
      return def;
    }
}

std::vector<String>
Configuration::getStringList(const String& key) const
{
  std::vector<String> res;
  for (SmartPtr<Entry> entry = get(key); entry; entry = entry->getNext())
    res.push_back(entry->getValue());
  return res;
}

int
Configuration::getInt(const SmartPtr<AbstractLogger>& logger, const String& key, int def) const
{
  if (SmartPtr<Entry> entry = get(key))
    {
      int res;
      if (entry->asInt(res))
	return res;
      else
	{
	  logger->out(LOG_WARNING, "syntax error for `%s' key in configuration (int expected)", key.c_str());
	  return def;
	}
    }
  else
    {
      logger->out(LOG_INFO, "missing `%s' key in configuration", key.c_str());
      return def;
    }
}

bool
Configuration::getBool(const SmartPtr<AbstractLogger>& logger, const String& key, bool def) const
{
  if (SmartPtr<Entry> entry = get(key))
    {
      bool res;
      if (entry->asBool(res))
	return res;
      else
	{
	  logger->out(LOG_WARNING, "syntax error for `%s' key in configuration (bool expected)", key.c_str());
	  return def;
	}
    }
  else
    {
      logger->out(LOG_INFO, "missing `%s' key in configuration", key.c_str());
      return def;
    }
}

RGBColor
Configuration::getRGBColor(const SmartPtr<AbstractLogger>& logger, const String& key, const RGBColor& def) const
{
  if (SmartPtr<Entry> entry = get(key))
    {
      RGBColor res;
      if (entry->asRGBColor(res))
	return res;
      else
	{
	  logger->out(LOG_WARNING, "syntax error for `%s' key in configuration (color expected)", key.c_str());
	  return def;
	}
    }
  else
    {
      logger->out(LOG_INFO, "missing `%s' key in configuration", key.c_str());
      return def;
    }
}

Configuration::Entry::Entry(const String& _value, const SmartPtr<Entry>& _next)
  : value(_value), next(_next)
{ }

Configuration::Entry::~Entry()
{ }

SmartPtr<Configuration::Entry>
Configuration::Entry::create(const String& value, const SmartPtr<Entry>& next)
{ return new Entry(value, next); }

SmartPtr<Configuration::Entry>
Configuration::Entry::getNext() const
{ return next; }

bool
Configuration::Entry::asInt(int& res) const
{
  const UCS4String uValue = UCS4StringOfString(value);
  UCS4String::const_iterator next;
  if (SmartPtr<Value> v = Parse<ScanInteger, int>::parse(uValue.begin(), uValue.end(), next))
    {
      res = ToInteger(v);
      return true;
    }
  else
    return false;
}

bool
Configuration::Entry::asBool(bool& res) const
{
  if (value == "true" || value == "on" || value == "yes")
    {
      res = true;
      return true;
    }
  else if (value == "false" || value == "off" || value == "yes")
    {
      res = false;
      return true;
    }
  else
    return false;
}

bool
Configuration::Entry::asRGBColor(RGBColor& res) const
{
  const UCS4String uValue = UCS4StringOfString(value);
  UCS4String::const_iterator next;  
  if (SmartPtr<Value> v = ParseRGBColor::parse(uValue.begin(), uValue.end(), next))
    {
      res = ToRGBColor(v);
      return true;
    }
  else
    return false;
}
