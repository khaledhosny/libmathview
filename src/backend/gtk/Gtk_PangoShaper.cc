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

#include <cassert>

#include "Gtk_AreaFactory.hh"
#include "Gtk_PangoShaper.hh"
#include "MathGraphicDevice.hh"
#include "MathMLElement.hh"
#include "MathVariantMap.hh"
#include "ShapingContext.hh"
#include "ShaperManager.hh"

struct HStretchyChar
{
  gunichar ch;
  gunichar left;
  gunichar glue;
  gunichar right;
};

struct VStretchyChar
{
  gunichar ch;
  gunichar top;
  gunichar glue;
  gunichar middle;
  gunichar bottom;
};

static HStretchyChar hMap[] =
  {
    { 0x2190, 0x2190, 0x23af, 0x0000 },
    { 0x2192, 0x0000, 0x23af, 0x2192 },
    { 0x2194, 0x2190, 0x23af, 0x2192 },
    { 0,      0,      0,      0      }
  };

static VStretchyChar vMap[] = 
  {
    { 0x0028, 0x239b, 0x239c, 0x0000, 0x239d }, // LEFT PAREN
    { 0x0029, 0x239e, 0x239f, 0x0000, 0x23a0 }, // RIGHT PAREN
    { 0x005b, 0x23a1, 0x23a2, 0x0000, 0x23a3 }, // LEFT SQUARE BRACKET
    { 0x005d, 0x23a4, 0x23a5, 0x0000, 0x23a6 }, // RIGHT SQUARE BRACKET
    { 0x007b, 0x23a7, 0x23aa, 0x23a8, 0x23a9 }, // LEFT CURLY BRACKET
    { 0x007d, 0x23ab, 0x23aa, 0x23ac, 0x23ad }, // RIGHT CURLY BRACKET
    { 0x2191, 0x2191, 0x23d0, 0x0000, 0x0000 }, // UP ARROW
    { 0x2193, 0x0000, 0x23d0, 0x0000, 0x2193 }, // BOTTOM ARROW
    { 0x222b, 0x2320, 0x23ae, 0x0000, 0x2321 }, // INTEGRAL
    { 0,      0,      0,      0,      0 }
  };

#define NORMAL_INDEX 0
#define H_STRETCHY_INDEX 1
#define V_STRETCHY_INDEX 2
#define MAPPED_BASE_INDEX 3

Gtk_PangoShaper::Gtk_PangoShaper(const SmartPtr<AbstractLogger>& l, const SmartPtr<Configuration>& conf)
  : Gtk_DefaultPangoShaper(l, conf)
{ }

Gtk_PangoShaper::~Gtk_PangoShaper()
{ }

SmartPtr<Gtk_PangoShaper>
Gtk_PangoShaper::create(const SmartPtr<AbstractLogger>& l, const SmartPtr<Configuration>& conf)
{ return new Gtk_PangoShaper(l, conf); }

void
Gtk_PangoShaper::registerShaper(const SmartPtr<class ShaperManager>& sm, unsigned shaperId)
{
  for (unsigned i = NORMAL_VARIANT; i <= MONOSPACE_VARIANT; i++)
    {
      for (Char16 ch = 0x21; ch < 0x100; ch++)
	{
	  Char32 vch = mapMathVariant(MathVariant(i), ch);
	  if (i == NORMAL_VARIANT || vch != ch)
	    sm->registerChar(vch, GlyphSpec(shaperId, MAPPED_BASE_INDEX + i - NORMAL_VARIANT, ch));
	}
    }

  for (unsigned i = 0; hMap[i].ch != 0; i++)
    sm->registerStretchyChar(hMap[i].ch, GlyphSpec(shaperId, H_STRETCHY_INDEX, i));

  for (unsigned i = 0; vMap[i].ch != 0; i++)
    sm->registerStretchyChar(vMap[i].ch, GlyphSpec(shaperId, V_STRETCHY_INDEX, i));
}

void
Gtk_PangoShaper::unregisterShaper(const SmartPtr<class ShaperManager>&, unsigned)
{

}

void
Gtk_PangoShaper::shape(ShapingContext& context) const
{
  const GlyphSpec& spec = context.getSpec();
  switch (spec.getFontId())
    {
    case H_STRETCHY_INDEX:
      context.pushArea(1, shapeStretchyCharH(context));
      break;
    case V_STRETCHY_INDEX:
      context.pushArea(1, shapeStretchyCharV(context));
      break;
    default:
      {
	const unsigned n = context.chunkSize();
	assert(n > 0);
	if (n > 1)
	  context.pushArea(n, shapeChunk(context, n));
	else
	  context.pushArea(1, shapeChar(context));
      }
      break;
    }
}

AreaRef
Gtk_PangoShaper::shapeChar(const ShapingContext& context) const
{
  gchar buffer[6];
  gint length = g_unichar_to_utf8(context.getSpec().getGlyphId(), buffer);

  PangoLayout* layout = createPangoLayout(buffer, length, context.getSize(),
					  getTextAttributes(MathVariant(context.getSpec().getFontId() - MAPPED_BASE_INDEX + NORMAL_VARIANT)));
  SmartPtr<Gtk_AreaFactory> factory = smart_cast<Gtk_AreaFactory>(context.getFactory());
  assert(factory);
  return factory->pangoLayoutLine(layout);
}

AreaRef
Gtk_PangoShaper::shapeChunk(const ShapingContext& context, unsigned n) const
{
  gunichar* uni_buffer = new gunichar[n];
  for (unsigned i = 0; i < n; i++)
    uni_buffer[i] = context.getSpec(i).getGlyphId();

  glong length;
  gchar* buffer = g_ucs4_to_utf8(uni_buffer, n, NULL, &length, NULL);
  delete [] uni_buffer;
  PangoLayout* layout = createPangoLayout(buffer, length,
					  context.getSize(),
					  getTextAttributes(MathVariant(context.getSpec().getFontId() - MAPPED_BASE_INDEX + NORMAL_VARIANT)));
  g_free(buffer);

  SmartPtr<Gtk_AreaFactory> factory = smart_cast<Gtk_AreaFactory>(context.getFactory());
  assert(factory);
  return factory->pangoLayoutLine(layout);
}

AreaRef
Gtk_PangoShaper::shapeStretchyCharH(const ShapingContext& context) const
{
  const HStretchyChar* charSpec = &hMap[context.getSpec().getGlyphId()];

  const AreaRef normal = (charSpec->ch != 0) ? shapeString(context, &charSpec->ch, 1) : 0;
  const AreaRef left = (charSpec->left != 0) ? shapeString(context, &charSpec->left, 1) : 0;
  const AreaRef glue = (charSpec->glue != 0) ? shapeString(context, &charSpec->glue, 1) : 0;
  const AreaRef right = (charSpec->right != 0) ? shapeString(context, &charSpec->right, 1) : 0;

  return composeStretchyCharH(context.getFactory(), normal, left, glue, right, context.getHSpan());
}

AreaRef
Gtk_PangoShaper::shapeStretchyCharV(const ShapingContext& context) const
{
  // we use 2 * so that text-mode parentheses are not required to stretch
  // when a glyph goes below the baseline (like p or y)
  const scaled span = context.getVSpan() - (2 * context.getSize()) / 10;

  const VStretchyChar* charSpec = &vMap[context.getSpec().getGlyphId()];

  AreaRef normal = (charSpec->ch != 0) ? shapeString(context, &charSpec->ch, 1) : 0;
  AreaRef top = (charSpec->top != 0) ? shapeString(context, &charSpec->top, 1) : 0;
  AreaRef glue = (charSpec->glue != 0) ? shapeString(context, &charSpec->glue, 1) : 0;
  AreaRef middle = (charSpec->middle != 0) ? shapeString(context, &charSpec->middle, 1) : 0;
  AreaRef bottom = (charSpec->bottom != 0) ? shapeString(context, &charSpec->bottom, 1) : 0;

  return composeStretchyCharV(context.getFactory(), normal, top, glue, middle, bottom, span);
}
