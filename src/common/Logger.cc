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
#include <stdarg.h>
#include <stdio.h>

#include "Logger.hh"

Logger::Logger()
{
  logLevel = LOG_ERROR;
  logFile  = stderr;
}

Logger::Logger(FILE* f)
{
  assert(f != NULL);
  logLevel = LOG_ERROR;
  logFile  = f;
}

Logger::~Logger()
{
}

void
Logger::SetLogLevel(int level)
{
  if (level < LOG_ERROR) level = LOG_ERROR;
  if (level > LOG_DEBUG) level = LOG_DEBUG;
  logLevel = (LogLevelId) level;
}

void
Logger::SetLogFile(FILE* f)
{
  assert(f != NULL);
  logFile = f;
}

void
Logger::operator()(LogLevelId id, const char* fmt, ...) const
{
  static const char* msg[] = { "Error", "Warning", "Info", "Debug" };

  va_list args;

  va_start(args, fmt);

  if (id > logLevel) return;

  fprintf(logFile, "*** %s[%d:%d]: ", msg[id], id, logLevel);
  vfprintf(logFile, fmt, args);
  fprintf(logFile, "\n");
  va_end(args);
}

