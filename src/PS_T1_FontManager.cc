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
#ifdef HAVE_UNISTD_H
#include <sys/types.h>
#include <unistd.h>
#endif
#ifdef HAVE_SYS_WAIT_H
#include <sys/wait.h>
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifdef HAVE_VFORK_H
#include <vfork.h>
#endif

#include <t1lib.h>

#include "T1_Font.hh"
#include "Iterator.hh"
#include "Container.hh"
#include "MathEngine.hh"
#include "PS_T1_FontManager.hh"

// auxiliary functions' prototypes

static const char* getFontFilePath(unsigned fontId);
static void dumpFontFile(FILE* source, FILE* target);
static void convertToPFA(const char* source, const char* target);

// PS_T1_FontManager implementation

PS_T1_FontManager::PS_T1_FontManager() : T1_FontManager()
{
}

PS_T1_FontManager::~PS_T1_FontManager()
{
}


void
PS_T1_FontManager::DumpFontDictionary(FILE* output) const
{
  assert(output != NULL);

  Container<unsigned> fontId;

  for (Iterator<Bucket*> i(content); i.More(); i.Next()) {
    assert(i() != NULL);
    if (i()->used) {
      const AFont* font = i()->font;
      assert(font != NULL);
      const T1_Font* t1_font = TO_T1_FONT(font);
      assert(t1_font != NULL);

      unsigned id = t1_font->GetNativeFontId();
      if (!fontId.Contains(id)) fontId.Append(id);
    }
  }

  for (Iterator<unsigned> id(fontId); id.More(); id.Next()) {
    if (id.IsFirst()) {
      fprintf(output, "%%%%DocumentSuppliedResources: font ");
    } else {
      fprintf(output, "%%%%+ font ");
    }
    fprintf(output, "%s\n", T1_GetFontName(id()));
    if (id.IsLast()) fprintf(output, "\n\n");
  }

  fprintf(output, "%%%%BeginSetup\n");

  for (Iterator<unsigned> id(fontId); id.More(); id.Next()) {
    const char* fileName = getFontFilePath(id());

    if (fileName != NULL) {
      char* path = new char[strlen(fileName) + 4];
      strcpy(path, fileName);
      unsigned len = strlen(path);

      if (len < 4 || path[len - 4] != '.'){
	strcat(path, ".pfb");
	len += 4;
      }

      strcpy(path + len - 4, ".pfa");
      FILE* f = fopen(path, "rt");

      if (f == NULL) {
	strcpy(path + len - 4, ".pfb");
	f = fopen(path, "rb");

	if (f != NULL) {
	  fclose(f);
	  convertToPFA(path, "/tmp/tmpfont.pfa");
	  f = fopen("/tmp/tmpfont.pfa", "rt");
	}
      }

      if (f != NULL) {
	fprintf(output, "%%%%BeginResource: font %s\n", T1_GetFontName(id()));
	dumpFontFile(f, output);
	fprintf(output, "%%%%EndResource\n\n");
	fclose(f);	
      } else
	MathEngine::logger(LOG_WARNING, "could not include font file `%s'", path);

      delete [] path;
    } else {
      MathEngine::logger(LOG_WARNING, "could not find file for font `%s'", T1_GetFontName(id()));
    }
  }

  fprintf(output, "%%%%EndSetup\n\n");

  for (Iterator<Bucket*> i(content); i.More(); i.Next()) {
    assert(i() != NULL);
    if (i()->used) {
      const AFont* font = i()->font;
      assert(font != NULL);
      const T1_Font* t1_font = TO_T1_FONT(font);
      assert(t1_font != NULL);

      fprintf(output, "/F%d /%s findfont %f scalefont def\n",
	      t1_font->GetFontId(),
	      T1_GetFontName(t1_font->GetNativeFontId()),
	      t1_font->GetScale());
    }
  }
}

// auxiliary functions

static const char*
getFontFilePath(unsigned fontId)
{
  const char* fontFileName = T1_GetFontFileName(fontId);
  assert(fontFileName != NULL);

  char* searchPath = strdup(T1_GetFileSearchPath(T1_PFAB_PATH));
  assert(searchPath != NULL);

  char* path = NULL;
  const char* dir = strtok(searchPath, ":");
  while (dir != NULL) {
    path = new char[strlen(dir) + strlen(fontFileName) + 2];
    strcpy(path, dir);
    if (path[strlen(path) - 1] != '/') strcat(path, "/");
    strcat(path, fontFileName);

    MathEngine::logger(LOG_DEBUG, "trying to find font file `%s'", path);
    FILE* f = fopen(path, "rb");
    if (f != NULL) {
      fclose(f);
      break;
    }

    delete [] path;
    dir = strtok(NULL, ":");
  }

  static char staticPath[128];
  strncpy(staticPath, path, 127);
  delete [] path;

  return staticPath;
}

static void
dumpFontFile(FILE* source, FILE* target)
{
  assert(source != NULL);
  assert(target != NULL);

  static char buffer[256];

  while (!feof(source) && fgets(buffer, 255, source))
    fputs(buffer, target);
}

static void
convertToPFA(const char* source, const char* target)
{
  assert(source != NULL);
  assert(target != NULL);

  MathEngine::logger(LOG_DEBUG, "found font file `%s', converting...", source);

  pid_t child = vfork();

  MathEngine::logger(LOG_DEBUG, "new pid %d", child);

  if (child == 0) {
    int res = execlp("pfb2pfa", "pfb2pfa", source, target, NULL);  
    if (res == -1) MathEngine::logger(LOG_ERROR, "unable to convert PFB font to PFA");
    exit(-1);
  } else {
    MathEngine::logger(LOG_DEBUG, "waiting for PID: %d", child);
    waitpid(child, NULL, 0);
    MathEngine::logger(LOG_DEBUG, "child %d died, returning", child);
  }
}

#endif // HAVE_LIBT1
