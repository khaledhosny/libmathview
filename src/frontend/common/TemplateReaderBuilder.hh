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

#ifndef __TemplateReaderBuilder_hh__
#define __TemplateReaderBuilder_hh__

#include "Builder.hh"
#include "String.hh"
#include "TemplateReaderModel.hh"

template <typename Reader>
class TemplateReaderBuilder : public Builder
{
protected:
  TemplateReaderBuilder(void) { }
  virtual ~TemplateReaderBuilder() { }

public:
  void setReader(const SmartPtr<Reader>& r) { reader = r; }
  SmartPtr<Reader> getReader(void) const { return reader; }

protected:
  void setRootModelElement(const SmartPtr<Reader>& r) { setReader(r); }
  SmartPtr<Reader> getRootModelElement(void) const
  {
    if (reader)
      {
	// In this case the reader must play the role of the root
	// document element, that is it must be in a state where
	// it "points to" the beginning of the document. Thus we
	// invoke the reset method on it.
	reader->reset();
	return reader;
      }
    else
      return 0;
  }

private:
  SmartPtr<Reader> reader;
};

#endif // __TemplateReaderBuilder_hh__
