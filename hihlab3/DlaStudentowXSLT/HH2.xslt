<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
<xsl:output method="xml" />
<xsl:template match="/">
	<xsl:element name="webpages">
		<xsl:apply-templates select="//link"/>
	</xsl:element>
</xsl:template>

<xsl:template match="link">
	<xsl:element name="address">
		<xsl:value-of select="@source" />
	</xsl:element>
</xsl:template>

</xsl:stylesheet>
