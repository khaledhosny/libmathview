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

#include <config.h>

#ifdef HAVE_LIBT1

#include <assert.h>
#include <stddef.h>
#include <ctype.h>
#include <time.h>

#include "UnitValue.hh"
#include "PS_T1_Font.hh"
#include "PS_DrawingArea.hh"

PS_DrawingArea::PS_DrawingArea(const GraphicsContextValues& values,
			       scaled xMargin, scaled yMargin, FILE* f) :
  DrawingArea(values, xMargin, yMargin)
{
  lastGC = NULL;
  lastFont = NULL;

  output = f;
  colors = true;
}

PS_DrawingArea::~PS_DrawingArea()
{
}

void
PS_DrawingArea::DisableColors()
{
  colors = false;
}

void
PS_DrawingArea::DumpHeader(const char* appName, const char* title, const Rectangle& rect) const
{
  assert(appName != NULL);
  assert(title != NULL);

  time_t curTime = time(NULL);

  // FIXME: is the output conformant???
  if (output != NULL) {
    fprintf(output, "%%!PS-Adobe-3.0 EPSF-3.0\n");
    fprintf(output, "%%%%BoundingBox: %d %d %d %d\n",
	    truncFloat(sp2ps(rect.x)), truncFloat(sp2ps(height - (rect.y + rect.height))),
	    roundFloat(sp2ps(rect.x + rect.width)), roundFloat(sp2ps(height - rect.y)));
    fprintf(output, "%%%%Creator: %s\n", appName);
    fprintf(output, "%%%%CreationDate: %s", asctime(localtime(&curTime)));
    fprintf(output, "%%%%Title: %s\n", title);
    fprintf(output, "%%%%Pages: 1\n");
    fprintf(output, "%%%%EndComments\n\n");
  }
}

void
PS_DrawingArea::DumpPreamble() const
{
  if (output != NULL)
    fprintf(output, "%%%%Page: 1 1\n\n");
}

void
PS_DrawingArea::DumpEpilogue() const
{
  if (output != NULL) {
    fprintf(output, "showpage\n");
    fprintf(output, "%%%%Trailer\n");
    fprintf(output, "%%%%EOF\n");
  }
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

  if (output != NULL)
    fprintf(output, "newpath %.2f %.2f moveto %.2f %.2f lineto stroke\n",
	    sp2ps(x1), sp2ps(height - y1), sp2ps(x2), sp2ps(height - y2));
  
  MoveTo(x2, y2);
}

void
PS_DrawingArea::DrawRectangle(const GraphicsContext* gc,
			      scaled x, scaled y, scaled w, scaled h) const
{
  SetGraphicsContext(gc);

  if (output != NULL) {
    fprintf(output, "newpath %.2f %.2f moveto\n", sp2ps(x), sp2ps(height - y));
    fprintf(output, "  %.2f %.2f rlineto %.2f %.2f rlineto %.2f %.2f rlineto closepath stroke\n",
	    sp2ps(w), 0.0, 0.0, sp2ps(-h), sp2ps(-w), 0.0);
  }
}

void
PS_DrawingArea::DrawChar(const GraphicsContext* gc, const AFont* font,
			 scaled x, scaled y, char ch) const
{
  const PS_T1_Font* ps_font = TO_PS_T1_FONT(font);
  assert(ps_font != NULL);
  ps_font->UseChar(ch);

  SetGraphicsContext(gc);
  SetFont(font);

  if (output != NULL)
    fprintf(output, "%.2f %.2f moveto (", sp2ps(x), sp2ps(height - y));

  DrawCharAux((unsigned char) ch);
  
  if (output != NULL)
    fprintf(output, ") show\n");
}

void
PS_DrawingArea::DrawCharAux(unsigned char ch) const
{
  if (output != NULL) {
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
}

void
PS_DrawingArea::DrawString(const GraphicsContext* gc, const AFont* font,
			   scaled x, scaled y,
			   const char* text,
			   unsigned len) const
{
  assert(text != NULL);

  const PS_T1_Font* ps_font = TO_PS_T1_FONT(font);
  assert(ps_font != NULL);
  ps_font->UseChars(text, len);

  SetGraphicsContext(gc);
  SetFont(font);

  if (output != NULL)
    fprintf(output, "%.2f %.2f moveto (", sp2ps(x), sp2ps(height - y));

  for (unsigned i = 0; i < len; i++) DrawCharAux((unsigned char) text[i]);

  if (output != NULL)
    fprintf(output, ") show\n");
}

void
PS_DrawingArea::Clear(const GraphicsContext* gc,
		      scaled x, scaled y, scaled w, scaled h) const
{
  SetGraphicsContext(gc);

  if (output != NULL) {
    fprintf(output, "newpath %.2f %.2f moveto\n", sp2ps(x), sp2ps(height - y));
    fprintf(output, "  %.2f %.2f rlineto %.2f %.2f rlineto %.2f %.2f rlineto closepath fill\n",
	    sp2ps(w), 0.0, 0.0, sp2ps(-h), sp2ps(-w), 0.0);
  }
}

void
PS_DrawingArea::Update(scaled x, scaled y, scaled w, scaled h) const
{
  // nothing to update when rendering PostScript
}

void
PS_DrawingArea::DumpGrid() const
{
  if (output != NULL) {
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
}

void
PS_DrawingArea::SetGraphicsContext(const GraphicsContext* gc) const
{
  assert(gc != NULL);

  if (gc != lastGC) {
    if (output != NULL) {
      if (colors && (lastGC == NULL || gc->GetForeground() != lastGC->GetForeground())) {
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
    if (output != NULL)
      fprintf(output, "F%d setfont\n", t1_font->GetFontId());

    lastFont = t1_font;
  }
}

void
PS_DrawingArea::SetOutputFile(FILE* f)
{
  // the next line needs some explanations: this method is usually
  // used to allow a null traversal of the document tree without
  // producing any output (setting output to NULL), so to know which
  // chars for each font are actually used. The, output is set again to
  // the wanted output file and another traversal is done. But due
  // to the caching mechanism for fonts, if the document is such that
  // the first and the last symbols draw are taken from the same font,
  // the on the second traversal (the important one for having the
  // output generated) the font is not set at the beginning of the
  // postscript file, and this usually means that a symbol is not
  // draw. Maybe this line should go somewhere else, but I don't want
  // to make a method for just this silly thing, so I'll keep it here
  // until something better comes to my mind.
  if (output != f) lastFont = NULL;

  output = f;
}

#endif // HAVE_LIBT1
