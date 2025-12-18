<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

  <xsl:output method="html" indent="yes" />

  <xsl:variable name="currentYear" select="2025" />
  <xsl:variable name="allMethods" select="//method" />

  <xsl:template match="/">
    <html>
      <head>
        <link rel="stylesheet" href="style.css" />
        <title>Rubiks cube project</title>
      </head>
      <body>
        <h1>Rubix cube project</h1>

        <xsl:apply-templates select="rubiksProject/inventor" />
        <hr />

        <xsl:apply-templates select="rubiksProject/methods" />
        <hr />

        <xsl:apply-templates select="rubiksProject/training" />
        <hr />

        <xsl:apply-templates select="rubiksProject/recommendations" />
        <hr />

        <xsl:apply-templates select="rubiksProject/resources" />

      </body>
    </html>
  </xsl:template>

  <xsl:template match="inventor">
    <h3>Wynalazca: <a href="{@source}" target="_blank" title="Idź do Wikipedii">
        <xsl:value-of select="name" />
      </a>
    </h3> Rok urodzenia: <xsl:value-of
      select="yearofbirth" /> (<xsl:value-of select="$currentYear - yearofbirth" /> lat) <br />
    Narodowość: <xsl:value-of select="nationality" /> <br /> Wykształcenie: <xsl:value-of
      select="profession" />
    <xsl:call-template name="awardsTemplate" />
  </xsl:template>

  <xsl:template match="methods">
    <h2>Lista Metod: <xsl:value-of select="count($allMethods)" /></h2> 
    <xsl:for-each select="method">
      <xsl:sort select="@year" data-type="number" order="ascending" />
      <xsl:call-template
        name="method" />
    </xsl:for-each>
  </xsl:template>

  <xsl:template match="training">
    <h2>Porady Treningowe</h2>
    <ul>
      <xsl:apply-templates select="tip" />
    </ul>
  </xsl:template>

  <xsl:template match="tip">
    <li>
      <xsl:attribute name="style">
        <xsl:if test="position() mod 2 = 0"> color: blue; </xsl:if>
        <xsl:if
          test="position() = last()"> font-weight: bold; color: red; </xsl:if>
      </xsl:attribute>
      <xsl:apply-templates />
    </li>
  </xsl:template>

  <xsl:template match="recommendations">
    <h2>Rekomendacje dla poziomów</h2>
    <ul>
      <li>
        <strong>Początkujący: </strong>
        <xsl:value-of select="beginner" />
      </li>
      <li>
        <strong>Średniozaawansowany: </strong>
        <xsl:value-of select="intermediate" />
      </li>
      <li>
        <strong>Zaawansowany: </strong>
        <xsl:value-of select="advanced" />
      </li>
    </ul>
    <p style="font-style: italic">
      <xsl:value-of select="note" />
    </p>
  </xsl:template>

  <xsl:template match="resources">
    <h3>Kanały YouTube warte polecenia</h3>
    <ul>
      <xsl:for-each select="youtubeChannel">
        <li>
          <xsl:value-of select="." />
        </li>
      </xsl:for-each>
    </ul>
  </xsl:template>

  <xsl:template name="awardsTemplate">
    <h4>Nagrody</h4>
    <ul>
      <xsl:for-each select="awards/award">
        <li><xsl:value-of select="name" /> - <xsl:value-of select="year" /></li>
      </xsl:for-each>
    </ul>
  </xsl:template>

  <xsl:template name="method">
    <h3>
      <xsl:value-of select="position()" />. Metoda: <xsl:value-of select="@name" />

      <xsl:if
        test="@creator"> (Twórca: <xsl:value-of select="@creator" />) </xsl:if>

      <xsl:choose>
        <xsl:when test="difficulty = 'easy'">
          <span style="color: green"> Łatwy</span>
        </xsl:when>
        <xsl:when test="contains(difficulty, 'medium')">
          <span style="color: orange"> Średni</span>
        </xsl:when>
        <xsl:otherwise>
          <span style="color: red"> Trudny</span>
        </xsl:otherwise>
      </xsl:choose>

    </h3>
    Rok: <xsl:value-of
      select="@year" />

    <span class="yearnumber"> (Metoda ma <xsl:value-of
        select="$currentYear - @year" /> lat) </span>
    
    <br /> Opis: <xsl:apply-templates
      select="description" />
    <br />     <h4>Kroki:</h4>
    <ol>
      <xsl:apply-templates select="steps/step" />
    </ol>

    <p>
      <strong>Linki:</strong>
      <ul>
        <xsl:apply-templates select="links/link" />
      </ul>
    </p>

    <div>
      <xsl:for-each select="image | gallery/image">
        <img src="{@src}" width="200" style="margin: 5px; border: 1px solid #ccc;" />
      </xsl:for-each>
    </div>
    <hr />
  </xsl:template>

  <xsl:template match="term">
    <span class="term" style="font-weight: bold; color: #d9534f;">
      <xsl:value-of select="." />
    </span>
  </xsl:template>

  <xsl:template match="step[@name]">
    <li>
      <span class="step-name"><xsl:value-of select="@name" />: </span>
      <xsl:value-of select="." />
    </li>
  </xsl:template>

  <xsl:template match="step">
    <li>
      <xsl:value-of select="." />
    </li>
  </xsl:template>

  <xsl:template match="links/link[@type='video']">
    <li>
      <a href="{.}" class="video-link" target="_blank">Wideo Tutorial</a>
    </li>
  </xsl:template>

  <xsl:template match="links/link[@type='pdf']">
    <li>
      <a href="{.}" class="pdf-link" target="_blank">Dokument PDF</a>
    </li>
  </xsl:template>

  <xsl:template match="links/link[@type='html']">
    <li>
      <a href="{.}" class="html-link" target="_blank">Strona WWW</a> (<xsl:value-of select="." />) </li>
  </xsl:template>

  <xsl:template match="link">
    <a href="{.}">
      <xsl:value-of select="." />
    </a>
  </xsl:template>

</xsl:stylesheet>
