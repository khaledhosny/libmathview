
#include <config.h>

#include <cassert>

#include "ShapingResult.hh"

unsigned
ShapingResult::chunkSize() const
{
  assert(!done());
  unsigned n = 0;
  unsigned si = getShaperId();
  while (index + n < spec.size() && spec[index + n].getShaperId() == si) n++;
  return n;
}

unsigned
ShapingResult::getShaperId() const
{
  assert(!done());
  return spec[index].getShaperId();
}

const DOM::Char32*
ShapingResult::data() const
{
  assert(!done());
  return source.data() + index;
}

AreaRef
ShapingResult::area() const
{
  if (res.size() == 1)
    return res[0];
  else
    return factory->createHorizontalArrayArea(res);
}

void
ShapingResult::advance(int n)
{
  assert(index + n >= 0 && index + n <= source.length());
  index += n;
}

DOM::Char32
ShapingResult::prevChar() const
{
  assert(index > 0);
  return source[index - 1];
}

DOM::Char32
ShapingResult::nextChar() const
{
  assert(index < source.length());
  return source[index];
}

DOM::UCS4String
ShapingResult::prevString(int l) const
{
  if (l < 0 || l > index) l = index;
  return source.substr(index - l, l);
}

DOM::UCS4String
ShapingResult::nextString(int l) const
{
  if (l < 0 || l > source.length() - index) l = source.length() - index;
  return source.substr(index, l);
}

