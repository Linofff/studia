<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="html" indent="yes" />

  <xsl:template match="/">
    <xsl:element name="webpages">

      <xsl:element name="student">
        <xsl:attribute name="name">
          <xsl:value-of select="/hypertext/author/name" />
        </xsl:attribute>
        <xsl:attribute
          name="surname">
          <xsl:value-of select="/hypertext/author/surname" />
        </xsl:attribute>
      </xsl:element>
            
      <xsl:apply-templates
        select="//link" />
    </xsl:element>
  </xsl:template>

  <xsl:template match="link">
    <xsl:element name="address">
      <xsl:value-of select="@source" />
    </xsl:element>
  </xsl:template>

</xsl:stylesheet>
