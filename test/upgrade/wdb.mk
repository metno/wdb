
#-----------------------------------------------------------------------------
# Install Scripts
#-----------------------------------------------------------------------------

NOINSTALL_SOURCES =		test/upgrade/upgrade_test.in.sh
									
#INSTALL_SQL 	= 		install/upgrade_database.in.sql

#NOINSTALL_SOURCES = 	install/move_database.in.sh

#pkglib_SCRIPTS += 		$(INSTALL_SOURCES:.in.sh=)

nodist_pkglib_SCRIPTS = $(NOINSTALL_SOURCES:.in.sh=)

#sql_DATA += 			$(INSTALL_SQL:.in.sql=.sql)
	
upgrade_test:			test/upgrade/upgrade_test.in.sh
						$(SH_COMPILE)

EXTRA_DIST +=			$(NOINSTALL_SOURCES) \
						test/upgrade/wdb.mk \
						test/upgrade/Makefile.am \
						test/upgrade/Makefile.in

CLEANFILES += 			$(NOINSTALL_SOURCES:.in.sh=) 

DISTCLEANFILES +=		test/upgrade/Makefile


# Local Makefile Targets
#-----------------------------------------------------------------------------

install/all: $(INSTALL_SOURCES:.in.sh=) $(NOINSTALL_SOURCES:.in.sh=) $(INSTALL_SQL:.in.sql=.sql)

install/clean: clean
