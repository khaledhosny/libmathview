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

#ifndef MathMLFrame_hh
#define MathMLFrame_hh

#include "Coords.hh"
#include "BoundingBox.hh"
#include "MathMLNode.hh"
#include "DrawingArea.hh"

class MathMLFrame : public MathMLNode {
public:
  MathMLFrame(void);
  virtual void   SetPosition(scaled, scaled) = 0;
  virtual void   Render(const DrawingArea&) = 0;
  virtual ~MathMLFrame();

  scaled         GetX(void) const { return position.x; }
  scaled         GetY(void) const { return position.y; }
  const BoundingBox& GetBoundingBox(void) const { return box; }
  virtual scaled GetLeftEdge(void) const = 0;
  virtual scaled GetRightEdge(void) const = 0;

  // flags facilities
  virtual bool 	 IsFrame(void) const;
  virtual bool 	 IsInside(scaled, scaled) const = 0;
  virtual bool 	 IsLast(void) const;
  bool         	 IsSelected(void) const { return selected != 0; }
  bool         	 IsDirty(void) const { return dirty != 0; }
  bool         	 HasDirtyChildren(void) const { return dirtyChildren != 0; }
  bool         	 HasDirtyBackground(void) const { return dirtyBackground != 0; }
  bool           HasDirtyLayout(void) const { return dirtyLayout != 0; }

  void         	 SetLast(void) { last = 1; }
  virtual void 	 SetDirty(const Rectangle* = NULL);
  virtual void 	 SetDirtyChildren(void);
  virtual void 	 SetSelected(void);
  virtual void 	 ResetSelected(void);
  void         	 ResetDirty(void) { dirty = dirtyChildren = dirtyBackground = 0; }
  void           ResetDirtyLayout(void) { dirtyLayout = 0; }
  virtual void 	 ResetLast(void);
  virtual void   SetDirtyLayout(void);  

  virtual BreakId GetBreakability(void) const;

protected:
  Coords      position;
  BoundingBox box;

  unsigned    selected : 1;
  unsigned    dirty : 1;
  unsigned    dirtyChildren : 1;
  unsigned    dirtyBackground : 1;
  unsigned    dirtyLayout : 1;
  unsigned    last : 1; // is != 0 is this frame is the last in a row
};

typedef MathMLFrame* MathMLFramePtr;

#define TO_FRAME(object) (dynamic_cast<MathMLFrame*>(object))

#endif // MathMLFrame_hh
