
  // TEMPORARY FACTORY METHODS FOR BOXML
  SmartPtr<MathMLElement> createAtElement(const SmartPtr<class MathMLView>& view) const
  { return BoxMLAtElement::create(view); }
  SmartPtr<MathMLElement> createGElement(const SmartPtr<class MathMLView>& view) const
  { return BoxMLGElement::create(view); }
  SmartPtr<MathMLElement> createHElement(const SmartPtr<class MathMLView>& view) const
  { return BoxMLHElement::create(view); }
  SmartPtr<MathMLElement> createInkElement(const SmartPtr<class MathMLView>& view) const
  { return BoxMLInkElement::create(view); }
  SmartPtr<MathMLElement> createBoxMLSpaceElement(const SmartPtr<class MathMLView>& view) const
  { return BoxMLSpaceElement::create(view); }
  SmartPtr<MathMLElement> createBoxMLTextElement(const SmartPtr<class MathMLView>& view) const
  { return BoxMLTextElement::create(view); }
  SmartPtr<MathMLElement> createVElement(const SmartPtr<class MathMLView>& view) const
  { return BoxMLVElement::create(view); }
  SmartPtr<MathMLElement> createBoxElement(const SmartPtr<class MathMLView>& view) const
  { return BoxMLboxElement::create(view); }
  SmartPtr<MathMLElement> createBoxMLActionElement(const SmartPtr<class MathMLView>& view) const
  { return BoxMLActionElement::create(view); }
  SmartPtr<MathMLElement> createBoxMLObjectElement(const SmartPtr<class MathMLView>& view) const
  { return BoxMLObjectElement::create(view); }
  
