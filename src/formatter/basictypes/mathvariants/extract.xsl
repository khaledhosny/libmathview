<xsl:stylesheet
  version="1.0"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:xh="http://www.w3.org/1999/xhtml"
>

<xsl:param name="temp" select="true()"/>

<xsl:output method="text" />

<xsl:param name="variant" />

<xsl:template match="/">
#include &lt;GdomeSmartDOM.hh&gt;

namespace DOM = GdomeSmartDOM;

  <xsl:apply-templates select="descendant::xh:table"/>
</xsl:template>

<xsl:template match="xh:table">
typedef struct TableEntry
{
  DOM::Char16 normal;
  DOM::Char32 variant;
} TableEntry;

static TableEntry table[] =
{
<xsl:apply-templates select="xh:tr/xh:td[2]">
    <xsl:sort select="."/>
  </xsl:apply-templates>  { 0, 0 }
};

<xsl:if test="$temp">
#include &lt;cstdio&gt;

int
main()
{
  unsigned i = 0;
  
  while (table[i].normal != 0)
  {
    unsigned i0 = i;
    while (table[i].normal == table[i + 1].normal - 1) i++;
    if (i0 == i)
      printf("  if (ch == 0x%04X) return 0x%04X;\n", table[i].normal, table[i].variant);
    else
      printf("  if (ch &gt;= 0x%04X and ch &lt; 0x%04X) return table[ch - 0x%04X].variant;\n", table[i0].normal, table[i].normal, table[i0].normal - i0);
    i++;
  }

  return 0;
}
</xsl:if>

</xsl:template>

<xsl:template match="xh:td">  { 0x<xsl:value-of select="."/>, 0x<xsl:value-of select="preceding-sibling::xh:td"/> }, 
</xsl:template>

</xsl:stylesheet>
