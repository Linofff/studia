<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:output method="xml" indent="yes" encoding="UTF-8" />

  <xsl:template match="/">
    <CubeRoot>
      <xsl:apply-templates select="rubiksProject" />
    </CubeRoot>
  </xsl:template>

  <xsl:template match="rubiksProject">
    <CreatorProfile>
      <xsl:attribute name="bio_link">
        <xsl:value-of select="inventor/@source" />
      </xsl:attribute>

      <PersonalDetails>
        <xsl:element name="{translate(inventor/name, ' ', '_')}">
          <xsl:text>Dane biograficzne twórcy kostki rubika</xsl:text>
        </xsl:element>

        <Citizenship>
          <xsl:attribute name="{translate(inventor/nationality, ' ', '_')}">
            <xsl:text>tak</xsl:text>
          </xsl:attribute>
        </Citizenship>

        <xsl:copy-of select="inventor/awards" />
      </PersonalDetails>
    </CreatorProfile>

    <MethodRepository>
      <xsl:for-each select="methods/method">
        <MethodEntry>
          <xsl:attribute name="name">
            <xsl:value-of select="@name" />
          </xsl:attribute>
          <xsl:attribute name="release_date">
            <xsl:value-of select="@year" />
          </xsl:attribute>

          <TechnicalDescription>
            <xsl:for-each select="description">
              <xsl:copy>
                <xsl:value-of select="." />
              </xsl:copy>
            </xsl:for-each>

            <DifficultyLevel>
              <xsl:value-of select="difficulty" />
            </DifficultyLevel>
          </TechnicalDescription>

        </MethodEntry>
      </xsl:for-each>
    </MethodRepository>
  </xsl:template>
</xsl:stylesheet>
