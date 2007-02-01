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

#ifndef __SMS_hh__
#define __SMS_hh__

#include <list>

#include "Fragment.hh"
#include "MathView.hh"
#include "Model.hh"
#include "Length.hh"
#include "scaled.hh"
#include "HashMap.hh"
#include "SVG_EvalRenderingContext.hh"

class SMS
{
public:
  SMS(const SmartPtr<class AbstractLogger>&, const SmartPtr<MathView>&);

  bool process(const String&, const String&);
  Model::Document getDocument(void) const { return doc; }

protected:
  typedef std::vector<SmartPtr<Value> > HandlerArgs;
  typedef SmartPtr<Value> (SMS::* Handler)(const HandlerArgs&) const;

  static scaled evaluate(const Length&, const scaled& = 0);
  float toUserUnits(const scaled&) const;
  scaled fromUserUnits(float) const;
  String serializeValue(const SmartPtr<Value>&) const;
  bool asLocation(const SmartPtr<Value>&, SmartPtr<Location>&) const;
  bool asNumber(const SmartPtr<Value>&, float&) const;
  bool asScalar(const SmartPtr<Value>&, scaled&) const;
  bool asPair(const SmartPtr<Value>&, Point&) const;
  void traverse(const Model::Node&);
  void substFragments(void);
  void evalAttributes(const Model::Node&);
  SmartPtr<Value> evalExpr(class Scanner&);
  bool evalAttribute(const Model::Element&, const String&, const String&);
  // bool evalScalarAttribute(const Model::Element& , const String&, const String&);
  bool evalPairAttribute(const Model::Element&, const String&, const String&, const String&);
  void dependAssocList(const Model::Node&, SmartPtr<Fragment>);
  bool isGmvNamespace(const xmlChar* ns);
  void createFragDepList();

  void findFragmentDependencies();
  void assoc(const Model::Node&, const SmartPtr<Fragment>&);
  bool sortFragments(std::list<SmartPtr<Fragment> >&) const;

  SmartPtr<Location> getLocationOfId(const String&) const;

  Handler getFunHandler(const String&) const;

  SmartPtr<Value> fun_pair(const HandlerArgs&) const;
  SmartPtr<Value> fun_x(const HandlerArgs&) const;
  SmartPtr<Value> fun_y(const HandlerArgs&) const;
  SmartPtr<Value> fun_origin(const HandlerArgs&) const;
  SmartPtr<Value> fun_center(const HandlerArgs&) const;
  SmartPtr<Value> fun_width(const HandlerArgs&) const;
  SmartPtr<Value> fun_height(const HandlerArgs&) const;
  SmartPtr<Value> fun_size(const HandlerArgs&) const;
  SmartPtr<Value> fun_depth(const HandlerArgs&) const;
  SmartPtr<Value> fun_add(const HandlerArgs&) const;
  SmartPtr<Value> fun_sub(const HandlerArgs&) const;
  SmartPtr<Value> fun_mul(const HandlerArgs&) const;
  SmartPtr<Value> fun_div(const HandlerArgs&) const;
  SmartPtr<Value> fun_neg(const HandlerArgs&) const;
  SmartPtr<Value> fun_north_west(const HandlerArgs&) const;
  SmartPtr<Value> fun_north(const HandlerArgs&) const;
  SmartPtr<Value> fun_north_east(const HandlerArgs&) const;
  SmartPtr<Value> fun_east(const HandlerArgs&) const;
  SmartPtr<Value> fun_south_east(const HandlerArgs&) const;
  SmartPtr<Value> fun_south(const HandlerArgs&) const;
  SmartPtr<Value> fun_south_west(const HandlerArgs&) const;
  SmartPtr<Value> fun_west(const HandlerArgs&) const;
  SmartPtr<Value> fun_depart(const HandlerArgs&) const;

private:
  SmartPtr<class AbstractLogger> logger;
  SmartPtr<MathView> view;
  SVG_EvalRenderingContext evalContext;
  Model::Document doc;
  std::list<SmartPtr<Fragment> > fragmentList;
  std::list<xmlChar*> getDepFromAttr(const xmlChar *value);

  typedef HASH_MAP_NS::hash_map<String,SmartPtr<Fragment>,StringHash,StringEq> IdFragmentMap;
  IdFragmentMap idFragmentMap;

  typedef HASH_MAP_NS::hash_map<String,Handler,StringHash,StringEq> FunMap;
  FunMap funMap;
};

#endif // __SMS_hh__
