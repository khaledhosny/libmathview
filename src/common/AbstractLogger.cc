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

#include <stdarg.h>

#include "AbstractLogger.hh"

AbstractLogger::AbstractLogger() : logLevel(LOG_WARNING)
{ }

AbstractLogger::~AbstractLogger()
{ }

void
AbstractLogger::setLogLevel(LogLevelId level) const
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
      snprintf(buffer, 256, "[MathView] *** %s[%d:%d]: ", msg[id], id, logLevel);
      res += buffer;
      vsnprintf(buffer, 256, fmt, args);
      res += buffer;
      outString(res);
    }

  va_end(args);
}

