<?xml version="1.0" encoding="UTF-8"?>

<xsl:stylesheet version="1.0"
                 xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
>

<xsl:output method="text"/>

<xsl:template match="/">
  <xsl:apply-templates select="XTFM"/>
</xsl:template>

<xsl:template match="XTFM">
static TFMFont font = {
  0x<xsl:value-of select="Unity/@value"/>,
  "<xsl:value-of select = "Font/@family"/>",
  0x<xsl:value-of select="Font/@face"/>,
  "<xsl:value-of select="Font/@coding-scheme"/>",
  0x<xsl:value-of select="Font/@design-size"/>,
  0x<xsl:value-of select="Font/@checksum"/>,
  <xsl:value-of select="count(Font/Dimensions/Dimension)"/>
};

static TFMDimension dimension[] = {
<xsl:apply-templates select="Font/Dimensions/Dimension"/>
};

<xsl:apply-templates select="Font/Data/Character" mode="kerning"/>
<xsl:apply-templates select="Font/Data/Character" mode="ligature"/>

static TFMCharacter character[] = {
<xsl:apply-templates select="Font/Data/Character"/>
};
</xsl:template>

<xsl:template match="Dimension">  { 0x<xsl:value-of select="@index"/>
  <xsl:text>, </xsl:text>"<xsl:value-of select="@name"/>
  <xsl:text>", </xsl:text>0x<xsl:value-of select="@value"/> }<xsl:if test="position() &lt; last()">,
</xsl:if>
</xsl:template>

<xsl:template match="Character" mode="kerning">
<xsl:if test="Kerning">
static TFMKerning C_<xsl:value-of select="@index"/>_Kerning[] = {
<xsl:apply-templates select="Kerning"/>
};
</xsl:if>
</xsl:template>

<xsl:template match="Character" mode="ligature">
<xsl:if test="Ligature">
static TFMLigature C_<xsl:value-of select="@index"/>_Ligature[] = {
<xsl:apply-templates select="Ligature"/>
};
</xsl:if>
</xsl:template>

<xsl:template match="Character">  { 0x<xsl:value-of select="@index"/>
    <xsl:text>, </xsl:text>0x<xsl:value-of select="@width"/>
    <xsl:text>, </xsl:text>0x<xsl:value-of select="@height"/>
    <xsl:text>, </xsl:text>0x<xsl:value-of select="@depth"/>
    <xsl:text>, </xsl:text>0x<xsl:value-of select="@italic"/>
    <xsl:text>, </xsl:text><xsl:value-of select="count(Kerning)"/>
    <xsl:text>, </xsl:text>
    <xsl:choose>
      <xsl:when test="Kerning">&amp;C_<xsl:value-of select="@index"/>_Kerning, </xsl:when>
      <xsl:otherwise>0, </xsl:otherwise>
    </xsl:choose>
    <xsl:value-of select="count(Ligature)"/>
    <xsl:text>, </xsl:text>
    <xsl:choose>
      <xsl:when test="Ligature">&amp;C_<xsl:value-of select="@index"/>_Ligature </xsl:when>
      <xsl:otherwise>0 </xsl:otherwise>
    </xsl:choose>}<xsl:if test="position() &lt; last()">,
</xsl:if>
</xsl:template>

<xsl:template match="Kerning">  { 0x<xsl:value-of select="@index"/>, 0x<xsl:value-of select="@value"/> }<xsl:if test="position() &lt; last()">,
</xsl:if>
</xsl:template>

<xsl:template match="Ligature">  { 0x<xsl:value-of select="@index"/>, 0x<xsl:value-of select="@mode"/>, 0x<xsl:value-of select="@result"/> }<xsl:if test="position() &lt; last()">,
</xsl:if>
</xsl:template>

</xsl:stylesheet>

