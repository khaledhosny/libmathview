
#ifndef __Gtk_PangoShaper_hh__
#define __Gtk_PangoShaper_hh__

#include <pango/pango.h>

#include "Shaper.hh"
#include "GObjectPtr.hh"

class Gtk_PangoShaper : public Shaper
{
public:
  Gtk_PangoShaper(void);
  virtual ~Gtk_PangoShaper();

  virtual void registerChars(class ShaperManager&, unsigned shaperId) const;
  virtual unsigned shape(class ShapingResult&) const;

  void setPangoContext(const GObjectPtr<PangoContext>& c) { context = c; }
  GObjectPtr<PangoContext> getPangoContext(void) const { return context; }

private:
  GObjectPtr<PangoContext> context;
};

#endif // __Gtk_PangoShaper_hh__
