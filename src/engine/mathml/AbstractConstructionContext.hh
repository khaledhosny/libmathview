
class AbstractConstructionContext
{
public:
  virtual void enter(const String&, const String&) = 0;
  virtual void exit(void) = 0;
  virtual bool hasNext(void) const = 0;
  virtual String getLocalName(void) const = 0;
  virtual String getNamespace(void) const = 0;
  virtual SmartPtr<MathMLElement> getElement(const SmartPtr<MathMLElement>& oldElement, bool create = true) = 0;
  virtual void skip(void) = 0;
  virtual String getAttribute(const String& name) const = 0;
  virtual String getAttribute(unsigned index) const = 0;
  virtual unsigned nAttributes(void) const = 0;
};
