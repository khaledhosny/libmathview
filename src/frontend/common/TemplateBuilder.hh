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

#ifndef __TemplateBuilder_hh__
#define __TemplateBuilder_hh__

#include <vector>

#include "defs.h"
#include "Clock.hh"
#include "MathML.hh"
#include "MathMLNamespaceContext.hh"
#include "MathMLAttributeSignatures.hh"
#include "BoxML.hh"
#include "BoxMLNamespaceContext.hh"
#include "BoxMLAttributeSignatures.hh"
#include "BoxMLMathMLAdapter.hh"
#include "ValueConversion.hh"
#include "Globals.hh"

template <class Model>
class TemplateBuilder : public Model::Builder
{
protected:
  template <typename ElementBuilder>
  SmartPtr<typename ElementBuilder::type>
  getElement(const typename Model::Element& el) const
  {
    if (SmartPtr<typename ElementBuilder::type> elem = smart_cast<typename ElementBuilder::type>(getLinker().assoc(el)))
      return elem;
    else
      {
	SmartPtr<typename ElementBuilder::type> elem = ElementBuilder::type::create(ElementBuilder::getContext(*this));
	getLinker().add(el, elem);
	return elem;
      }
  }

  template <typename ElementBuilder>
  SmartPtr<typename ElementBuilder::target_type>
  updateElement(const typename Model::Element& el) const
  {
    SmartPtr<typename ElementBuilder::type> elem = getElement<ElementBuilder>(el);
#if 0
    std::cerr << "BEFORE"
	      << " updateElement " << static_cast<typename ElementBuilder::type*>(elem)
	      << " dirtyAttribute=" << elem->dirtyAttribute() 
	      << " dirtyAttributeP=" << elem->dirtyAttributeP()
	      << " dirtyStructure=" << elem->dirtyStructure()
	      << " dirtyLayout=" << elem->dirtyLayout() << std::endl;
#endif
    if (elem->dirtyAttribute() || elem->dirtyAttributeP() || elem->dirtyStructure())
      {
	ElementBuilder::begin(*this, el, elem);
	ElementBuilder::refine(*this, el, elem);
	ElementBuilder::construct(*this, el, elem);
	// NOTE: it is necessary to reset the dirtyAttribute flag after
	// construction because construction might set the dirtyAttributeP flag
	elem->resetDirtyStructure();
	elem->resetDirtyAttribute();
	ElementBuilder::end(*this, el, elem);
      }
#if 0
    std::cerr << "AFTER"
	      << " updateElement " << static_cast<typename ElementBuilder::type*>(elem)
	      << " dirtyAttribute=" << elem->dirtyAttribute() 
	      << " dirtyAttributeP=" << elem->dirtyAttributeP()
	      << " dirtyStructure=" << elem->dirtyStructure()
	      << " dirtyLayout=" << elem->dirtyLayout() << std::endl;
#endif
    return elem;
  }

  TemplateBuilder()
  {
    static struct
    {
      String tag;
      MathMLCreationMethod create;
    } mathml_tab[] = {
      { "math",          &TemplateBuilder::updateElement<MathML_math_ElementBuilder> },
      { "mi",            &TemplateBuilder::updateElement<MathML_mi_ElementBuilder> },
      { "mn",            &TemplateBuilder::updateElement<MathML_mn_ElementBuilder> },
      { "mo",            &TemplateBuilder::updateElement<MathML_mo_ElementBuilder> },
      { "mtext",         &TemplateBuilder::updateElement<MathML_mtext_ElementBuilder> },
      { "mspace",        &TemplateBuilder::updateElement<MathML_mspace_ElementBuilder> },
      { "ms",            &TemplateBuilder::updateElement<MathML_ms_ElementBuilder> },
      { "mrow",          &TemplateBuilder::updateElement<MathML_mrow_ElementBuilder> },
      { "mfrac",         &TemplateBuilder::updateElement<MathML_mfrac_ElementBuilder> },
      { "msqrt",         &TemplateBuilder::updateElement<MathML_msqrt_ElementBuilder> },
      { "mroot",         &TemplateBuilder::updateElement<MathML_mroot_ElementBuilder> },
      { "mstyle",        &TemplateBuilder::updateElement<MathML_mstyle_ElementBuilder> },
      { "merror",        &TemplateBuilder::updateElement<MathML_merror_ElementBuilder> },
      { "mpadded",       &TemplateBuilder::updateElement<MathML_mpadded_ElementBuilder> },
      { "mphantom",      &TemplateBuilder::updateElement<MathML_mphantom_ElementBuilder> },
      { "mfenced",       &TemplateBuilder::update_MathML_mfenced_Element },
      { "msub",          &TemplateBuilder::updateElement<MathML_msub_ElementBuilder> },
      { "msup",          &TemplateBuilder::updateElement<MathML_msup_ElementBuilder> },
      { "msubsup",       &TemplateBuilder::updateElement<MathML_msubsup_ElementBuilder> },
      { "munder",        &TemplateBuilder::updateElement<MathML_munder_ElementBuilder> },
      { "mover",         &TemplateBuilder::updateElement<MathML_mover_ElementBuilder> },
      { "munderover",    &TemplateBuilder::updateElement<MathML_munderover_ElementBuilder> },
      { "mmultiscripts", &TemplateBuilder::updateElement<MathML_mmultiscripts_ElementBuilder> },
      { "mtable",        &TemplateBuilder::updateElement<MathML_mtable_ElementBuilder> },
      { "maligngroup",   &TemplateBuilder::updateElement<MathML_maligngroup_ElementBuilder> },
      { "malignmark",    &TemplateBuilder::updateElement<MathML_malignmark_ElementBuilder> },
      { "maction",       &TemplateBuilder::updateElement<MathML_maction_ElementBuilder> },
      { "menclose",      &TemplateBuilder::updateElement<MathML_menclose_ElementBuilder> },
      { "semantics",     &TemplateBuilder::update_MathML_semantics_Element },

      { "",              0 }
    };

    for (unsigned i = 0; mathml_tab[i].create; i++)
      mathmlMap[mathml_tab[i].tag] = mathml_tab[i].create;

    static struct
    {
      String tag;
      BoxMLCreationMethod create;
    } boxml_tab[] = {
      { "at",            &TemplateBuilder::updateElement<BoxML_at_ElementBuilder> },
      { "layout",        &TemplateBuilder::updateElement<BoxML_layout_ElementBuilder> },
      { "h",             &TemplateBuilder::updateElement<BoxML_h_ElementBuilder> },
      { "ink",           &TemplateBuilder::updateElement<BoxML_ink_ElementBuilder> },
      { "space",         &TemplateBuilder::updateElement<BoxML_space_ElementBuilder> },
      { "text",          &TemplateBuilder::updateElement<BoxML_text_ElementBuilder> },
      { "v",             &TemplateBuilder::updateElement<BoxML_v_ElementBuilder> },
      { "box",           &TemplateBuilder::updateElement<BoxML_box_ElementBuilder> },
      { "action",        &TemplateBuilder::updateElement<BoxML_action_ElementBuilder> },
      { "obj",           &TemplateBuilder::update_BoxML_obj_Element },
      { "par",           &TemplateBuilder::updateElement<BoxML_par_ElementBuilder> },
      { "",              0 }
    };

    for (unsigned i = 0; boxml_tab[i].create; i++)
      boxmlMap[boxml_tab[i].tag] = boxml_tab[i].create;
  }

  ////////////////////////////////////
  // SPECIALIZED MATHML UPDATE METHODS
  ////////////////////////////////////

  SmartPtr<MathMLElement>
  update_MathML_mfenced_Element(const typename Model::Element& el) const
  {
    String open = ToString(getAttributeValue(el, ATTRIBUTE_SIGNATURE(MathML, Fenced, open)));
    String close = ToString(getAttributeValue(el, ATTRIBUTE_SIGNATURE(MathML, Fenced, close)));
    String separators = ToString(getAttributeValue(el, ATTRIBUTE_SIGNATURE(MathML, Fenced, separators)));

    std::vector<SmartPtr<MathMLElement> > content;
    getChildMathMLElements(el, content);

    SmartPtr<MathMLOperatorElement> openElem = MathMLOperatorElement::create(getMathMLNamespaceContext());
    openElem->setSize(0);
    openElem->append(open);
    openElem->SetFence();

    SmartPtr<MathMLOperatorElement> closeElem = MathMLOperatorElement::create(getMathMLNamespaceContext());
    closeElem->setSize(0);
    closeElem->append(close);
    closeElem->SetFence();
      
    std::vector< SmartPtr<MathMLElement> > outerRowContent;
    outerRowContent.reserve(3);
    outerRowContent.push_back(openElem);
    if (content.size() == 1)
      outerRowContent.push_back(content[0]);
    else
      {
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
	SmartPtr<MathMLRowElement> innerRow = MathMLRowElement::create(getMathMLNamespaceContext());
	innerRow->swapContent(innerRowContent);
      }
    outerRowContent.push_back(closeElem);

    // this can probably remain attached to the model
    SmartPtr<MathMLRowElement> outerRow = MathMLRowElement::create(getMathMLNamespaceContext());
    outerRow->swapContent(outerRowContent);

    return outerRow;
  }

  SmartPtr<MathMLElement>
  update_MathML_semantics_Element(const typename Model::Element& el) const
  {
    typename Model::ElementIterator iter(el, MATHML_NS_URI);
    if (SmartPtr<MathMLElement> elem = getMathMLElementNoCreate(iter.element()))
      return elem;

    if (iter.more()) iter.next();
    while (typename Model::Element e = iter.element())
      {
	if (e.get_localName() == "annotation-xml")
	  {
	    String encoding = e.getAttribute("encoding");
	    if (encoding == "MathML-Presentation")
	      return getMathMLElement(typename Model::ElementIterator(e, MATHML_NS_URI).element());
#if 0
	    else if (encoding == "BoxML")
	      {
		// this element can probably be associated with the model element
		SmartPtr<MathMLBoxMLAdapter> adapter = getMathMLElement<MathMLBoxMLAdapter>(el);
		adapter->setChild(getBoxMLElement(typename Model::ElementIterator(e, BOXML_NS_URI).element()));
		return adapter;
	      }
#endif
	  }
	iter.next();
      }

    return createMathMLDummyElement();
  }

  SmartPtr<MathMLTextNode>
  update_MathML_mglyph_Node(const typename Model::Element& el) const
  {
    assert(el);
    
    String alt        = el.getAttribute("alt");
    String fontFamily = el.getAttribute("fontfamily");
    String index      = el.getAttribute("index");
    
    if (alt.empty() || fontFamily.empty() || index.empty())
      {
	Globals::logger(LOG_WARNING, "malformed `mglyph' element (some required attribute is missing)\n");
	return MathMLStringNode::create("?");
      }
    
    return MathMLGlyphNode::create(fontFamily, index, alt);
  }

  SmartPtr<MathMLTextNode>
  update_MathML_malignmark_Node(const typename Model::Element& el) const
  {
    assert(el);
    
    String edge = el.getAttribute("edge");
    
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

  ///////////////////////////////////
  // SPECIALIZED BOXML UPDATE METHODS
  ///////////////////////////////////

  SmartPtr<BoxMLElement>
  update_BoxML_obj_Element(const typename Model::Element& el) const
  {
    String encoding = el.getAttribute("encoding");
    if (encoding == "BoxML")
      return getBoxMLElement(typename Model::ElementIterator(el, BOXML_NS_URI).element());
    else if (encoding == "MathML-Presentation")
      {
	// this element can be associated to the corresponding model element
	SmartPtr<BoxMLMathMLAdapter> adapter = getElement<BoxMLMathMLAdapterBuilder>(el);
	adapter->setChild(getMathMLElement(typename Model::ElementIterator(el, MATHML_NS_URI).element()));
	return adapter;
      }
    else
      return createBoxMLDummyElement();
  }

  //////////////////
  // MATHML BUILDERS
  //////////////////

  struct MathMLElementBuilder
  {
    typedef MathMLElement target_type;

    static SmartPtr<MathMLNamespaceContext>
    getContext(const TemplateBuilder& builder)
    { return builder.getMathMLNamespaceContext(); }

    static void
    begin(const TemplateBuilder&, const typename Model::Node&, const SmartPtr<MathMLElement>&)
    { }

    static void
    end(const TemplateBuilder&, const typename Model::Node&, const SmartPtr<MathMLElement>&)
    { }

    static void
    refine(const TemplateBuilder&, const typename Model::Node&, const SmartPtr<MathMLElement>&)
    { }

    static void
    construct(const TemplateBuilder&, const typename Model::Node&, const SmartPtr<MathMLElement>&)
    { }
  };

  struct MathMLBinContainerElementBuilder : public MathMLElementBuilder
  {
    static void
    construct(const TemplateBuilder& builder, const typename Model::Node& el, const SmartPtr<MathMLBinContainerElement>& elem)
    { elem->setChild(builder.getMathMLElement(el)); }
  };

  struct MathMLNormalizingContainerElementBuilder : public MathMLBinContainerElementBuilder
  {
    static void
    construct(const TemplateBuilder& builder, const typename Model::Node& el, const SmartPtr<MathMLNormalizingContainerElement>& elem)
    {
      std::vector<SmartPtr<MathMLElement> > content;
      builder.getChildMathMLElements(el, content);
    
      if (content.size() == 1)
	elem->setChild(content[0]);
      else
	{
	  SmartPtr<MathMLInferredRowElement> row = MathMLInferredRowElement::create(builder.getMathMLNamespaceContext());
	  row->swapContent(content);
	  elem->setChild(row);
	}
    }
  };

  struct MathMLLinearContainerElementBuilder : public MathMLElementBuilder
  {
    static void construct(const TemplateBuilder& builder, const typename Model::Node& el, const SmartPtr<MathMLLinearContainerElement>& elem)
    {
      std::vector<SmartPtr<MathMLElement> > content;
      builder.getChildMathMLElements(el, content);
      elem->swapContent(content);
    }
  };

  struct MathMLTokenElementBuilder : public MathMLElementBuilder
  {
    static void
    refine(const TemplateBuilder& builder, const typename Model::Node& el, const SmartPtr<MathMLTokenElement>& elem)
    {
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Token, mathvariant));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Token, mathsize));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Token, mathcolor));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Token, mathbackground));
    }

    static void
    construct(const TemplateBuilder& builder, const typename Model::Node& el, const SmartPtr<MathMLTokenElement>& elem)
    {
      std::vector<SmartPtr<MathMLTextNode> > content;
      builder.getChildMathMLTextNodes(el, content);
      elem->swapContent(content); // should normalize spaces etc.
    }
  };

  struct MathML_mi_ElementBuilder : public MathMLTokenElementBuilder
  { typedef MathMLIdentifierElement type; };

  struct MathML_mn_ElementBuilder : public MathMLTokenElementBuilder
  { typedef MathMLNumberElement type; };

  struct MathML_mo_ElementBuilder : public MathMLTokenElementBuilder
  {
    typedef MathMLOperatorElement type;

    static void
    refine(const TemplateBuilder& builder, const typename Model::Node& el, const SmartPtr<MathMLOperatorElement>& elem)
    {
      MathMLTokenElementBuilder::refine(builder, el, elem);
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Operator, form));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Operator, fence));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Operator, separator));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Operator, lspace));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Operator, rspace));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Operator, stretchy));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Operator, symmetric));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Operator, maxsize));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Operator, minsize));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Operator, largeop));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Operator, movablelimits));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Operator, accent));
    }
  };

  struct MathML_mtext_ElementBuilder : public MathMLTokenElementBuilder
  { typedef MathMLTextElement type; };

  struct MathML_ms_ElementBuilder : public MathMLTokenElementBuilder
  { 
    typedef MathMLStringLitElement type;

    static void
    refine(const TemplateBuilder& builder, const typename Model::Node& el, const SmartPtr<MathMLStringLitElement>& elem)
    {
      MathMLTokenElementBuilder::refine(builder, el, elem);
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, StringLit, lquote));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, StringLit, rquote));
    }
  };

  struct MathML_mspace_ElementBuilder : public MathMLElementBuilder
  {
    typedef MathMLSpaceElement type;

    static void
    refine(const TemplateBuilder& builder, const typename Model::Node& el, const SmartPtr<MathMLSpaceElement>& elem)
    {
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Space, width));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Space, height));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Space, depth));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Space, linebreak));
    }
  };

  struct MathML_mrow_ElementBuilder : public MathMLLinearContainerElementBuilder
  { typedef MathMLRowElement type; };

  struct MathML_mstyle_ElementBuilder : public MathMLNormalizingContainerElementBuilder
  {
    typedef MathMLStyleElement type;

    static void
    begin(const TemplateBuilder& builder, const typename Model::Node& el, const SmartPtr<MathMLStyleElement>&)
    { builder.refinementContext.push(el); }

    static void
    end(const TemplateBuilder& builder, const typename Model::Node&, const SmartPtr<MathMLStyleElement>&)
    { builder.refinementContext.pop(); }

    static void
    refine(const TemplateBuilder& builder, const typename Model::Node& el, const SmartPtr<MathMLStyleElement>& elem)
    {
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Style, scriptlevel));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Style, displaystyle));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Style, scriptsizemultiplier));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Style, scriptminsize));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Style, mathcolor));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Style, mathbackground));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Style, color));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Style, background));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Style, negativeveryverythickmathspace));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Style, negativeverythickmathspace));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Style, negativethickmathspace));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Style, negativemediummathspace));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Style, negativethinmathspace));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Style, negativeverythinmathspace));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Style, negativeveryverythinmathspace));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Style, veryverythinmathspace));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Style, verythinmathspace));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Style, thinmathspace));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Style, mediummathspace));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Style, thickmathspace));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Style, verythickmathspace));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Style, veryverythickmathspace));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Style, mathsize));
    }
  };

  struct MathML_merror_ElementBuilder : public MathMLNormalizingContainerElementBuilder
  { typedef MathMLErrorElement type;
  };

  struct MathML_mpadded_ElementBuilder : public MathMLNormalizingContainerElementBuilder
  {
    typedef MathMLPaddedElement type;
    
    static void
    refine(const TemplateBuilder& builder, const typename Model::Node& el, const SmartPtr<MathMLPaddedElement>& elem)
    {
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Padded, width));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Padded, lspace));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Padded, height));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Padded, depth));
    }
  };

  struct MathML_mphantom_ElementBuilder : public MathMLNormalizingContainerElementBuilder
  { typedef MathMLPhantomElement type; };

  struct MathML_maligngroup_ElementBuilder : public MathMLElementBuilder
  { 
    typedef MathMLAlignGroupElement type;

    static void
    refine(const TemplateBuilder& builder, const typename Model::Node& el, const SmartPtr<MathMLAlignGroupElement>& elem)
    {
      // NO ATTRIBUTES TO REFINE???
    }
  };

  struct MathML_malignmark_ElementBuilder : public MathMLElementBuilder
  {
    typedef MathMLAlignMarkElement type;

    static void
    refine(const TemplateBuilder& builder, const typename Model::Node& el, const SmartPtr<MathMLAlignMarkElement>& elem)
    { builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, AlignMark, edge)); }
  };

  struct MathML_maction_ElementBuilder : public MathMLLinearContainerElementBuilder
  { 
    typedef MathMLActionElement type;

    static void
    refine(const TemplateBuilder& builder, const typename Model::Node& el, const SmartPtr<MathMLActionElement>& elem)
    {
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Action, actiontype));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Action, selection));
    }
  };

  struct MathML_menclose_ElementBuilder : public MathMLNormalizingContainerElementBuilder
  {
    typedef MathMLEncloseElement type;

    static void
    refine(const TemplateBuilder& builder, const typename Model::Node& el, const SmartPtr<MathMLEncloseElement>& elem)
    { builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Enclose, notation)); }
  };

  struct MathML_math_ElementBuilder : public MathMLNormalizingContainerElementBuilder
  {
    typedef MathMLmathElement type;

    static void
    refine(const TemplateBuilder& builder, const typename Model::Node& el, const SmartPtr<MathMLmathElement>& elem)
    {
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, math, mode));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, math, display));
    }
  };

  struct MathML_mfrac_ElementBuilder : public MathMLElementBuilder
  {
    typedef MathMLFractionElement type;

    static void
    refine(const TemplateBuilder& builder, const typename Model::Node& el, const SmartPtr<MathMLFractionElement>& elem)
    {
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Fraction, numalign));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Fraction, denomalign));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Fraction, linethickness));
    }

    static void
    construct(const TemplateBuilder& builder, const typename Model::Node& el, const SmartPtr<MathMLFractionElement>& elem)
    {
      typename Model::ElementIterator iter(el, MATHML_NS_URI);
      elem->setNumerator(builder.getMathMLElement(iter.element()));
      if (iter.more()) iter.next();
      elem->setDenominator(builder.getMathMLElement(iter.element()));
    }
  };

  struct MathML_mroot_ElementBuilder : public MathMLElementBuilder
  {
    typedef MathMLRadicalElement type;

    static void
    construct(const TemplateBuilder& builder, const typename Model::Node& el, const SmartPtr<MathMLRadicalElement>& elem)
    {
      typename Model::ElementIterator iter(el, MATHML_NS_URI);
      elem->setBase(builder.getMathMLElement(iter.element()));
      if (iter.more()) iter.next();
      elem->setIndex(builder.getMathMLElement(iter.element()));
    }
  };

  struct MathML_msqrt_ElementBuilder : public MathMLElementBuilder
  {
    typedef MathMLRadicalElement type;

    static void
    construct(const TemplateBuilder& builder, const typename Model::Node& el, const SmartPtr<MathMLRadicalElement>& elem)
    {
      std::vector<SmartPtr<MathMLElement> > content;
      builder.getChildMathMLElements(el, content);
      if (content.size() == 1)
	elem->setBase(content[0]);
      else
	{
	  SmartPtr<MathMLInferredRowElement> row = MathMLInferredRowElement::create(builder.getMathMLNamespaceContext());
	  row->swapContent(content);
	  elem->setBase(row);
	}
      elem->setIndex(0);
    }
  };

  struct MathML_msub_ElementBuilder : public MathMLElementBuilder
  {
    typedef MathMLScriptElement type;

    static void
    refine(const TemplateBuilder& builder, const typename Model::Node& el, const SmartPtr<MathMLScriptElement>& elem)
    { builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Script, subscriptshift)); }

    static void
    construct(const TemplateBuilder& builder, const typename Model::Node& el, const SmartPtr<MathMLScriptElement>& elem)
    {
      typename Model::ElementIterator iter(el, MATHML_NS_URI);
      elem->setBase(builder.getMathMLElement(iter.element()));
      if (iter.more()) iter.next();
      elem->setSubScript(builder.getMathMLElement(iter.element()));
      elem->setSuperScript(0);
    }
  };

  struct MathML_msup_ElementBuilder : public MathMLElementBuilder
  {
    typedef MathMLScriptElement type;

    static void
    refine(const TemplateBuilder& builder, const typename Model::Node& el, const SmartPtr<MathMLScriptElement>& elem)
    { builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Script, superscriptshift)); }

    static void
    construct(const TemplateBuilder& builder, const typename Model::Node& el, const SmartPtr<MathMLScriptElement>& elem)
    {
      typename Model::ElementIterator iter(el, MATHML_NS_URI);
      elem->setBase(builder.getMathMLElement(iter.element()));
      if (iter.more()) iter.next();
      elem->setSubScript(0);
      elem->setSuperScript(builder.getMathMLElement(iter.element()));
    }
  };
  
  struct MathML_msubsup_ElementBuilder : public MathMLElementBuilder
  {
    typedef MathMLScriptElement type;

    static void
    refine(const TemplateBuilder& builder, const typename Model::Node& el, const SmartPtr<MathMLScriptElement>& elem)
    {
      MathML_msub_ElementBuilder::refine(builder, el, elem);
      MathML_msup_ElementBuilder::refine(builder, el, elem);
    }

    static void
    construct(const TemplateBuilder& builder, const typename Model::Node& el, const SmartPtr<MathMLScriptElement>& elem)
    {
      typename Model::ElementIterator iter(el, MATHML_NS_URI);
      elem->setBase(builder.getMathMLElement(iter.element()));
      if (iter.more()) iter.next();
      elem->setSubScript(builder.getMathMLElement(iter.element()));
      if (iter.more()) iter.next();
      elem->setSuperScript(builder.getMathMLElement(iter.element()));
    }
  };

  struct MathML_munder_ElementBuilder : public MathMLElementBuilder
  {
    typedef MathMLUnderOverElement type;

    static void
    refine(const TemplateBuilder& builder, const typename Model::Node& el, const SmartPtr<MathMLUnderOverElement>& elem)
    { builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, UnderOver, accent)); }

    static void
    construct(const TemplateBuilder& builder, const typename Model::Node& el, const SmartPtr<MathMLUnderOverElement>& elem)
    {
      typename Model::ElementIterator iter(el, MATHML_NS_URI);
      elem->setBase(builder.getMathMLElement(iter.element()));
      if (iter.more()) iter.next();
      elem->setUnderScript(builder.getMathMLElement(iter.element()));
      elem->setOverScript(0);
    }
  };

  struct MathML_mover_ElementBuilder : public MathMLElementBuilder
  {
    typedef MathMLUnderOverElement type;

    static void
    refine(const TemplateBuilder& builder, const typename Model::Node& el, const SmartPtr<MathMLUnderOverElement>& elem)
    { builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, UnderOver, accent)); }

    static void
    construct(const TemplateBuilder& builder, const typename Model::Node& el, const SmartPtr<MathMLUnderOverElement>& elem)
    {
      typename Model::ElementIterator iter(el, MATHML_NS_URI);
      elem->setBase(builder.getMathMLElement(iter.element()));
      if (iter.more()) iter.next();
      elem->setUnderScript(0);
      elem->setOverScript(builder.getMathMLElement(iter.element()));
    }
  };

  struct MathML_munderover_ElementBuilder : public MathMLElementBuilder
  {
    typedef MathMLUnderOverElement type;

    static void
    refine(const TemplateBuilder& builder, const typename Model::Node& el, const SmartPtr<MathMLUnderOverElement>& elem)
    {
      MathML_munder_ElementBuilder::refine(builder, el, elem);
      MathML_mover_ElementBuilder::refine(builder, el, elem);
    }

    static void
    construct(const TemplateBuilder& builder, const typename Model::Node& el, const SmartPtr<MathMLUnderOverElement>& elem)
    {
      typename Model::ElementIterator iter(el, MATHML_NS_URI);
      elem->setBase(builder.getMathMLElement(iter.element()));
      if (iter.more()) iter.next();
      elem->setUnderScript(builder.getMathMLElement(iter.element()));
      if (iter.more()) iter.next();
      elem->setOverScript(builder.getMathMLElement(iter.element()));
    }
  };

  struct MathML_mtable_ElementBuilder : public MathMLElementBuilder
  {
    typedef MathMLTableElement type;

    static void
    refine(const TemplateBuilder& builder, const typename Model::Node& el, const SmartPtr<MathMLTableElement>& elem)
    {
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Table, align));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Table, rowalign));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Table, columnalign));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Table, groupalign));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Table, alignmentscope));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Table, columnwidth));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Table, rowspacing));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Table, columnspacing));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Table, rowlines));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Table, columnlines));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Table, frame));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Table, framespacing));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Table, equalrows));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Table, equalcolumns));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Table, displaystyle));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Table, side));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Table, minlabelspacing));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Table, width));
    }

    static void
    construct(const TemplateBuilder& builder, const typename Model::Node& el, const SmartPtr<MathMLTableElement>& elem)
    {
      assert(false); // to be implemented
    }
  };

  struct MathML_mmultiscripts_ElementBuilder : public MathMLElementBuilder
  {
    typedef MathMLMultiScriptsElement type;

    static void
    refine(const TemplateBuilder& builder, const typename Model::Node& el, const SmartPtr<MathMLMultiScriptsElement>& elem)
    {
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, MultiScripts, subscriptshift));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, MultiScripts, superscriptshift));
    }

    static void
    construct(const TemplateBuilder& builder, const typename Model::Node& el, const SmartPtr<MathMLMultiScriptsElement>& elem)
    {
      typename Model::ElementIterator iter(el, MATHML_NS_URI);
      unsigned i = 0;
      unsigned nScripts = 0;
      unsigned nPreScripts = 0;
      bool preScripts = false;

      elem->setBase(builder.getMathMLElement(iter.element()));
      if (iter.more()) iter.next();
      while (iter.more())
	{
	  typename Model::Element node = iter.element();
	  assert(node);
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
		  SmartPtr<MathMLElement> sub = (nodeName == "none") ? 0 : builder.getMathMLElement(node);
		  if (preScripts) elem->setPreSubScript(nPreScripts, sub);
		  else elem->setSubScript(nScripts, sub);
		}
	      else // super script
		{
		  SmartPtr<MathMLElement> sup = (nodeName == "none") ? 0 : builder.getMathMLElement(node);
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
  };

  /////////////////
  // BOXML BUILDERS
  /////////////////

  struct BoxMLElementBuilder
  {
    typedef BoxMLElement target_type;

    static SmartPtr<BoxMLNamespaceContext>
    getContext(const TemplateBuilder& builder)
    { return builder.getBoxMLNamespaceContext(); }

    static void
    begin(const TemplateBuilder&, const typename Model::Node&, const SmartPtr<BoxMLElement>&)
    { }

    static void
    end(const TemplateBuilder&, const typename Model::Node&, const SmartPtr<BoxMLElement>&)
    { }

    static void
    refine(const TemplateBuilder&, const typename Model::Node&, const SmartPtr<BoxMLElement>& elem)
    { }

    static void
    construct(const TemplateBuilder&, const typename Model::Node&, const SmartPtr<BoxMLElement>& elem)
    { }
  };

  struct BoxMLMathMLAdapterBuilder : public BoxMLElementBuilder
  { typedef BoxMLMathMLAdapter type; };

  struct BoxMLBinContainerElementBuilder : public BoxMLElementBuilder
  {
    static void
    construct(const TemplateBuilder& builder, const typename Model::Node& el, const SmartPtr<BoxMLBinContainerElement>& elem)
    { elem->setChild(builder.getBoxMLElement(typename Model::ElementIterator(el, BOXML_NS_URI).element())); }
  };

  struct BoxMLLinearContainerElementBuilder : public BoxMLElementBuilder
  {
    static void
    construct(const TemplateBuilder& builder, const typename Model::Node& el, const SmartPtr<BoxMLLinearContainerElement>& elem)
    {
      std::vector<SmartPtr<BoxMLElement> > content;
      builder.getChildBoxMLElements(el, content);
      elem->swapContent(content);
    }
  };

  struct BoxML_box_ElementBuilder : public BoxMLBinContainerElementBuilder
  { typedef BoxMLboxElement type; };

  struct BoxML_action_ElementBuilder : public BoxMLLinearContainerElementBuilder
  {
    typedef BoxMLActionElement type;

    static void
    refine(const TemplateBuilder& builder, const typename Model::Node& el, const SmartPtr<BoxMLActionElement>& elem)
    {
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, Action, selection));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, Action, actiontype));
    }
  };

  struct BoxML_at_ElementBuilder : public BoxMLBinContainerElementBuilder
  {
    typedef BoxMLAtElement type;

    static void
    refine(const TemplateBuilder& builder, const typename Model::Node& el, const SmartPtr<BoxMLAtElement>& elem)
    {
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, At, x));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, At, y));
    }
  };

  struct BoxML_g_ElementBuilder : public BoxMLLinearContainerElementBuilder
  {
    typedef BoxMLGroupElement type;

    static void
    refine(const TemplateBuilder& builder, const typename Model::Node& el, const SmartPtr<BoxMLGroupElement>& elem)
    {
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, Text, size));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, Text, color));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, Text, background));
    }
  };

  struct BoxML_h_ElementBuilder : public BoxMLLinearContainerElementBuilder
  {
    typedef BoxMLHElement type;

    static void
    refine(const TemplateBuilder& builder, const typename Model::Node& el, const SmartPtr<BoxMLHElement>& elem)
    { builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, H, align)); }
  };
  
  struct BoxML_ink_ElementBuilder : public BoxMLElementBuilder
  {
    typedef BoxMLInkElement type;
    
    static void
    refine(const TemplateBuilder& builder, const typename Model::Node& el, const SmartPtr<BoxMLInkElement>& elem)
    {
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, Ink, color));
      BoxML_space_ElementBuilder::refine(builder, el, elem);
    }
  };

  struct BoxML_layout_ElementBuilder : public BoxMLLinearContainerElementBuilder
  {
    typedef BoxMLLayoutElement type;

    static void
    refine(const TemplateBuilder& builder, const typename Model::Node& el, const SmartPtr<BoxMLLayoutElement>& elem)
    {
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, Layout, width));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, Layout, height));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, Layout, depth));
    }
  };

  struct BoxML_par_ElementBuilder : public BoxMLElementBuilder
  {
    typedef BoxMLParagraphElement type;

    static void
    refine(const TemplateBuilder& builder, const typename Model::Node& el, const SmartPtr<BoxMLParagraphElement>& elem)
    {
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, V, align));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, V, minlinespacing));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, Text, width));
    }

    static void
    construct(const TemplateBuilder& builder, const typename Model::Node& el, const SmartPtr<BoxMLParagraphElement>& elem)
    {
      std::vector<SmartPtr<BoxMLElement> > content;
      for (typename Model::NodeIterator iter(el); iter.more(); iter.next())
	{
	  typename Model::Node p = iter.node();
	  assert(p);
	  if (p.get_nodeType() == Model::Node::TEXT_NODE)
	    {
	      SmartPtr<BoxMLTextElement> text = builder.getElement<BoxML_text_ElementBuilder>(el);
	      text->setContent(p.get_nodeValue());
	      content.push_back(text);
	    }
	  else if (p.get_nodeType() == Model::Node::ELEMENT_NODE && p.get_namespaceURI() == BOXML_NS_URI)
	    content.push_back(builder.getBoxMLElement(p));
	}
      elem->swapContent(content);
    }
  };

  struct BoxML_space_ElementBuilder : public BoxMLElementBuilder
  {
    typedef BoxMLSpaceElement type;

    static void
    refine(const TemplateBuilder& builder, const typename Model::Node& el, const SmartPtr<BoxMLSpaceElement>& elem)
    {
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, Space, width));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, Space, height));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, Space, depth));
    }
  };

  struct BoxML_text_ElementBuilder : public BoxMLElementBuilder
  {
    typedef BoxMLTextElement type;
    
    static void
    refine(const TemplateBuilder& builder, const typename Model::Node& el, const SmartPtr<BoxMLTextElement>& elem)
    {
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, Text, size));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, Text, color));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, Text, background));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, Text, width));
    }

    static void
    construct(const TemplateBuilder& builder, const typename Model::Node& el, const SmartPtr<BoxMLTextElement>& elem)
    {
      typename Model::NodeIterator iter(el);
      String content;
      for (typename Model::NodeIterator iter(el); iter.more(); iter.next())
	{
	  typename Model::Node p = iter.node();
	  assert(p);
	  if (p.get_nodeType() == Model::Node::TEXT_NODE)
	    content += p.get_nodeValue();
	}
      content = trimSpacesLeft(trimSpacesRight(collapseSpaces(content)));
      elem->setContent(content);
    }
  };

  struct BoxML_v_ElementBuilder : public BoxMLLinearContainerElementBuilder
  {
    typedef BoxMLVElement type;

    static void
    refine(const TemplateBuilder& builder, const typename Model::Node& el, const SmartPtr<BoxMLVElement>& elem)
    {
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, V, align));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, V, enter));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, V, exit));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, V, minlinespacing));
    }
  };

  ////////////////////////////
  // BUILDER AUXILIARY METHODS
  ////////////////////////////

  SmartPtr<Attribute>
  getAttribute(const typename Model::Element& el, const AttributeSignature& signature) const
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
  getAttributeValue(const typename Model::Element& el, const AttributeSignature& signature) const
  {
    if (SmartPtr<Attribute> attr = getAttribute(el, signature))
      return attr->getValue();
    else
      return 0;
  }

  void
  refineAttribute(const SmartPtr<Element>& elem,
		  const typename Model::Node& el, const AttributeSignature& signature) const
  {
    if (SmartPtr<Attribute> attr = getAttribute(el, signature)) elem->setAttribute(attr);
    else elem->removeAttribute(signature);
  }

  ///////////////////////////////////////
  // BUILDER AUXILIARY METHODS FOR MATHML
  ///////////////////////////////////////

  SmartPtr<MathMLElement>
  getMathMLElementNoCreate(const typename Model::Node& el) const
  {
    if (el)
      {
	//std::cout << "createMathMLElement " << el.get_localName() << std::endl;
	typename MathMLBuilderMap::const_iterator m = mathmlMap.find(el.get_localName());
	if (m != mathmlMap.end()) 
	  return (this->*(m->second))(el);
      }
    return 0;
  }

  SmartPtr<MathMLElement>
  getMathMLElement(const typename Model::Node& el) const
  {
    if (SmartPtr<MathMLElement> elem = getMathMLElementNoCreate(el))
      return elem;
    else
      return createMathMLDummyElement();
  }

  SmartPtr<MathMLTextNode>
  getMathMLTextNode(const typename Model::Node& node) const
  {
    switch (node.get_nodeType())
      {
      case Model::Node::TEXT_NODE:
	{
	  // ok, we have a chunk of text
	  String s = collapseSpaces(node.get_nodeValue());

	  // ...but spaces at the at the beginning (end) are deleted only if this
	  // is the very first (last) chunk in the token.
	  if (!node.get_previousSibling()) s = trimSpacesLeft(s);
	  if (!node.get_nextSibling()) s = trimSpacesRight(s);
	  
	  return MathMLStringNode::create(s);
	}
      break;
      
      case Model::Node::ELEMENT_NODE:
	{	    
	  if (node.get_namespaceURI() == MATHML_NS_URI)
	    {
	      if (node.get_localName() == "mglyph")
		return update_MathML_mglyph_Node(node);
	      else if (node.get_localName() == "malignmark")
		return update_MathML_malignmark_Node(node);
	    }
	}
      break;
      
      default:
	break;
      }
    
    return 0;
  }

  void
  getChildMathMLElements(const typename Model::Node& el, std::vector<SmartPtr<MathMLElement> >& content) const
  {
    content.clear();
    for (typename Model::ElementIterator iter(el, MATHML_NS_URI); iter.more(); iter.next())
      content.push_back(getMathMLElement(iter.element()));
  }

  void
  getChildMathMLTextNodes(const typename Model::Node& el, std::vector<SmartPtr<MathMLTextNode> >& content) const
  {
    content.clear();
    for (typename Model::NodeIterator iter(el); iter.more(); iter.next())
      {
	typename Model::Node n = iter.node();
	assert(n);
	if (SmartPtr<MathMLTextNode> text = getMathMLTextNode(n))
	  content.push_back(text);
	else
	  {
	    std::string name = n.get_nodeName();
	    Globals::logger(LOG_WARNING, "unknown or invalid text node with name %s\n", name.c_str());
	  }
      }
  }

  SmartPtr<MathMLElement>
  createMathMLDummyElement(void) const
  {
    return MathMLDummyElement::create(getMathMLNamespaceContext());
  }

  //////////////////////////////////////
  // BUILDER AUXILIARY METHODS FOR BOXML
  //////////////////////////////////////

  SmartPtr<BoxMLElement>
  getBoxMLElement(const typename Model::Node& el) const
  {
    //std::cerr << "createBoxMLElement " << el.get_localName() << std::endl;
    typename BoxMLBuilderMap::const_iterator m = boxmlMap.find(el.get_localName());
    if (m != boxmlMap.end())
      return (this->*(m->second))(el);
    else
      return createBoxMLDummyElement();
  }

  void
  getChildBoxMLElements(const typename Model::Node& el, std::vector<SmartPtr<BoxMLElement> >& content) const
  {
    content.clear();
    for (typename Model::ElementIterator iter(el, BOXML_NS_URI); iter.more(); iter.next())
      content.push_back(getBoxMLElement(iter.element()));
  }

  SmartPtr<BoxMLElement>
  createBoxMLDummyElement(void) const
  {
    return 0;
  }

public:
  static SmartPtr<typename Model::Builder> create(void) { return new TemplateBuilder(); }

  virtual SmartPtr<Element>
  getRootElement() const
  {
    if (typename Model::Element root = getRootModelElement())
      {
	Clock perf;
	
	perf.Start();
	SmartPtr<Element> res;
	String ns = root.get_namespaceURI();
	if (ns == MATHML_NS_URI) res = getMathMLElement(root);
	else if (ns == BOXML_NS_URI) res = getBoxMLElement(root);
	perf.Stop();
	Globals::logger(LOG_INFO, "build time: %dms", perf());
#if 0
	std::cerr << "FOUND ROOT ELEMENT = " << static_cast<Element*>(res)
		  << " DIRTY? " << res->dirtyLayout() << std::endl;
#endif
	return res;
      }
    else
      return 0;
  }

private:
  typedef SmartPtr<class MathMLElement> (TemplateBuilder::* MathMLCreationMethod)(const typename Model::Element&) const;
  typedef SmartPtr<class BoxMLElement> (TemplateBuilder::* BoxMLCreationMethod)(const typename Model::Element&) const;

  typedef HASH_MAP_NS::hash_map<String, MathMLCreationMethod, StringHash, StringEq> MathMLBuilderMap;
  typedef HASH_MAP_NS::hash_map<String, BoxMLCreationMethod, StringHash, StringEq> BoxMLBuilderMap;

  MathMLBuilderMap mathmlMap;
  BoxMLBuilderMap boxmlMap;

  mutable typename Model::RefinementContext refinementContext;
};

#endif // __TemplateBuilder_hh__
