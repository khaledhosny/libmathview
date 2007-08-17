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
  gettimeofday(&start, NULL);
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
  gettimeofday(&stop, NULL);
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
