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

#ifndef __AbstractLogger_hh__
#define __AbstractLogger_hh__

#include "Object.hh"
#include "String.hh"

enum LogLevelId { LOG_ERROR, LOG_WARNING, LOG_INFO, LOG_DEBUG };

class GMV_MathView_EXPORT AbstractLogger : public Object
{
protected:
  AbstractLogger(void);
  virtual ~AbstractLogger();

public:
  void setLogLevel(LogLevelId) const;
  LogLevelId getLogLevel(void) const { return logLevel; }

  void out(LogLevelId, const char*, ...) const;

protected:
  virtual void outString(const String&) const = 0;

private:
  mutable LogLevelId logLevel;  
};

#endif // __AbstractLogger_hh__
