
#include <config.h>

#include <cassert>

#include "MathVariant.hh"
#include "MathVariantMap.hh"

#define DECLARE_MAP(n) DOM::Char32 map_##n##_variant(DOM::Char16);

namespace DOM = GdomeSmartDOM;

DECLARE_MAP(bold)
DECLARE_MAP(italic)
DECLARE_MAP(bold_italic)
DECLARE_MAP(double_struck)
DECLARE_MAP(bold_fraktur)
DECLARE_MAP(script)
DECLARE_MAP(bold_script)
DECLARE_MAP(fraktur)
DECLARE_MAP(sans_serif)
DECLARE_MAP(bold_sans_serif)
DECLARE_MAP(sans_serif_italic)
DECLARE_MAP(sans_serif_bold_italic)
DECLARE_MAP(monospace)

static DOM::Char32 map_normal_variant(DOM::Char16 ch)
{ return ch; }

DOM::Char32
mapMathVariant(MathVariant variant, DOM::Char16 ch)
{

  static DOM::Char32 (*map[])(DOM::Char16) = 
    {
      map_normal_variant,
      map_bold_variant,
      map_italic_variant,
      map_bold_italic_variant,
      map_double_struck_variant,
      map_bold_fraktur_variant,
      map_script_variant,
      map_bold_script_variant,
      map_fraktur_variant,
      map_sans_serif_variant,
      map_bold_sans_serif_variant,
      map_sans_serif_italic_variant,
      map_sans_serif_bold_italic_variant,
      map_monospace_variant
    };

  assert(variant >= NORMAL_VARIANT && variant <= MONOSPACE_VARIANT);
  return map[variant - NORMAL_VARIANT](ch);
}
