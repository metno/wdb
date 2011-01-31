
#-----------------------------------------------------------------------------
# Install Scripts
#-----------------------------------------------------------------------------

INSTALL_SOURCES =		install/install_database.in.sh \
						install/upgrade_database.in.sh \
						install/uninstall_database.in.sh
					
NOINSTALL_SOURCES = 	install/move_database.in.sh

nodist_pkglib_SCRIPTS =	$(INSTALL_SOURCES:.in.sh=)

nodist_pkglib_SCRIPTS += $(NOINSTALL_SOURCES:.in.sh=)

EXTRA_DIST +=			$(INSTALL_SOURCES) \
						$(NOINSTALL_SOURCES) \
						install/Makefile.am \
						install/Makefile.in

CLEANFILES += 			$(INSTALL_SOURCES:.in.sh=) \
						$(NOINSTALL_SOURCES:.in.sh=)

DISTCLEANFILES +=		install/Makefile


# Local Makefile Targets
#-----------------------------------------------------------------------------

install/all: $(INSTALL_SOURCES:.in.sh=) $(NOINSTALL_SOURCES:.in.sh=) $(INSTALL_SQL:.in.sql=.sql)

install/clean: clean
