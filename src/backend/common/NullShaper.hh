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

#ifndef __NullShaper_hh__
#define __NullShaper_hh__

#include "Shaper.hh"

class GMV_MathView_EXPORT NullShaper : public Shaper
{
protected:
  NullShaper(const SmartPtr<class AbstractLogger>&);
  virtual ~NullShaper();

public:
  static SmartPtr<NullShaper> create(const SmartPtr<class AbstractLogger>&);

  virtual void registerShaper(const SmartPtr<class ShaperManager>&, unsigned);
  virtual void unregisterShaper(const SmartPtr<class ShaperManager>&, unsigned);
  virtual void shape(class ShapingContext&) const;
  virtual bool isDefaultShaper(void) const;

private:
  SmartPtr<class AbstractLogger> logger;
};

#endif // __NullShaper_hh__
