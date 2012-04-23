
#-----------------------------------------------------------------------------
# WDB Cleaner
#-----------------------------------------------------------------------------

wdb_la_SOURCES += 		src/cleaningProgram/cleanDb.c

CLEANER_SOURCES =		src/cleaningProgram/cleanDb.in.sql

sql_DATA += 			$(CLEANER_SOURCES:.in.sql=.sql)

CLEANFILES +=			$(CLEANER_SOURCES:.in.sql=.sql)

cleansqldir = 			$(sqldir)/cleaning

cleansql_DATA =

EXTRA_DIST +=			$(CLEANER_SOURCES) \
						$(cleansql_DATA) \
						src/cleaningProgram/wdb.mk \
						src/cleaningProgram/Makefile.am \
						src/cleaningProgram/Makefile.in

DISTCLEANFILES +=		src/cleaningProgram/Makefile


pkglibexec_SCRIPTS += 	src/cleaningProgram/clean_wdb.sh

src/cleaningProgram/clean_wdb.sh: src/cleaningProgram/clean_wdb.in.sh
						$(SED_SUBSTITUTION)

CLEANFILES += 			src/cleaningProgram/clean_wdb.sh

EXTRA_DIST +=			src/cleaningProgram/clean_wdb.in.sh \
						src/cleaningProgram/wdb.mk \
						src/cleaningProgram/Makefile.am \
						src/cleaningProgram/Makefile.in

DISTCLEANFILES +=		src/cleaningProgram/Makefile

# Local Makefile Targets
#-----------------------------------------------------------------------------

src/cleaningProgram/all: $(CLEANER_SOURCES:.in.sql=.sql)

src/cleaningProgram/clean: clean

include src/cleaningProgram/remove_old_referencetimes/wdb.mk
include src/cleaningProgram/remove_low_dataversions/wdb.mk