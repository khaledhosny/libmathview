// Copyright (C) 2000-2001, Luca Padovani <luca.padovani@cs.unibo.it>.
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
// http://www.cs.unibo.it/helm/mml-widget, or send a mail to
// <luca.padovani@cs.unibo.it>

#ifndef MathMLizer_hh
#define MathMLizer_hh

#if defined(HAVE_MINIDOM)
#include <minidom.h>
#elif defined(HAVE_GMETADOM)
#include "gmetadom.hh"
#endif

#include "keyword.hh"

class MathMLizer {
public:
#if defined(HAVE_MINIDOM)
  MathMLizer(mDOMDocRef);
#elif defined(HAVE_GMETADOM)
  MathMLizer(const GMetaDOM::Document&);
#endif
  ~MathMLizer();

  class MathMLDocument* ize(void);

protected:
#if defined(HAVE_MINIDOM)
  void MathMLizeNode(mDOMNodeRef, class MathMLContainerElement*);
  void MathMLizeContainerContent(mDOMNodeRef, class MathMLContainerElement*);
  void MathMLizeTokenContent(mDOMNodeRef, class MathMLTokenElement*);
  class String* SubstituteMCharElement(mDOMNodeRef);
  class MathMLTextNode* SubstituteMGlyphElement(mDOMNodeRef);
  class MathMLTextNode* SubstituteAlignMarkElement(mDOMNodeRef);

  mDOMDocRef  doc;
#elif defined(HAVE_GMETADOM)
  void MathMLizeNode(const GMetaDOM::Element&, class MathMLContainerElement*);
  void MathMLizeContainerContent(const GMetaDOM::Element&, class MathMLContainerElement*);
  void MathMLizeTokenContent(const GMetaDOM::Element&, class MathMLTokenElement*);
  class MathMLTextNode* SubstituteMGlyphElement(const GMetaDOM::Element&);
  class MathMLTextNode* SubstituteAlignMarkElement(const GMetaDOM::Element&);

  GMetaDOM::Document doc;
#endif
};

#endif // MathMLizer_hh
