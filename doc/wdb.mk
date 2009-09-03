.xml.html:
	$(DOCBOOK) html-nochunks $< -o `dirname $@`

DOCUMENTATION_SRC = \
	doc/user/wci_user-manual.xml \
	doc/user/wdb_user-manual.xml \
	doc/developer/feltload_system-design-specification.xml \
	doc/developer/feltload_system-test-specification.xml \
	doc/developer/gribload_system-design-specification.xml \
	doc/developer/gribload_system-test-specification.xml \
	doc/developer/kvalobsLoad_system-design-specification.xml \
	doc/developer/loaderDaemon_system-design-specification.xml \
	doc/developer/wci_system-design-specification.xml \
	doc/developer/wci_system-test-specification.xml \
	doc/developer/wdb_developer-manual.xml \
	doc/developer/wdb_software-architecture.xml \
	doc/developer/wdb_test-plan.xml \
	doc/developer/wdb_vision.xml

USED_DOCS = \
	doc/user/feltLoad.man.xml \
	doc/user/gribLoad.man.xml \
	doc/user/load_user-manual.xml \
	doc/user/wdbConfiguration.man.xml \
	doc/user/wdb.conf.man.xml \
	doc/user/wdb.man.xml \
	doc/user/gfx/wdb_users-architecture-overview.dia \
	doc/user/gfx/wdb_users-architecture-overview.png \
	doc/user/gfx/wdb_value-dimensions.dia \
	doc/docbook/wdb_entities.ent \
	doc/docbook/xinclude.mod
	
EXTRA_DIST += $(DOCUMENTATION_SRC) $(USED_DOCS)

DOCUMENTATION_OUTPUT = $(DOCUMENTATION_SRC:.xml=.html)
	
documentation:	$(DOCUMENTATION_OUTPUT)

html_DATA = $(DOCUMENTATION_OUTPUT)

CLEANFILES += $(DOCUMENTATION_OUTPUT)
