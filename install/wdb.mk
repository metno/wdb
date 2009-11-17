
#-----------------------------------------------------------------------------
# Install Scripts
#-----------------------------------------------------------------------------

INSTALL_SOURCES =		install/install_database.in.sh \
						install/uninstall_database.in.sh \
						install/upgrade_database.in.sh
						
						
INSTALL_SQL 	= 		install/upgrade_database.in.sql \
						install/upgrade_test.in.sql	\
						install/upgrade_metadata.in.sql					

NOINSTALL_SOURCES = 	install/move_database.in.sh

nodist_pkglib_SCRIPTS =	$(INSTALL_SOURCES:.in.sh=)

nodist_pkglib_SCRIPTS += $(NOINSTALL_SOURCES:.in.sh=)

sql_DATA += 			$(INSTALL_SQL:.in.sql=.sql)

EXTRA_DIST +=			$(INSTALL_SOURCES) \
						$(NOINSTALL_SOURCES) \
						$(INSTALL_SQL) \
						install/wdb.mk \
						install/Makefile.am \
						install/Makefile.in

CLEANFILES += 			$(INSTALL_SOURCES:.in.sh=) \
						$(NOINSTALL_SOURCES:.in.sh=) \
						$(INSTALL_SQL:.in.sql=.sql) 

DISTCLEANFILES +=		install/Makefile


# Local Makefile Targets
#-----------------------------------------------------------------------------

install/all: $(INSTALL_SOURCES:.in.sh=) $(NOINSTALL_SOURCES:.in.sh=) $(INSTALL_SQL:.in.sql=.sql)

install/clean: clean
