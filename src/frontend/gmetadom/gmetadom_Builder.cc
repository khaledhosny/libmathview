// Copyright (C) 2000-2004, Luca Padovani <luca.padovani@cs.unibo.it>.
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
// http://helm.cs.unibo.it/mml-widget/, or send a mail to
// <lpadovan@cs.unibo.it>

#include <config.h>

#include "Globals.hh"
#include "defs.h"
#include "gmetadom.hh"
#include "gmetadomIterators.hh"
#include "MathML.hh"
#include "BoxML.hh"
#include "BoxMLMathMLAdapter.hh"
#include "gmetadom_Builder.hh"
#include "MathMLAttributeSignatures.hh"
#include "BoxMLAttributeSignatures.hh"
#include "MathMLNamespaceContext.hh"
#include "BoxMLNamespaceContext.hh"
#include "ValueConversion.hh"

gmetadom_Builder::gmetadom_Builder()
{
  static struct
  {
    String tag;
    MathMLCreationMethod create;
  } mathml_tab[] = {
    { "math",          &gmetadom_Builder::create_MathML_math_Element },
    { "mi",            &gmetadom_Builder::create_MathML_mi_Element },
    { "mn",            &gmetadom_Builder::create_MathML_mn_Element },
    { "mo",            &gmetadom_Builder::create_MathML_mo_Element },
    { "mtext",         &gmetadom_Builder::create_MathML_mtext_Element },
    { "mspace",        &gmetadom_Builder::create_MathML_mspace_Element },
    { "ms",            &gmetadom_Builder::create_MathML_ms_Element },
    { "mrow",          &gmetadom_Builder::create_MathML_mrow_Element },
    { "mfrac",         &gmetadom_Builder::create_MathML_mfrac_Element },
    { "msqrt",         &gmetadom_Builder::create_MathML_msqrt_Element },
    { "mroot",         &gmetadom_Builder::create_MathML_mroot_Element },
    { "mstyle",        &gmetadom_Builder::create_MathML_mstyle_Element },
    { "merror",        &gmetadom_Builder::create_MathML_merror_Element },
    { "mpadded",       &gmetadom_Builder::create_MathML_mpadded_Element },
    { "mphantom",      &gmetadom_Builder::create_MathML_mphantom_Element },
    { "mfenced",       &gmetadom_Builder::create_MathML_mfenced_Element },
    { "msub",          &gmetadom_Builder::create_MathML_msub_Element },
    { "msup",          &gmetadom_Builder::create_MathML_msup_Element },
    { "msubsup",       &gmetadom_Builder::create_MathML_msubsup_Element },
    { "munder",        &gmetadom_Builder::create_MathML_munder_Element },
    { "mover",         &gmetadom_Builder::create_MathML_mover_Element },
    { "munderover",    &gmetadom_Builder::create_MathML_munderover_Element },
    { "mmultiscripts", &gmetadom_Builder::create_MathML_mmultiscripts_Element },
    { "mtable",        &gmetadom_Builder::create_MathML_mtable_Element },
    { "maligngroup",   &gmetadom_Builder::create_MathML_maligngroup_Element },
    { "malignmark",    &gmetadom_Builder::create_MathML_malignmark_Element },
    { "maction",       &gmetadom_Builder::create_MathML_maction_Element },
    { "menclose",      &gmetadom_Builder::create_MathML_menclose_Element },
    { "semantics",     &gmetadom_Builder::create_MathML_semantics_Element },
    { "",              0 }
  };

  for (unsigned i = 0; mathml_tab[i].create; i++)
    mathmlMap[mathml_tab[i].tag] = mathml_tab[i].create;

  static struct
  {
    String tag;
    BoxMLCreationMethod create;
  } boxml_tab[] = {
    { "at",            &gmetadom_Builder::create_BoxML_at_Element },
    { "layout",        &gmetadom_Builder::create_BoxML_layout_Element },
    { "h",             &gmetadom_Builder::create_BoxML_h_Element },
    { "ink",           &gmetadom_Builder::create_BoxML_ink_Element },
    { "space",         &gmetadom_Builder::create_BoxML_space_Element },
    { "text",          &gmetadom_Builder::create_BoxML_text_Element },
    { "v",             &gmetadom_Builder::create_BoxML_v_Element },
    { "box",           &gmetadom_Builder::create_BoxML_box_Element },
    { "action",        &gmetadom_Builder::create_BoxML_action_Element },
    { "obj",           &gmetadom_Builder::create_BoxML_obj_Element },
    { "par",           &gmetadom_Builder::create_BoxML_par_Element },
    { "",              0 }
  };

  for (unsigned i = 0; boxml_tab[i].create; i++)
    boxmlMap[mathml_tab[i].tag] = boxml_tab[i].create;
}

gmetadom_Builder::~gmetadom_Builder()
{ }

SmartPtr<Element>
gmetadom_Builder::getRootElement() const
{
  if (root)
    {
      String ns = root.get_namespaceURI();
      if (ns == MATHML_NS_URI) return getMathMLElement(root);
      else if (ns == BOXML_NS_URI) return getBoxMLElement(root);
      else return 0;
    }
  else
    return 0;
}

SmartPtr<Attribute>
gmetadom_Builder::getAttribute(const DOM::Element& el, const AttributeSignature& signature) const
{
  SmartPtr<Attribute> attr;
  
  if (signature.fromElement)
    if (el.hasAttribute(signature.name))
      attr = Attribute::create(signature, el.getAttribute(signature.name));

  if (!attr && signature.fromContext)
    attr = refinementContext.get(signature);

  return attr;
}

SmartPtr<Value>
gmetadom_Builder::getAttributeValue(const DOM::Element& el, const AttributeSignature& signature) const
{
  if (SmartPtr<Attribute> attr = getAttribute(el, signature))
    return attr->getValue();
  else
    return 0;
}

void
gmetadom_Builder::refineAttribute(const SmartPtr<Element>& elem,
				  const DOM::Element& el, const AttributeSignature& signature) const
{
  if (SmartPtr<Attribute> attr = getAttribute(el, signature)) elem->setAttribute(attr);
  else elem->removeAttribute(signature);
}

SmartPtr<MathMLElement>
gmetadom_Builder::getMathMLElement(const DOM::Element& el) const
{
  SmartPtr<MathMLElement> res = createMathMLElement(el);
  assert(res);
  res->resetDirtyAttribute();
  res->resetDirtyStructure();
  return res;
}

SmartPtr<MathMLElement>
gmetadom_Builder::getMathMLElementNoCreate(const DOM::Element& el) const
{
  return 0;
}

SmartPtr<BoxMLElement>
gmetadom_Builder::getBoxMLElement(const DOM::Element& el) const
{
  return createBoxMLElement(el);
}

SmartPtr<MathMLTextNode>
gmetadom_Builder::getMathMLTextNode(const DOM::Node& node) const
{
  switch (node.get_nodeType())
    {
    case DOM::Node::TEXT_NODE:
      {
	// ok, we have a chunk of text
	String s = collapseSpaces(DOMX::fromDOMString(node.get_nodeValue()));

	// ...but spaces at the at the beginning (end) are deleted only if this
	// is the very first (last) chunk in the token.
	if (!node.get_previousSibling()) s = trimSpacesLeft(s);
	if (!node.get_nextSibling()) s = trimSpacesRight(s);
	    
	return MathMLStringNode::create(s);
      }
    break;

    case DOM::Node::ELEMENT_NODE:
      {	    
	if (node.get_namespaceURI() == MATHML_NS_URI)
	  {
	    if (node.get_localName() == "mglyph")
	      return create_MathML_mglyph_Node(node);
	    else if (node.get_localName() == "malignmark")
	      return create_MathML_malignmark_Node(node);
	  }
      }
    break;
		
    default:
      break;
    }

  return 0;
}

SmartPtr<MathMLElement>
gmetadom_Builder::createMathMLElement(const DOM::Element& el) const
{
  std::cout << "createMathMLElement " << el.get_localName() << std::endl;
  MathMLBuilderMap::const_iterator m = mathmlMap.find(el.get_localName());
  if (m != mathmlMap.end()) 
    return (this->*(m->second))(el);
  else
    return createMathMLDummyElement();
}

SmartPtr<BoxMLElement>
gmetadom_Builder::createBoxMLElement(const DOM::Element& el) const
{
  BoxMLBuilderMap::const_iterator m = boxmlMap.find(el.get_localName());
  if (m != boxmlMap.end())
    return (this->*(m->second))(el);
  else
    return createBoxMLDummyElement();
}

void
gmetadom_Builder::getChildMathMLElements(const DOM::Element& el, std::vector<SmartPtr<MathMLElement> >& content) const
{
  content.clear();
  DOMX::ChildElementsIterator iter(el, MATHML_NS_URI);
  while (DOM::Element e = iter.element())
    content.push_back(getMathMLElement(e));
}

void
gmetadom_Builder::getChildBoxMLElements(const DOM::Element& el, std::vector<SmartPtr<BoxMLElement> >& content) const
{
  content.clear();
  DOMX::ChildElementsIterator iter(el, BOXML_NS_URI);
  while (DOM::Element e = iter.element())
    content.push_back(getBoxMLElement(e));
}

void
gmetadom_Builder::getChildMathMLTextNodes(const DOM::Element& el, std::vector<SmartPtr<MathMLTextNode> >& content) const
{
  content.clear();
  DOMX::ChildNodesIterator iter(el);
  while (DOM::Node n = iter.node())
    if (SmartPtr<MathMLTextNode> text = getMathMLTextNode(n))
      content.push_back(text);
    else
      {
	std::string name = n.get_nodeName();
	Globals::logger(LOG_WARNING, "unknown or invalid text node with name %s\n", name.c_str());
      }
}

/////////////////////////////////
// AUXILIARY CONSTRUCTION METHODS
/////////////////////////////////

void
gmetadom_Builder::constructMathMLLinearContainerElement(const DOM::Element& el,
							const SmartPtr<MathMLLinearContainerElement>& elem) const
{
  std::vector<SmartPtr<MathMLElement> > content;
  getChildMathMLElements(el, content);
  elem->swapContent(content);
}

void
gmetadom_Builder::constructMathMLNormalizingContainerElement(const DOM::Element& el,
							     const SmartPtr<MathMLNormalizingContainerElement>& elem) const
{
  std::vector<SmartPtr<MathMLElement> > content;
  getChildMathMLElements(el, content);

  std::cerr << "constructMathMLNormalizingContainerElement: " << el.get_nodeName() << " content = " << content.size() << std::endl;

  if (content.size() == 1)
    elem->setChild(content[0]);
  else
    {
      SmartPtr<MathMLRowElement> row = MathMLRowElement::create(mathmlContext);
      row->swapContent(content);
      elem->setChild(row);
    }
}

void
gmetadom_Builder::constructMathMLTokenElement(const DOM::Element& el, const SmartPtr<MathMLTokenElement>& elem) const
{
  std::vector<SmartPtr<MathMLTextNode> > content;
  getChildMathMLTextNodes(el, content);
  elem->swapContent(content); // should normalize spaces etc.
}

void
gmetadom_Builder::constructMathMLEmptyElement(const DOM::Element& el, const SmartPtr<MathMLElement>& elem) const
{
  // nothing to do or issue a warning if the content is not empty
}

void
gmetadom_Builder::constructBoxMLEmptyElement(const DOM::Element& el, const SmartPtr<BoxMLElement>& elem) const
{
  // nothing to do or issue a warning if the content is not empty
}

void
gmetadom_Builder::constructBoxMLBinContainerElement(const DOM::Element& el,
						    const SmartPtr<BoxMLBinContainerElement>& elem) const
{
  DOMX::ChildElementsIterator iter(el, BOXML_NS_URI);
  elem->setChild(getBoxMLElement(iter.element()));
}

void
gmetadom_Builder::constructBoxMLLinearContainerElement(const DOM::Element& el,
						       const SmartPtr<BoxMLLinearContainerElement>& elem) const
{
  std::vector<SmartPtr<BoxMLElement> > content;
  getChildBoxMLElements(el, content);
  elem->swapContent(content);
}

///////////////////////////////////
// PER-ELEMENT CONSTRUCTION METHODS
///////////////////////////////////

void
gmetadom_Builder::construct_MathML_mfrac_Element(const DOM::Element& el, const SmartPtr<MathMLFractionElement>& elem) const
{
  DOMX::ChildElementsIterator iter(el, MATHML_NS_URI);
  elem->setNumerator(getMathMLElement(iter.element()));
  elem->setDenominator(getMathMLElement(iter.element()));
}

void
gmetadom_Builder::construct_MathML_mroot_Element(const DOM::Element& el, const SmartPtr<MathMLRadicalElement>& elem) const
{
  DOMX::ChildElementsIterator iter(el, MATHML_NS_URI);
  elem->setBase(getMathMLElement(iter.element()));
  elem->setIndex(getMathMLElement(iter.element()));
}

void
gmetadom_Builder::construct_MathML_msqrt_Element(const DOM::Element& el, const SmartPtr<MathMLRadicalElement>& elem) const
{
  std::vector<SmartPtr<MathMLElement> > content;
  getChildMathMLElements(el, content);
  if (content.size() == 1)
    elem->setBase(content[0]);
  else
    {
      SmartPtr<MathMLRowElement> row = MathMLRowElement::create(mathmlContext);
      row->swapContent(content);
      elem->setBase(row);
    }
  elem->setIndex(0);
}

void
gmetadom_Builder::construct_MathML_msub_Element(const DOM::Element& el, const SmartPtr<MathMLScriptElement>& elem) const
{
  DOMX::ChildElementsIterator iter(el, MATHML_NS_URI);
  elem->setBase(getMathMLElement(iter.element()));
  elem->setSubScript(getMathMLElement(iter.element()));
  elem->setSuperScript(0);
}

void
gmetadom_Builder::construct_MathML_msup_Element(const DOM::Element& el, const SmartPtr<MathMLScriptElement>& elem) const
{
  DOMX::ChildElementsIterator iter(el, MATHML_NS_URI);
  elem->setBase(getMathMLElement(iter.element()));
  elem->setSubScript(0);
  elem->setSuperScript(getMathMLElement(iter.element()));
}

void
gmetadom_Builder::construct_MathML_msubsup_Element(const DOM::Element& el, const SmartPtr<MathMLScriptElement>& elem) const
{
  DOMX::ChildElementsIterator iter(el, MATHML_NS_URI);
  elem->setBase(getMathMLElement(iter.element()));
  elem->setSubScript(getMathMLElement(iter.element()));
  elem->setSuperScript(getMathMLElement(iter.element()));
}

void
gmetadom_Builder::construct_MathML_munder_Element(const DOM::Element& el,
						  const SmartPtr<MathMLUnderOverElement>& elem) const
{
  DOMX::ChildElementsIterator iter(el, MATHML_NS_URI);
  elem->setBase(getMathMLElement(iter.element()));
  elem->setUnderScript(getMathMLElement(iter.element()));
  elem->setOverScript(0);
}

void
gmetadom_Builder::construct_MathML_mover_Element(const DOM::Element& el,
						 const SmartPtr<MathMLUnderOverElement>& elem) const
{
  DOMX::ChildElementsIterator iter(el, MATHML_NS_URI);
  elem->setBase(getMathMLElement(iter.element()));
  elem->setUnderScript(0);
  elem->setOverScript(getMathMLElement(iter.element()));
}

void
gmetadom_Builder::construct_MathML_munderover_Element(const DOM::Element& el,
						      const SmartPtr<MathMLUnderOverElement>& elem) const
{
  DOMX::ChildElementsIterator iter(el, MATHML_NS_URI);
  elem->setBase(getMathMLElement(iter.element()));
  elem->setUnderScript(getMathMLElement(iter.element()));
  elem->setOverScript(getMathMLElement(iter.element()));
}

void
gmetadom_Builder::construct_MathML_mtable_Element(const DOM::Element& el,
						  const SmartPtr<MathMLTableElement>& elem) const
{
  assert(false); // to be implemented
}

void
gmetadom_Builder::construct_MathML_mmultiscripts_Element(const DOM::Element& el,
							 const SmartPtr<MathMLMultiScriptsElement>& elem) const
{
  DOMX::ChildElementsIterator iter(el, MATHML_NS_URI);
  unsigned i = 0;
  unsigned nScripts = 0;
  unsigned nPreScripts = 0;
  bool preScripts = false;

  elem->setBase(getMathMLElement(iter.element()));
  while (DOM::Element node = iter.element())
    {
      String nodeName = node.get_localName();
      if (nodeName == "mprescripts")
	{
	  if (preScripts)
	    Globals::logger(LOG_WARNING, "multiple <mprescripts> elements in mmultiscript");
	  else
	    {
	      if (i % 2 == 1) elem->setSuperScript(nScripts, 0);
	      preScripts = true;
	      i = 0;
	    }
	}
      else
	{
	  if (i % 2 == 0) // sub script
	    {
	      SmartPtr<MathMLElement> sub = (nodeName == "none") ? 0 : getMathMLElement(node);
	      if (preScripts) elem->setPreSubScript(nPreScripts, sub);
	      else elem->setSubScript(nScripts, sub);
	    }
	  else // super script
	    {
	      SmartPtr<MathMLElement> sup = (nodeName == "none") ? 0 : getMathMLElement(node);
	      if (preScripts)
		{
		  elem->setPreSuperScript(nPreScripts, sup);
		  nPreScripts++;
		}
	      else
		{
		  elem->setSuperScript(nScripts, sup);
		  nScripts++;
		}
	    }
	  i++;
	}
    }

  if (preScripts && i % 2 == 1) elem->setPreSuperScript(nPreScripts, 0);
  elem->setScriptsSize(nScripts);
  elem->setPreScriptsSize(nPreScripts);
}

// BoxML

void
gmetadom_Builder::construct_BoxML_par_Element(const DOM::Element& el,
					      const SmartPtr<BoxMLParagraphElement>& elem) const
{
  DOMX::ChildNodesIterator iter(el);
  std::vector<SmartPtr<BoxMLElement> > content;
  while (DOM::Node p = iter.node())
    if (p.get_nodeType() == DOM::Node::TEXT_NODE)
      {
	SmartPtr<BoxMLTextElement> text = BoxMLTextElement::create(boxmlContext);
	text->setContent(p.get_nodeValue());
	content.push_back(text);
      }
    else if (p.get_nodeType() == DOM::Node::ELEMENT_NODE && p.get_namespaceURI() == BOXML_NS_URI)
      content.push_back(getBoxMLElement(p));
  elem->swapContent(content);
}

void
gmetadom_Builder::construct_BoxML_text_Element(const DOM::Element& el,
					       const SmartPtr<BoxMLTextElement>& elem) const
{
  DOMX::ChildNodesIterator iter(el);
  String content;
  while (DOM::Node p = iter.node())
    if (p.get_nodeType() == DOM::Node::TEXT_NODE)
      content += DOMX::fromDOMString(p.get_nodeValue());
  content = trimSpacesLeft(trimSpacesRight(collapseSpaces(content)));
  elem->setContent(content);
}

///////////////////////////////
// AUXILIARY REFINEMENT METHODS
///////////////////////////////

void
gmetadom_Builder::refineMathMLTokenElement(const DOM::Element& el, const SmartPtr<MathMLTokenElement>& elem) const
{
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Token, mathvariant));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Token, mathsize));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Token, mathcolor));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Token, mathbackground));
}

/////////////////////////////////
// PER-ELEMENT REFINEMENT METHODS
/////////////////////////////////

void
gmetadom_Builder::refine_MathML_math_Element(const DOM::Element& el, const SmartPtr<MathMLmathElement>& elem) const
{
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, math, mode));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, math, display));
}

void
gmetadom_Builder::refine_MathML_mo_Element(const DOM::Element& el, const SmartPtr<MathMLOperatorElement>& elem) const
{
  refineMathMLTokenElement(el, elem);
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Operator, form));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Operator, fence));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Operator, separator));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Operator, lspace));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Operator, rspace));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Operator, stretchy));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Operator, symmetric));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Operator, maxsize));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Operator, minsize));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Operator, largeop));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Operator, movablelimits));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Operator, accent));
}

void
gmetadom_Builder::refine_MathML_ms_Element(const DOM::Element& el, const SmartPtr<MathMLStringLitElement>& elem) const
{
  refineMathMLTokenElement(el, elem);
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, StringLit, lquote));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, StringLit, rquote));
}

void
gmetadom_Builder::refine_MathML_mspace_Element(const DOM::Element& el, const SmartPtr<MathMLSpaceElement>& elem) const
{
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Space, width));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Space, height));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Space, depth));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Space, linebreak));
}

void
gmetadom_Builder::refine_MathML_mfrac_Element(const DOM::Element& el, const SmartPtr<MathMLFractionElement>& elem) const
{
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Fraction, numalign));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Fraction, denomalign));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Fraction, linethickness));
}

void
gmetadom_Builder::refine_MathML_msub_Element(const DOM::Element& el, const SmartPtr<MathMLScriptElement>& elem) const
{
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Script, subscriptshift));
}

void
gmetadom_Builder::refine_MathML_msup_Element(const DOM::Element& el, const SmartPtr<MathMLScriptElement>& elem) const
{
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Script, superscriptshift));
}

void
gmetadom_Builder::refine_MathML_msubsup_Element(const DOM::Element& el, const SmartPtr<MathMLScriptElement>& elem) const
{
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Script, subscriptshift));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Script, superscriptshift));
}

void
gmetadom_Builder::refine_MathML_mmultiscripts_Element(const DOM::Element& el,
						      const SmartPtr<MathMLMultiScriptsElement>& elem) const
{
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, MultiScripts, subscriptshift));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, MultiScripts, superscriptshift));
}

void
gmetadom_Builder::refine_MathML_maction_Element(const DOM::Element& el, const SmartPtr<MathMLActionElement>& elem) const
{
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Action, actiontype));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Action, selection));
}

void
gmetadom_Builder::refine_MathML_menclose_Element(const DOM::Element& el, const SmartPtr<MathMLEncloseElement>& elem) const
{
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Enclose, notation));
}

void
gmetadom_Builder::refine_MathML_mpadded_Element(const DOM::Element& el, const SmartPtr<MathMLPaddedElement>& elem) const
{
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Padded, width));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Padded, lspace));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Padded, height));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Padded, depth));
}

void
gmetadom_Builder::refine_MathML_mstyle_Element(const DOM::Element& el, const SmartPtr<MathMLStyleElement>& elem) const
{
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Style, scriptlevel));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Style, displaystyle));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Style, scriptsizemultiplier));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Style, scriptminsize));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Style, mathcolor));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Style, mathbackground));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Style, color));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Style, background));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Style, negativeveryverythickmathspace));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Style, negativeverythickmathspace));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Style, negativethickmathspace));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Style, negativemediummathspace));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Style, negativethinmathspace));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Style, negativeverythinmathspace));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Style, negativeveryverythinmathspace));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Style, veryverythinmathspace));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Style, verythinmathspace));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Style, thinmathspace));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Style, mediummathspace));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Style, thickmathspace));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Style, verythickmathspace));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Style, veryverythickmathspace));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Style, mathsize));
}

void
gmetadom_Builder::refine_MathML_mtable_Element(const DOM::Element& el, const SmartPtr<MathMLTableElement>& elem) const
{
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Table, align));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Table, rowalign));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Table, columnalign));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Table, groupalign));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Table, alignmentscope));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Table, columnwidth));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Table, rowspacing));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Table, columnspacing));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Table, rowlines));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Table, columnlines));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Table, frame));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Table, framespacing));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Table, equalrows));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Table, equalcolumns));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Table, displaystyle));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Table, side));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Table, minlabelspacing));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Table, width));
}

void
gmetadom_Builder::refine_MathML_munder_Element(const DOM::Element& el, const SmartPtr<MathMLUnderOverElement>& elem) const
{
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, UnderOver, accent));
}

void
gmetadom_Builder::refine_MathML_mover_Element(const DOM::Element& el, const SmartPtr<MathMLUnderOverElement>& elem) const
{
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, UnderOver, accent));
}

void
gmetadom_Builder::refine_MathML_munderover_Element(const DOM::Element& el,
						   const SmartPtr<MathMLUnderOverElement>& elem) const
{
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, UnderOver, accentunder));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, UnderOver, accent));
}

void
gmetadom_Builder::refine_MathML_malignmark_Element(const DOM::Element& el,
						   const SmartPtr<MathMLAlignMarkElement>& elem) const
{
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, AlignMark, edge));
}

void
gmetadom_Builder::refine_MathML_maligngroup_Element(const DOM::Element& el,
						    const SmartPtr<MathMLAlignGroupElement>& elem) const
{
  // NO ATTRIBUTES TO REFINE???
}

// BoxML

void
gmetadom_Builder::refine_BoxML_action_Element(const DOM::Element& el,
					      const SmartPtr<BoxMLActionElement>& elem) const
{
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, Action, selection));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, Action, actiontype));
}

void
gmetadom_Builder::refine_BoxML_at_Element(const DOM::Element& el,
					  const SmartPtr<BoxMLAtElement>& elem) const
{
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, At, x));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, At, y));
}

void
gmetadom_Builder::refine_BoxML_g_Element(const DOM::Element& el,
					 const SmartPtr<BoxMLGroupElement>& elem) const
{
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, Text, size));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, Text, color));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, Text, background));
}

void
gmetadom_Builder::refine_BoxML_h_Element(const DOM::Element& el,
					 const SmartPtr<BoxMLHElement>& elem) const
{
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, H, align));
}

void
gmetadom_Builder::refine_BoxML_ink_Element(const DOM::Element& el,
					   const SmartPtr<BoxMLInkElement>& elem) const
{
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, Ink, color));
  refine_BoxML_space_Element(el, elem);
}

void
gmetadom_Builder::refine_BoxML_layout_Element(const DOM::Element& el,
					      const SmartPtr<BoxMLLayoutElement>& elem) const
{
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, Layout, width));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, Layout, height));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, Layout, depth));
}

void
gmetadom_Builder::refine_BoxML_par_Element(const DOM::Element& el,
					   const SmartPtr<BoxMLParagraphElement>& elem) const
{
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, V, align));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, V, minlinespacing));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, Text, width));
}

void
gmetadom_Builder::refine_BoxML_space_Element(const DOM::Element& el,
					     const SmartPtr<BoxMLSpaceElement>& elem) const
{
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, Space, width));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, Space, height));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, Space, depth));
}

void
gmetadom_Builder::refine_BoxML_text_Element(const DOM::Element& el,
					    const SmartPtr<BoxMLTextElement>& elem) const
{
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, Text, size));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, Text, color));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, Text, background));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, Text, width));
}

void
gmetadom_Builder::refine_BoxML_v_Element(const DOM::Element& el,
					 const SmartPtr<BoxMLVElement>& elem) const
{
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, V, align));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, V, enter));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, V, exit));
  refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, V, minlinespacing));
}

/////////////////////////////
// AUXILIARY CREATION METHODS
/////////////////////////////

SmartPtr<MathMLElement>
gmetadom_Builder::createMathMLDummyElement(void) const
{
  return MathMLDummyElement::create(mathmlContext);
}

SmartPtr<BoxMLElement>
gmetadom_Builder::createBoxMLDummyElement(void) const
{
  // return BoxMLDummyElement::create(boxmlContext); is this needed?
  return 0;
}

////////////////////////////
// PER-NODE CREATION METHODS
////////////////////////////

SmartPtr<MathMLTextNode>
gmetadom_Builder::create_MathML_mglyph_Node(const DOM::Element& el) const
{
  assert(el);

  DOM::GdomeString alt        = el.getAttribute("alt");
  DOM::GdomeString fontFamily = el.getAttribute("fontfamily");
  DOM::GdomeString index      = el.getAttribute("index");

  if (alt.empty() || fontFamily.empty() || index.empty()) {
    Globals::logger(LOG_WARNING, "malformed `mglyph' element (some required attribute is missing)\n");
    return MathMLStringNode::create("?");
  }

  SmartPtr<MathMLGlyphNode> glyph = MathMLGlyphNode::create(DOMX::fromDOMString(fontFamily),
							    DOMX::fromDOMString(index),
							    DOMX::fromDOMString(alt));

  return glyph;
}

SmartPtr<MathMLTextNode>
gmetadom_Builder::create_MathML_malignmark_Node(const DOM::Element& el) const
{
  assert(el);

  DOM::GdomeString edge = el.getAttribute("edge");

  TokenId align = T__NOTVALID;

  if (!edge.empty())
    {
      if      (edge == "left") align = T_LEFT;
      else if (edge == "right") align = T_RIGHT;
      else
	{
	  std::string s_edge = edge;
	  Globals::logger(LOG_WARNING,
			  "malformed `malignmark' element, attribute `edge' has invalid value `%s' (ignored)",
			  s_edge.c_str());
	}
    }

  return MathMLMarkNode::create(align);
}

///////////////////////////////
// PER-ELEMENT CREATION METHODS
///////////////////////////////

SmartPtr<MathMLElement>
gmetadom_Builder::create_MathML_math_Element(const DOM::Element& el) const
{
  SmartPtr<MathMLmathElement> elem = MathMLmathElement::create(mathmlContext);
  refine_MathML_math_Element(el, elem);
  constructMathMLNormalizingContainerElement(el, elem);
  return elem;
}

SmartPtr<MathMLElement>
gmetadom_Builder::create_MathML_mi_Element(const DOM::Element& el) const
{
  SmartPtr<MathMLTokenElement> elem = MathMLIdentifierElement::create(mathmlContext);
  refineMathMLTokenElement(el, elem);
  constructMathMLTokenElement(el, elem);
  return elem;
}

SmartPtr<MathMLElement>
gmetadom_Builder::create_MathML_mn_Element(const DOM::Element& el) const
{
  SmartPtr<MathMLTokenElement> elem = MathMLNumberElement::create(mathmlContext);
  refineMathMLTokenElement(el, elem);
  constructMathMLTokenElement(el, elem);
  return elem;
}

SmartPtr<MathMLElement>
gmetadom_Builder::create_MathML_mo_Element(const DOM::Element& el) const
{
  SmartPtr<MathMLOperatorElement> elem = MathMLOperatorElement::create(mathmlContext);
  refine_MathML_mo_Element(el, elem);
  constructMathMLTokenElement(el, elem);
  return elem;
}

SmartPtr<MathMLElement>
gmetadom_Builder::create_MathML_mtext_Element(const DOM::Element& el) const
{
  SmartPtr<MathMLTokenElement> elem = MathMLTextElement::create(mathmlContext);
  refineMathMLTokenElement(el, elem);
  constructMathMLTokenElement(el, elem);
  return elem;
}

SmartPtr<MathMLElement>
gmetadom_Builder::create_MathML_mspace_Element(const DOM::Element& el) const
{
  SmartPtr<MathMLSpaceElement> elem = MathMLSpaceElement::create(mathmlContext);
  refine_MathML_mspace_Element(el, elem);
  constructMathMLEmptyElement(el, elem);
  return elem;
}

SmartPtr<MathMLElement>
gmetadom_Builder::create_MathML_ms_Element(const DOM::Element& el) const
{
  SmartPtr<MathMLStringLitElement> elem = MathMLStringLitElement::create(mathmlContext);
  refine_MathML_ms_Element(el, elem);
  constructMathMLTokenElement(el, elem);
  return elem;
}

SmartPtr<MathMLElement>
gmetadom_Builder::create_MathML_mrow_Element(const DOM::Element& el) const
{
  SmartPtr<MathMLRowElement> elem = MathMLRowElement::create(mathmlContext);
  constructMathMLLinearContainerElement(el, elem);
  return elem;
}

SmartPtr<MathMLElement>
gmetadom_Builder::create_MathML_mfrac_Element(const DOM::Element& el) const
{
  SmartPtr<MathMLFractionElement> elem = MathMLFractionElement::create(mathmlContext);
  refine_MathML_mfrac_Element(el, elem);
  construct_MathML_mfrac_Element(el, elem);
  return elem;
}

SmartPtr<MathMLElement>
gmetadom_Builder::create_MathML_msqrt_Element(const DOM::Element& el) const
{
  SmartPtr<MathMLRadicalElement> elem = MathMLRadicalElement::create(mathmlContext);
  construct_MathML_msqrt_Element(el, elem);
  return elem;
}

SmartPtr<MathMLElement>
gmetadom_Builder::create_MathML_mroot_Element(const DOM::Element& el) const
{
  SmartPtr<MathMLRadicalElement> elem = MathMLRadicalElement::create(mathmlContext);
  construct_MathML_mroot_Element(el, elem);
  return elem;
}

SmartPtr<MathMLElement>
gmetadom_Builder::create_MathML_mstyle_Element(const DOM::Element& el) const
{
  SmartPtr<MathMLStyleElement> elem = MathMLStyleElement::create(mathmlContext);
  refine_MathML_mstyle_Element(el, elem);
  refinementContext.push(el);
  constructMathMLNormalizingContainerElement(el, elem);
  refinementContext.pop();
  return elem;
}

SmartPtr<MathMLElement>
gmetadom_Builder::create_MathML_merror_Element(const DOM::Element& el) const
{
  SmartPtr<MathMLErrorElement> elem = MathMLErrorElement::create(mathmlContext);
  constructMathMLNormalizingContainerElement(el, elem);
  return elem;
}

SmartPtr<MathMLElement>
gmetadom_Builder::create_MathML_mpadded_Element(const DOM::Element& el) const
{
  SmartPtr<MathMLPaddedElement> elem = MathMLPaddedElement::create(mathmlContext);
  refine_MathML_mpadded_Element(el, elem);
  constructMathMLNormalizingContainerElement(el, elem);
  return elem;
}

SmartPtr<MathMLElement>
gmetadom_Builder::create_MathML_mphantom_Element(const DOM::Element& el) const
{
  SmartPtr<MathMLPhantomElement> elem = MathMLPhantomElement::create(mathmlContext);
  constructMathMLNormalizingContainerElement(el, elem);
  return elem;
}

SmartPtr<MathMLElement>
gmetadom_Builder::create_MathML_mfenced_Element(const DOM::Element& el) const
{
  std::vector<SmartPtr<MathMLElement> > content;
  getChildMathMLElements(el, content);

  SmartPtr<MathMLOperatorElement> open = MathMLOperatorElement::create(mathmlContext);
  open->setSize(0);
  open->append(ToString(getAttributeValue(el, ATTRIBUTE_SIGNATURE(MathML, Fenced, open))));
  open->SetFence();

  SmartPtr<MathMLOperatorElement> close = MathMLOperatorElement::create(mathmlContext);
  close->setSize(0);
  close->append(ToString(getAttributeValue(el, ATTRIBUTE_SIGNATURE(MathML, Fenced, close))));
  close->SetFence();
      
  std::vector< SmartPtr<MathMLElement> > outerRowContent;
  outerRowContent.reserve(3);
  outerRowContent.push_back(open);
  if (content.size() == 1)
    outerRowContent.push_back(content[0]);
  else
    {
      String separators = ToString(getAttributeValue(el, ATTRIBUTE_SIGNATURE(MathML, Fenced, separators)));

      std::vector< SmartPtr<MathMLElement> > innerRowContent;
      if (content.size() > 0)
	innerRowContent.reserve(separators.empty() ? content.size() : (2 * content.size() - 1));
      for (unsigned i = 0; i < content.size(); i++)
	{
	  innerRowContent.push_back(content[i]);
	  if (i + 1 < content.size())
	    {
	      SmartPtr<MathMLOperatorElement> sep = MathMLOperatorElement::create(mathmlContext);
	      unsigned offset = (i < separators.length()) ? i : separators.length() - 1;
	      sep->setSize(0);
	      sep->append(separators.substr(offset, 1));
	      sep->SetSeparator();
	      innerRowContent.push_back(sep);
	    }
	}
      SmartPtr<MathMLRowElement> innerRow = MathMLRowElement::create(mathmlContext);
      innerRow->swapContent(innerRowContent);
    }
  outerRowContent.push_back(close);
  
  SmartPtr<MathMLRowElement> outerRow = MathMLRowElement::create(mathmlContext);
  outerRow->swapContent(outerRowContent);

  return outerRow;
}

SmartPtr<MathMLElement>
gmetadom_Builder::create_MathML_msub_Element(const DOM::Element& el) const
{
  SmartPtr<MathMLScriptElement> elem = MathMLScriptElement::create(mathmlContext);
  refine_MathML_msub_Element(el, elem);
  construct_MathML_msub_Element(el, elem);
  return elem;
}

SmartPtr<MathMLElement>
gmetadom_Builder::create_MathML_msup_Element(const DOM::Element& el) const
{
  SmartPtr<MathMLScriptElement> elem = MathMLScriptElement::create(mathmlContext);
  refine_MathML_msup_Element(el, elem);
  construct_MathML_msup_Element(el, elem);
  return elem;
}

SmartPtr<MathMLElement>
gmetadom_Builder::create_MathML_msubsup_Element(const DOM::Element& el) const
{
  SmartPtr<MathMLScriptElement> elem = MathMLScriptElement::create(mathmlContext);
  refine_MathML_msubsup_Element(el, elem);
  construct_MathML_msubsup_Element(el, elem);
  return elem;
}

SmartPtr<MathMLElement>
gmetadom_Builder::create_MathML_munder_Element(const DOM::Element& el) const
{
  SmartPtr<MathMLUnderOverElement> elem = MathMLUnderOverElement::create(mathmlContext);
  refine_MathML_munder_Element(el, elem);
  construct_MathML_munder_Element(el, elem);
  return elem;
}

SmartPtr<MathMLElement>
gmetadom_Builder::create_MathML_mover_Element(const DOM::Element& el) const
{
  SmartPtr<MathMLUnderOverElement> elem = MathMLUnderOverElement::create(mathmlContext);
  refine_MathML_mover_Element(el, elem);
  construct_MathML_mover_Element(el, elem);
  return elem;
}

SmartPtr<MathMLElement>
gmetadom_Builder::create_MathML_munderover_Element(const DOM::Element& el) const
{
  SmartPtr<MathMLUnderOverElement> elem = MathMLUnderOverElement::create(mathmlContext);
  refine_MathML_munderover_Element(el, elem);
  construct_MathML_munderover_Element(el, elem);
  return elem;
}

SmartPtr<MathMLElement>
gmetadom_Builder::create_MathML_mmultiscripts_Element(const DOM::Element& el) const
{
  SmartPtr<MathMLMultiScriptsElement> elem = MathMLMultiScriptsElement::create(mathmlContext);
  refine_MathML_mmultiscripts_Element(el, elem);
  construct_MathML_mmultiscripts_Element(el, elem);
  return elem;
}

SmartPtr<MathMLElement>
gmetadom_Builder::create_MathML_mtable_Element(const DOM::Element& el) const
{
  SmartPtr<MathMLTableElement> elem = MathMLTableElement::create(mathmlContext);
  refine_MathML_mtable_Element(el, elem);
  construct_MathML_mtable_Element(el, elem);
  return elem;
}

SmartPtr<MathMLElement>
gmetadom_Builder::create_MathML_maligngroup_Element(const DOM::Element& el) const
{
  SmartPtr<MathMLAlignGroupElement> elem = MathMLAlignGroupElement::create(mathmlContext);
  refine_MathML_maligngroup_Element(el, elem);
  constructMathMLEmptyElement(el, elem);
  return elem;
}

SmartPtr<MathMLElement>
gmetadom_Builder::create_MathML_malignmark_Element(const DOM::Element& el) const
{
  SmartPtr<MathMLAlignMarkElement> elem = MathMLAlignMarkElement::create(mathmlContext);
  refine_MathML_malignmark_Element(el, elem);
  constructMathMLEmptyElement(el, elem);
  return elem;
}

SmartPtr<MathMLElement>
gmetadom_Builder::create_MathML_maction_Element(const DOM::Element& el) const
{
  SmartPtr<MathMLActionElement> elem = MathMLActionElement::create(mathmlContext);
  refine_MathML_maction_Element(el, elem);
  constructMathMLLinearContainerElement(el, elem);
  return elem;
}

SmartPtr<MathMLElement>
gmetadom_Builder::create_MathML_menclose_Element(const DOM::Element& el) const
{
  SmartPtr<MathMLEncloseElement> elem = MathMLEncloseElement::create(mathmlContext);
  refine_MathML_menclose_Element(el, elem);
  constructMathMLNormalizingContainerElement(el, elem);
  return elem;
}

SmartPtr<MathMLElement>
gmetadom_Builder::create_MathML_semantics_Element(const DOM::Element& el) const
{
  DOMX::ChildElementsIterator iter(el, MATHML_NS_URI);
  if (SmartPtr<MathMLElement> elem = getMathMLElementNoCreate(iter.element()))
    return elem;

  while (DOM::Element e = iter.element())
    if (e.get_localName() == "annotation-xml")
      {
	DOM::GdomeString encoding = e.getAttribute("encoding");
	if (encoding == "MathML-Presentation")
	  {
	    DOMX::ChildElementsIterator innerIter(e, MATHML_NS_URI);
	    return getMathMLElement(innerIter.element());
	  }
#if 0
	else if (encoding == "BoxML")
	  {
	    SmartPtr<MathMLBoxMLAdapter> adapter = MathMLBoxMLAdapter::create(mathmlContext);
	    DOMX::ChildElementsIterator innerIter(e, BOXML_NS_URI);
	    adapter->setChild(getBoxMLElement(innerIter.element()));
	    return adapter;
	  }
#endif
      }

  return createMathMLDummyElement();
}

// BoxML

SmartPtr<BoxMLElement>
gmetadom_Builder::create_BoxML_action_Element(const DOM::Element& el) const
{
  SmartPtr<BoxMLActionElement> elem = BoxMLActionElement::create(boxmlContext);
  refine_BoxML_action_Element(el, elem);
  constructBoxMLLinearContainerElement(el, elem);
  return elem;
}

SmartPtr<BoxMLElement>
gmetadom_Builder::create_BoxML_at_Element(const DOM::Element& el) const
{
  SmartPtr<BoxMLAtElement> elem = BoxMLAtElement::create(boxmlContext);
  refine_BoxML_at_Element(el, elem);
  constructBoxMLBinContainerElement(el, elem);
  return elem;
}

SmartPtr<BoxMLElement>
gmetadom_Builder::create_BoxML_g_Element(const DOM::Element& el) const
{
  SmartPtr<BoxMLGroupElement> elem = BoxMLGroupElement::create(boxmlContext);
  constructBoxMLLinearContainerElement(el, elem);
  refine_BoxML_g_Element(el, elem);
  return elem;
}

SmartPtr<BoxMLElement>
gmetadom_Builder::create_BoxML_h_Element(const DOM::Element& el) const
{
  SmartPtr<BoxMLHElement> elem = BoxMLHElement::create(boxmlContext);
  refine_BoxML_h_Element(el, elem);
  constructBoxMLLinearContainerElement(el, elem);
  return elem;
}

SmartPtr<BoxMLElement>
gmetadom_Builder::create_BoxML_ink_Element(const DOM::Element& el) const
{
  SmartPtr<BoxMLInkElement> elem = BoxMLInkElement::create(boxmlContext);
  refine_BoxML_ink_Element(el, elem);
  constructBoxMLEmptyElement(el, elem);
  return elem;
}

SmartPtr<BoxMLElement>
gmetadom_Builder::create_BoxML_layout_Element(const DOM::Element& el) const
{
  SmartPtr<BoxMLLayoutElement> elem = BoxMLLayoutElement::create(boxmlContext);
  refine_BoxML_layout_Element(el, elem);
  constructBoxMLLinearContainerElement(el, elem);
  return elem;
}

SmartPtr<BoxMLElement>
gmetadom_Builder::create_BoxML_obj_Element(const DOM::Element& el) const
{
  String encoding = el.getAttribute("encoding");
  if (encoding == "BoxML")
    {
      DOMX::ChildElementsIterator iter(el, BOXML_NS_URI);
      return getBoxMLElement(iter.element());
    }
  else if (encoding == "MathML-Presentation")
    {
      SmartPtr<BoxMLMathMLAdapter> adapter = BoxMLMathMLAdapter::create(boxmlContext);
      DOMX::ChildElementsIterator iter(el, MATHML_NS_URI);
      adapter->setChild(getMathMLElement(iter.element()));
      return adapter;
    }
  else
    return createBoxMLDummyElement();
}

SmartPtr<BoxMLElement>
gmetadom_Builder::create_BoxML_par_Element(const DOM::Element& el) const
{
  SmartPtr<BoxMLParagraphElement> elem = BoxMLParagraphElement::create(boxmlContext);
  refine_BoxML_par_Element(el, elem);
  construct_BoxML_par_Element(el, elem);
  return elem;
}

SmartPtr<BoxMLElement>
gmetadom_Builder::create_BoxML_space_Element(const DOM::Element& el) const
{
  SmartPtr<BoxMLSpaceElement> elem = BoxMLSpaceElement::create(boxmlContext);
  refine_BoxML_space_Element(el, elem);
  constructBoxMLEmptyElement(el, elem);
  return elem;
}

SmartPtr<BoxMLElement>
gmetadom_Builder::create_BoxML_text_Element(const DOM::Element& el) const
{
  SmartPtr<BoxMLTextElement> elem = BoxMLTextElement::create(boxmlContext);
  refine_BoxML_text_Element(el, elem);
  construct_BoxML_text_Element(el, elem);
  return elem;
}

SmartPtr<BoxMLElement>
gmetadom_Builder::create_BoxML_v_Element(const DOM::Element& el) const
{
  SmartPtr<BoxMLVElement> elem = BoxMLVElement::create(boxmlContext);
  refine_BoxML_v_Element(el, elem);
  constructBoxMLLinearContainerElement(el, elem);
  return elem;
}

SmartPtr<BoxMLElement>
gmetadom_Builder::create_BoxML_box_Element(const DOM::Element& el) const
{
  SmartPtr<BoxMLboxElement> elem = BoxMLboxElement::create(boxmlContext);
  constructBoxMLBinContainerElement(el, elem);
  return elem;
}
