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

#ifndef MathMLObject_hh
#define MathMLObject_hh

#include <stddef.h>

#include "defs.h"

#define INDENT_AMOUNT 2

class MathMLObject {
public:
  MathMLObject(void);
  virtual bool IsNode(void) const;
  virtual bool IsText(void) const;
  virtual bool IsString(void) const;
  virtual bool IsChar(void) const;
  virtual bool IsStretchyChar(void) const;
  virtual bool IsCombinedChar(void) const;
  virtual bool IsSpace(void) const;
  virtual bool IsMark(void) const;
  virtual bool IsElement(void) const;
  virtual bool IsToken(void) const;
  virtual bool IsContainer(void) const;
  virtual bool IsDocument(void) const;
  virtual bool IsOperator(void) const;
  virtual bool IsEmbellishedOperator(void) const;
  virtual bool IsBreakable(void) const;
  virtual ~MathMLObject();

#ifdef DEBUG
  static int GetCounter(void) { return counter; }
#endif // DEBUG

protected:
  void PrintIndentation(void) const;
  void IndentMore(void) const { indentLevel += INDENT_AMOUNT; }
  void IndentLess(void) const { indentLevel -= INDENT_AMOUNT; }

private:
#ifdef DEBUG
  static int counter;
#endif // DEBUG
  static unsigned indentLevel;
};

typedef MathMLObject* MathMLObjectPtr;

#endif // MathMLObject_hh
