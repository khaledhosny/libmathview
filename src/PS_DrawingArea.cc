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
// http://cs.unibo.it/~lpadovan/mml-widget, or send a mail to
// <luca.padovani@cs.unibo.it>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <assert.h>
#include <stddef.h>
#include <ctype.h>

#include "T1_Font.hh"
#include "Iterator.hh"
#include "UnitValue.hh"
#include "PS_DrawingArea.hh"

PS_DrawingArea::PS_DrawingArea(const GraphicsContextValues& values,
			       scaled xMargin, scaled yMargin, FILE* f) :
  DrawingArea(values, xMargin, yMargin)
{
  assert(f != NULL);

  lastGC = NULL;
  lastFont = NULL;

  output = f;
}

PS_DrawingArea::~PS_DrawingArea()
{
}

void
PS_DrawingArea::DumpPreamble() const
{
}

void
PS_DrawingArea::DumpEpilogue() const
{
  fprintf(output, "showpage\n");
}

const GraphicsContext*
PS_DrawingArea::GetGC(const GraphicsContextValues& values) const
{
  return new GraphicsContext(values);
}

void
PS_DrawingArea::DrawLine(const GraphicsContext* gc,
			 scaled x1, scaled y1,
			 scaled x2, scaled y2) const
{
  SetGraphicsContext(gc);

  fprintf(output, "newpath %.2f %.2f moveto %.2f %.2f lineto stroke\n",
	  sp2ps(mx + x1), sp2ps(height - my - y1), sp2ps(mx + x2), sp2ps(height - my - y2));

  MoveTo(x2, y2);
}

void
PS_DrawingArea::DrawRectangle(const GraphicsContext* gc,
			      scaled x, scaled y, scaled w, scaled h) const
{
  SetGraphicsContext(gc);

  fprintf(output, "newpath %.2f %.2f moveto\n", sp2ps(mx + x), sp2ps(height - my - y));
  fprintf(output, "  %.2f %.2f rlineto %.2f %.2f rlineto %.2f %.2f rlineto closepath stroke\n",
	  sp2ps(w), 0.0, 0.0, sp2ps(-h), sp2ps(-w), 0.0);
}

void
PS_DrawingArea::DrawChar(const GraphicsContext* gc, const AFont* font,
			 scaled x, scaled y, char ch) const
{
  SetGraphicsContext(gc);
  SetFont(font);

  fprintf(output, "%.2f %.2f moveto (", sp2ps(mx + x), sp2ps(height - my - y));
  DrawCharAux((unsigned char) ch);
  fprintf(output, ") show\n");
}

void
PS_DrawingArea::DrawCharAux(unsigned char ch) const
{
  switch (ch) {
  case '(': fprintf(output, "\\("); break;
  case ')': fprintf(output, "\\)"); break;
  case '\\': fprintf(output, "\\\\"); break;
  default:
    if (isprint(ch) && !isspace(ch)) fputc(ch, output);
    else fprintf(output, "\\%d%d%d", (ch / 64) % 8, (ch / 8) % 8, ch % 8);
    break;
  }
}

void
PS_DrawingArea::DrawString(const GraphicsContext* gc, const AFont* font,
			   scaled x, scaled y,
			   const char* text,
			   unsigned len) const
{
  assert(text != NULL);

  SetGraphicsContext(gc);
  SetFont(font);

  fprintf(output, "%.2f %.2f moveto (", sp2ps(mx + x), sp2ps(height - my - y));
  for (unsigned i = 0; i < len; i++) DrawCharAux((unsigned char) text[i]);
  fprintf(output, ") show\n");
}

void
PS_DrawingArea::Clear(const GraphicsContext* gc,
		      scaled x, scaled y, scaled w, scaled h) const
{
  SetGraphicsContext(gc);

  fprintf(output, "newpath %.2f %.2f moveto\n", sp2ps(mx + x), sp2ps(height - my - y));
  fprintf(output, "  %.2f %.2f rlineto %.2f %.2f rlineto %.2f %.2f rlineto closepath fill\n",
	  sp2ps(w), 0.0, 0.0, sp2ps(-h), sp2ps(-w), 0.0);
}

void
PS_DrawingArea::Update(scaled x, scaled y, scaled w, scaled h) const
{
  // nothing to update when rendering PostScript
}

void
PS_DrawingArea::DumpGrid() const
{
  fprintf(output, "%.2f setlinewidth\n", sp2ps(mm2sp(0.1)));
  fprintf(output, "0.5 setgray\n");
  fprintf(output, "newpath %.2f %.2f moveto\n", sp2ps(mx), sp2ps(my));
  fprintf(output, "  %.2f %.2f rlineto %.2f %.2f rlineto %.2f %.2f rlineto closepath stroke\n",
	  sp2ps(width - 2 * mx), 0.0,
	  0.0, sp2ps(height - 2 * my),
	  sp2ps(-width + 2 * mx), 0.0);

  scaled sy = 0;
  unsigned i = 0;
  while (sy < height - 2 * my) {
    fprintf(output, "newpath %.2f %.2f moveto %.2f %.2f rlineto stroke\n",
	    sp2ps(mx), sp2ps(my + sy),
	    ((i % 5) == 0) ? sp2ps(mm2sp(3)) : sp2ps(mm2sp(1)), 0.0);
    sy += mm2sp(1);
    i++;
  }
}

void
PS_DrawingArea::SetGraphicsContext(const GraphicsContext* gc) const
{
  assert(gc != NULL);

  if (gc != lastGC) {
    if (lastGC == NULL || gc->GetForeground() != lastGC->GetForeground()) {
      RGBValue color = gc->GetForeground();
      fprintf(output, "%.2f %.2f %.2f setrgbcolor\n",
	      GETRED(color) / 255.0, GETGREEN(color) / 255.0, GETBLUE(color) / 255.0);
    }

    if (lastGC == NULL || gc->GetLineStyle() != lastGC->GetLineStyle()) {
      if (gc->GetLineStyle() == LINE_STYLE_SOLID)
	fprintf(output, "[] 0");
      else
	fprintf(output, "[3] 0");
      fprintf(output, " setdash\n");
    }

    if (lastGC == NULL || gc->GetLineWidth() != lastGC->GetLineWidth()) {
      fprintf(output, "%f setlinewidth\n", sp2ps(gc->GetLineWidth()));
    }

    lastGC = gc;
  }
}

void
PS_DrawingArea::SetFont(const AFont* font) const
{
  assert(font != NULL);
  const T1_Font* t1_font = TO_T1_FONT(font);
  assert(t1_font != NULL);

  if (t1_font != lastFont) {
    fprintf(output, "F%d setfont\n", t1_font->GetFontId());

    lastFont = t1_font;
  }
}
