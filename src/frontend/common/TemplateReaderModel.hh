
#ifndef __TemplateReaderModel_hh__
#define __TemplateReaderModel_hh__

#include "SmartPtr.hh"
#include "String.hh"

#include "TemplateLinker.hh"
#include "TemplateReaderNodeIterator.hh"
#include "TemplateReaderElementIterator.hh"
#include "TemplateReaderRefinementContext.hh"
#include "TemplateReaderBuilder.hh"

template <class Reader>
struct TemplateReaderModel
{
  enum {
    TEXT_NODE = Reader::TEXT_NODE,
    ELEMENT_NODE = Reader::ELEMENT_NODE
  };

  typedef SmartPtr<Reader> Node;
  typedef SmartPtr<Reader> Element;
  typedef SmartPtr<Reader> Document;
  typedef TemplateReaderNodeIterator<Reader> NodeIterator;
  typedef TemplateReaderElementIterator<Reader> ElementIterator;
  typedef TemplateReaderRefinementContext<Reader> RefinementContext;

  static SmartPtr<Reader> document(const class AbstractLogger&, const String& path, bool subst = false)
  { return Reader::create(path, subst); }

  static SmartPtr<Reader> getDocumentElement(const SmartPtr<Reader>& reader) { return reader; }

  static void             reset(const SmartPtr<Reader>& reader) { reader->reset(); }
  static SmartPtr<Reader> asNode(const SmartPtr<Reader>& reader) { return reader; }
  static SmartPtr<Reader> asElement(const SmartPtr<Reader>& reader) { return reader; }

  static unsigned getNodeType(const SmartPtr<Reader>& reader) { return reader->getNodeType(); }
  static String getNodeName(const SmartPtr<Reader>& reader) { return reader->getNodeName(); }
  static String getNodeValue(const SmartPtr<Reader>& reader) { return reader->getNodeValue(); }
  static String getNodeNamespaceURI(const SmartPtr<Reader>& reader) { return reader->getNodeNamespaceURI(); }
  
  static String getElementValue(const SmartPtr<Reader>& reader)
  {
    String value;
    for (NodeIterator iter(reader); iter.more(); iter.next())
      if (reader->getNodeType() == TEXT_NODE) value += reader->getNodeValue();
    return value;
  }
  static String getAttribute(const SmartPtr<Reader>& reader, const String& name) { return reader->getAttribute(name); }
  static bool hasAttribute(const SmartPtr<Reader>& reader, const String& name) { return reader->hasAttribute(name); }

  struct Hash
  {
    size_t operator()(const SmartPtr<Reader>& reader) const
    { return reader->getNodeId(); }
  };
};

#endif // __TemplateReaderModel_hh__
