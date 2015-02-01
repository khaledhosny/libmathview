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

#include <config.h>

#include <stdio.h>

#ifdef WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

#include "Clock.hh"

Clock::Clock()
{
  start.tv_sec = stop.tv_sec = 0;
  start.tv_usec = stop.tv_usec = 0;
}

void Clock::Start()
{
#ifdef WIN32
  SYSTEMTIME st;
  ::GetSystemTime(&st);
  start.tv_sec = st.wSecond;
  start.tv_usec = st.wMilliseconds;  
#else
  gettimeofday(&start, nullptr);
#endif
}

void Clock::Stop()
{
#ifdef WIN32
  SYSTEMTIME st;
  ::GetSystemTime(&st); 
  stop.tv_sec = st.wSecond;
  stop.tv_usec = st.wMilliseconds;  
#else
  gettimeofday(&stop, nullptr);
#endif
}

long Clock::Get() const
{
  struct timeval diff;

  // Compute the time remaining to wait. `tv_usec' is certainly positive.
  diff.tv_sec = stop.tv_sec - start.tv_sec;
  diff.tv_usec = stop.tv_usec - start.tv_usec;
     
  return (diff.tv_sec * 1000000 + diff.tv_usec) / 1000;
}

void Clock::Dump(const char* msg) const
{
  printf("%s\t\t%ldms\n", msg, Get());
}
