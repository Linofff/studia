<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:msxsl="urn:schemas-microsoft-com:xslt" exclude-result-prefixes="msxsl"
>
    <xsl:output method="html" indent="yes"/>

	<xsl:template match="/">

		<html>
			<head>
				<meta charset="UTF-8" />
				<title>Hypertext hypermedia</title>
				<meta name="viewport" content="width=device-width, initial-scale=1.0" />
				<link rel="stylesheet" href="index.css" />
			</head>
			<body>
				<div id="wrapper">
					<header>
						<h1>Hypertext hypermedia</h1>
					</header>
					
					<xsl:call-template name="menu"/>

					<div id="content">
						<h2 id="scientists">Scientists</h2>
						<!-- a place for information about hypertext scientists -->
						<xsl:apply-templates select="hypertext/people/person"/>

						<h2 id="systems">Systems</h2>
						<!-- a place for information about hypertext systems -->
						<xsl:apply-templates select="hypertext/systemList/system"/>

					</div>
					<!-- end content -->
				</div>
				<!-- end wrapper -->
				<footer>
					<xsl:apply-templates select="hypertext/author"/>	<!-- wywołanie szablonu pozwalającego na wyświetlenie informacji o imieniu oraz nazwisku studenta -->
				</footer>
			</body>
		</html>

	</xsl:template>
	
	<xsl:template match="person">
		<div class="person">
			
			<h3>
				<xsl:value-of select="name"/>
				<xsl:text> </xsl:text>
				<xsl:value-of select="surname"/>
			</h3>
			
			<p>Date of birth: <xsl:value-of select="birth"/></p>
			
			<xsl:if test="@alive='no'">
				<p>Date of death: <xsl:value-of select="death"/></p>
			</xsl:if>
			
			<xsl:apply-templates select="image"/>
			
			<div class="about">
				<p><xsl:value-of select="about"/></p>
			</div>
			
			<div class="link">
				<ul>
					<xsl:apply-templates select="link"/>
				</ul>
			</div>
			
			<xsl:for-each select="achievementList/achievement">
				<xsl:sort select="title" order="ascending"/>
				
				<div class="achievement">
					<h4>
						<xsl:number value="position()"/>.
						<xsl:text> </xsl:text>
						<xsl:value-of select="title"/>
					</h4>
					
					<p><xsl:value-of select="description"/></p>
					
					<img src="{image/@source}" alt="{image}"/>
				</div>
				
			</xsl:for-each>
			
		</div>
	</xsl:template>
	
	<xsl:template match="image">
		<div class="photo">
			<img class="right" src="{@source}" alt="{.}"/>
		</div>
	</xsl:template>
	
	<xsl:template match="link">
		<xsl:if test="position() != last()">
			<li>
				<a href="{@source}">
					<xsl:value-of select="."/>
				</a>
			</li>
		</xsl:if>
	</xsl:template>
	
	
	<xsl:template match="author">
		Copyright 2022,
		<xsl:value-of select="name"/> <xsl:value-of select="surname"/>
	</xsl:template>
	
	<xsl:template match="system">
		<div class="system">
			
			<h3><xsl:value-of select="name"/></h3>
			
			<p><xsl:value-of select="description"/></p>
			
			<ul>
				<xsl:apply-templates select="link"/>
			</ul>
			
		</div>
	</xsl:template>

	<xsl:template name="menu">
		<nav>
			<ul>
				<li> <a href="#scientists">Scientists</a> </li>
				<li> <a href="#systems">Systems</a></li>
			</ul>
		</nav>	
	</xsl:template>

</xsl:stylesheet>
