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
#include <stddef.h>

#include "MathML.hh"
#include "Globals.hh"
#include "MathMLDocument.hh"
#include "MathMLDummyElement.hh"
#include "RenderingEnvironment.hh"

MathMLDocument::MathMLDocument()
#if defined(HAVE_GMETADOM)
  : DOMdoc(0)
#endif
{
  Init();
}

#if defined(HAVE_GMETADOM)
MathMLDocument::MathMLDocument(const DOM::Document& doc)
  : MathMLBinContainerElement()
    , DOMdoc(doc)
    , DOMroot(doc.get_documentElement())
{
  Init();
}

MathMLDocument::MathMLDocument(const DOM::Element& root)
  : MathMLBinContainerElement()
    , DOMdoc(root.get_ownerDocument())
    , DOMroot(root)
{
  Init();
}

void
MathMLDocument::Init()
{
  if (DOMdoc)
    {
      setFormattingNode(DOMdoc, this);

      DOM::EventTarget et(DOMdoc);
      assert(et);

      subtreeModifiedListener = new DOMSubtreeModifiedListener(this);
      attrModifiedListener = new DOMAttrModifiedListener(this);

      et.addEventListener("DOMSubtreeModified", *subtreeModifiedListener, false);
      et.addEventListener("DOMAttrModified", *attrModifiedListener, false);
    }
}
#endif

MathMLDocument::~MathMLDocument()
{
#if defined(HAVE_GMETADOM)
  if (DOMdoc)
    {
      DOM::EventTarget et(DOMdoc);
      assert(et);

      et.removeEventListener("DOMSubtreeModified", *subtreeModifiedListener, false);
      et.removeEventListener("DOMAttrModified", *attrModifiedListener, false);

      delete subtreeModifiedListener;
      delete attrModifiedListener;
      subtreeModifiedListener = 0;
      attrModifiedListener = 0;
    }
#endif
}

void
MathMLDocument::Normalize()
{
  if (DirtyStructure())
    {
#if defined(HAVE_GMETADOM)
      DOM::NodeList nodeList = GetDOMDocument().getElementsByTagNameNS(MATHML_NS_URI, "math");
      if (DOM::Node mathRoot = nodeList.item(0))
	{
	  Ptr<MathMLElement> elem = getFormattingNode(mathRoot);
	  assert(elem);
	  SetChild(elem);
	}	  
      else
        SetChild(0);
#endif // HAVE_GMETADOM

      if (GetChild()) GetChild()->Normalize(this);

      ResetDirtyStructure();
    }
}

void
MathMLDocument::Setup(RenderingEnvironment& env)
{
  if (DirtyAttribute() || DirtyAttributeP())
    {
      env.Push();
      env.SetDocument(this);
      MathMLBinContainerElement::Setup(env);
      env.Drop();
      ResetDirtyAttribute();
    }
}

void
MathMLDocument::SetDirtyAttribute()
{
  MathMLBinContainerElement::SetDirtyAttribute();
  // changing an attribute at this level amounts at
  // invalidating the whole tree. It is the notification
  // of a change in the context, rather than in a real attribute
  SetFlagDown(FDirtyAttribute);
}

#if defined(HAVE_GMETADOM)

void
MathMLDocument::DOMSubtreeModifiedListener::handleEvent(const DOM::Event& ev)
{
  const DOM::MutationEvent& me(ev);
  assert(me);
  assert(doc);
  const DOM::Node node(me.get_target());
  assert(node);

  //cout << "GTKMATHVIEW RECEIVED EVENT: " << ev.get_type() << " target = " << node.get_nodeName() << endl;
  doc->notifySubtreeModified(node);
}

void
MathMLDocument::notifySubtreeModified(const DOM::Node& node) const
{
  assert(node);
  //cout << "have to notify " << static_cast<GdomeNode*>(node) << "(" << node.get_nodeName() << ")" << " formatting node? " << static_cast<MathMLElement*>(findFormattingNode(node)) << endl;
  if (Ptr<MathMLElement> elem = findFormattingNode(node))
    {
      //cout << "notifying subtree modified event" << endl;
      elem->SetDirtyStructure();
      elem->SetDirtyAttributeD();
    }
}

void
MathMLDocument::DOMAttrModifiedListener::handleEvent(const DOM::Event& ev)
{
  const DOM::MutationEvent& me(ev);
  assert(me);
  assert(doc);
  doc->notifyAttributeModified(me.get_target());
}

void
MathMLDocument::notifyAttributeModified(const DOM::Node& node) const
{
  assert(node);
  if (Ptr<MathMLElement> elem = findFormattingNode(node))
    {
      //cout << "notifying attribute modified event" << endl;
      elem->SetDirtyAttribute();
    }
}

Ptr<MathMLElement>
MathMLDocument::getFormattingNodeNoCreate(const DOM::Node& node) const
{
  assert(node);

  DOMNodeMap::iterator p = nodeMap.find(node);
  if (p != nodeMap.end()) return (*p).second;
  else return 0;
}

Ptr<MathMLElement>
MathMLDocument::findFormattingNode(const DOM::Node& node) const
{
  for (DOM::Node p = node; p; p = p.get_parentNode())
    if (Ptr<MathMLElement> fNode = getFormattingNodeNoCreate(p))
      return fNode;
  
  return 0;
}

Ptr<MathMLElement>
MathMLDocument::getFormattingNode(const DOM::Node& node) const
{
  if (!node) return 0;

  DOMNodeMap::iterator p = nodeMap.find(node);
  if (p != nodeMap.end()) return (*p).second;

  DOM::Element el(node);
  assert(el);

  static struct
  {
    TagId tag;
    Ptr<MathMLElement> (*create)(const DOM::Element&);
  } tab[] = {
    { TAG_MATH,          &MathMLmathElement::create },
    { TAG_MI,            &MathMLIdentifierElement::create },
    { TAG_MN,            &MathMLTokenElement::create },
    { TAG_MO,            &MathMLOperatorElement::create },
    { TAG_MTEXT,         &MathMLTextElement::create },
    { TAG_MSPACE,        &MathMLSpaceElement::create },
    { TAG_MS,            &MathMLStringLitElement::create },
    { TAG_MROW,          &MathMLRowElement::create },
    { TAG_MFRAC,         &MathMLFractionElement::create },
    { TAG_MSQRT,         &MathMLRadicalElement::create },
    { TAG_MROOT,         &MathMLRadicalElement::create },
    { TAG_MSTYLE,        &MathMLStyleElement::create },
    { TAG_MERROR,        &MathMLErrorElement::create },
    { TAG_MPADDED,       &MathMLPaddedElement::create },
    { TAG_MPHANTOM,      &MathMLPhantomElement::create },
    { TAG_MFENCED,       &MathMLFencedElement::create },
    { TAG_MSUB,          &MathMLScriptElement::create },
    { TAG_MSUP,          &MathMLScriptElement::create },
    { TAG_MSUBSUP,       &MathMLScriptElement::create },
    { TAG_MUNDER,        &MathMLUnderOverElement::create },
    { TAG_MOVER,         &MathMLUnderOverElement::create },
    { TAG_MUNDEROVER,    &MathMLUnderOverElement::create },
    { TAG_MMULTISCRIPTS, &MathMLMultiScriptsElement::create },
    { TAG_MTABLE,        &MathMLTableElement::create },
    { TAG_MTR,           &MathMLTableRowElement::create },
    { TAG_MLABELEDTR,    &MathMLLabeledTableRowElement::create },
    { TAG_MTD,           &MathMLTableCellElement::create },
    { TAG_MALIGNGROUP,   &MathMLAlignGroupElement::create },
    { TAG_MALIGNMARK,    &MathMLAlignMarkElement::create },
    { TAG_MACTION,       &MathMLActionElement::create },
    { TAG_MENCLOSE,      &MathMLEncloseElement::create },
    { TAG_SEMANTICS,     &MathMLSemanticsElement::create },

    { TAG_NOTVALID,      0 }
  };

  std::string s_tag = nodeLocalName(el);
  TagId tag = TagIdOfName(s_tag.c_str());
  if (tag == TAG_NOTVALID)
    {
      Globals::logger(LOG_WARNING, "unrecognized element `%s'", s_tag.c_str());
      return MathMLDummyElement::create(el);
    }

  unsigned i;
  for (i = 0; tab[i].tag != TAG_NOTVALID && tab[i].tag != tag; i++) ;
  assert(tab[i].create != 0);

  if (Ptr<MathMLElement> res = tab[i].create(el))
    {
      setFormattingNode(el, res);
      return res;
    }
  else
    return MathMLDummyElement::create(el);
}

void
MathMLDocument::setFormattingNode(const DOM::Node& node, const Ptr<MathMLElement>& elem) const
{
  assert(node);
  nodeMap[node] = elem;
}

#endif // HAVE_GMETADOM
