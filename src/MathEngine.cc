// Copyright (C) 2000, Luca Padovani <luca.padovani@cs.unibo.it>.
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
// http://cs.unibo.it/~lpadovan/mml-widget/, or send a mail to
// <luca.padovani@cs.unibo.it>

#include <config.h>
#include <assert.h>

#include "Clock.hh"
#include "Parser.hh"
#include "MathEngine.hh"
#include "CharMapper.hh"
#include "MathMLDocument.hh"
#include "T1_Gtk_DrawingArea.hh"
#include "RenderingEnvironment.hh"

EntitiesTable      MathEngine::entitiesTable;
OperatorDictionary MathEngine::dictionary;
Logger             MathEngine::logger;
bool               MathEngine::kerning = false;

void
MathEngine::InitGlobalData()
{
  static volatile bool firstTime = true;
  assert(firstTime);

  bool res;

  //res = entitiesTable.Load("/usr/local/share/gtkmathview/entities-table.xml", false);
  //if (!res) res = entitiesTable.Load("config/entities-table.xml", true);
  entitiesTable.LoadInternalTable();

  res = dictionary.Load("/usr/local/share/gtkmathview/dictionary.xml");
  dictionary.Load("config/dictionary.xml");

  firstTime = false;
}

MathEngine::MathEngine()
{
  area = NULL;
  fontManager = NULL;
  charMapper = NULL;

  defaultFontSize = DEFAULT_FONT_SIZE;
  antiAliasing = false;
  kerning = false;

  document = NULL;
  root = NULL;
  selectionFirst = selectionRoot = selection = NULL;
}

MathEngine::~MathEngine()
{
  delete charMapper;
}

void
MathEngine::Init(class DrawingArea* a, class FontManager* fm)
{
  assert(a != NULL);
  assert(fm != NULL);

  area = a;
  fontManager = fm;

  if (charMapper != NULL) delete charMapper;
  charMapper = new CharMapper(*fm);
  charMapper->Load("/usr/local/share/gtkmathview/font-configuration.xml");
  charMapper->Load("config/font-configuration.xml");
}

bool
MathEngine::Load(const char* fileName)
{
  assert(fileName != NULL);

  Clock perf;
  perf.Start();
  MathMLParser *parser = new MathMLParser(fileName);
  document = parser->Parse();
  perf.Stop();
  logger(LOG_INFO, "parsing time: %dms", perf());
  delete parser;

  if (document != NULL) {
    perf.Start();
    document->Normalize();
    perf.Stop();
    logger(LOG_INFO, "normalization time: %dms", perf());
    
    root = document->GetRoot();
    assert(root != NULL);

    Setup();

    return true;
  } else
    logger(LOG_WARNING, "error while parsing `%s'", fileName);

  return false;
}

void
MathEngine::Unload()
{
  delete document;

  document = NULL;  
  root = NULL;

  selectionFirst = selectionRoot = selection = NULL;
}

void
MathEngine::Setup()
{
  if (root == NULL) return;

  Clock perf;

  UnitValue size;
  size.Set(defaultFontSize, UNIT_PT);

  assert(charMapper != NULL);
  RenderingEnvironment env(*charMapper);
  env.SetFontSize(size);
  perf.Start();
  root->Setup(&env);
  perf.Stop();
  logger(LOG_INFO, "setup time: %dms", perf());

  perf.Start();
  root->DoBoxedLayout(LAYOUT_MIN);
  perf.Stop();
  logger(LOG_INFO, "minimum layout time: %dms", perf());

  perf.Start();
  root->DoBoxedLayout(LAYOUT_MAX);
  perf.Stop();
  logger(LOG_INFO, "maximum layout time: %dms", perf());
}

void
MathEngine::Layout()
{
  assert(area != NULL);

  if (root == NULL) return;

  Clock perf;
  perf.Start();
  root->DoBoxedLayout(LAYOUT_AUTO, scaledMax(0, area->GetWidth() -  2 * area->GetXMargin()));
  root->SetPosition(area->GetXMargin(), area->GetYMargin() + root->GetBoundingBox().ascent);
  root->Freeze();
  perf.Stop();
  logger(LOG_INFO, "layout time: %dms", perf());
}

void
MathEngine::SetDirty(const Rectangle* rect)
{
  if (root == NULL) return;
  root->SetDirty(rect);
}

void
MathEngine::Render(const Rectangle* rect)
{
  if (root == NULL) return;
  root->SetDirty(rect);
  Update(rect);
}

void
MathEngine::Update(const Rectangle* rect)
{
  assert(area != NULL);

  if (root == NULL) return;

  Clock perf;
  perf.Start();
  root->Render(*area);
  if (rect != NULL) area->Update(*rect);
  else area->Update();
  perf.Stop();
  logger(LOG_INFO, "rendering time: %dms", perf());
}

void
MathEngine::GetDocumentBoundingBox(BoundingBox& box) const
{
  if (root == NULL) {
    box.Null();
    return;
  }

  box = root->GetBoundingBox();  
}

void
MathEngine::SetSelectionFirst(scaled x, scaled y)
{
  selectionFirst = GetElementAt(x, y);
}

MathMLElement*
MathEngine::SelectMinimumTree(MathMLElement* first, MathMLElement* last)
{
  assert(first != NULL);
  assert(last != NULL);

  if (first != last) {
    unsigned firstDepth = first->GetDepth();
    unsigned lastDepth  = last->GetDepth();

    while (firstDepth > lastDepth) {
      first = first->GetParent();
      firstDepth--;
    }

    while (lastDepth > firstDepth) {
      last = last->GetParent();
      lastDepth--;
    }

    assert(firstDepth == lastDepth);

    while (first != NULL && last != NULL && first != last) {
      first = first->GetParent();
      last = last->GetParent();
    }
  }

  assert(first == last);

  return first;
}

void
MathEngine::SetSelectionLast(scaled x, scaled y)
{
  if (selectionFirst == NULL);

  MathMLElement* selectionLast = GetElementAt(x, y);
  if (selectionLast == NULL) return;

  selectionRoot = SelectMinimumTree(selectionFirst, selectionLast);
  while (selectionRoot != NULL && selectionRoot->GetDOMNode() == NULL)
    selectionRoot = selectionRoot->GetParent();
}

void
MathEngine::ResetSelectionRoot()
{
  selectionFirst = selectionRoot = NULL;
}

void
MathEngine::SetSelection(MathMLElement* selected)
{
  if (selection == selected) return;

  if (selection != NULL) selection->ResetSelected();
  selection = selected;
  if (selection != NULL) selection->SetSelected();

  Update();
}

MathMLElement*
MathEngine::GetElementAt(scaled x, scaled y) const
{
  if (root == NULL) return NULL;
  // WARNING: x and y must be absolute coordinates w.r.t. the drawing area, because
  // at this level we do not known whether the drawing area is scrollable (as in
  // the case of Gtk_DrawingArea) or not (PS_DrawingArea). The caller must
  // properly adjust x and y before calling this method
  return root->Inside(x, y);
}

void
MathEngine::SetDefaultFontSize(unsigned size)
{
  assert(size > 0);
  defaultFontSize = size;
}

void
MathEngine::SetVerbosity(int level)
{
  logger.SetLogLevel(level);
}

int
MathEngine::GetVerbosity() const
{
  return logger.GetLogLevel();
}

void
MathEngine::SetAntiAliasing(bool aa)
{
  assert(area != NULL);

  antiAliasing = aa;

#ifdef HAVE_LIBT1
  T1_Gtk_DrawingArea* t1_area = TO_T1_GTK_DRAWING_AREA(area);
  if (t1_area != NULL) {
    t1_area->SetAntiAliasing(aa);
    return;
  }
#endif
  logger(LOG_WARNING, "anti-aliasing is available with the T1 font manager only");
}

void
MathEngine::SetKerning(bool k)
{
  kerning = k;
#ifndef HAVE_LIBT1
  logger(LOG_WARNING, "kerning is available with the T1 font manager only");
#endif
}
