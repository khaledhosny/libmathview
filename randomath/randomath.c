/*
 * Copyright (C) 2000, Luca Padovani <luca.padovani@cs.unibo.it>.
 * 
 * This file is part of GtkMathView, a Gtk widget for MathML.
 * 
 * GtkMathView is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * GtkMathView is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with GtkMathView; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 * 
 * For details, see the GtkMathView World-Wide-Web page,
 * http://cs.unibo.it/~lpadovan/mml-widget, or send a mail to
 * <luca.padovani@cs.unibo.it>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <glib.h>

guint max_depth = 10;

void random_term(guint);
void random_op(guint);
void random_expr(guint);

int choice(guint n)
{
  g_return_val_if_fail(n > 0, 0);
  return (random() % n == 0);
}

int rnd_range(guint m, guint M)
{
  g_return_val_if_fail(M >= m, m);
  return m + random() % (M - m + 1);
}

int rnd(guint m)
{
  g_return_val_if_fail(m > 0, 0);
  return rnd_range(0, m - 1);
}

void random_token(guint depth)
{
  if (choice(2)) {
    printf("<mn color=\"#104e8b\">%c%d</mn>\n", (choice(5) ? '-' : ' '), random() % 10);
  } else {
      printf("<mi color=\"#a52a2a\">");
    if (choice(2))
      printf("%c", rnd_range('a', 'z'));
    else
      printf("&#x%x;", 0x3b1 + rnd_range(0, 'z' - 'a'));
    printf("</mi>\n");
  }
}

void random_script(guint depth)
{
  const char* script = choice(3) ? "msup" : "msub";
  printf("<%s>\n", script);
  random_term(depth + 1);
  random_term(depth + 1);
  printf("</%s>\n", script);
}

void random_subsup(guint depth)
{
  printf("<msubsup>\n");
  random_term(depth + 1);
  random_term(depth + 1);
  random_term(depth + 1);
  printf("</msubsup>\n");
}

void random_matrix(guint depth)
{
  char op_fence[] = "(|[{";
  char cl_fence[] = ")|]}";

  guint n = rnd_range(2, 6);
  guint m = rnd_range(2, 6);
  int i;
  int j;

  guint fence = rnd(strlen(op_fence));

  printf("<mfenced open=\"%c\" close=\"%c\">\n", op_fence[fence], cl_fence[fence]);
  printf("<mtable equalcolumns=\"%s\" equalrows=\"%s\">\n",
	 choice(2) ? "true" : "false",
	 choice(2) ? "true" : "false");
  for (i = 0; i < n; i++) {
    printf("<mtr>\n");
    for (j = 0; j < m; j++) {
      printf("<mtd>\n");
      random_term(depth + 2);
      printf("</mtd>\n");
    }
    printf("</mtr>\n");
  }
  printf("</mtable>\n");
  printf("</mfenced>");
}

void random_under_over(guint depth)
{
  const char* uo = choice(2) ? "munder" : "mover";

  printf("<%s>\n", uo);
  random_term(depth + 2);
  random_op(depth + 1);
  printf("</%s>\n", uo);
}

void random_radical(guint depth)
{
  int n = choice(4);

  const char* s = n ? "mroot" : "msqrt";

  printf("<%s>\n", s);
  random_term(depth + 1);
  if (n) random_term(depth + 5);
  printf("</%s>\n", s);
}

void random_tuple(guint depth)
{
  guint n = rnd_range(1, 5);

  printf("<mrow>\n");
  while (n-- > 0) {
    random_term(depth + 2);
    if (n > 0) printf("<mo>,</mo>\n");
  }
  printf("</mrow>\n");
}

void random_app(guint depth)
{
  printf("<mrow><mi>%c</mi><mo>&af;</mo>\n", rnd_range('a', 'z'));
  printf("<mrow><mo>(</mo>\n");
  random_tuple(depth + 1);
  printf("<mo>)</mo></mrow>\n");
  printf("</mrow>\n");
}

void random_op(guint depth)
{
  const char* op[] = {
    "plus", "minus", "times", "rarr", "circle", "UnionPlus", "UnionMinus", "gg", "ll",
    "subset", "subseteq", "sim"
  };

  guint n = rnd(sizeof(op) / sizeof(void*));
  printf("<mo><mchar name=\"%s\"/></mo>\n", op[n]);
}

void random_fenced(guint depth)
{
  char op_fence[] = "(|[{";
  char cl_fence[] = ")|]}";

  guint fence = rnd(strlen(op_fence));

  printf("<mfenced open=\"%c\" close=\"%c\">\n", op_fence[fence], cl_fence[fence]);
  random_term(depth + 1);
  printf("</mfenced>");  
}

void random_term(guint depth)
{
  void (*random_t[])(guint) = {
    random_token,
    random_token,
    random_script,
    random_subsup,
    random_fenced,
    random_matrix,
    random_under_over,
    random_radical,
    random_app,
    random_app,
    random_app,
    random_tuple,
    random_expr
  };

  guint t = rnd(sizeof(random_t) / sizeof(void*));
  if (depth > max_depth) t = 0;

  random_t[t](depth);
}

void random_expr(guint depth)
{
  guint n = rnd_range(2, 5);

  printf("<mrow>\n");
  while (n-- > 0) {
    random_term(depth + 1);
    if (n > 0) random_op(depth + 1);
  }
  printf("</mrow>\n");
}

void random_prop(guint depth)
{
  const char* relop[] = {
    "equals", "NotEqualTilde", "approxeq", "bumpeq", "gt", "lt", "le", "ge", "Implies"
  };

  guint n = rnd_range(1, 3);
  guint op = rnd(sizeof(relop) / sizeof(char*));

  printf("<mrow>\n");
  while (n-- > 0) {
    random_expr(depth + 1);
    if (n > 0) printf("<mo><mchar name=\"%s\"/></mo>\n", relop[op]);
  }
  printf("</mrow>\n");
}

void random_math(guint depth)
{
  printf("<math display=\"%s\">\n", choice(2) ? "block" : "inline");
  random_prop(depth + 1);
  printf("</math>\n");
}

int main(int argc, char** argv)
{
  if (argc > 1) max_depth = atoi(argv[1]);
  srandom(time(NULL));
  random_math(0);
  return 0;
}
