<!ELEMENT xi:include (xi:fallback?) >
<!ATTLIST xi:include
    xmlns:xi   CDATA       #FIXED    "http://www.w3.org/2003/XInclude"
    href       CDATA       #REQUIRED
    parse      (xml|text)  "xml"
    encoding   CDATA       #IMPLIED >

<!ELEMENT xi:fallback ANY>
<!ATTLIST xi:fallback
    xmlns:xi   CDATA   #FIXED   "http://www.w3.org/2003/XInclude" >

<!ENTITY % local.chapter.class "| xi:include">
<!ENTITY % local.divcomponent.mix "| xi:include">   <!-- inside chapter or section elements                -->
<!ENTITY % local.para.char.mix "| xi:include">      <!-- inside para, programlisting, literallayout, etc.  -->
<!ENTITY % local.info.class "| xi:include">         <!-- inside bookinfo, chapterinfo, etc.                -->

