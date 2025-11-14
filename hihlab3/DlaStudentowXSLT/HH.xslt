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

					<!-- menu do poruszania w obrębie strony -->
					<!-- menu for moving within a page -->


					<div id="content">
						<h2 id="scientists">Scientists</h2>
						<!-- a place for information about hypertext scientists -->

						<h2 id="systems">Systems</h2>
						<!-- a place for information about hypertext systems -->


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

	<xsl:template match="author">
		Copyright 2022, 
	</xsl:template>

<!--		<nav>
			<ul>
				<li> <a href="#scientists">Scientists</a> </li>
				<li> <a href="#systems">Systems</a> 	</li>
			</ul>
		</nav>	
-->

</xsl:stylesheet>
