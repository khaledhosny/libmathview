// Copyright (C) 2000-2003, Luca Padovani <luca.padovani@cs.unibo.it>.
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
// http://helm.cs.unibo.it/mml-widget, or send a mail to
// <luca.padovani@cs.unibo.it>

#ifndef __SparseMap_hh__
#define __SparseMap_hh__

template <class T, int M, int N>
class SparseMap
{
public:
  SparseMap(void)
  {
    for (unsigned i = 0; i < (1 << N); i++)
      defData[i] = T();
    for (unsigned i = 0; i < (1 << M); i++)
      data[i] = defData;
  }

  ~SparseMap()
  {
    for (unsigned i = 0; i < (1 << M); i++)
      {
	if (data[i] != defData) delete [] data[i];
	data[i] = 0;
      }
  }

protected:
  unsigned I(unsigned index) const
  { return index >> N; }

  unsigned J(unsigned index) const
  { return index & ((1 << N) - 1); }

public:
  void set(size_t index, const T& v)
  {
    unsigned i = I(index);
    assert(i < (1 << M));
    if (data[i] == defData)
      {
	data[i] = new T[1 << N];
	for (unsigned j = 0; j < (1 << N); j++)
	  data[i][j] = T();
      }
    data[i][J(index)] = v;
  }

  T& operator[](size_t index) const
  { return data[I(index)][J(index)]; }

private:
  typedef T* TBlock;

  TBlock data[1 << M];
  T defData[1 << N];
};

#endif // __SparseMap_hh__
