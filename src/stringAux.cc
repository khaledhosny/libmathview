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
#include <assert.h>
#include <iconv.h>
#include <wctype.h>

#include "stringAux.hh"
#include "StringUnicode.hh"
#include "allocTextNode.hh"
#include "MathMLCharNode.hh"
#include "MathMLStringNode.hh"

union endian_union {
  Char8 b[4];
  Char32 ch;
};

#ifndef WORD_BIGENDIAN
static Char32 swac(Char32 ch)
{
  endian_union u1;
  endian_union u2;

  u1.ch = ch;
  u2.b[0] = u1.b[3];
  u2.b[1] = u1.b[2];
  u2.b[2] = u1.b[1];
  u2.b[3] = u1.b[0];

  return u2.ch;
}
#endif

String* allocString(mDOMConstStringRef str)
{
  assert(str != NULL);

  static bool first = true;

  static iconv_t cd;
  if (first) {
    cd = iconv_open("UCS4", "UTF-8");
    assert(cd != (iconv_t) -1);
    first = false;
  }

  assert(str != NULL);
  String* sValue = NULL;

  const char* s = reinterpret_cast<const char*>(str);

  static Char32 buffer[128];
  size_t inBytesLeft = strlen(s);
  size_t outBytesLeft = sizeof(buffer);
  const char* inbuf = s;
  char* outbuf = reinterpret_cast<char*>(buffer);
  char* outbuf0 = outbuf;
  size_t nConv = 0;
  unsigned n;

  String* res = NULL;

  if (isPlain(s)) res = new StringU<Char8>(s);
  else {
    do {
      String* chunk = NULL;
      n = 0;
      nConv = iconv(cd, &inbuf, &inBytesLeft, &outbuf, &outBytesLeft);
      if (nConv != (size_t) -1) {
	unsigned n = (outbuf - outbuf0) / sizeof(Char32);
	if (n > 0) {
#ifndef WORD_BIGENDIAN
	  for (unsigned i = 0; i < n; i++) buffer[i] = swac(buffer[i]);
#endif
	  chunk = new StringU<Char32>(buffer, n);
	}
      } else {
	// the error "invalid argument" is probably due to an old version
	// of libxml (1.x.y)
	perror("iconv: ");
      }

      if (chunk != NULL) {
	if (sValue == NULL) sValue = chunk;
	else sValue->Append(*chunk);
      }
    } while (inBytesLeft > 0 && n > 0);

    if (sValue != NULL) {
      Char big = sValue->GetBiggestChar();
      if (isPlain(big)) {
	res = new StringU<Char8>(*sValue);
	delete sValue;
      } else if (isUnicode16(big)) {
	res = new StringU<Char16>(*sValue);
	delete sValue;
      } else res = sValue;
    }
  }

  if (res == NULL) res = new StringC("?");

  return res;
}

String* allocString(const String& str, unsigned offset, unsigned length)
{
  assert(length >= 1);

  String* sValue = NULL;

  Char big = str.GetBiggestChar(offset, length);
  if      (isPlain(big)) sValue = new StringU<Char8>(str, offset, length);
  else if (isUnicode16(big)) sValue = new StringU<Char16>(str, offset, length);
  else sValue = new StringU<Char32>(str, offset, length);

  return sValue;
}
