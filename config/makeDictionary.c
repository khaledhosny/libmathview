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
#include <string.h>

char buffer[257];

main(int argc, char* argv[])
{
  printf("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\n");
  printf("<!DOCTYPE dictionary SYSTEM \"%s\">\n", argv[1]);
  printf("<dictionary>\n");
  while (!feof(stdin)) {
    unsigned len;

    fgets(buffer, 256, stdin);
    
    len = strlen(buffer);
    if (len > 0 && buffer[0] != '#') {
      if (buffer[len - 1] == '\n') buffer[len - 1] = '\0';
      printf("  <operator name=%s/>\n", buffer);
    }
  }
  printf("</dictionary>\n");
}

