#include test/install/cleaningProgram/remove_old_referencetimes/wdb.mk
#include test/install/cleaningProgram/remove_low_dataversions/wdb.mk

EXTRA_DIST +=	test/install/cleaningProgram/Makefile.am \
				test/install/cleaningProgram/Makefile.in


DISTCLEANFILES +=		test/install/cleaningProgram/Makefile



# Local Makefile Targets
#-----------------------------------------------------------------------------


test/install/cleaningProgram/all: $(CLEANDBTEST_SOURCES:.in.test=.test) $(CLEANDBTEST_SUPPORT:.in.sh=.sh)

test/install/cleaningProgram/clean: clean
