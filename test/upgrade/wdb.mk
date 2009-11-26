
#-----------------------------------------------------------------------------
# Install Scripts
#-----------------------------------------------------------------------------

UPGRADE_NOINSTALL_SOURCES =		test/upgrade/upgrade_test.in.sh
									
#INSTALL_SQL 	= 		install/upgrade_database.in.sql

#NOINSTALL_SOURCES = 	install/move_database.in.sh

#pkglib_SCRIPTS += 		$(INSTALL_SOURCES:.in.sh=)

nodist_pkglib_SCRIPTS += $(UPGRADE_NOINSTALL_SOURCES:.in.sh=.sh)

#sql_DATA += 			$(INSTALL_SQL:.in.sql=.sql)
	
upgrade_test:			test/upgrade/upgrade_test.in.sh
						$(SH_COMPILE)

EXTRA_DIST +=			$(UPGRADE_NOINSTALL_SOURCES) \
						test/upgrade/wdb.mk \
						test/upgrade/Makefile.am \
						test/upgrade/Makefile.in

CLEANFILES += 			$(UPGRADE_NOINSTALL_SOURCES:.in.sh=.sh) 

DISTCLEANFILES +=		test/upgrade/Makefile


# Local Makefile Targets
#-----------------------------------------------------------------------------

install/all: $(UPGRADE_NOINSTALL_SOURCES:.in.sh=.sh)

install/clean: clean
