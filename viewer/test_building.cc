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

/* Generate a continued fraction that represents the square root of
 * the first argument (default = 2.0) to the second argument (default
 * = 2) levels.  The third argument optional argument is a file to
 * save the resulting xml to. */

#include <gtk/gtk.h>

#include "gtkmathview_libxml2.h"
#include <stdio.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <math.h>

int main(int argc, char *argv[]) {
  gtk_init (&argc, &argv);
    
  GtkWidget *window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_widget_show  (window);

  g_signal_connect(GTK_OBJECT(window), 
		   "delete_event", 
		   (GtkSignalFunc) gtk_main_quit, NULL);

  GtkWidget* math_view = gtk_math_view_new(NULL, NULL);

  /* 
   * Creates a new document, a node and set it as a root node
   */
  xmlDocPtr doc = xmlNewDoc(BAD_CAST "1.0");
  xmlNodePtr root_node = xmlNewNode(NULL, BAD_CAST "math");
  xmlDocSetRootElement(doc, root_node);
  
  // gtk_math_view_load_document is very picky about the namespace
  // used, so we have to make sure we set the ns for the document.
  
  xmlNsPtr ns = xmlNewNs(root_node, BAD_CAST "http://www.w3.org/1998/Math/MathML", BAD_CAST "m");
  xmlNewProp(root_node, BAD_CAST "display" , BAD_CAST "block");
  xmlSetNs(root_node, ns);

  /* 
   * xmlNewChild() creates a new node, which is "attached" as child node
   * of root_node node. 
   */

/* Here is the sort of mathml we want to generate.
    <mi>1</mi>
    <mo>+</mo>
    <mfrac>
      <mi>1</mi>
      <mrow>
        <mi>1</mi>
	<mo>+</mo>
	<mfrac>
	  <mi>1</mi>
	  <mrow>
	    <mi>1</mi>
	    <mo>+</mo>
	    <mi>...</mi>
	  </mrow>
	</mfrac>
      </mrow>
    </mfrac>
*/
  double rootand = 2;
  if(argc > 1) {
      sscanf(argv[1], "%lf", &rootand);
  }
  // We could just let a = 1 and b = rootand - 1, but this is more fun.
  double a = trunc(sqrt(rootand));
  double b = rootand - a*a;
  printf("rooting %g (%g^2 + %g)\n", rootand, a, b);
  
  char rootand_str[256], a_str[256], b_str[256];
  sprintf(rootand_str, "%g", rootand);
  sprintf(a_str, "%g", a);
  sprintf(b_str, "%g", b);
  
  xmlNodePtr sqrt_node = xmlNewChild(root_node, NULL, BAD_CAST "msqrt", NULL);
    xmlNewChild(sqrt_node, ns, BAD_CAST "mi", BAD_CAST rootand_str);
  xmlNewChild(root_node, ns, BAD_CAST "mo", BAD_CAST "=");
  xmlNewChild(root_node, ns, BAD_CAST "mi", BAD_CAST a_str);
  xmlNewChild(root_node, ns, BAD_CAST "mo", BAD_CAST "+");

  // Now we must create the continued fraction
  sprintf(rootand_str, "%g", rootand-1);

  xmlNodePtr node = root_node;
  int layers = 2;
  if(argc > 2) {
      sscanf(argv[2], "%d", &layers);
  }
  
  for (; --layers >= 0;) {
      node = xmlNewChild(node, NULL, BAD_CAST "mfrac", NULL);
      xmlNewChild(node, ns, BAD_CAST "mi", BAD_CAST b_str);
      xmlNodePtr denom = xmlNewChild(node, NULL, BAD_CAST "mrow", NULL);
        xmlNewChild(denom, ns, BAD_CAST "mi", BAD_CAST a_str);
        xmlNewChild(denom, ns, BAD_CAST "mo", BAD_CAST "+");
        node = denom;
  }
  xmlNewChild(node, ns, BAD_CAST "mi", BAD_CAST "...");
  
  // If we have a second argument, save the mathml to disk.
  if(argc > 3)
      xmlSaveFormatFileEnc(argv[3], doc, "UTF-8", 1);

  gtk_math_view_load_document(GTK_MATH_VIEW(math_view), doc);

  gtk_container_add (GTK_CONTAINER (window), math_view);
    
  gtk_widget_show (math_view);
    
  gtk_main ();
  
  /*free the document */
  xmlFreeDoc(doc);

  /*
   *Free the global variables that may
   *have been allocated by the parser.
   */
  xmlCleanupParser();
  
  return 0;
}

