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

#ifndef Layout_hh
#define Layout_hh

#include <stddef.h>

#include "scaled.hh"
#include "keyword.hh"
#include "Container.hh"

#define INDENT_SPACING SCALED_POINTS_PER_CM
#define MAX_PENALTY    10000

class Layout {
public:
  explicit Layout(scaled, BreakId = BREAK_NO);
  ~Layout();

  void   In(void);
  void   Out(void);
  int    GetPenalty(void) const { return penalty; }
  void   Append(class MathMLFrame*, scaled, BreakId = BREAK_NO);
  void   Append(scaled, BreakId = BREAK_NO);
  void   DoLayout(LayoutId) const;
  void   SetLastBreakability(BreakId);

  void   SetLineSpacing(scaled);
  void   SetMinimumLineSpacing(scaled);
  scaled GetLineSpacing(void) const { return minLineSpacing; }

  void   SetPosition(scaled, scaled, ColumnAlignId = COLUMN_ALIGN_LEFT);
  void   GetBoundingBox(struct BoundingBox&, LayoutId = LAYOUT_AUTO) const;

private:
  struct Atom {
    class MathMLFrame* frame;
    scaled spacing; // spacing after the frame (or just spacing if frame == NULL)
    int    penalty; // penalty if breaking after this atom

    scaled GetWidth(LayoutId) const;
    void   GetBoundingBox(struct BoundingBox&, LayoutId) const;
    bool   IsFrame(void) const { return frame != NULL; }
    bool   IsSpace(void) const { return frame == NULL; }
    bool   IsDiscardable(void) const { return IsSpace() && penalty < MAX_PENALTY; }
    void   SetPosition(scaled, scaled);
  };

  struct Row {
    Row(void);

    Container<Atom*> content;
    scaled minLineSpacing;

    scaled GetWidth(LayoutId) const;
    void   GetBoundingBox(struct BoundingBox&, LayoutId) const;
    scaled GetMinimumSpacing(void) const;
    scaled RemoveDiscardableSpacesRight(void);
    scaled RemoveDiscardableSpacesLeft(void);
    scaled BreakUpTo(Atom*, Row*);
    void   DoLayout(LayoutId, scaled) const;
    void   SetPosition(scaled, scaled);
  };

  int  GetPenalty(BreakId) const;
  void AppendAtom(class MathMLFrame*, scaled, BreakId);
  void NewRow(void);
  void FindBreakPoint(Row*);
  void UpdateBreakPoint(Atom*);

  Container<Row*> content;

  Atom*    best;
  int      bestPenalty;

  int      penalty;
  BreakId  breakability;
  scaled   totalWidth;
  scaled   availableWidth;

  scaled   minAscent;
  scaled   minDescent;
  scaled   minLineSpacing;
};

#endif // Layout_hh
