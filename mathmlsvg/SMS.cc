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
#include "Point.hh"
#include "TemplateStringParsers.hh"

#define SVG_NS_URI "http://www.w3.org/2000/svg"
#define GMV_NS_URI "http://helm.cs.unibo.it/2005/GtkMathView"
 
SMS::SMS(const SmartPtr<AbstractLogger>& _logger,
	 const SmartPtr<MathView>& _view,
	 const scaled& _pageWidth, const scaled& _pageHeight,
	 float _dpi)
  : logger(_logger), view(_view), evalContext(logger, view),
    pageWidth(_pageWidth), pageHeight(_pageHeight), dpi(_dpi)
{
  funMap["pair"] = &SMS::fun_pair;
  funMap["add"] = &SMS::fun_add;
  funMap["sub"] = &SMS::fun_sub;
  funMap["mul"] = &SMS::fun_mul;
  funMap["div"] = &SMS::fun_div;  
  funMap["neg"] = &SMS::fun_neg;
  funMap["x"] = &SMS::fun_x;
  funMap["y"] = &SMS::fun_y;
  funMap["origin"] = &SMS::fun_origin;
  funMap["center"] = &SMS::fun_center;
  funMap["width"] = &SMS::fun_width;
  funMap["height"] = &SMS::fun_height;
  funMap["depth"] = &SMS::fun_depth;
  funMap["size"] = &SMS::fun_size;

  funMap["north-west"] = &SMS::fun_north_west;
  funMap["north"] = &SMS::fun_north;
  funMap["north-east"] = &SMS::fun_north_east;
  funMap["east"] = &SMS::fun_east;
  funMap["south-east"] = &SMS::fun_south_east;
  funMap["south"] = &SMS::fun_south;
  funMap["south-west"] = &SMS::fun_south_west;
  funMap["west"] = &SMS::fun_west;

  funMap["depart"]  = &SMS::fun_depart;
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

bool
SMS::sortFragments(std::list<SmartPtr<Fragment> >& sortedFragmentList) const
{
  for (std::list<SmartPtr<Fragment> >::const_iterator f = fragmentList.begin(); f != fragmentList.end(); f++)
    if ((*f)->visit(sortedFragmentList))
      return true;
  return false;
}


bool xmlIsNull(const xmlChar* str)
{
  return(str[0] == (xmlChar)'0');
}

bool SMS::isGmvNamespace(const xmlChar* ns)
{
  int i;
  char *gmvNs = GMV_NS_URI;
  for (i = 0; i < 41; i++)
    {
      if (ns[i] != gmvNs[i])
	return(false);
    }
  return(true);
}

// Funzione che cerca gli ID all'interno degli attributi
// deve essere sostituita con una funzione di parsing dell'intero attributo

std::list<xmlChar*>
SMS::getDepFromAttr(const xmlChar *value)
{
  std::list<xmlChar *> idRetLst;
  String attribute((char *)value);
  Scanner scanner(UCS4StringOfString(attribute), false);
  
  while (true)
    {
      switch(scanner.getToken())
	{
	case Scanner::RAW :
	  scanner.advance();
	  break;
	  
	case Scanner::EOS :
	  return(idRetLst);
	  break;
	  
	case Scanner::DOLLAR :
	  scanner.advance();
	  if (scanner.getToken() == Scanner::ID)
	    {
	      String tmp("");
	      tmp.append(StringOfUCS4String(scanner.getString()));
	      scanner.advance();
	      xmlChar *xmlTmp = (xmlChar *)calloc(sizeof(tmp.c_str()), sizeof(xmlChar));
	      strcpy((char *)xmlTmp, (char *)tmp.c_str());
	      idRetLst.push_back((xmlChar *)xmlTmp);
	      break;
	    }
	  else
	    {
	      scanner.advance();
	      break;
	    }
	  
	case Scanner::ID :
	  scanner.advance();
	  break;
	  
	  
	default :
	  scanner.advance();
	  break;
	}
    }
}

void
SMS::assoc(const Model::Node& node, const SmartPtr<Fragment>& frag)
{
  if (Model::Element el = Model::asElement(node))
    {
      if (Model::hasAttribute(el, "id"))
        idFragmentMap[Model::getAttribute(el, "id")] = frag;
      for (Model::NodeIterator p(Model::asNode(el)); p.more(); p.next())
	assoc(p.node(), frag);
    }
}

void
SMS::findFragmentDependencies()
{
  for (std::list<SmartPtr<Fragment> >::iterator f = fragmentList.begin();
       f != fragmentList.end();
       f++)
    if (xmlAttr* attr = Model::asNode((*f)->getOldRoot())->properties)
      for (; attr != NULL; attr = attr->next)
	{
	  if (Model::Node attrNode = (xmlNode*) attr)
	    if (Model::getNodeNamespaceURI(attrNode) == GMV_NS_URI)
	      for (Scanner scanner(UCS4StringOfString(Model::getNodeValue(attrNode)), false); scanner.more(); scanner.advance())
// FIX MEEEEEEEEEE!!!!!!!!
//
		if (scanner.getToken() == Scanner::ID)
                {
                  String readString = StringOfUCS4String(scanner.getString());
                  if (readString == "width" || readString == "height" || readString == "depth")
                  {
                    scanner.advance();
                    while (scanner.getToken() != Scanner::ID) scanner.advance();
                    scanner.advance();
                  }
		  else if (SmartPtr<Fragment> tmp = idFragmentMap[StringOfUCS4String(scanner.getString())])
		    (*f)->addDependency(tmp);
	        }
        }
}

void
SMS::traverse(const Model::Node& node)
{
  assert(node);
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
	      view->loadRootElement(mmlRoot);
	      // set available width with width attribute on foreignObject
	      if (SmartPtr<Element> root = view->getRootElement())
		{
		  // questa prima resa serve per conoscere le dimensioni del frammento
		  // e di tutti gli elementi MathML in esso contenuti
		  
		  view->render(evalContext, 0, 0);
		  std::ostringstream os;
		  SVG_libxml2_StreamRenderingContext context(logger, os, view);
		  const BoundingBox box = view->getBoundingBox();
		  context.documentStart(box);
		  // questa seconda resa serve per generare l'SVG corrispondente al
		  // frammento di MathML
		  view->render(context, 0, 0);
		  context.documentEnd();
		  
		  // creazione albero SVG equivalente
		  
		  const String buffer = os.str();
		  if (Model::Document doc = xmlParseMemory(buffer.c_str(), buffer.length()))
		    {
		      if (Model::Element root = Model::getDocumentElement(doc))
			{
			  Model::ElementIterator p(root, SVG_NS_URI, "g");
			  Model::Element clonedFragment = Model::asElement(xmlCopyNode(Model::asNode(p.element()), 1));
			  assert(clonedFragment);
			  SmartPtr<Fragment> frag = Fragment::create(elem, clonedFragment, box);
			  // aggiungi un fragment alla lista fragList
			  fragmentList.push_back(frag);
			  // crea una lista associativa fra id e fragment di riferimento
			  assoc(node, frag);
			  // crea una lista fra fragment e id dai quali dipende
			  
			  
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

float
SMS::toUserUnits(const scaled& s) const
{
  return (s.toFloat() * viewBoxWidth) / canvasWidth.toFloat();
}

scaled
SMS::fromUserUnits(float f) const
{
  return (canvasWidth * f) / viewBoxWidth;
}

void
SMS::substFragments()
{
  for (std::list<SmartPtr<Fragment> >::const_iterator p = fragmentList.begin(); p != fragmentList.end(); p++)
    {
      const SmartPtr<Fragment> fragment = *p;
      std::ostringstream os;
      const scaled ox = fragment->getX() - fragment->getBoundingBox().width / 2;
      const scaled oy = fragment->getY() + (fragment->getBoundingBox().height - fragment->getBoundingBox().depth) / 2;
      os << "translate(" << toUserUnits(ox)
	 << ", " << toUserUnits(oy) << ")";
      Model::setAttribute(fragment->getNewRoot(), "transform", os.str());
      xmlReplaceNode(Model::asNode(fragment->getOldRoot()), 
		     Model::asNode(fragment->getNewRoot()));
    }
}

typedef Variant<float> NumberValue;
typedef Variant<scaled> ScalarValue;
typedef Variant<SmartPtr<Location> > RefValue;
typedef Variant<Point> PairValue;

String
SMS::serializeValue(const SmartPtr<Value>& value) const
{
  if (is_a<RefValue>(value))
    return smart_cast<RefValue>(value)->getValue()->getId();
  else if (is_a<NumberValue>(value))
    {
      std::ostringstream os;
      os << smart_cast<NumberValue>(value)->getValue();
      return os.str();
    }
  else if (is_a<ScalarValue>(value))
    {
      std::ostringstream os;
      os << toUserUnits(smart_cast<ScalarValue>(value)->getValue());
      return os.str();
    }
  else if (is_a<PairValue>(value))
    {
      const SmartPtr<PairValue> pair = smart_cast<PairValue>(value);
      std::ostringstream os;
      os << toUserUnits(pair->getValue().x) << "," << toUserUnits(pair->getValue().y);
      return os.str();
    }
  else
    {
      assert(false);
      return "???";
    }
}

bool
SMS::asLocation(const SmartPtr<Value>& value, SmartPtr<Location>& res) const
{
  if (is_a<RefValue>(value)) {
    res = smart_cast<RefValue>(value)->getValue();
    return true;
  }
  return false;
}

bool
SMS::asNumber(const SmartPtr<Value>& value, float& res) const
{
  if (is_a<NumberValue>(value)) {
    res = smart_cast<NumberValue>(value)->getValue();
    return true;
  }
  return false;
}

bool
SMS::asScalar(const SmartPtr<Value>& value, scaled& res) const
{
  if (is_a<NumberValue>(value)) {
    res = fromUserUnits(smart_cast<NumberValue>(value)->getValue());
    return true;
  } if (is_a<ScalarValue>(value)) {
    res = smart_cast<ScalarValue>(value)->getValue();
    return true;
  }
  return false;
}

bool
SMS::asPair(const SmartPtr<Value>& value, Point& res) const
{
  if (is_a<PairValue>(value)) {
    res = smart_cast<PairValue>(value)->getValue();
    return true;
  }
  return false;
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
SMS::fun_pair(const HandlerArgs& args) const
{
  if (args.size() != 2)
    return 0;

  scaled s1;
  scaled s2;
  if (asScalar(args[0], s1) && asScalar(args[1], s2))
    return PairValue::create(Point(s1, s2));
  else
    return 0;
}

SmartPtr<Value>
SMS::fun_x(const HandlerArgs& args) const
{
  if (args.size() != 1)
    return 0;

  Point point;
  if (asPair(args[0], point))
    return ScalarValue::create(point.x);
  else
    return 0;
}

SmartPtr<Value>
SMS::fun_y(const HandlerArgs& args) const
{
  if (args.size() != 1)
    return 0;
  
  Point point;
  if (asPair(args[0], point))
    return ScalarValue::create(point.y);
  else
    return 0;
}

SmartPtr<Value>
SMS::fun_origin(const HandlerArgs& args) const
{
  if (args.size() != 1)
    return 0;
  
  SmartPtr<Location> loc;
  if (asLocation(args[0], loc))
    return PairValue::create(Point(loc->getX(), loc->getY()));
  else
    return 0;
}

SmartPtr<Value>
SMS::fun_center(const HandlerArgs& args) const
{
  if (args.size() != 1)
    return 0;
  
  SmartPtr<Location> loc;
  if (asLocation(args[0], loc))
    return PairValue::create(Point(loc->getX() + loc->getBoundingBox().width / 2,
				   loc->getY() - (loc->getBoundingBox().height - loc->getBoundingBox().depth) / 2));
  else
    return 0;
}

SmartPtr<Value>
SMS::fun_size(const HandlerArgs& args) const
{
  if (args.size() != 1)
    return 0;
  
  SmartPtr<Location> loc;
  if (asLocation(args[0], loc))
    return PairValue::create(Point(loc->getBoundingBox().width, loc->getBoundingBox().height + loc->getBoundingBox().depth));
  else
    return 0;
}

SmartPtr<Value>
SMS::fun_width(const HandlerArgs& args) const
{
  if (args.size() != 1)
    return 0;
  
  SmartPtr<Location> loc;
  if (asLocation(args[0], loc))
    return ScalarValue::create(loc->getBoundingBox().width);
  else
    return 0;
}

SmartPtr<Value>
SMS::fun_height(const HandlerArgs& args) const
{
  if (args.size() != 1)
    return 0;
  
  SmartPtr<Location> loc;
  if (asLocation(args[0], loc))
    return ScalarValue::create(loc->getBoundingBox().height);
  else
    return 0;
}

SmartPtr<Value>
SMS::fun_depth(const HandlerArgs& args) const
{
  if (args.size() != 1)
    return 0;
  
  SmartPtr<Location> loc;
  if (asLocation(args[0], loc))
    return ScalarValue::create(loc->getBoundingBox().depth);
  else
    return 0;
}

SmartPtr<Value>
SMS::fun_add(const HandlerArgs& args) const
{  
  if (args.size() != 2)
    return 0;
  
  float n1;
  float n2;
  Point pair1;
  Point pair2;
  scaled s1;
  scaled s2;
  if (asNumber(args[0], n1) && asNumber(args[1], n2))
    return NumberValue::create(n1 + n2);
  else if (asScalar(args[0], s1) && asScalar(args[1], s2))
    return ScalarValue::create(s1 + s2);
  else if (asPair(args[0], pair1) && asPair(args[1], pair2))
    return PairValue::create(Point(pair1.x + pair2.x, pair1.y + pair2.y));
  else
    return 0;

}

SmartPtr<Value>
SMS::fun_sub(const HandlerArgs& args) const
{
  if (args.size() != 2)
    return 0;
  float f1;
  float f2;
  Point pair1;
  Point pair2;
  scaled s1;
  scaled s2;
  if (asNumber(args[0], f1) && asNumber(args[1], f2))
    return(NumberValue::create(f1 - f2));
  if (asScalar(args[0], s1) && asScalar(args[1], s2))
    return ScalarValue::create(s1 - s2);
  else if (asPair(args[0], pair1) && asPair(args[1], pair2))
    return PairValue::create(Point(pair1.x - pair2.x, pair1.y - pair2.y));
  else
    return 0;

}

SmartPtr<Value>
SMS::fun_mul(const HandlerArgs& args) const
{
  if (args.size() != 2)
    return 0;
  
  Point pair;
  scaled s;
  float n1;
  float n2;
  if (asNumber(args[0], n1) && asNumber(args[1], n2))
    return NumberValue::create(n1 * n2);
  else if (asScalar(args[0], s) && asNumber(args[1], n2))
    return ScalarValue::create(s * n2);
  else if (asPair(args[0], pair) && asNumber(args[1], n2))
    return PairValue::create(Point(pair.x * n2, pair.y * n2));
  else
    return 0;
}

SmartPtr<Value>
SMS::fun_neg(const HandlerArgs& args) const
{
  if (args.size() != 1)
    return 0;
  
  float n;
  scaled s;
  Point p;
  if (asNumber(args[0], n))
    return NumberValue::create(-n);
  if (asScalar(args[0], s))
    return ScalarValue::create(-s);
  if (asPair(args[0], p))
    return PairValue::create(Point(-p.x, -p.y));
  else return 0;
}

SmartPtr<Value>
SMS::fun_north_west(const HandlerArgs& args) const
{
  if (args.size() != 1)
    return 0;
  SmartPtr<Location> loc;
  if (asLocation(args[0], loc))
    return PairValue::create(Point(loc->getX(), loc->getY() - loc->getBoundingBox().height));
  else return 0;
}

SmartPtr<Value>
SMS::fun_north(const HandlerArgs& args) const
{
  if (args.size() != 1)
    return 0;
  SmartPtr<Location> loc;
  if (asLocation(args[0], loc))
    return PairValue::create(Point(loc->getX() + (loc->getBoundingBox().width / 2), loc->getY() - loc->getBoundingBox().height));
  else return 0;
}

SmartPtr<Value>
SMS::fun_north_east(const HandlerArgs& args) const
{
  if (args.size() != 1)
    return 0;
  SmartPtr<Location> loc;
  if (asLocation(args[0], loc))
    return PairValue::create(Point(loc->getX() + loc->getBoundingBox().width , loc->getY() - loc->getBoundingBox().height));
  else return 0;
}

SmartPtr<Value>
SMS::fun_east(const HandlerArgs& args) const
{
  if (args.size() != 1)
    return 0;
  SmartPtr<Location> loc;
  if (asLocation(args[0], loc))
    return PairValue::create(Point(loc->getX() + loc->getBoundingBox().width, loc->getY() - loc->getBoundingBox().height + ((loc->getBoundingBox().height + loc->getBoundingBox().depth) / 2)));
  else return 0;
}

SmartPtr<Value>
SMS::fun_south_east(const HandlerArgs& args) const
{
  if (args.size() != 1)
    return 0;
  SmartPtr<Location> loc;
  if (asLocation(args[0], loc))
    return PairValue::create(Point(loc->getX() + loc->getBoundingBox().width, loc->getY() + loc->getBoundingBox().depth));
  else return 0;
}

SmartPtr<Value>
SMS::fun_south(const HandlerArgs& args) const
{
  if (args.size() != 1)
    return 0;
  SmartPtr<Location> loc;
  if (asLocation(args[0], loc))
    return PairValue::create(Point(loc->getX() + (loc->getBoundingBox().width / 2), loc->getY() + loc->getBoundingBox().depth));
  else return 0;
}

SmartPtr<Value>
SMS::fun_south_west(const HandlerArgs& args) const
{
  if (args.size() != 1)
    return 0;
  SmartPtr<Location> loc;
  if (asLocation(args[0], loc))
    return PairValue::create(Point(loc->getX(), loc->getY() + loc->getBoundingBox().depth));
  else return 0;
}

SmartPtr<Value>
SMS::fun_west(const HandlerArgs& args) const
{
  if (args.size() != 1)
    return 0;
  SmartPtr<Location> loc;
  if (asLocation(args[0], loc))
    return PairValue::create(Point(loc->getX(), loc->getY() - loc->getBoundingBox().height + ((loc->getBoundingBox().height + loc->getBoundingBox().depth) / 2)));
  else return 0;
}

SmartPtr<Value>
SMS::fun_depart(const HandlerArgs& args) const
{
  if (args.size() != 2)
    return 0;
  SmartPtr<Location> loc1, loc2;
  if (asLocation(args[0], loc1) && asLocation(args[1], loc2))
  // delete "return 0" and insert here the code
    return 0;
}

SmartPtr<Value>
SMS::fun_div(const HandlerArgs& args) const
{
  if (args.size() != 2)
   return 0;
  
  Point pair;
  scaled s;
  float n1;
  float n2;
  if (asNumber(args[0], n1) && asNumber(args[1], n2))
    return NumberValue::create(n1 / n2);
  else if (asScalar(args[0], s) && asNumber(args[1], n2))
    return ScalarValue::create(s / n2);
  else if (asPair(args[0], pair) && asNumber(args[1], n2))
    return PairValue::create(Point(pair.x / n2, pair.y / n2));
  else
    return 0;
 
}

SmartPtr<Location>
SMS::getLocationOfId(const String& name) const
{
  const IdFragmentMap::const_iterator p = idFragmentMap.find(name);
  if (p == idFragmentMap.end())
    return 0;
  
  SmartPtr<Fragment> frag = p->second;
  assert(frag != 0);
  if (SmartPtr<Location> loc = evalContext.getLocation(name))
    {
      const scaled lx = frag->getX() - frag->getBoundingBox().width / 2 + loc->getX();
      const scaled ly = frag->getY() + (frag->getBoundingBox().height - frag->getBoundingBox().depth) / 2 - loc->getY();
      return Location::create(name, lx, ly, loc->getBoundingBox());
    }
  
  return 0;
}

SmartPtr<Value>
SMS::evalExpr(Scanner& scanner)
{
  int a = scanner.getToken();
  switch (a)
    {
    case Scanner::NUMBER:
      {
	const float value = scanner.getNumber();
	scanner.advance();
	return NumberValue::create(value);
      }
      break;
    case Scanner::DOLLAR:
      {
	scanner.advance();
	if (scanner.getToken() != Scanner::ID)
	  return 0;
	const String name = StringOfUCS4String(scanner.getString());
	scanner.advance();
	if (SmartPtr<Location> loc = getLocationOfId(name))
	  return RefValue::create(loc);
	else
	  return 0;
      }
      break;
    case Scanner::ID:
      {
	const String id = StringOfUCS4String(scanner.getString());
	scanner.advance();
	if (scanner.getToken() != Scanner::LPAREN)
          return 0;
	scanner.advance();

	std::vector<SmartPtr<Value> > args;
	for (;;)
	  {
	    if (!scanner.more())
	      return 0;
	    if (SmartPtr<Value> v = evalExpr(scanner))
	      args.push_back(v);
	    else
	      return 0;
            if (scanner.getToken() == Scanner::RPAREN) {
	      scanner.advance();
	      break;
	    }
            if (scanner.getToken() != Scanner::COMMA)
	      return 0;
	    else scanner.advance();
          }

	if (Handler handler = getFunHandler(id))
	  return (this->*handler)(args);
	else
	  return 0;
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
    case Scanner::LBRACE:
      {
	scanner.advance();
	const SmartPtr<Value> value = evalExpr(scanner);
	if (scanner.getToken() == Scanner::RBRACE)
	  {
	    scanner.advance();
	    return value;
	  }
	else
	  return 0;
      }
      break;
    case Scanner::ERROR:
    case Scanner::EOS:
      return 0;
    default:
      std::cout << StringOfUCS4String(scanner.getString()) << " " << a << std::endl;
      assert(false);
    }
  
  return 0;
}

bool
SMS::evalAttribute(const Model::Element& el, const String& value, const String& name)
{
  Scanner scanner(UCS4StringOfString(value), true);
  String acc;

  while (scanner.more())
    {
      if (scanner.getToken() == Scanner::RAW)
	{
	  acc.append(StringOfUCS4String(scanner.getString()));
	  scanner.advance();
	}
      else if (SmartPtr<Value> v = evalExpr(scanner))
	acc.append(serializeValue(v));
      else
	return false;
    }

  Model::setAttribute(el, name, acc);
  return true;
}

#if 0
bool
SMS::evalScalarAttribute(const Model::Element& el, const String& value, const String& name)
{
  Scanner scanner(UCS4StringOfString(value));
  scaled v;
  if (asScalar(evalExpr(scanner), v)) {
    std::ostringstream os;
    os << toUserUnits(v); 
    Model::setAttribute(el, name, os.str());
    return true;
  }
  return false;
}
#endif

bool
SMS::evalPairAttribute(const Model::Element& el, const String& value, const String& name1, const String& name2)
{
  Scanner scanner(UCS4StringOfString(value));
  Point p;
  if (asPair(evalExpr(scanner), p)) {
    std::ostringstream os1;
    std::ostringstream os2;
    os1 << toUserUnits(p.x);
    os2 << toUserUnits(p.y);
    Model::setAttribute(el, name1, os1.str());
    Model::setAttribute(el, name2, os2.str());
    return true;
  }
  return false;
}

void
SMS::evalAttributes(const Model::Node& node)
{
  assert(node);
  if (Model::getNodeType(node) == Model::ELEMENT_NODE)
    {
      const Model::Element elem = Model::asElement(node);
      const String elemName = Model::getNodeName(node);
      for (xmlAttr *attr =  node->properties; attr; attr = attr->next) {
	if (attr->ns && Model::fromModelString(attr->ns->href) == GMV_NS_URI)
	  {
	    const String name = Model::getNodeName((xmlNode*) attr);
	    const String value = Model::getNodeValue((xmlNode*) attr);
	    if ((elemName == "rect" || elemName == "text" || elemName == "foreignObject") && name == "at")
	      evalPairAttribute(elem, value, "x", "y");
	    else if (elemName == "rect" && name == "size")
	      evalPairAttribute(elem, value, "width", "height");
	    else if ((elemName == "rect" || elemName == "ellipse") && name == "radius")
	      evalPairAttribute(elem, value, "rx", "ry");
	    else if ((elemName == "circle" || elemName == "ellipse") && name == "at")
	      evalPairAttribute(elem, value, "cx", "cy");
	    else if (elemName == "line" && name == "from")
	      evalPairAttribute(elem, value, "x1", "y1");
	    else if (elemName == "line" && name == "to")
	      evalPairAttribute(elem, value, "x2", "y2");
	    else
	      evalAttribute(elem, value, name);
	  }
      }
      for (Model::NodeIterator p(node); p.more(); p.next())
	evalAttributes(p.node());
    }
}

bool
SMS::process(const String& uri, const String& outName)
{
  const Model::Document doc = Model::document(*logger, uri, false);
  if (!doc) return false;

  const Model::Element root = Model::getDocumentElement(doc);
  assert(root);

  logger->out(LOG_INFO, "Page size: %f x %f", pageWidth.toFloat(), pageHeight.toFloat());

  viewBoxX = 0;
  viewBoxY = 0;
  viewBoxWidth = 0;
  viewBoxHeight = 0;
  if (Model::hasAttribute(root, "viewBox")) {
    const UCS4String viewbox = UCS4StringOfString(Model::getAttribute(root, "viewBox"));
    UCS4String::const_iterator p = viewbox.begin();
    UCS4String::const_iterator next = viewbox.begin();

    if (SmartPtr<Value> v = Parse<ScanInteger, int>::parse(p, viewbox.end(), next)) {
      viewBoxX = ToInteger(v);
      p = next;
    }
    
    if (SmartPtr<Value> v = Parse<ScanInteger, int>::parse(p, viewbox.end(), next)) {
      viewBoxY = ToInteger(v);
      p = next;
    }
    
    if (SmartPtr<Value> v = Parse<ScanInteger, int>::parse(p, viewbox.end(), next)) {
      viewBoxWidth = ToInteger(v);
      p = next;
    }
    
    if (SmartPtr<Value> v = Parse<ScanInteger, int>::parse(p, viewbox.end(), next)) {
      viewBoxHeight = ToInteger(v);
      p = next;
    }
  }

  if (viewBoxWidth > EPSILON && viewBoxHeight > EPSILON) {
    const float ratio = ((float) viewBoxWidth) / ((float) viewBoxHeight);
    const scaled adjustedPageWidth = pageHeight * ratio;
    const scaled adjustedPageHeight = pageWidth / ratio;

    if (adjustedPageWidth > pageWidth) {
      assert(adjustedPageHeight <= pageHeight);
      pageHeight = adjustedPageHeight;
    } else {
      assert(adjustedPageWidth <= pageWidth);
      pageWidth = adjustedPageWidth;
    }
  }

  canvasWidth = pageWidth;
  canvasHeight = pageHeight;

  logger->out(LOG_INFO, "Adjusted page size: %f x %f", canvasWidth.toFloat(), canvasHeight.toFloat());

  if (Model::hasAttribute(root, "width")) {
    const UCS4String s = UCS4StringOfString(Model::getAttribute(root, "width"));
    UCS4String::const_iterator next;
    if (SmartPtr<Value> v = ParseLength::parse(s.begin(), s.end(), next))
      canvasWidth = evaluate(ToLength(v), canvasWidth);
  }

  if (Model::hasAttribute(root, "height")) {
    const UCS4String s = UCS4StringOfString(Model::getAttribute(root, "height"));
    UCS4String::const_iterator next;
    if (SmartPtr<Value> v = ParseLength::parse(s.begin(), s.end(), next))
      canvasHeight = evaluate(ToLength(v), canvasHeight);
  }

  logger->out(LOG_INFO, "Canvas size: %f x %f", canvasWidth.toFloat(), canvasHeight.toFloat());

  if (viewBoxWidth < EPSILON)
    viewBoxWidth = dpi * canvasWidth.toFloat() / 72.27f;

  if (viewBoxHeight < EPSILON)
    viewBoxHeight = dpi * canvasHeight.toFloat() / 72.27f;

  logger->out(LOG_INFO, "Viewbox size: %f x %f", viewBoxWidth, viewBoxHeight);
  logger->out(LOG_DEBUG, "Ratio: %f x %f",
	      viewBoxWidth / canvasWidth.toFloat(),
	      viewBoxHeight / canvasHeight.toFloat());

#if 0
  // conversion from physical to user coordinates
  // 1in : 90 = width : x  ==>  x = (90 * width) / 1in

  // (1) if there is a viewbox attribute on the root svg element, adjust
  //     the page width and height so that they are in the same ratio as
  //     the viewbox width and height
  // (2) if there is no width (height) attribute in the root svg element,
  //     set it with the page width (height)
  // (3) if there is no viewbox in the source document, set the viewbox
  //     in the target document computed with the formula above
  // (4) use the viewbox attribute to compute the ratio between scaled
  //     points and user coordinates
#endif

  traverse(Model::asNode(root));
  findFragmentDependencies();
#if 0
  std::cerr << "stampa dipendenze" << std::endl;
  for (std::list<SmartPtr<Fragment> >::iterator it = fragmentList.begin();
       it != fragmentList.end();
       it++)
    {
      std::cerr << *it << std::endl;
      (*it)->printDep();
    }
#endif
  
  std::list<SmartPtr<Fragment> > sortedFragments;
  if (sortFragments(sortedFragments))
    {
      logger->out(LOG_ERROR, "circular dependencies in MathML fragments");
      exit(1);
    }
  
  for (std::list<SmartPtr<Fragment> >::const_iterator f = sortedFragments.begin();
       f != sortedFragments.end();
       f++)
    {
      Model::Element object = (*f)->getOldRoot();
      evalAttributes(Model::asNode(object));
      const String x = Model::getAttributeNoNS(object, "x");
      const String y = Model::getAttributeNoNS(object, "y");
      const scaled sx = fromUserUnits(atof(x.c_str()));
      const scaled sy = fromUserUnits(atof(y.c_str()));
      (*f)->setPosition(sx, sy);

      //std::cerr << "FRAGMENT AT " << x << ", " << y << " SIZE = " << (*f)->getBoundingBox() << std::endl;
    }
  evalAttributes(Model::asNode(Model::getDocumentElement(doc)));
  substFragments();
  xmlSaveFile(outName.c_str(), doc);

  return true;
}

