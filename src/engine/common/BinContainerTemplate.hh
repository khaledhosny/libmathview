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

#ifndef __BinContainerTemplate_hh__
#define __BinContainerTemplate_hh__

template <class E, class T, class TPtr = SmartPtr<T> >
class BinContainerTemplate
{
public:
  BinContainerTemplate(void) { }

  TPtr getChild(void) const { return child; }

  void setChild(E* elem, const TPtr& newChild)
  {
    if (child != newChild)
      {
	if (newChild) T::setParent(newChild, elem);
	child = newChild;
	elem->setDirtyLayout();
      }
  }

  void setFlagDown(Element::Flags f)
  {
    if (child) child->setFlagDown(f);
  }

  void resetFlagDown(Element::Flags f)
  {
    if (child) child->resetFlagDown(f);
  }

private:
  TPtr child;
};

#endif // __BinContainerTemplate_hh__
