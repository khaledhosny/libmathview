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

#ifndef __gmetadom_Builder_hh__
#define __gmetadom_Builder_hh__

#include "HashMap.hh"
#include "Builder.hh"
#include "RefinementContext.hh"

class gmetadom_Builder : public Builder
{
protected:
  gmetadom_Builder(void);
  virtual ~gmetadom_Builder();

public:
  static SmartPtr<gmetadom_Builder> create(void)
  { return new gmetadom_Builder(); }

  void setRootDOMElement(const DOM::Element& el) { root = el; }
  DOM::Element getRootDOMElement(void) const { return root; }

  virtual SmartPtr<class Element> getRootElement(void) const;

protected:
  SmartPtr<class Attribute> getAttribute(const DOM::Element&, const class AttributeSignature&) const;
  SmartPtr<class Value> getAttributeValue(const DOM::Element&, const class AttributeSignature&) const;
  void refineAttribute(const SmartPtr<Element>&, const DOM::Element&, const class AttributeSignature&) const;

  SmartPtr<class MathMLElement> getMathMLElement(const DOM::Element&) const;
  SmartPtr<class MathMLElement> getMathMLElementNoCreate(const DOM::Element&) const;
  SmartPtr<class MathMLTextNode> getMathMLTextNode(const DOM::Node&) const;
  SmartPtr<class BoxMLElement> getBoxMLElement(const DOM::Element&) const;

  void getChildMathMLElements(const DOM::Element&, std::vector<SmartPtr<class MathMLElement> >&) const;
  void getChildBoxMLElements(const DOM::Element&, std::vector<SmartPtr<class BoxMLElement> >&) const;
  void getChildMathMLTextNodes(const DOM::Element&, std::vector<SmartPtr<class MathMLTextNode> >&) const;

  // auxiliary construction methods
  void constructMathMLLinearContainerElement(const DOM::Element&,
					     const SmartPtr<class MathMLLinearContainerElement>&) const;
  void constructMathMLNormalizingContainerElement(const DOM::Element&,
						  const SmartPtr<class MathMLNormalizingContainerElement>&) const;
  void constructMathMLTokenElement(const DOM::Element&, const SmartPtr<class MathMLTokenElement>&) const;
  void constructMathMLEmptyElement(const DOM::Element&, const SmartPtr<class MathMLElement>&) const;
  void constructBoxMLEmptyElement(const DOM::Element&, const SmartPtr<class BoxMLElement>&) const;
  void constructBoxMLBinContainerElement(const DOM::Element&, const SmartPtr<class BoxMLBinContainerElement>&) const;
  void constructBoxMLLinearContainerElement(const DOM::Element&,
					    const SmartPtr<class BoxMLLinearContainerElement>&) const;

  // construction methods
  // MathML
  void construct_MathML_mfrac_Element(const DOM::Element&, const SmartPtr<class MathMLFractionElement>&) const;
  void construct_MathML_mroot_Element(const DOM::Element&, const SmartPtr<class MathMLRadicalElement>&) const;
  void construct_MathML_msqrt_Element(const DOM::Element&, const SmartPtr<class MathMLRadicalElement>&) const;
  void construct_MathML_msub_Element(const DOM::Element&, const SmartPtr<class MathMLScriptElement>&) const;
  void construct_MathML_msup_Element(const DOM::Element&, const SmartPtr<class MathMLScriptElement>&) const;
  void construct_MathML_msubsup_Element(const DOM::Element&, const SmartPtr<class MathMLScriptElement>&) const;
  void construct_MathML_munder_Element(const DOM::Element&, const SmartPtr<class MathMLUnderOverElement>&) const;
  void construct_MathML_mover_Element(const DOM::Element&, const SmartPtr<class MathMLUnderOverElement>&) const;
  void construct_MathML_munderover_Element(const DOM::Element&, const SmartPtr<class MathMLUnderOverElement>&) const;
  void construct_MathML_mtable_Element(const DOM::Element&, const SmartPtr<class MathMLTableElement>&) const;
  void construct_MathML_mmultiscripts_Element(const DOM::Element&, const SmartPtr<class MathMLMultiScriptsElement>&) const;
  // BoxML
  void construct_BoxML_layout_Element(const DOM::Element&, const SmartPtr<class BoxMLLayoutElement>&) const;
  void construct_BoxML_par_Element(const DOM::Element&, const SmartPtr<class BoxMLParagraphElement>&) const;
  void construct_BoxML_text_Element(const DOM::Element&, const SmartPtr<class BoxMLTextElement>&) const;
  void construct_BoxML_at_Element(const DOM::Element&, const SmartPtr<class BoxMLAtElement>&) const;
  void construct_BoxML_g_Element(const DOM::Element&, const SmartPtr<class BoxMLGElement>&) const;

  // refinement methods
  // MathML
  void refineMathMLTokenElement(const DOM::Element&, const SmartPtr<class MathMLTokenElement>&) const;
  void refine_MathML_math_Element(const DOM::Element&, const SmartPtr<class MathMLmathElement>&) const;
  void refine_MathML_mo_Element(const DOM::Element&, const SmartPtr<class MathMLOperatorElement>&) const;
  void refine_MathML_ms_Element(const DOM::Element&, const SmartPtr<class MathMLStringLitElement>&) const;
  void refine_MathML_mspace_Element(const DOM::Element&, const SmartPtr<class MathMLSpaceElement>&) const;
  void refine_MathML_mfrac_Element(const DOM::Element&, const SmartPtr<class MathMLFractionElement>&) const;
  void refine_MathML_msub_Element(const DOM::Element&, const SmartPtr<class MathMLScriptElement>&) const;
  void refine_MathML_msup_Element(const DOM::Element&, const SmartPtr<class MathMLScriptElement>&) const;
  void refine_MathML_msubsup_Element(const DOM::Element&, const SmartPtr<class MathMLScriptElement>&) const;
  void refine_MathML_mmultiscripts_Element(const DOM::Element&, const SmartPtr<class MathMLMultiScriptsElement>&) const;
  void refine_MathML_maction_Element(const DOM::Element&, const SmartPtr<class MathMLActionElement>&) const;
  void refine_MathML_menclose_Element(const DOM::Element&, const SmartPtr<class MathMLEncloseElement>&) const;
  void refine_MathML_mpadded_Element(const DOM::Element&, const SmartPtr<class MathMLPaddedElement>&) const;
  void refine_MathML_mstyle_Element(const DOM::Element&, const SmartPtr<class MathMLStyleElement>&) const;
  void refine_MathML_mtable_Element(const DOM::Element&, const SmartPtr<class MathMLTableElement>&) const;
  void refine_MathML_munder_Element(const DOM::Element&, const SmartPtr<class MathMLUnderOverElement>&) const;
  void refine_MathML_mover_Element(const DOM::Element&, const SmartPtr<class MathMLUnderOverElement>&) const;
  void refine_MathML_munderover_Element(const DOM::Element&, const SmartPtr<class MathMLUnderOverElement>&) const;
  void refine_MathML_malignmark_Element(const DOM::Element&, const SmartPtr<class MathMLAlignMarkElement>&) const;
  void refine_MathML_maligngroup_Element(const DOM::Element&, const SmartPtr<class MathMLAlignGroupElement>&) const;
  // BoxML
  void refine_BoxML_action_Element(const DOM::Element&, const SmartPtr<class BoxMLActionElement>&) const;
  void refine_BoxML_space_Element(const DOM::Element&, const SmartPtr<class BoxMLSpaceElement>&) const;
  void refine_BoxML_h_Element(const DOM::Element&, const SmartPtr<class BoxMLHElement>&) const;
  void refine_BoxML_v_Element(const DOM::Element&, const SmartPtr<class BoxMLVElement>&) const;
  void refine_BoxML_ink_Element(const DOM::Element&, const SmartPtr<class BoxMLInkElement>&) const;
  void refine_BoxML_layout_Element(const DOM::Element&, const SmartPtr<class BoxMLLayoutElement>&) const;
  void refine_BoxML_par_Element(const DOM::Element&, const SmartPtr<class BoxMLParagraphElement>&) const;
  void refine_BoxML_text_Element(const DOM::Element&, const SmartPtr<class BoxMLTextElement>&) const;
  void refine_BoxML_at_Element(const DOM::Element&, const SmartPtr<class BoxMLAtElement>&) const;
  void refine_BoxML_g_Element(const DOM::Element&, const SmartPtr<class BoxMLGroupElement>&) const;

  // auxiliary creation methods
  SmartPtr<class MathMLElement> createMathMLDummyElement(void) const;
  SmartPtr<class MathMLElement> createMathMLElement(const DOM::Element&) const;
  SmartPtr<class BoxMLElement> createBoxMLDummyElement(void) const;
  SmartPtr<class BoxMLElement> createBoxMLElement(const DOM::Element&) const;

  // creation methods
  // MathML (text nodes)
  SmartPtr<class MathMLTextNode> create_MathML_mglyph_Node(const DOM::Element&) const;
  SmartPtr<class MathMLTextNode> create_MathML_malignmark_Node(const DOM::Element&) const;
  // MathML
  SmartPtr<class MathMLElement> create_MathML_math_Element(const DOM::Element&) const;
  SmartPtr<class MathMLElement> create_MathML_mi_Element(const DOM::Element&) const;
  SmartPtr<class MathMLElement> create_MathML_mn_Element(const DOM::Element&) const;
  SmartPtr<class MathMLElement> create_MathML_mo_Element(const DOM::Element&) const;
  SmartPtr<class MathMLElement> create_MathML_mtext_Element(const DOM::Element&) const;
  SmartPtr<class MathMLElement> create_MathML_mspace_Element(const DOM::Element&) const;
  SmartPtr<class MathMLElement> create_MathML_ms_Element(const DOM::Element&) const;
  SmartPtr<class MathMLElement> create_MathML_mrow_Element(const DOM::Element&) const;
  SmartPtr<class MathMLElement> create_MathML_mfrac_Element(const DOM::Element&) const;
  SmartPtr<class MathMLElement> create_MathML_msqrt_Element(const DOM::Element&) const;
  SmartPtr<class MathMLElement> create_MathML_mroot_Element(const DOM::Element&) const;
  SmartPtr<class MathMLElement> create_MathML_mstyle_Element(const DOM::Element&) const;
  SmartPtr<class MathMLElement> create_MathML_merror_Element(const DOM::Element&) const;
  SmartPtr<class MathMLElement> create_MathML_mpadded_Element(const DOM::Element&) const;
  SmartPtr<class MathMLElement> create_MathML_mphantom_Element(const DOM::Element&) const;
  SmartPtr<class MathMLElement> create_MathML_mfenced_Element(const DOM::Element&) const;
  SmartPtr<class MathMLElement> create_MathML_msub_Element(const DOM::Element&) const;
  SmartPtr<class MathMLElement> create_MathML_msup_Element(const DOM::Element&) const;
  SmartPtr<class MathMLElement> create_MathML_msubsup_Element(const DOM::Element&) const;
  SmartPtr<class MathMLElement> create_MathML_munder_Element(const DOM::Element&) const;
  SmartPtr<class MathMLElement> create_MathML_mover_Element(const DOM::Element&) const;
  SmartPtr<class MathMLElement> create_MathML_munderover_Element(const DOM::Element&) const;
  SmartPtr<class MathMLElement> create_MathML_mmultiscripts_Element(const DOM::Element&) const;
  SmartPtr<class MathMLElement> create_MathML_mtable_Element(const DOM::Element&) const;
  SmartPtr<class MathMLElement> create_MathML_maligngroup_Element(const DOM::Element&) const;
  SmartPtr<class MathMLElement> create_MathML_malignmark_Element(const DOM::Element&) const;
  SmartPtr<class MathMLElement> create_MathML_maction_Element(const DOM::Element&) const;
  SmartPtr<class MathMLElement> create_MathML_menclose_Element(const DOM::Element&) const;
  SmartPtr<class MathMLElement> create_MathML_semantics_Element(const DOM::Element&) const;
  // BoxML
  SmartPtr<class BoxMLElement> create_BoxML_action_Element(const DOM::Element&) const;
  SmartPtr<class BoxMLElement> create_BoxML_box_Element(const DOM::Element&) const;
  SmartPtr<class BoxMLElement> create_BoxML_h_Element(const DOM::Element&) const;
  SmartPtr<class BoxMLElement> create_BoxML_obj_Element(const DOM::Element&) const;
  SmartPtr<class BoxMLElement> create_BoxML_ink_Element(const DOM::Element&) const;
  SmartPtr<class BoxMLElement> create_BoxML_g_Element(const DOM::Element&) const;
  SmartPtr<class BoxMLElement> create_BoxML_layout_Element(const DOM::Element&) const;
  SmartPtr<class BoxMLElement> create_BoxML_at_Element(const DOM::Element&) const;
  SmartPtr<class BoxMLElement> create_BoxML_par_Element(const DOM::Element&) const;
  SmartPtr<class BoxMLElement> create_BoxML_space_Element(const DOM::Element&) const;
  SmartPtr<class BoxMLElement> create_BoxML_text_Element(const DOM::Element&) const;
  SmartPtr<class BoxMLElement> create_BoxML_v_Element(const DOM::Element&) const;

  typedef SmartPtr<class MathMLElement> (gmetadom_Builder::* MathMLCreationMethod)(const DOM::Element&) const;
  typedef SmartPtr<class BoxMLElement> (gmetadom_Builder::* BoxMLCreationMethod)(const DOM::Element&) const;

  typedef HASH_MAP_NS::hash_map<String, MathMLCreationMethod, StringHash, StringEq> MathMLBuilderMap;
  typedef HASH_MAP_NS::hash_map<String, BoxMLCreationMethod, StringHash, StringEq> BoxMLBuilderMap;

private:
  MathMLBuilderMap mathmlMap;
  BoxMLBuilderMap boxmlMap;

  DOM::Element root;
  mutable RefinementContext refinementContext;
};

#endif // __gmetadom_Builder_hh__
