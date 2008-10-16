
#-----------------------------------------------------------------------------
# Install Scripts
#-----------------------------------------------------------------------------

INSTALL_SOURCES =		install/install_database.in.sh \
						install/uninstall_database.in.sh \
						install/upgrade_database.in.sh
						
INSTALL_SQL 	= 		install/upgrade_database.in.sql						

pkglib_SCRIPTS += 		$(INSTALL_SOURCES:.in.sh=)

sql_DATA += 			$(INSTALL_SQL:.in.sql=.sql)

install_database:		install/install_database.in.sh
						$(SH_COMPILE)

uninstall_database:		install/uninstall_database.in.sh
						$(SH_COMPILE)
	
upgrade_database:		install/upgrade_database.in.sh
						$(SH_COMPILE)


EXTRA_DIST +=			$(INSTALL_SOURCES) \
						$(INSTALL_SQL) \
						install/wdb.mk \
						install/Makefile.am \
						install/Makefile.in

CLEANFILES += 			$(INSTALL_SOURCES:.in.sh=) \
						$(INSTALL_SQL:.in.sql=.sql)

DISTCLEANFILES +=		install/Makefile


# Local Makefile Targets
#-----------------------------------------------------------------------------

install/all: $(INSTALL_SOURCES:.in.sh=) $(INSTALL_SQL:.in.sql=.sql)

install/clean: clean
