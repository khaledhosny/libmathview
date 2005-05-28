// Copyright (C) 2000-2005, Luca Padovani <luca.padovani@cs.unibo.it>.
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

#include <sstream>

#include "defs.h"
#include "AbstractLogger.hh"
#include "ValueConversion.hh"
#include "Fragment.hh"
#include "Location.hh"
#include "SMS.hh"
#include "Scanner.hh"
#include "SVG_libxml2_StreamRenderingContext.hh"
#include "LengthAux.hh"
#include "Element.hh"

#define SVG_NS_URI "http://www.w3.org/2000/svg"
#define GMV_NS_URI "http://helm.cs.unibo.it/2005/GtkMathView"
 
SMS::SMS(const SmartPtr<AbstractLogger>& _logger,
	 const SmartPtr<MathView>& _view)
  : logger(_logger), view(_view), evalContext(logger, view)
{
  funMap["x"] = &SMS::fun_x;
  funMap["y"] = &SMS::fun_y;
  funMap["width"] = &SMS::fun_width;
  funMap["height"] = &SMS::fun_height;
  funMap["depth"] = &SMS::fun_depth;
  funMap["add"] = &SMS::fun_add;
  funMap["sub"] = &SMS::fun_sub;
  funMap["mul"] = &SMS::fun_mul;
  funMap["div"] = &SMS::fun_div;  
  funMap["neg"] = &SMS::fun_neg;
}

scaled
SMS::evaluate(const Length& length, const scaled& defaultValue)
{
  switch (length.type)
    {
    case Length::INFINITY_UNIT:
      return scaled::max();
    case Length::IN_UNIT:
      return scaled(72.27 * length.value);
    case Length::CM_UNIT:
      return scaled(72.27 * (length.value / 2.54));
    case Length::MM_UNIT:
      return scaled(72.27 * (length.value / 25.4));
    case Length::PT_UNIT:
      return scaled(length.value);
    case Length::PC_UNIT:
      return scaled(12 * 72.27 * length.value);
    case Length::PERCENTAGE_UNIT:
      return (defaultValue * length.value) / 100.0;
    case Length::UNDEFINED_UNIT:
    case Length::EM_UNIT:
    case Length::EX_UNIT:
    case Length::PX_UNIT:
    case Length::PURE_UNIT:
    default:
      assert(false);
    }
}

#include "scaledAux.hh"

void
SMS::traverse(const Model::Node& node)
{
  assert(node);
  std::cerr << "found " << Model::getNodeName(node) << std::endl;
  if (Model::getNodeType(node) == Model::ELEMENT_NODE)
    {
      Model::Element elem = Model::asElement(node);
      assert(elem);
      if (Model::getNodeNamespaceURI(node) == SVG_NS_URI
	  && Model::getNodeName(node) == "foreignObject"
	  && Model::getAttribute(elem, "requiredExtension") == MATHML_NS_URI)
	{
	  Model::ElementIterator p(elem, MATHML_NS_URI);
	  if (Model::Element mmlRoot = p.element())
	    {
	      std::cerr << "found MathML fragment" << std::endl;
	      view->loadRootElement(mmlRoot);
	      // set available width with width attribute on foreignObject
	      if (SmartPtr<Element> root = view->getRootElement())
		{
		  //AreaRef area = root->getArea();
		  //assert(area);

		  const scaled x = evalScaled(Model::getAttribute(elem, "x"), scaled::zero());
		  const scaled y = evalScaled(Model::getAttribute(elem, "y"), scaled::zero());
		  std::cerr << "found coordinates " << x << "," << x << std::endl;

		  view->render(evalContext, x, -y);

		  std::ostringstream os;
		  SVG_libxml2_StreamRenderingContext context(logger, os, view);
		  //std::cerr << "found fragment area " << area->box() << std::endl;
		  const BoundingBox box = view->getBoundingBox();
		  context.documentStart(box);
		  view->render(context, x, -y);
		  context.documentEnd();
		  const String buffer = os.str();
		  std::cerr << "fragment SVG" << std::endl << buffer << std::endl;
		  if (Model::Document doc = xmlParseMemory(buffer.c_str(), buffer.length()))
		    {
		      std::cerr << "fragment parsed" << std::endl;
		      if (Model::Element root = Model::getDocumentElement(doc))
			{
			  std::cerr << "got parsed root" << std::endl;
			  Model::ElementIterator p(root, SVG_NS_URI, "g");
			  Model::Element clonedFragment = Model::asElement(xmlCopyNode(Model::asNode(p.element()), 1));
			  assert(clonedFragment);
			  SmartPtr<Fragment> frag = Fragment::create(elem, clonedFragment);
			  fragList.push_back(frag);
			}
		      xmlFreeDoc(doc);
		    }
		}
	      view->resetRootElement();
	    }
	}
      else
	{
	  for (Model::NodeIterator p(node); p.more(); p.next())
	    traverse(p.node());
	}
    }
}

#include "BoundingBoxAux.hh"

void
SMS::substFragments()
{
  for (std::list<SmartPtr<Fragment> >::const_iterator p = fragList.begin(); p != fragList.end(); p++)
    {
      const SmartPtr<Fragment> fragment = *p;
      xmlReplaceNode(Model::asNode(fragment->getOldRoot()), 
		     Model::asNode(fragment->getNewRoot()));
    }
}

typedef Variant<scaled> ScaledValue;
typedef Variant<float> NumberValue;
typedef Variant<String> StringValue;
typedef Variant<SmartPtr<Location> > LocationValue;
typedef Variant<Point> PointValue;

static SmartPtr<Location>
asLocation(const SmartPtr<Value>& value)
{
  if (is_a<LocationValue>(value))
    return smart_cast<LocationValue>(value)->getValue();
  else
    return 0;
}

static scaled
asScaled(const SmartPtr<Value>& value)
{
  const SmartPtr<ScaledValue> v = smart_cast<ScaledValue>(value);
  assert(v);
  return v->getValue();
}

static float
asNumber(const SmartPtr<Value>& value)
{
  const SmartPtr<NumberValue> v = smart_cast<NumberValue>(value);
  assert(v);
  return v->getValue();
}

static String
asString(const SmartPtr<Value>& value)
{
  const SmartPtr<StringValue> v = smart_cast<StringValue>(value);
  assert(v);
  return v->getValue();
}

SMS::Handler
SMS::getFunHandler(const String& name) const
{
  const FunMap::const_iterator p = funMap.find(name);
  if (p != funMap.end())
    return p->second;
  else
    return 0;
}

SmartPtr<Value>
SMS::fun_x(const HandlerArgs& args) const
{
  if (args.size() != 1)
    return 0;

  if (const SmartPtr<Location> loc = asLocation(args[0]))
    return ScaledValue::create(loc->getX());
  else
    return 0;
}

SmartPtr<Value>
SMS::fun_y(const HandlerArgs& args) const
{
  if (args.size() != 1)
    return 0;

  if (const SmartPtr<Location> loc = asLocation(args[0]))
    return ScaledValue::create(-loc->getY());
  else
    return 0;
}

SmartPtr<Value>
SMS::fun_width(const HandlerArgs& args) const
{
  if (args.size() != 1)
    return 0;

  if (const SmartPtr<Location> loc = asLocation(args[0]))
    return ScaledValue::create(loc->getBox().width);
  else
    return 0;
}

SmartPtr<Value>
SMS::fun_height(const HandlerArgs& args) const
{
  if (args.size() != 1)
    return 0;

  if (const SmartPtr<Location> loc = asLocation(args[0]))
    return ScaledValue::create(loc->getBox().height);
  else
    return 0;
}

SmartPtr<Value>
SMS::fun_depth(const HandlerArgs& args) const
{
  if (args.size() != 1)
    return 0;

  if (const SmartPtr<Location> loc = asLocation(args[0]))
    return ScaledValue::create(loc->getBox().depth);
  else
    return 0;
}

SmartPtr<Value>
SMS::fun_add(const HandlerArgs& args) const
{
  scaled res = 0;
  for (HandlerArgs::const_iterator p = args.begin(); p != args.end(); p++)
    {
      if (SmartPtr<ScaledValue> v = smart_cast<ScaledValue>(*p))
	res += v->getValue();
      else
	return 0;
    }
  return ScaledValue::create(res);
}

SmartPtr<Value>
SMS::fun_sub(const HandlerArgs& args) const
{
  if (args.size() != 2)
    return 0;

  if (SmartPtr<ScaledValue> v1 = smart_cast<ScaledValue>(args[0]))
    if (SmartPtr<ScaledValue> v2 = smart_cast<ScaledValue>(args[1]))
      return ScaledValue::create(v1->getValue() - v2->getValue());
  return 0;
}

SmartPtr<Value>
SMS::fun_mul(const HandlerArgs& args) const
{
  if (args.size() != 2)
    return 0;

  if (SmartPtr<ScaledValue> v1 = smart_cast<ScaledValue>(args[0]))
    if (SmartPtr<NumberValue> v2 = smart_cast<NumberValue>(args[1]))
      return ScaledValue::create(v1->getValue() * v2->getValue());
  return 0;
}

SmartPtr<Value>
SMS::fun_neg(const HandlerArgs& args) const
{
  if (args.size() != 1)
    return 0;

  if (SmartPtr<ScaledValue> v = smart_cast<ScaledValue>(args[0]))
    return ScaledValue::create(-v->getValue());
  return 0;
}

SmartPtr<Value>
SMS::fun_div(const HandlerArgs& args) const
{
  if (args.size() != 2)
    return 0;

  if (SmartPtr<ScaledValue> v1 = smart_cast<ScaledValue>(args[0]))
    if (SmartPtr<NumberValue> v2 = smart_cast<NumberValue>(args[1]))
      return ScaledValue::create(v1->getValue() / v2->getValue());
  return 0;
}

SmartPtr<Value>
SMS::evalExpr(Scanner& scanner)
{
  std::cerr << "SMS::evalExpr ENTER get token " << scanner.getToken() << std::endl;
  switch (scanner.getToken())
    {
    case Scanner::NUMBER:
      {
	const float value = scanner.getNumber();
	scanner.advance();
	std::cerr << "read number " << value << std::endl;
	if (scanner.getToken() == Scanner::ID)
	  {
	    const String unit = StringOfUCS4String(scanner.getString());
	    Length::Unit unitId;
	    if (fromString(unit, unitId))
	      {
		std::cerr << "read unit " << unit << std::endl;
		scanner.advance();
		return ScaledValue::create(evaluate(Length(value, unitId), scaled::zero()));
	      }
	  }
	else
	  return NumberValue::create(value);
      }
      break;
    case Scanner::SHARP:
      {
	scanner.advance();
	if (scanner.getToken() != Scanner::ID)
	  return 0;
	const String name = StringOfUCS4String(scanner.getString());
	scanner.advance();
	std::cerr << "Found Location " << name << std::endl;
	return LocationValue::create(evalContext.getLocation(name));
      }
      break;
    case Scanner::ID:
      {
	const String id = StringOfUCS4String(scanner.getString());
	scanner.advance();
	std::cerr << "found ID = " << id << std::endl;

	std::vector<SmartPtr<Value> > args;
	while (scanner.getToken() != Scanner::EOS 
	       && scanner.getToken() != Scanner::ERROR
	       && scanner.getToken() != Scanner::RPAREN
	       && scanner.getToken() != Scanner::RBRACE)
	  {
	    if (SmartPtr<Value> v = evalExpr(scanner))
	      args.push_back(v);
	    else
	      return 0;
	  }

	std::cerr << "found " << args.size() << " arguments" << std::endl;

	switch (scanner.getToken())
	  {
	  case Scanner::RPAREN:
	  case Scanner::RBRACE:
	    if (Handler handler = getFunHandler(id))
	      return (this->*handler)(args);
	    else
	      return 0;
	    break;
	  case Scanner::ERROR:
	  case Scanner::EOS:
	    return 0;
	  default:
	    assert(false);
	  }
      }
    case Scanner::LPAREN:
      {
	scanner.advance();
	const SmartPtr<Value> value = evalExpr(scanner);
	if (scanner.getToken() == Scanner::RPAREN)
	  {
	    scanner.advance();
	    return value;
	  }
	else
	  return 0;
      }
      break;
    default:
      break;
    }

  return 0;
}

String
SMS::evalAttribute(const String& value)
{
  std::ostringstream os;
  Scanner scanner(UCS4StringOfString(value));
  while (true)
    {
      switch (scanner.getToken())
	{
	case Scanner::RAW:
	  os << StringOfUCS4String(scanner.getString());
	  scanner.advance();
	  break;
	case Scanner::UNDEFINED:
	  assert(false);
	  break;
	case Scanner::EOS:
	  return os.str();
	case Scanner::ERROR:
	  return value;
	case Scanner::LBRACE:
	  {
	    scanner.advance();
	    const SmartPtr<Value> v = evalExpr(scanner);
	    if (scanner.getToken() != Scanner::RBRACE)
	      return value;
	    scanner.advance();
	    if (is_a<ScaledValue>(v))
	      os << asScaled(v).toFloat() << "pt";
	    else if (is_a<NumberValue>(v))
	      os << asNumber(v);
	    else if (is_a<StringValue>(v))
	      os << asString(v);
	    else
	      return value;
	  }
	  break;
	default:
	  assert(false);
	  return value;
	}
    }
  return value;
}

#if 0
bool
SMS::evalExpr(const UCS4String::const_iterator& begin,
	      const UCS4String::const_iterator& end,
	      UCS4String::const_iterator& next,
	      scaled& res)
{
  UCS4String::const_iterator p = begin;
  ScanSpaces::scan(p, end, next);
  if (next == end) return false;
  p = next;
  
  if (ScanLiteral<'+'>::scan(p, end, next)
      || ScanLiteral<'-'>::scan(p, end, next)
      || ScanLiteral<'*'>::scan(p, end, next)
      || ScanLiteral<'/'>::scan(p, end, next))
    {
      UCS4String::value_type op = *p;
      p = next;
      scaled a;
      if (!evalExpr(p, end, next, a)) return false;
      p = next;
      scaled b;
      if (!evalExpr(p, end, next, b)) return false;
      switch (op)
	{
	case '+':
	  res = a + b;
	  break;
	case '-':
	  res = a - b;
	  break;
	case '*':
	  res = a * b;
	  break;
	case '/':
	  assert(false);
	  //res = a / b;
	  break;
	default:
	  return false;
	}
      return true;
    }
  else if (ScanLiteral<'#'>::scan(p, end, next))
    {
      std::cerr << "found reference 1" << std::endl;
      p = next;
      SmartPtr<Value> idV = ParseId::parse(p, end, next);
      if (!idV) return false;
      p = next;
      std::cerr << "found reference 2 " << ToString(idV) << std::endl;
      if (!ScanLiteral<'@'>::scan(p, end, next))
	return false;
      p = next;
      std::cerr << "found reference 3" << std::endl;
      SmartPtr<Value> propV = ParseKeyword::parse(p, end, next);
      if (!propV) return false;

      const String id = ToString(idV);
      const String prop = ToString(propV);
      std::cerr << "searching for property " << id << "@" << prop << std::endl;
      if (const SmartPtr<Location> loc = evalContext.getLocation(id))
	{
	  if (prop == "width")
	    res = loc->getBox().width;
	  else if (prop == "height")
	    res = loc->getBox().height;
	  else if (prop == "depth")
	    res = loc->getBox().depth;
	  else if (prop == "x")
	    res = loc->getX();
	  else if (prop == "y")
	    res = -loc->getY();
	  else
	    return false;
	  return true;
	}
      else
	return false;
    }
  else if (SmartPtr<Value> v = ParseLength::parse(p, end, next))
    {
      const Length l = ToLength(v);
      res = evaluate(l);
      return true;
    }
  else if (SmartPtr<Value> v = ParseKeyword::parse(p, end, next))
    {
      assert(false);
      // a function?
      // abs max min sin cos tan
      return false;
    }
}
#endif

scaled
SMS::evalScaled(const String& s, const scaled& defaultValue)
{
  const UCS4String us = UCS4StringOfString(s);
  Scanner scanner(us, false);
  if (SmartPtr<ScaledValue> v = smart_cast<ScaledValue>(evalExpr(scanner)))
    return v->getValue();
  else
    return defaultValue;
}

void
SMS::evalAttributes(const Model::Node& node)
{
  assert(node);
  if (Model::getNodeType(node) == Model::ELEMENT_NODE)
    {
      Model::Element elem = Model::asElement(node);
      for (Model::Node attr = (Model::Node) elem->attributes; attr; attr = attr->next)
	{
	  const String value = Model::getNodeValue(attr);
	  const String v1 = evalAttribute(value);
	  std::cerr << "found attribute " << Model::getNodeName(attr) << "=" << value << " -> " << v1 << std::endl;
	  xmlNodeSetContent(attr, Model::toModelString(v1));
	}
      
      for (Model::NodeIterator p(node); p.more(); p.next())
	evalAttributes(p.node());
    }
}

bool
SMS::process(const String& uri)
{
  doc = Model::document(*logger, uri, false);
  if (!doc) return false;

  std::cerr << "TRAVERSE" << std::endl;
  traverse(Model::asNode(Model::getDocumentElement(doc)));
  std::cerr << "SUBST" << std::endl;
  substFragments();
  std::cerr << "EVAL" << std::endl;
  evalAttributes(Model::asNode(Model::getDocumentElement(doc)));
  std::cerr << "DONE" << std::endl;
  xmlSaveFile("a.svg", doc);

  return true;
}

