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

#ifndef __t1lib_TFM_T1FontManager_hh__
#define __t1lib_TFM_T1FontManager_hh__

#include "t1lib_T1FontManager.hh"

class GMV_MathView_EXPORT t1lib_TFM_T1FontManager : public t1lib_T1FontManager
{
protected:
  t1lib_TFM_T1FontManager(const SmartPtr<class AbstractLogger>&,
			  const SmartPtr<class Configuration>&,
			  const SmartPtr<class TFMManager>&);
  virtual ~t1lib_TFM_T1FontManager();

public:
  static SmartPtr<t1lib_TFM_T1FontManager> create(const SmartPtr<class AbstractLogger>&,
						  const SmartPtr<class Configuration>&,
						  const SmartPtr<class TFMManager>&);

protected:
  virtual SmartPtr<class t1lib_T1Font> createT1Font(const String&, const scaled&) const;

private:
  SmartPtr<class TFMManager> tfmManager;
};

#endif // __t1lib_TFM_T1FontManager_hh__
