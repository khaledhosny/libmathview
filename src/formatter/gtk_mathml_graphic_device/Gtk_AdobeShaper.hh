
#ifndef __Gtk_AdobeShaper_hh__
#define __Gtk_AdobeShaper_hh__

#include "AShaper.hh"

class Gtk_AdobeShaper : public AShaper
{
public:
  Gtk_AdobeShaper(void) { }
  virtual ~Gtk_AdobeShaper() { }

  virtual registerChars(class ShaperManager&, unsigned shaperId) const;
  virtual AreaRef shapeChar(DOM::Char32 ch, const GlyphSpec& spec, unsigned size) const;
  virtual AreaRef combineWith(const AreaRef& base, DOM::Char32 ch) const;
  virtual AreaRef stretchH(const AreaRef& base, const scaled& hSpan) const;
  virtual AreaRef stretchV(const AreaRef& base, const scaled& vSpan) const;
};

#endif // __Gtk_AdobeShaper_hh__
