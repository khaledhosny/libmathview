// Copyright (C) 2000-2004, Luca Padovani <luca.padovani@cs.unibo.it>.
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

#include <stdarg.h>

#include "AbstractLogger.hh"

AbstractLogger::AbstractLogger() : logLevel(LOG_WARNING)
{ }

AbstractLogger::~AbstractLogger()
{ }

void
AbstractLogger::setLogLevel(LogLevelId level)
{
  if (level < LOG_ERROR) level = LOG_ERROR;
  if (level > LOG_DEBUG) level = LOG_DEBUG;
  logLevel = LogLevelId(level);
}

void
AbstractLogger::out(LogLevelId id, const char* fmt, ...) const
{
  static const char* msg[] = { "Error", "Warning", "Info", "Debug" };

  va_list args;
  va_start(args, fmt);

  if (id <= logLevel)
    {
      String res;

      static char buffer[256];
      snprintf(buffer, 256, "*** %s[%d:%d]: ", msg[id], id, logLevel);
      res += buffer;
      vsnprintf(buffer, 256, fmt, args);
      res += buffer;
      outString(res);
    }

  va_end(args);
}

