
#-----------------------------------------------------------------------------
# WDB Cleaner
#-----------------------------------------------------------------------------

CLEANER_SOURCES =		src/cleaningProgram/cleanDb.in.sql

sql_DATA += $(CLEANER_SOURCES:.in.sql=.sql)

CLEANFILES +=			$(CLEANER_SOURCES:.in.sql=.sql)

EXTRA_DIST +=			$(CLEANER_SOURCES) \
						src/cleaningProgram/wdb.mk \
						src/cleaningProgram/Makefile.am \
						src/cleaningProgram/Makefile.in

DISTCLEANFILES +=		src/cleaningProgram/Makefile


pkglib_SCRIPTS += src/cleaningProgram/clean_wdb.sh

src/cleaningProgram/clean_wdb.sh: src/cleaningProgram/clean_wdb.in.sh
	cp $< $@

EXTRA_DIST += src/cleaningProgram/clean_wdb.in.sh
CLEANFILES += src/cleaningProgram/clean_wdb.sh

EXTRA_DIST +=			src/cleaningProgram/wdb.mk \
						src/cleaningProgram/Makefile.am \
						src/cleaningProgram/Makefile.in

DISTCLEANFILES +=		src/cleaningProgram/Makefile

# Local Makefile Targets
#-----------------------------------------------------------------------------

src/cleaningProgram/all: $(CLEANER_SOURCES:.in.sql=.sql)

src/cleaningProgram/clean: clean
