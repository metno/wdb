
#-----------------------------------------------------------------------------
# Install Scripts
#-----------------------------------------------------------------------------

INSTALL_SOURCES =		install/install_database.in.sh \
						install/uninstall_database.in.sh \
						install/upgrade_database.in.sh
						
INSTALL_SQL 	= 		install/upgrade_database.in.sql \
						install/upgrade_test.in.sql	\
						install/upgrade_metadata.in.sql					

EXTRA_DIST += install/move_db.sh

pkglib_SCRIPTS += 		$(INSTALL_SOURCES:.in.sh=)

nodist_pkglib_SCRIPTS = install/move_db

install/move_db:	install/move_db.sh
	 cp $< $@

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
						$(INSTALL_SQL:.in.sql=.sql) \
						install/move_db

DISTCLEANFILES +=		install/Makefile


# Local Makefile Targets
#-----------------------------------------------------------------------------

install/all: $(INSTALL_SOURCES:.in.sh=) $(INSTALL_SQL:.in.sql=.sql)

install/clean: clean
