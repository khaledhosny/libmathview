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

#ifndef MathMLDocument_hh
#define MathMLDocument_hh

// !!! BEGIN WARNING: hash_map is not part of the STL !!!
#if defined(HAVE_EXT_HASH_MAP)
#include <ext/hash_map>
#elif defined(HAVE_HASH_MAP)
#include <hash_map>
#else
#error "no implementation of hash_map could be found"
#endif
// !!! END WARNING: hash_map is not part of the STL !!!

#if defined(HAVE_GMETADOM)
#include "gmetadom.hh"
#endif

#include "MathMLBinContainerElement.hh"

class MathMLDocument : public MathMLBinContainerElement
{
protected:
  MathMLDocument(void);
#if defined(HAVE_GMETADOM)
  MathMLDocument(const DOM::Document&);
  MathMLDocument(const DOM::Element&);
  void Init(void);
#endif
  virtual ~MathMLDocument();

public:
  static Ptr<MathMLDocument> create(void)
  { return Ptr<MathMLDocument>(new MathMLDocument()); }
#if defined(HAVE_GMETADOM)
  static Ptr<MathMLDocument> create(const DOM::Document& doc)
  { return Ptr<MathMLDocument>(new MathMLDocument(doc)); }
  static Ptr<MathMLDocument> create(const DOM::Element& root)
  { return Ptr<MathMLDocument>(new MathMLDocument(root)); }
#endif

  virtual void Normalize(void);
  virtual void Setup(class RenderingEnvironment&);
  virtual void SetDirtyAttribute(void);

  Ptr<MathMLElement> GetRoot(void) const { return GetChild(); }

  Ptr<MathMLElement> findFormattingNode(const DOM::Node&) const;
  Ptr<MathMLElement> getFormattingNodeNoCreate(const DOM::Node&) const;
  Ptr<MathMLElement> getFormattingNode(const DOM::Node&) const;
  void               setFormattingNode(const DOM::Node&, const Ptr<MathMLElement>&) const;

  void               notifySubtreeModified(const DOM::Node&) const;
  void               notifyAttributeModified(const DOM::Node&) const;

#if defined(HAVE_GMETADOM)
  const DOM::Document& GetDOMDocument(void) const { return DOMdoc; }
  const DOM::Element& GetDOMElement(void) const { return DOMroot; }

protected:

  class DOMSubtreeModifiedListener : public DOM::EventListener
  {
  public:
    DOMSubtreeModifiedListener(const Ptr<MathMLDocument>& d) : doc(d) { };
    virtual ~DOMSubtreeModifiedListener() { };
    virtual void handleEvent(const DOM::Event&);

  private:
    Ptr<MathMLDocument> doc;
  };

  class DOMAttrModifiedListener : public DOM::EventListener
  {
  public:
    DOMAttrModifiedListener(const Ptr<MathMLDocument>& d) : doc(d) { };
    virtual ~DOMAttrModifiedListener() { };
    virtual void handleEvent(const DOM::Event&);

  private:
    Ptr<MathMLDocument> doc;
  };

  DOMSubtreeModifiedListener* subtreeModifiedListener;
  DOMAttrModifiedListener*    attrModifiedListener;

  DOM::Document DOMdoc;  // can be 0
  DOM::Element  DOMroot; // cannot be 0

  struct DOM_hash : public std::unary_function< DOM::Node, size_t >
  {
    size_t operator()(const DOM::Node& node) const
    {
      assert(node);
      size_t res = reinterpret_cast<size_t>(static_cast<GdomeNode*>(node));
      return res;
    }
  };

#if defined(HAVE_EXT_HASH_MAP)
  typedef __gnu_cxx::hash_map< DOM::Node, Ptr<MathMLElement>, DOM_hash > DOMNodeMap;
#elif defined(HAVE_HASH_MAP)
  typedef std::hash_map< DOM::Node, Ptr<MathMLElement>, DOM_hash > DOMNodeMap;
#endif
  mutable DOMNodeMap nodeMap;
#endif // HAVE_GMETADOM
};

#endif // MathMLDocument_hh
