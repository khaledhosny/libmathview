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
#include <stdio.h>

#include "ValueSequence.hh"

ValueSequence::ValueSequence()
{
  Init();
}

ValueSequence::ValueSequence(const ValueSequence& seq)
{
  size = seq.size;
  value = new ValuePtr[size];

  for (unsigned i = 0; i < size; i++) {
    if (seq.value[i] != NULL)
      value[i] = new Value(*seq.value[i]);
    else
      value[i] = NULL;
  }
}

ValueSequence::~ValueSequence()
{
  for (unsigned i = 0; i < size; i++) {
    delete value[i];
    value[i] = NULL;
  }

  delete [] value;
  Init();
}

void
ValueSequence::Init()
{
  size = 0;
  value = NULL;
}

void
ValueSequence::AddValue(const Value* v)
{
  SetSize(size + 1);
  value[size - 1] = v;
}

const Value*
ValueSequence::GetFirstValue() const
{
  assert(size > 0);
  return value[0];
}

const Value*
ValueSequence::GetLastValue() const
{
  assert(size > 0);
  return value[size - 1];
}

const Value*
ValueSequence::GetValue(unsigned i) const
{
  assert(i < size);
  return value[i];
}

const Value*
ValueSequence::GetLastValue(unsigned i) const
{
  assert(size > 0);
  return (i < size) ? value[i] : value[size - 1];
}

void
ValueSequence::SetSize(unsigned newSize)
{
  if (newSize == 0) {
    if (value != NULL) delete [] value;
    size = 0;
    return;
  }
    
  ValuePtr* newValue = new ValuePtr[newSize];
  
  for (unsigned i = 0; i < newSize; i++)
    if (i < size) newValue[i] = value[i];
    else newValue[i] = NULL;

  if (value != NULL) delete [] value;
  value = newValue;
  size = newSize;
}

void
ValueSequence::Dump() const
{
  for (unsigned i = 0; i < size; i++) {
    putchar(' ');
    value[i]->Dump();
  }
}
