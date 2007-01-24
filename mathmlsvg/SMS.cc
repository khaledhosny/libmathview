// Copyright (C) 2000-2005, Luca Padovani <luca.padovani@cs.unibo.it>.
//
// This file is part of GtkMath	View, a Gtk widget for MathML.
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
#include "Point.hh"

#define SVG_NS_URI "http://www.w3.org/2000/svg"
#define GMV_NS_URI "http://helm.cs.unibo.it/2005/GtkMathView"
 
SMS::SMS(const SmartPtr<AbstractLogger>& _logger,
	 const SmartPtr<MathView>& _view)
  : logger(_logger), view(_view), evalContext(logger, view)
{
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
  funMap["nw"] = &SMS::fun_nw;
  funMap["n"] = &SMS::fun_n;
  funMap["ne"] = &SMS::fun_ne;
  funMap["e"] = &SMS::fun_e;
  funMap["se"] = &SMS::fun_se;
  funMap["s"] = &SMS::fun_s;
  funMap["sw"] = &SMS::fun_sw;
  funMap["w"] = &SMS::fun_w;
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

std::list<xmlChar*> SMS::getDepFromAttr(const xmlChar *value)
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


#include "BoundingBoxAux.hh"

float
SMS::toUserUnits(const scaled& s) const
{
  return (s.toFloat() * 72.27) / 90;
  //return (s.toFloat());
  
}

scaled
SMS::fromUserUnits(float f) const
{
  return (f * 90) / 72.27;
  //return (f);
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
SMS::fun_nw(const HandlerArgs& args) const
{
  if (args.size() != 1)
    return 0;
  SmartPtr<Location> loc;
  if (asLocation(args[0], loc))
    return PairValue::create(Point(loc->getX(), loc->getY() - loc->getBoundingBox().height));
  else return 0;
}

SmartPtr<Value>
SMS::fun_n(const HandlerArgs& args) const
{
  if (args.size() != 1)
    return 0;
  SmartPtr<Location> loc;
  if (asLocation(args[0], loc))
    return PairValue::create(Point(loc->getX() + (loc->getBoundingBox().width / 2), loc->getY() - loc->getBoundingBox().height));
  else return 0;
}

SmartPtr<Value>
SMS::fun_ne(const HandlerArgs& args) const
{
  if (args.size() != 1)
    return 0;
  SmartPtr<Location> loc;
  if (asLocation(args[0], loc))
    return PairValue::create(Point(loc->getX() + loc->getBoundingBox().width , loc->getY() - loc->getBoundingBox().height));
  else return 0;
}

SmartPtr<Value>
SMS::fun_e(const HandlerArgs& args) const
{
  if (args.size() != 1)
    return 0;
  SmartPtr<Location> loc;
  if (asLocation(args[0], loc))
    return PairValue::create(Point(loc->getX() + loc->getBoundingBox().width, loc->getY() - loc->getBoundingBox().height + ((loc->getBoundingBox().height + loc->getBoundingBox().depth) / 2)));
  else return 0;
}

SmartPtr<Value>
SMS::fun_se(const HandlerArgs& args) const
{
  if (args.size() != 1)
    return 0;
  SmartPtr<Location> loc;
  if (asLocation(args[0], loc))
    return PairValue::create(Point(loc->getX() + loc->getBoundingBox().width, loc->getY() + loc->getBoundingBox().depth));
  else return 0;
}

SmartPtr<Value>
SMS::fun_s(const HandlerArgs& args) const
{
  if (args.size() != 1)
    return 0;
  SmartPtr<Location> loc;
  if (asLocation(args[0], loc))
    return PairValue::create(Point(loc->getX() + (loc->getBoundingBox().width / 2), loc->getY() + loc->getBoundingBox().depth));
  else return 0;
}

SmartPtr<Value>
SMS::fun_sw(const HandlerArgs& args) const
{
  if (args.size() != 1)
    return 0;
  SmartPtr<Location> loc;
  if (asLocation(args[0], loc))
    return PairValue::create(Point(loc->getX(), loc->getY() + loc->getBoundingBox().depth));
  else return 0;
}

SmartPtr<Value>
SMS::fun_w(const HandlerArgs& args) const
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

Point intersezione(Point lato1, Point lato2, Point retta1, Point retta2)
{
  if (retta1.x == retta2.x)
    if (lato1.y == lato2.y)
      return Point(retta1.x, lato1.y);
    else assert(false);
  else if (lato1.x == lato2.x)
    return Point(lato1.x, retta1.y);
  else assert(false);

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
SMS::evalScalarAttribute(const Model::Element& el, const String& value, const String& name)
{
  Scanner scanner(UCS4StringOfString(value));
  scaled v;
  if (asScalar(evalExpr(scanner), v)) {
    std::ostringstream os;
    // os << v.toFloat();
    os << toUserUnits(v); 
    Model::setAttribute(el, name, os.str());
    return true;
  }
  return false;
}

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
      Model::Element elem = Model::asElement(node);
	for (xmlAttr *attr =  node->properties; attr; attr = attr->next) {
        if (attr->ns && Model::fromModelString(attr->ns->href) == GMV_NS_URI)
	{
          String name = Model::getNodeName((xmlNode*) attr);
          String value = Model::getNodeValue((xmlNode*) attr);
         if (name == "at")
            evalPairAttribute(elem, value, "x", "y");
          else if (name == "from")
            evalPairAttribute(elem, value, "x1", "y1");
          else if (name == "to")
            evalPairAttribute(elem, value, "x2", "y2");
          else if (name == "size")
            evalPairAttribute(elem, value, "width", "height");
          else if (name == "r")
            evalPairAttribute(elem, value, "rx", "ry");
          else if (name == "c")
            evalPairAttribute(elem, value, "cx", "cy");
          else
            evalScalarAttribute(elem, value, name);
	}
	}
      for (Model::NodeIterator p(node); p.more(); p.next())
	evalAttributes(p.node());
    }
}

void printDocument(Model::Node node)
{
  for (Model::Node ptr = node;
    ptr; 
      ptr = ptr->next)
  {
    std::cerr << ptr->name;
    if (ptr->content)
      std::cerr << ptr->content;
    std::cerr << " ( ";
    if (ptr->children){
      for (Model::Node tmp = ptr->children; tmp; tmp = tmp->next)
        printDocument(tmp);
      std::cerr << " ) ";}
  }
}

bool
SMS::process(const String& uri)
{
  doc = Model::document(*logger, uri, false);
  if (!doc) return false;
  traverse(Model::asNode(Model::getDocumentElement(doc)));
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
      std::cerr << "circular dependencies\n";
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
//  printDocument(Model::asNode(Model::getDocumentElement(doc)));
  xmlSaveFile("a.svg", doc);
  return true;
}

