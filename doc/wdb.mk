if HAS_DOCBOOK

#-----------------------------------------------------------------------------
# Documentation for WDB
#-----------------------------------------------------------------------------

# Note: xmlto does not function properly with --searchpath, so in order
# to be able to use validator in our xml editor and get VPATH compiles
# to function properly, we need to perform this rather complicated
# copying of files when compiling.

# Docbook
DOCBOOK2HTML = 				$(DOCBOOK) html
DOCBOOK2PDF =				$(DOCBOOK) pdf
DOCBOOK2MAN = 				$(DOCBOOK) man 

DOC_WORKDIR= 				@builddir@/doc/work
DOC_INCWORKDIR=				@builddir@/doc/work/docbook
DOC_SRCWORKDIR= 			@builddir@/doc/work/src

DOCUMENTATION_AUX =			doc/docbook/wdb_entities.ent \
							doc/docbook/xinclude.mod

# Developer Documentation 
DOCUMENTATION_SRC =			doc/developer/wdb_developer-manual.xml \
							doc/developer/wdb_vision.xml \
							doc/developer/wdb_software-architecture.xml \
							doc/developer/wdb_test-plan.xml

DOCUMENTATION_AUX +=		doc/developer/gfx/wdb_logical-architecture.png \
							doc/developer/gfx/wdb_physical-architecture.png \
							doc/developer/gfx/wdb_process-architecture.png \
							doc/developer/gfx/wdb_production-overview.png \
							doc/developer/gfx/wdb_standalone-overview.png \
							doc/developer/gfx/feltLoad_class-diagram.png

# User Documentation
DOCUMENTATION_SRC +=		doc/user/wdb_user-manual.xml \
							doc/user/load_user-manual.xml \
							doc/user/wci_user-manual.xml

# Manuals
DEVELOPER_MANUAL_DOCDIR =	@srcdir@/doc/developer
USER_MANUAL_DOCDIR =		@srcdir@/doc/user

DEVELOPER_MANUAL_HMTLDIR =	@builddir@/doc/html/developer
DEVELOPER_MANUAL_PDFDIR =	@builddir@/doc/pdf
WDB_MANUAL_HMTLDIR =		@builddir@/doc/html/user
WDB_MANUAL_PDFDIR =			@builddir@/doc/pdf
LOAD_MANUAL_HMTLDIR =		@builddir@/doc/html/load
LOAD_MANUAL_PDFDIR =		@builddir@/doc/pdf
WCI_MANUAL_HMTLDIR =		@builddir@/doc/html/wci
WCI_MANUAL_PDFDIR =			@builddir@/doc/pdf

ENTITY_COMPILE=	sed s/__WDB_VERSION__/$(VERSION)/g @srcdir@/doc/docbook/wdb_entities.ent \
						| sed s¤__WDB_LIB_PATH__¤"$(LD_LIBRARY_PATH)"¤g \
						| sed s:__WDB_BUILDDIR__:@abs_builddir@:g \
						| sed s:__WDB_SRCDIR__:@srcdir@:g \
			 			| sed s:__WDB_BINDIR__:@bindir@:g \
			 			| sed s:__WDB_LIBDIR__:@libdir@:g \
			 			| sed s:__WDB_PKGLIBDIR__:$(libdir)/wdb:g \
			 			| sed s:__WDB_PKGDATADIR__:$(pkgdatadir):g \
			 			| sed s:__WDB_DATADIR__:$(wdbdatadir):g \
			 			| sed s:__WDB_SYSCONFDIR__:@sysconfdir@:g \
			 			| sed s:__WDB_LOCALSTATEDIR__:@localstatedir@:g \
			 			> $(DOC_INCWORKDIR)/wdb_entities.ent

COPY_DOCFILES=  mkdir -p $(DOC_WORKDIR); \
				mkdir -p $(DOC_INCWORKDIR); \
				mkdir -p $(DOC_SRCWORKDIR); \
				cp $(DEVELOPER_MANUAL_DOCDIR)/*.xml \
					$(DOC_SRCWORKDIR); \
				cp $(USER_MANUAL_DOCDIR)/*.xml \
					$(DOC_SRCWORKDIR); \
				cp @srcdir@/doc/docbook/xinclude.mod \
					$(DOC_INCWORKDIR)
					 			 
CLEAN_DOCFILES= rm -rf $(DOC_WORKDIR)


# HTML Documentation
#-----------------------------------------------------------------------------

html-local:
				@echo "Creating HTML documentation..."
				@$(COPY_DOCFILES)
				@$(ENTITY_COMPILE)
				@mkdir -p @builddir@/doc/html
				@echo "* WDB Developer's Manual"
				@-rm -rf $(DEVELOPER_MANUAL_HMTLDIR)
				@mkdir $(DEVELOPER_MANUAL_HMTLDIR)
				@mkdir $(DEVELOPER_MANUAL_HMTLDIR)/gfx
				@cp $(DEVELOPER_MANUAL_DOCDIR)/gfx/*.png \
				    $(DEVELOPER_MANUAL_HMTLDIR)/gfx
				@$(DOCBOOK2HTML) \
					-o $(DEVELOPER_MANUAL_HMTLDIR) \
					$(DOC_SRCWORKDIR)/wdb_developer-manual.xml
				@echo "* WDB user Manual"
				@-rm -rf $(WDB_MANUAL_HMTLDIR)
				@mkdir $(WDB_MANUAL_HMTLDIR)
				@mkdir $(WDB_MANUAL_HMTLDIR)/gfx
				@cp $(USER_MANUAL_DOCDIR)/gfx/*.png \
				    $(WDB_MANUAL_HMTLDIR)/gfx
				@$(DOCBOOK2HTML) \
					-o $(WDB_MANUAL_HMTLDIR) \
					$(DOC_SRCWORKDIR)/wdb_user-manual.xml
				@echo "* Loading Program Manual"
				@-rm -rf $(LOAD_MANUAL_HMTLDIR)
				@mkdir $(LOAD_MANUAL_HMTLDIR)
				@$(DOCBOOK2HTML) \
					-o $(LOAD_MANUAL_HMTLDIR) \
					$(DOC_SRCWORKDIR)/load_user-manual.xml
				@echo "* WCI User Manual"
				@-rm -rf $(WCI_MANUAL_HMTLDIR)
				@mkdir $(WCI_MANUAL_HMTLDIR)
				@$(DOCBOOK2HTML) \
					-o $(WCI_MANUAL_HMTLDIR) \
					$(DOC_SRCWORKDIR)/wci_user-manual.xml
				@$(CLEAN_DOCFILES)
				@echo "HTML documentation... done"


# PDF Documentation
#-----------------------------------------------------------------------------

pdf-local:
				@echo "Creating PDF documentation..."
				@$(COPY_DOCFILES)
				@$(ENTITY_COMPILE)
				@mkdir -p @builddir@/doc/pdf
				@mkdir -p @buildir@/doc/pdf/gfx
				@echo "* WDB Developer's Manual"
				@cp $(DEVELOPER_MANUAL_DOCDIR)/gfx/*.png \
				    $(DEVELOPER_MANUAL_PDFDIR)/gfx
				@$(DOCBOOK2PDF) \
					-o $(DEVELOPER_MANUAL_PDFDIR) \
					$(DOC_SRCWORKDIR)/wdb_developer-manual.xml
				@echo "* WDB User Manual"
				@cp $(USER_MANUAL_DOCDIR)/gfx/*.png \
				    $(WDB_MANUAL_PDFDIR)/gfx
				@$(DOCBOOK2PDF) \
					-o $(WDB_MANUAL_PDFDIR) \
					$(DOC_SRCWORKDIR)/wdb_user-manual.xml
				@echo "* Loading Program Manual"
				@$(DOCBOOK2PDF) \
					-o $(LOAD_MANUAL_PDFDIR) \
					$(DOC_SRCWORKDIR)/load_user-manual.xml
				@echo "* WCI User Manual"
				@$(DOCBOOK2PDF) \
					-o $(WCI_MANUAL_PDFDIR) \
					$(DOC_SRCWORKDIR)/wci_user-manual.xml
				@$(CLEAN_DOCFILES)
				@echo "PDF documentation... done"


# Man (Manual) Pages
#-----------------------------------------------------------------------------

man1_MANS = 				doc/user/wdbConfiguration.1 \
							doc/user/wdb.1 \
							doc/user/gribLoad.1 \
							doc/user/feltLoad.1

man5_MANS = 				doc/user/wdb.conf.5


doc/user/wdb.1: 			doc/user/wdb.man.xml
							$(DOCBOOK2MAN) -o doc/user/ $<
	
doc/user/wdbConfiguration.1: doc/user/wdbConfiguration.man.xml
							$(DOCBOOK2MAN) -o doc/user/ $<

doc/user/gribLoad.1: 		doc/user/gribLoad.man.xml
							$(DOCBOOK2MAN) -o doc/user/ $<
	
doc/user/feltLoad.1: 		doc/user/feltLoad.man.xml
							$(DOCBOOK2MAN) -o doc/user/ $<
	
doc/user/wdb.conf.5: doc/user/wdb.conf.man.xml
							$(DOCBOOK2MAN) -o doc/user/ $<


# Cleaning
#-----------------------------------------------------------------------------

DOCCLEAN_HOOK =				rm -rf @builddir@/doc/html; \
							rm -rf @builddir@/doc/pdf

CLEANFILES +=				doc/Makefile $(man1_MANS) $(man5_MANS)

EXTRA_DIST += 				$(man1_MANS:.1=.man.xml) \
							$(man5_MANS:.5=.man.xml) \
							$(DOCUMENTATION_SRC) \
							$(DOCUMENTATION_AUX) \
							doc/wdb.mk \
							doc/Makefile.am \
							doc/Makefile.in



# Local Makefile Targets
#-----------------------------------------------------------------------------

doc/all: pdf

doc/clean: clean

endif