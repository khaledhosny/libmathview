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

#ifndef __TemplateBuilder_hh__
#define __TemplateBuilder_hh__

#include <vector>

#include "defs.h"
#include "Attribute.hh"
#include "MathML.hh"
#include "MathMLTableContentFactory.hh"
#include "MathMLNamespaceContext.hh"
#include "MathMLAttributeSignatures.hh"
#if GMV_ENABLE_BOXML
  #include "BoxML.hh"
  #include "BoxMLNamespaceContext.hh"
  #include "BoxMLAttributeSignatures.hh"
  #include "BoxMLMathMLAdapter.hh"
  #include "MathMLBoxMLAdapter.hh"
#endif // GMV_ENABLE_BOXML
#include "ValueConversion.hh"
#include "AbstractLogger.hh"
#include "HashMap.hh"
#include "StringHash.hh"

template <class Model, class Builder, class RefinementContext>
class TemplateBuilder : public Builder
{
protected:
  template <typename ElementBuilder>
  SmartPtr<typename ElementBuilder::type>
  getElement(const typename Model::Element& el) const
  {
    if (SmartPtr<typename ElementBuilder::type> elem = smart_cast<typename ElementBuilder::type>(linkerAssoc(el)))
      return elem;
    else
      {
	SmartPtr<typename ElementBuilder::type> elem = ElementBuilder::type::create(ElementBuilder::getContext(*this));
	linkerAdd(el, elem);
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
      MathMLUpdateMethod update;
    } mathml_tab[] = {
      { "math",          &TemplateBuilder::template updateElement<MathML_math_ElementBuilder> },
      { "mi",            &TemplateBuilder::template updateElement<MathML_mi_ElementBuilder> },
      { "mn",            &TemplateBuilder::template updateElement<MathML_mn_ElementBuilder> },
      { "mo",            &TemplateBuilder::template updateElement<MathML_mo_ElementBuilder> },
      { "mtext",         &TemplateBuilder::template updateElement<MathML_mtext_ElementBuilder> },
      { "mspace",        &TemplateBuilder::template updateElement<MathML_mspace_ElementBuilder> },
      { "ms",            &TemplateBuilder::template updateElement<MathML_ms_ElementBuilder> },
      { "mrow",          &TemplateBuilder::template updateElement<MathML_mrow_ElementBuilder> },
      { "mfrac",         &TemplateBuilder::template updateElement<MathML_mfrac_ElementBuilder> },
      { "msqrt",         &TemplateBuilder::template updateElement<MathML_msqrt_ElementBuilder> },
      { "mroot",         &TemplateBuilder::template updateElement<MathML_mroot_ElementBuilder> },
      { "mstyle",        &TemplateBuilder::template updateElement<MathML_mstyle_ElementBuilder> },
      { "merror",        &TemplateBuilder::template updateElement<MathML_merror_ElementBuilder> },
      { "mpadded",       &TemplateBuilder::template updateElement<MathML_mpadded_ElementBuilder> },
      { "mphantom",      &TemplateBuilder::template updateElement<MathML_mphantom_ElementBuilder> },
      { "mfenced",       &TemplateBuilder::update_MathML_mfenced_Element },
      { "msub",          &TemplateBuilder::template updateElement<MathML_msub_ElementBuilder> },
      { "msup",          &TemplateBuilder::template updateElement<MathML_msup_ElementBuilder> },
      { "msubsup",       &TemplateBuilder::template updateElement<MathML_msubsup_ElementBuilder> },
      { "munder",        &TemplateBuilder::template updateElement<MathML_munder_ElementBuilder> },
      { "mover",         &TemplateBuilder::template updateElement<MathML_mover_ElementBuilder> },
      { "munderover",    &TemplateBuilder::template updateElement<MathML_munderover_ElementBuilder> },
      { "mmultiscripts", &TemplateBuilder::template updateElement<MathML_mmultiscripts_ElementBuilder> },
      { "mtable",        &TemplateBuilder::template updateElement<MathML_mtable_ElementBuilder> },
      { "mtd",           &TemplateBuilder::template updateElement<MathML_mtd_ElementBuilder> },
      { "maligngroup",   &TemplateBuilder::template updateElement<MathML_maligngroup_ElementBuilder> },
      { "malignmark",    &TemplateBuilder::template updateElement<MathML_malignmark_ElementBuilder> },
      { "maction",       &TemplateBuilder::template updateElement<MathML_maction_ElementBuilder> },
      { "menclose",      &TemplateBuilder::template updateElement<MathML_menclose_ElementBuilder> },
      { "semantics",     &TemplateBuilder::update_MathML_semantics_Element },

      { "",              0 }
    };

    if (!mathmlMapInitialized)
      {
	for (unsigned i = 0; mathml_tab[i].update; i++)
	  mathmlMap[mathml_tab[i].tag] = mathml_tab[i].update;

	mathmlMapInitialized = true;
      }

#if GMV_ENABLE_BOXML
    static struct
    {
      String tag;
      BoxMLUpdateMethod update;
    } boxml_tab[] = {
      { "at",            &TemplateBuilder::template updateElement<BoxML_at_ElementBuilder> },
      { "layout",        &TemplateBuilder::template updateElement<BoxML_layout_ElementBuilder> },
      { "h",             &TemplateBuilder::template updateElement<BoxML_h_ElementBuilder> },
      { "ink",           &TemplateBuilder::template updateElement<BoxML_ink_ElementBuilder> },
      { "space",         &TemplateBuilder::template updateElement<BoxML_space_ElementBuilder> },
      { "text",          &TemplateBuilder::template updateElement<BoxML_text_ElementBuilder> },
      { "v",             &TemplateBuilder::template updateElement<BoxML_v_ElementBuilder> },
      { "hv",            &TemplateBuilder::template updateElement<BoxML_hv_ElementBuilder> },
      { "hov",           &TemplateBuilder::template updateElement<BoxML_hov_ElementBuilder> },
      { "box",           &TemplateBuilder::template updateElement<BoxML_box_ElementBuilder> },
      { "action",        &TemplateBuilder::template updateElement<BoxML_action_ElementBuilder> },
      { "obj",           &TemplateBuilder::update_BoxML_obj_Element },
      { "decor",         &TemplateBuilder::template updateElement<BoxML_decor_ElementBuilder> },
      { "",              0 }
    };

    if (!boxmlMapInitialized)
      {
	for (unsigned i = 0; boxml_tab[i].update; i++)
	  boxmlMap[boxml_tab[i].tag] = boxml_tab[i].update;

	boxmlMapInitialized = true;
      }
#endif // GMV_ENABLE_BOXML
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

    SmartPtr<MathMLOperatorElement> openElem = MathMLOperatorElement::create(this->getMathMLNamespaceContext());
    openElem->setSize(0);
    openElem->append(open);
    openElem->SetFence();

    SmartPtr<MathMLOperatorElement> closeElem = MathMLOperatorElement::create(this->getMathMLNamespaceContext());
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
	    if (!separators.empty() && i + 1 < content.size())
	      {
		SmartPtr<MathMLOperatorElement> sep = MathMLOperatorElement::create(this->mathmlContext);
		unsigned offset = (i < separators.length()) ? i : separators.length() - 1;
		sep->setSize(0);
		sep->append(separators.substr(offset, 1));
		sep->SetSeparator();
		innerRowContent.push_back(sep);
	      }
	  }
	SmartPtr<MathMLRowElement> innerRow = MathMLRowElement::create(this->getMathMLNamespaceContext());
	innerRow->swapContent(innerRowContent);
	outerRowContent.push_back(innerRow);
      }
    outerRowContent.push_back(closeElem);

    // this can probably remain attached to the model
    SmartPtr<MathMLRowElement> outerRow = MathMLRowElement::create(this->getMathMLNamespaceContext());
    outerRow->swapContent(outerRowContent);

    return outerRow;
  }

  SmartPtr<MathMLElement>
  update_MathML_semantics_Element(const typename Model::Element& el) const
  {
    typename Model::ElementIterator iter(el, MATHML_NS_URI);
    if (iter.more())
      if (typename Model::Element e = iter.element())
	if (Model::getNodeName(Model::asNode(e)) != "annotation"
	    && Model::getNodeName(Model::asNode(e)) != "annotation-xml")
	  if (SmartPtr<MathMLElement> elem = getMathMLElementNoCreate(iter.element()))
	    return elem;
	  else
	    iter.next();

    while (typename Model::Element e = iter.element())
      {
	if (Model::getNodeName(Model::asNode(e)) == "annotation-xml")
	  {
	    String encoding = Model::getAttribute(e, "encoding");
	    if (encoding == "MathML-Presentation")
	      return getMathMLElement(typename Model::ElementIterator(e, MATHML_NS_URI).element());
#if GMV_ENABLE_BOXML
	    else if (encoding == "BoxML")
	      {
		// this element can probably be associated with the model element
		SmartPtr<MathMLBoxMLAdapter> adapter = getElement<MathMLBoxMLAdapterBuilder>(el);
		assert(adapter);
		adapter->setChild(getBoxMLElement(typename Model::ElementIterator(e, BOXML_NS_URI).element()));
		adapter->resetDirtyStructure();
		adapter->resetDirtyAttribute();
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
    
    String alt        = Model::getAttribute(el, "alt");
    String fontFamily = Model::getAttribute(el, "fontfamily");
    String index      = Model::getAttribute(el, "index");
    
    if (alt.empty() || fontFamily.empty() || index.empty())
      {
	this->getLogger()->out(LOG_WARNING, "malformed `mglyph' element (some required attribute is missing)\n");
	return MathMLStringNode::create("?");
      }
    
    return MathMLGlyphNode::create(fontFamily, index, alt);
  }

  SmartPtr<MathMLTextNode>
  update_MathML_malignmark_Node(const typename Model::Element& el) const
  {
    assert(el);
    
    const String edge = Model::getAttribute(el, "edge");
    
    TokenId align = T__NOTVALID;
    
    if (!edge.empty())
      {
	if      (edge == "left") align = T_LEFT;
	else if (edge == "right") align = T_RIGHT;
	else
	  this->getLogger()->out(LOG_WARNING,
				 "malformed `malignmark' element, attribute `edge' has invalid value `%s' (ignored)",
				 std::string(edge).c_str());
      }
    
    return MathMLMarkNode::create(align);
  }

#if GMV_ENABLE_BOXML
  ///////////////////////////////////
  // SPECIALIZED BOXML UPDATE METHODS
  ///////////////////////////////////

  SmartPtr<BoxMLElement>
  update_BoxML_obj_Element(const typename Model::Element& el) const
  {
    String encoding = Model::getAttribute(el, "encoding");
    if (encoding == "BoxML")
      return getBoxMLElement(typename Model::ElementIterator(el, BOXML_NS_URI).element());
    else /* if (encoding == "MathML-Presentation") */
      {
	// this element can be associated to the corresponding model element
	SmartPtr<BoxMLMathMLAdapter> adapter = getElement<BoxMLMathMLAdapterBuilder>(el);
	assert(adapter);
	adapter->setChild(getMathMLElement(typename Model::ElementIterator(el, MATHML_NS_URI).element()));
	adapter->resetDirtyStructure();
	adapter->resetDirtyAttribute();
	return adapter;
      }
#if 0
    else
      return createBoxMLDummyElement();
#endif
  }
#endif // GMV_ENABLE_BOXML

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
    begin(const TemplateBuilder&, const typename Model::Element&, const SmartPtr<MathMLElement>&)
    { }

    static void
    end(const TemplateBuilder&, const typename Model::Element&, const SmartPtr<MathMLElement>&)
    { }

    static void
    refine(const TemplateBuilder&, const typename Model::Element&, const SmartPtr<MathMLElement>&)
    { }

    static void
    construct(const TemplateBuilder&, const typename Model::Element&, const SmartPtr<MathMLElement>&)
    { }
  };

  struct MathMLBinContainerElementBuilder : public MathMLElementBuilder
  {
    static void
    construct(const TemplateBuilder& builder, const typename Model::Element& el, const SmartPtr<MathMLBinContainerElement>& elem)
    { elem->setChild(builder.getMathMLElement(el)); }
  };

  struct MathMLNormalizingContainerElementBuilder : public MathMLBinContainerElementBuilder
  {
    static void
    construct(const TemplateBuilder& builder, const typename Model::Element& el, const SmartPtr<MathMLNormalizingContainerElement>& elem)
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
    static void construct(const TemplateBuilder& builder, const typename Model::Element& el, const SmartPtr<MathMLLinearContainerElement>& elem)
    {
      std::vector<SmartPtr<MathMLElement> > content;
      builder.getChildMathMLElements(el, content);
      elem->swapContent(content);
    }
  };

  struct MathMLTokenElementBuilder : public MathMLElementBuilder
  {
    static void
    refine(const TemplateBuilder& builder, const typename Model::Element& el, const SmartPtr<MathMLTokenElement>& elem)
    {
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Token, mathvariant));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Token, mathsize));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Token, mathcolor));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Token, mathbackground));
    }

    static void
    construct(const TemplateBuilder& builder, const typename Model::Element& el, const SmartPtr<MathMLTokenElement>& elem)
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
    refine(const TemplateBuilder& builder, const typename Model::Element& el, const SmartPtr<MathMLOperatorElement>& elem)
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
    refine(const TemplateBuilder& builder, const typename Model::Element& el, const SmartPtr<MathMLStringLitElement>& elem)
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
    refine(const TemplateBuilder& builder, const typename Model::Element& el, const SmartPtr<MathMLSpaceElement>& elem)
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
    begin(const TemplateBuilder& builder, const typename Model::Element& el, const SmartPtr<MathMLStyleElement>&)
    { builder.refinementContext.push(el); }

    static void
    end(const TemplateBuilder& builder, const typename Model::Element&, const SmartPtr<MathMLStyleElement>&)
    { builder.refinementContext.pop(); }

    static void
    refine(const TemplateBuilder& builder, const typename Model::Element& el, const SmartPtr<MathMLStyleElement>& elem)
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
    refine(const TemplateBuilder& builder, const typename Model::Element& el, const SmartPtr<MathMLPaddedElement>& elem)
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
    refine(const TemplateBuilder&, const typename Model::Element&, const SmartPtr<MathMLAlignGroupElement>&)
    {
      // NO ATTRIBUTES TO REFINE???
    }
  };

  struct MathML_malignmark_ElementBuilder : public MathMLElementBuilder
  {
    typedef MathMLAlignMarkElement type;

    static void
    refine(const TemplateBuilder& builder, const typename Model::Element& el, const SmartPtr<MathMLAlignMarkElement>& elem)
    { builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, AlignMark, edge)); }
  };

  struct MathML_maction_ElementBuilder : public MathMLLinearContainerElementBuilder
  { 
    typedef MathMLActionElement type;

    static void
    refine(const TemplateBuilder& builder, const typename Model::Element& el, const SmartPtr<MathMLActionElement>& elem)
    {
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Action, actiontype));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Action, selection));
    }
  };

  struct MathML_menclose_ElementBuilder : public MathMLNormalizingContainerElementBuilder
  {
    typedef MathMLEncloseElement type;

    static void
    refine(const TemplateBuilder& builder, const typename Model::Element& el, const SmartPtr<MathMLEncloseElement>& elem)
    { builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Enclose, notation)); }
  };

  struct MathML_math_ElementBuilder : public MathMLNormalizingContainerElementBuilder
  {
    typedef MathMLmathElement type;

    static void
    refine(const TemplateBuilder& builder, const typename Model::Element& el, const SmartPtr<MathMLmathElement>& elem)
    {
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, math, mode));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, math, display));
    }
  };

  struct MathML_mfrac_ElementBuilder : public MathMLElementBuilder
  {
    typedef MathMLFractionElement type;

    static void
    refine(const TemplateBuilder& builder, const typename Model::Element& el, const SmartPtr<MathMLFractionElement>& elem)
    {
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Fraction, numalign));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Fraction, denomalign));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Fraction, linethickness));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Fraction, bevelled));
    }

    static void
    construct(const TemplateBuilder& builder, const typename Model::Element& el, const SmartPtr<MathMLFractionElement>& elem)
    {
      typename Model::ElementIterator iter(el, MATHML_NS_URI);
      elem->setNumerator(builder.getMathMLElement(iter.element()));
      iter.next();
      elem->setDenominator(builder.getMathMLElement(iter.element()));
    }
  };

  struct MathML_mroot_ElementBuilder : public MathMLElementBuilder
  {
    typedef MathMLRadicalElement type;

    static void
    construct(const TemplateBuilder& builder, const typename Model::Element& el, const SmartPtr<MathMLRadicalElement>& elem)
    {
      typename Model::ElementIterator iter(el, MATHML_NS_URI);
      elem->setBase(builder.getMathMLElement(iter.element()));
      iter.next();
      elem->setIndex(builder.getMathMLElement(iter.element()));
    }
  };

  struct MathML_msqrt_ElementBuilder : public MathMLElementBuilder
  {
    typedef MathMLRadicalElement type;

    static void
    construct(const TemplateBuilder& builder, const typename Model::Element& el, const SmartPtr<MathMLRadicalElement>& elem)
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
    refine(const TemplateBuilder& builder, const typename Model::Element& el, const SmartPtr<MathMLScriptElement>& elem)
    { builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Script, subscriptshift)); }

    static void
    construct(const TemplateBuilder& builder, const typename Model::Element& el, const SmartPtr<MathMLScriptElement>& elem)
    {
      typename Model::ElementIterator iter(el, MATHML_NS_URI);
      elem->setBase(builder.getMathMLElement(iter.element()));
      iter.next();
      elem->setSubScript(builder.getMathMLElement(iter.element()));
      elem->setSuperScript(0);
    }
  };

  struct MathML_msup_ElementBuilder : public MathMLElementBuilder
  {
    typedef MathMLScriptElement type;

    static void
    refine(const TemplateBuilder& builder, const typename Model::Element& el, const SmartPtr<MathMLScriptElement>& elem)
    { builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, Script, superscriptshift)); }

    static void
    construct(const TemplateBuilder& builder, const typename Model::Element& el, const SmartPtr<MathMLScriptElement>& elem)
    {
      typename Model::ElementIterator iter(el, MATHML_NS_URI);
      elem->setBase(builder.getMathMLElement(iter.element()));
      iter.next();
      elem->setSubScript(0);
      elem->setSuperScript(builder.getMathMLElement(iter.element()));
    }
  };
  
  struct MathML_msubsup_ElementBuilder : public MathMLElementBuilder
  {
    typedef MathMLScriptElement type;

    static void
    refine(const TemplateBuilder& builder, const typename Model::Element& el, const SmartPtr<MathMLScriptElement>& elem)
    {
      MathML_msub_ElementBuilder::refine(builder, el, elem);
      MathML_msup_ElementBuilder::refine(builder, el, elem);
    }

    static void
    construct(const TemplateBuilder& builder, const typename Model::Element& el, const SmartPtr<MathMLScriptElement>& elem)
    {
      typename Model::ElementIterator iter(el, MATHML_NS_URI);
      elem->setBase(builder.getMathMLElement(iter.element()));
      iter.next();
      elem->setSubScript(builder.getMathMLElement(iter.element()));
      iter.next();
      elem->setSuperScript(builder.getMathMLElement(iter.element()));
    }
  };

  struct MathML_munder_ElementBuilder : public MathMLElementBuilder
  {
    typedef MathMLUnderOverElement type;

    static void
    refine(const TemplateBuilder& builder, const typename Model::Element& el, const SmartPtr<MathMLUnderOverElement>& elem)
    { builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, UnderOver, accentunder)); }

    static void
    construct(const TemplateBuilder& builder, const typename Model::Element& el, const SmartPtr<MathMLUnderOverElement>& elem)
    {
      typename Model::ElementIterator iter(el, MATHML_NS_URI);
      elem->setBase(builder.getMathMLElement(iter.element()));
      iter.next();
      elem->setUnderScript(builder.getMathMLElement(iter.element()));
      elem->setOverScript(0);
    }
  };

  struct MathML_mover_ElementBuilder : public MathMLElementBuilder
  {
    typedef MathMLUnderOverElement type;

    static void
    refine(const TemplateBuilder& builder, const typename Model::Element& el, const SmartPtr<MathMLUnderOverElement>& elem)
    { builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, UnderOver, accent)); }

    static void
    construct(const TemplateBuilder& builder, const typename Model::Element& el, const SmartPtr<MathMLUnderOverElement>& elem)
    {
      typename Model::ElementIterator iter(el, MATHML_NS_URI);
      elem->setBase(builder.getMathMLElement(iter.element()));
      iter.next();
      elem->setUnderScript(0);
      elem->setOverScript(builder.getMathMLElement(iter.element()));
    }
  };

  struct MathML_munderover_ElementBuilder : public MathMLElementBuilder
  {
    typedef MathMLUnderOverElement type;

    static void
    refine(const TemplateBuilder& builder, const typename Model::Element& el, const SmartPtr<MathMLUnderOverElement>& elem)
    {
      MathML_munder_ElementBuilder::refine(builder, el, elem);
      MathML_mover_ElementBuilder::refine(builder, el, elem);
    }

    static void
    construct(const TemplateBuilder& builder, const typename Model::Element& el, const SmartPtr<MathMLUnderOverElement>& elem)
    {
      typename Model::ElementIterator iter(el, MATHML_NS_URI);
      elem->setBase(builder.getMathMLElement(iter.element()));
      iter.next();
      elem->setUnderScript(builder.getMathMLElement(iter.element()));
      iter.next();
      elem->setOverScript(builder.getMathMLElement(iter.element()));
    }
  };

  struct MathML_mtd_ElementBuilder : public MathMLNormalizingContainerElementBuilder
  { typedef MathMLTableCellElement type; };

  struct MathML_mtable_ElementBuilder : public MathMLElementBuilder
  {
    typedef MathMLTableElement type;

    static void
    refine(const TemplateBuilder& builder, const typename Model::Element& el, const SmartPtr<MathMLTableElement>& elem)
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

    static const SmartPtr<Value>
    refineAlignAttribute(const SmartPtr<Value>& cellAlign,
			 const SmartPtr<Value>& rowAlign,
			 const SmartPtr<Value>& tableAlign)
    {
#if 1
      if (cellAlign) return cellAlign;
      else if (rowAlign) return rowAlign;
      else if (tableAlign) return tableAlign;
      else return 0;
#else
      // Because of a bug in GCC-3.4 the following code, which is
      // syntactically and semantically correct, does not compile
      // and the compiler issues a misleading error message
      return (cellAlign ? cellAlign : (rowAlign ? rowAlign : (tableAlign ? tableAlign : 0)));
#endif
    }

    static void
    construct(const TemplateBuilder& builder, const typename Model::Element& el, const SmartPtr<MathMLTableElement>& elem)
    {
      MathMLTableContentFactory tableContentFactory;

      unsigned rowIndex = 0;

      const SmartPtr<Value> tableRowAlign = builder.getAttributeValue(el, ATTRIBUTE_SIGNATURE(MathML, Table, rowalign));
      const SmartPtr<Value> tableColumnAlign = builder.getAttributeValue(el, ATTRIBUTE_SIGNATURE(MathML, Table, columnalign));
      const SmartPtr<Value> tableGroupAlign = builder.getAttributeValue(el, ATTRIBUTE_SIGNATURE(MathML, Table, groupalign));
      for (typename Model::ElementIterator iter(el, MATHML_NS_URI); iter.more(); iter.next())
	{
	  typename Model::Element row = iter.element();

	  const SmartPtr<Value> rowRowAlign = builder.getAttributeValue(row, ATTRIBUTE_SIGNATURE(MathML, TableRow, rowalign));
	  const SmartPtr<Value> rowColumnAlign = builder.getAttributeValue(row, ATTRIBUTE_SIGNATURE(MathML, TableRow, columnalign));
	  const SmartPtr<Value> rowGroupAlign = builder.getAttributeValue(row, ATTRIBUTE_SIGNATURE(MathML, TableRow, groupalign));

	  const String name = Model::getNodeName(Model::asNode(row));
	  if (name == "mtr" || name == "mlabeledtr")
	    {
	      unsigned columnIndex = 0;
	      for (typename Model::ElementIterator iter(row, MATHML_NS_URI); iter.more(); iter.next())
		{
		  typename Model::Element cell = iter.element();

		  const SmartPtr<Value> cellRowAlign = builder.getAttributeValue(cell, ATTRIBUTE_SIGNATURE(MathML, TableCell, rowalign));
		  const SmartPtr<Value> cellColumnAlign = builder.getAttributeValue(cell, ATTRIBUTE_SIGNATURE(MathML, TableCell, columnalign));
		  const SmartPtr<Value> cellGroupAlign = builder.getAttributeValue(cell, ATTRIBUTE_SIGNATURE(MathML, TableCell, groupalign));
		  const int cellRowSpan = ToInteger(builder.getAttributeValue(cell, ATTRIBUTE_SIGNATURE(MathML, TableCell, rowspan)));
		  const int cellColumnSpan = ToInteger(builder.getAttributeValue(cell, ATTRIBUTE_SIGNATURE(MathML, TableCell, columnspan)));
		  SmartPtr<MathMLElement> elem = builder.getMathMLElement(cell);
		  SmartPtr<MathMLTableCellElement> cellElem = smart_cast<MathMLTableCellElement>(elem);
		  if (!cellElem)
		    {
		      cellElem = MathMLTableCellElement::create(builder.getMathMLNamespaceContext());
		      cellElem->setChild(elem);
		      // WARNING: should we clear the dirty flags?
		    }
		  if (name == "mtr" || columnIndex > 0)
		    {
		      cellElem->setSpan(cellRowSpan, cellColumnSpan);
		      columnIndex = tableContentFactory.setChild(rowIndex, columnIndex, cellRowSpan, cellColumnSpan, cellElem);
		    }
		  else
		    {
		      cellElem->setSpan(1, 1);
		      tableContentFactory.setLabelChild(rowIndex, cellElem);
		    }

		  // now rowIndex and columnIndex are final values
		  cellElem->setPosition(rowIndex, columnIndex);

		  const SmartPtr<Value> rowAlign =
		    refineAlignAttribute(cellRowAlign,
					 rowRowAlign,
					 GetComponent(tableRowAlign, rowIndex));
		  const SmartPtr<Value> columnAlign =
		    refineAlignAttribute(cellColumnAlign,
					 GetComponent(rowColumnAlign, columnIndex),
					 GetComponent(tableColumnAlign, columnIndex));
		  const SmartPtr<Value> groupAlign =
		    refineAlignAttribute(cellGroupAlign,
					 GetComponent(rowGroupAlign, columnIndex),
					 GetComponent(tableGroupAlign, columnIndex));

		  cellElem->setAlignment(ToTokenId(rowAlign), ToTokenId(columnAlign));
		  //cellElem->setGroupAlignment(groupAlign);
		}

	      rowIndex++;
	    }
	  else
	    {
	      // issue a warning message or promote to mtr with single mtd inside
	    }
	}

      elem->updateContent(tableContentFactory);
    }
  };

  struct MathML_mmultiscripts_ElementBuilder : public MathMLElementBuilder
  {
    typedef MathMLMultiScriptsElement type;

    static void
    refine(const TemplateBuilder& builder, const typename Model::Element& el, const SmartPtr<MathMLMultiScriptsElement>& elem)
    {
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, MultiScripts, subscriptshift));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(MathML, MultiScripts, superscriptshift));
    }

    static void
    construct(const TemplateBuilder& builder, const typename Model::Element& el, const SmartPtr<MathMLMultiScriptsElement>& elem)
    {
      typename Model::ElementIterator iter(el, MATHML_NS_URI);
      unsigned i = 0;
      unsigned nScripts = 0;
      unsigned nPreScripts = 0;
      bool preScripts = false;

      elem->setBase(builder.getMathMLElement(iter.element()));
      iter.next();
      while (iter.more())
	{
	  typename Model::Element node = iter.element();
	  assert(node);
	  const String nodeName = Model::getNodeName(Model::asNode(node));
	  if (nodeName == "mprescripts")
	    {
	      if (preScripts)
		builder.getLogger()->out(LOG_WARNING, "multiple <mprescripts> elements in mmultiscript");
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

	  iter.next();
	}

      if (preScripts && i % 2 == 1) elem->setPreSuperScript(nPreScripts, 0);
      elem->setScriptsSize(nScripts);
      elem->setPreScriptsSize(nPreScripts);
    }
  };

#if GMV_ENABLE_BOXML
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
    begin(const TemplateBuilder&, const typename Model::Element&, const SmartPtr<BoxMLElement>&)
    { }

    static void
    end(const TemplateBuilder&, const typename Model::Element&, const SmartPtr<BoxMLElement>&)
    { }

    static void
    refine(const TemplateBuilder&, const typename Model::Element&, const SmartPtr<BoxMLElement>&)
    { }

    static void
    construct(const TemplateBuilder&, const typename Model::Element&, const SmartPtr<BoxMLElement>&)
    { }
  };

  struct MathMLBoxMLAdapterBuilder : public MathMLElementBuilder
  { typedef MathMLBoxMLAdapter type; };

  struct BoxMLMathMLAdapterBuilder : public BoxMLElementBuilder
  { typedef BoxMLMathMLAdapter type; };

  struct BoxMLBinContainerElementBuilder : public BoxMLElementBuilder
  {
    static void
    construct(const TemplateBuilder& builder, const typename Model::Element& el, const SmartPtr<BoxMLBinContainerElement>& elem)
    { elem->setChild(builder.getBoxMLElement(typename Model::ElementIterator(el, BOXML_NS_URI).element())); }
  };

  struct BoxMLLinearContainerElementBuilder : public BoxMLElementBuilder
  {
    static void
    construct(const TemplateBuilder& builder, const typename Model::Element& el, const SmartPtr<BoxMLLinearContainerElement>& elem)
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
    refine(const TemplateBuilder& builder, const typename Model::Element& el, const SmartPtr<BoxMLActionElement>& elem)
    {
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, Action, selection));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, Action, actiontype));
    }
  };

  struct BoxML_at_ElementBuilder : public BoxMLBinContainerElementBuilder
  {
    typedef BoxMLAtElement type;

    static void
    refine(const TemplateBuilder& builder, const typename Model::Element& el, const SmartPtr<BoxMLAtElement>& elem)
    {
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, At, x));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, At, y));
    }
  };

  struct BoxML_decor_ElementBuilder : public BoxMLBinContainerElementBuilder
  {
    typedef BoxMLDecorElement type;

    static void
    refine(const TemplateBuilder& builder, const typename Model::Element& el, const SmartPtr<BoxMLDecorElement>& elem)
    {
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, Decor, type));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, Decor, color));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, Decor, thickness));
    }
  };

  struct BoxML_g_ElementBuilder : public BoxMLLinearContainerElementBuilder
  {
    typedef BoxMLGroupElement type;

    static void
    refine(const TemplateBuilder& builder, const typename Model::Element& el, const SmartPtr<BoxMLGroupElement>& elem)
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
    refine(const TemplateBuilder& builder, const typename Model::Element& el, const SmartPtr<BoxMLHElement>& elem)
    { builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, H, spacing)); }
  };
  
  struct BoxML_ink_ElementBuilder : public BoxMLElementBuilder
  {
    typedef BoxMLInkElement type;
    
    static void
    refine(const TemplateBuilder& builder, const typename Model::Element& el, const SmartPtr<BoxMLInkElement>& elem)
    {
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, Ink, color));
      BoxML_space_ElementBuilder::refine(builder, el, elem);
    }
  };

  struct BoxML_layout_ElementBuilder : public BoxMLLinearContainerElementBuilder
  {
    typedef BoxMLLayoutElement type;

    static void
    refine(const TemplateBuilder& builder, const typename Model::Element& el, const SmartPtr<BoxMLLayoutElement>& elem)
    {
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, Layout, width));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, Layout, height));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, Layout, depth));
    }
  };

#if 0
  struct BoxML_par_ElementBuilder : public BoxMLElementBuilder
  {
    typedef BoxMLParagraphElement type;

    static void
    refine(const TemplateBuilder& builder, const typename Model::Element& el, const SmartPtr<BoxMLParagraphElement>& elem)
    {
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, V, align));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, V, minlinespacing));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, Text, width));
    }

    static void
    construct(const TemplateBuilder& builder, const typename Model::Element& el, const SmartPtr<BoxMLParagraphElement>& elem)
    {
      std::vector<SmartPtr<BoxMLElement> > content;
      for (typename Model::NodeIterator iter(Model::asNode(el)); iter.more(); iter.next())
	{
	  typename Model::Node p = iter.node();
	  assert(p);
	  if (Model::getNodeType(p) == Model::TEXT_NODE)
	    {
	      SmartPtr<BoxMLTextElement> text = builder.getElement<BoxML_text_ElementBuilder>(el);
	      text->setContent(Model::getNodeValue(p));
	      content.push_back(text);
	    }
	  else if (Model::getNodeType(p) == Model::ELEMENT_NODE && Model::getNodeNamespaceURI(p) == BOXML_NS_URI)
	    content.push_back(builder.getBoxMLElement(Model::asElement(p)));
	}
      elem->swapContent(content);
    }
  };
#endif

  struct BoxML_space_ElementBuilder : public BoxMLElementBuilder
  {
    typedef BoxMLSpaceElement type;

    static void
    refine(const TemplateBuilder& builder, const typename Model::Element& el, const SmartPtr<BoxMLSpaceElement>& elem)
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
    refine(const TemplateBuilder& builder, const typename Model::Element& el, const SmartPtr<BoxMLTextElement>& elem)
    {
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, Text, size));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, Text, color));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, Text, background));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, Text, width));
    }

    static void
    construct(const TemplateBuilder&, const typename Model::Element& el, const SmartPtr<BoxMLTextElement>& elem)
    {
      String content;
      for (typename Model::NodeIterator iter(Model::asNode(el)); iter.more(); iter.next())
	{
	  typename Model::Node p = iter.node();
	  assert(p);
	  if (Model::getNodeType(p) == Model::TEXT_NODE)
	    content += Model::getNodeValue(p);
	}
      content = trimSpacesLeft(trimSpacesRight(collapseSpaces(content)));
      elem->setContent(content);
    }
  };

  struct BoxML_v_ElementBuilder : public BoxMLLinearContainerElementBuilder
  {
    typedef BoxMLVElement type;

    static void
    refine(const TemplateBuilder& builder, const typename Model::Element& el, const SmartPtr<BoxMLVElement>& elem)
    {
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, V, enter));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, V, exit));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, V, indent));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, V, minlinespacing));
    }
  };

  struct BoxML_hv_ElementBuilder : public BoxMLLinearContainerElementBuilder
  {
    typedef BoxMLHVElement type;

    static void
    refine(const TemplateBuilder& builder, const typename Model::Element& el, const SmartPtr<BoxMLHVElement>& elem)
    {
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, HV, spacing));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, HV, indent));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, HV, minlinespacing));
    }
  };

  struct BoxML_hov_ElementBuilder : public BoxMLLinearContainerElementBuilder
  {
    typedef BoxMLHOVElement type;

    static void
    refine(const TemplateBuilder& builder, const typename Model::Element& el, const SmartPtr<BoxMLHOVElement>& elem)
    {
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, HOV, spacing));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, HOV, indent));
      builder.refineAttribute(elem, el, ATTRIBUTE_SIGNATURE(BoxML, HOV, minlinespacing));
    }
  };
#endif // GMV_ENABLE_BOXML

  ////////////////////////////
  // BUILDER AUXILIARY METHODS
  ////////////////////////////

  SmartPtr<Attribute>
  getAttribute(const typename Model::Element& el, const AttributeSignature& signature) const
  {
    SmartPtr<Attribute> attr;
  
    if (signature.fromElement)
      if (Model::hasAttribute(el, signature.name))
	attr = Attribute::create(signature, Model::getAttribute(el, signature.name));

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
      return signature.getDefaultValue();
  }

  void
  refineAttribute(const SmartPtr<Element>& elem,
		  const typename Model::Element& el, const AttributeSignature& signature) const
  {
    if (SmartPtr<Attribute> attr = getAttribute(el, signature)) elem->setAttribute(attr);
    else elem->removeAttribute(signature);
  }

  ///////////////////////////////////////
  // BUILDER AUXILIARY METHODS FOR MATHML
  ///////////////////////////////////////

  SmartPtr<MathMLElement>
  getMathMLElementNoCreate(const typename Model::Element& el) const
  {
    if (el)
      {
	//std::cout << "createMathMLElement " << el.get_localName() << std::endl;
	typename MathMLBuilderMap::const_iterator m = mathmlMap.find(Model::getNodeName(Model::asNode(el)));
	if (m != mathmlMap.end()) 
	  {
	    SmartPtr<MathMLElement> elem = (this->*(m->second))(el);
	    assert(elem);
	    elem->resetDirtyStructure();
	    elem->resetDirtyAttribute();
	    return elem;
	  }
      }

    return 0;
  }

  SmartPtr<MathMLElement>
  getMathMLElement(const typename Model::Element& el) const
  {
    if (SmartPtr<MathMLElement> elem = getMathMLElementNoCreate(el))
      return elem;
    else
      return createMathMLDummyElement();
  }

  void
  getChildMathMLElements(const typename Model::Element& el, std::vector<SmartPtr<MathMLElement> >& content) const
  {
    content.clear();
    for (typename Model::ElementIterator iter(el, MATHML_NS_URI); iter.more(); iter.next())
      content.push_back(getMathMLElement(iter.element()));
  }

  void
  getChildMathMLTextNodes(const typename Model::Element& el, std::vector<SmartPtr<MathMLTextNode> >& content) const
  {
    bool first = true;
    content.clear();
    for (typename Model::NodeIterator iter(Model::asNode(el)); iter.more(); )
      {
	typename Model::Node n = iter.node();
	assert(n);

	switch (Model::getNodeType(n))
	  {
	  case Model::TEXT_NODE:
	    {
	      // ok, we have a chunk of text
	      String s = collapseSpaces(Model::getNodeValue(n));
	      iter.next();

	      // ...but spaces at the at the beginning (end) are deleted only if this
	      // is the very first (last) chunk in the token.
	      if (first) s = trimSpacesLeft(s);
	      if (!iter.more()) s = trimSpacesRight(s);
	      
	      content.push_back(createMathMLTextNode(s));
	    }
	    break;
      
	  case Model::ELEMENT_NODE:
	    {	    
	      if (Model::getNodeNamespaceURI(n) == MATHML_NS_URI)
		{
		  const String nodeName = Model::getNodeName(n);
		  if (nodeName == "mglyph")
		    content.push_back(update_MathML_mglyph_Node(Model::asElement(n)));
		  else if (nodeName == "malignmark")
		    content.push_back(update_MathML_malignmark_Node(Model::asElement(n)));
		}
	    }
	    iter.next();
	    break;
	    
	  default:
	    iter.next();
	    break;
	  }
	first = false;
      }
  }

#if ENABLE_BUILDER_CACHE
  SmartPtr<MathMLTextNode>
  createMathMLTextNode(const String& content) const
  {
    std::pair<MathMLTextNodeCache::iterator, bool> r =
      mathmlTextNodeCache.insert(std::make_pair(content, SmartPtr<MathMLTextNode>(0)));
    if (r.second)
      {
	if (content == MathMLFunctionApplicationNode::getContent())
	  r.first->second = MathMLFunctionApplicationNode::create();
	else if (content == MathMLInvisibleTimesNode::getContent())
	  r.first->second = MathMLInvisibleTimesNode::create();
	else
	  r.first->second = MathMLStringNode::create(content);
	return r.first->second;
      }
    else
      return r.first->second;
  }
#else
  SmartPtr<MathMLTextNode>
  createMathMLTextNode(const String& content) const
  {
    if (content == MathMLFunctionApplicationNode::getContent())
      return MathMLFunctionApplicationNode::create();
    else if (content == MathMLInvisibleTimesNode::getContent())
      return MathMLInvisibleTimesNode::create();
    else
      return MathMLStringNode::create(content);
  }
#endif // ENABLE_BUILDER_CACHE

  SmartPtr<MathMLElement>
  createMathMLDummyElement(void) const
  {
    SmartPtr<MathMLElement> elem = MathMLDummyElement::create(this->getMathMLNamespaceContext());
    elem->resetDirtyStructure();
    elem->resetDirtyAttribute();
    return elem;
  }

#if GMV_ENABLE_BOXML
  //////////////////////////////////////
  // BUILDER AUXILIARY METHODS FOR BOXML
  //////////////////////////////////////

  SmartPtr<BoxMLElement>
  getBoxMLElement(const typename Model::Element& el) const
  {
    if (el)
      {
	typename BoxMLBuilderMap::const_iterator m = boxmlMap.find(Model::getNodeName(Model::asNode(el)));
	if (m != boxmlMap.end())
	  {
	    SmartPtr<BoxMLElement> elem = (this->*(m->second))(el);
	    assert(elem);
	    elem->resetDirtyStructure();
	    elem->resetDirtyAttribute();
	    return elem;
	  }
      }

    return createBoxMLDummyElement();
  }

  void
  getChildBoxMLElements(const typename Model::Element& el, std::vector<SmartPtr<BoxMLElement> >& content) const
  {
    content.clear();
    for (typename Model::ElementIterator iter(el, BOXML_NS_URI); iter.more(); iter.next())
      content.push_back(getBoxMLElement(iter.element()));
  }

  SmartPtr<BoxMLElement>
  createBoxMLDummyElement(void) const
  {
    assert(false);
    return 0;
  }
#endif // GMV_ENABLE_BOXML

public:
  static SmartPtr<Builder> create(void) { return new TemplateBuilder(); }

  virtual void
  forgetElement(Element* elem) const
  { this->linkerRemove(elem); }

  virtual SmartPtr<Element>
  getRootElement() const
  {
    if (typename Model::Element root = this->getRootModelElement())
      {
	const String ns = Model::getNodeNamespaceURI(Model::asNode(root));
	if (ns == MATHML_NS_URI) return getMathMLElement(root);
#if GMV_ENABLE_BOXML
	else if (ns == BOXML_NS_URI) return getBoxMLElement(root);
#endif // GMV_ENABLE_BOXML
      }
    return 0;
  }

private:
#if ENABLE_BUILDER_CACHE
  typedef HASH_MAP_NS::hash_map<String, SmartPtr<MathMLTextNode>, StringHash, StringEq> MathMLTextNodeCache;
  mutable MathMLTextNodeCache mathmlTextNodeCache;
#endif // ENABLE_BUILDER_CACHE
  typedef SmartPtr<class MathMLElement> (TemplateBuilder::* MathMLUpdateMethod)(const typename Model::Element&) const;
  typedef HASH_MAP_NS::hash_map<String, MathMLUpdateMethod, StringHash, StringEq> MathMLBuilderMap;
  static MathMLBuilderMap mathmlMap;
  static bool mathmlMapInitialized;
#if GMV_ENABLE_BOXML
  typedef SmartPtr<class BoxMLElement> (TemplateBuilder::* BoxMLUpdateMethod)(const typename Model::Element&) const;
  typedef HASH_MAP_NS::hash_map<String, BoxMLUpdateMethod, StringHash, StringEq> BoxMLBuilderMap;
  static BoxMLBuilderMap boxmlMap;
  static bool boxmlMapInitialized;
#endif // GMV_ENABLE_BOXML

  mutable RefinementContext refinementContext;
};

template <class Model, class Builder, class RefinementContext>
typename TemplateBuilder<Model,Builder,RefinementContext>::MathMLBuilderMap TemplateBuilder<Model,Builder,RefinementContext>::mathmlMap;

template <class Model, class Builder, class RefinementContext>
bool TemplateBuilder<Model,Builder,RefinementContext>::mathmlMapInitialized = false;

#ifdef GMV_ENABLE_BOXML
template <class Model, class Builder, class RefinementContext>
typename TemplateBuilder<Model,Builder,RefinementContext>::BoxMLBuilderMap TemplateBuilder<Model,Builder,RefinementContext>::boxmlMap;

template <class Model, class Builder, class RefinementContext>
bool TemplateBuilder<Model,Builder,RefinementContext>::boxmlMapInitialized = false;
#endif // GMV_ENABLE_BOXML

#endif // __TemplateBuilder_hh__
