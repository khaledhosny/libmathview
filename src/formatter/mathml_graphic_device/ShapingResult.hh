
#ifndef __ShapingResult_hh__
#define __ShapingResult_hh__

#include "AreaFactory.hh"

class ShapingResult
{
public:
  ShapingResult(const SmartPtr<AreaFactory>&,
		const DOM::UCS4String& src,
		const std::vector<GlyphSpec>& s,
		const scaled& fs, const scaled& v = 0, const scaled& h = 0)
    : factory(f), source(src), spec(s), fontSize(fs), vSpan(v), hSpan(h), index(0) { }

  DOM::UCS4String getSource(void) const { return source; }
  bool done(void) const { return index == source.length(); }
  bool empty(void) const { return res.empty(); }
  bool vSpan(void) const { return vSpan > 0; }
  bool hSpan(void) const { return hSpan > 0; }
  bool stretchy(void) const { return vSpan() || hSpan(); }
  SmartPtr<AreaFactory> getFactory(void) const { return factory; }
  scaled getFontSize(void) const { return fontSize; }
  scaled getVSpan(void) const { return vSpan; }
  scaled getHSpan(void) const { return hSpan; }
  unsigned chunkSize(void) const;
  unsigned getShaperId(void) const;

  AreaRef popArea(void);
  void pushArea(const AreaRef&);
  AreaRef getArea(int = -1) const;
  AreaRef area(void) const;

  void advance(int = 1);
  DOM::Char32 prevChar(void) const;
  DOM::Char32 nextChar(void) const;
  DOM::UCS4String prevString(int = -1) const;
  DOM::UCS4String nextString(int = -1) const;

private:
  SmartPtr<AreaFactory> factory;
  DOM::UCS4String source;
  std::vector<GlyphSpec> spec;
  scaled fontSize;
  scaled hSpan;
  scaled vSpan;
  unsigned index;
  std::vector<AreaRef> res;
};

#endif // __ShapingResult_hh__
