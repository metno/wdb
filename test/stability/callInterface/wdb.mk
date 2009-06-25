
#-----------------------------------------------------------------------------
# WDB Call Interface Performance Tests
#-----------------------------------------------------------------------------

noinst_PROGRAMS +=				wciStabilityTester

wciStabilityTester_SOURCES =	test/stability/callInterface/stabilityTest.cpp \
								test/stability/callInterface/StabilityTestConfiguration.h \
								test/stability/callInterface/StabilityTestConfiguration.cpp \
								test/stability/callInterface/wciRowStructures.h \
								test/stability/callInterface/transactors/wciTransactors.h \
								test/stability/callInterface/transactors/RandomPointTest.h \
								test/stability/callInterface/transactors/RandomPolygonTest.h \
								test/stability/callInterface/transactors/RandomFieldTest.h 

wciStabilityTester_CPPFLAGS = 	$(AM_CPPFLAGS) -I$(srcdir)/test/stability/callInterface

wciStabilityTester_LDFLAGS =	$(AM_LDFLAGS) \
								-lwdbConfig \
								-lwdbLog


WCISTABTEST_SUPPORT =			test/stability/callInterface/testEnvironment.in.sh
#test/stability/callInterface/buildUp.in.sh \
#test/stability/callInterface/tearDown.in.sh 

STABILITYTESTS +=				wciStabilityTest.sh

noinst_SCRIPTS +=				wciStabilityTest.sh \
								$(WCISTABTEST_SUPPORT:.in.sh=.sh)

CLEANFILES +=					wciStabilityTest.sh \
								$(WCISTABTEST_SUPPORT:.in.sh=.sh)

EXTRA_DIST +=					test/stability/callInterface/wciStabilityTest.in.sh \
								$(WCISTABTEST_SUPPORT) \
								test/stability/callInterface/wdb.mk \
								test/stability/callInterface/Makefile.am \
								test/stability/callInterface/Makefile.in

DISTCLEANFILES +=				test/stability/callInterface/Makefile

wciStabilityTest.sh:			test/stability/callInterface/wciStabilityTest.in.sh
								sed s/__WDB_VERSION__/$(VERSION)/ $<\
								| sed s¤__WDB_LIB_PATH__¤$(LD_LIBRARY_PATH)¤ \
					 			| sed s:__WDB_BINDIR__:$(bindir): \
								| sed s:__WDB_BUILDDIR__:"@abs_builddir@": \
								| sed s¤__WDB_SRCDIR__¤$(srcdir)¤ \
					 			| sed s:__WDB_SYSCONFDIR__:$(sysconfdir): \
					 			| sed s:__WDB_LOCALSTATEDIR__:$(localstatedir): \
					 			> $@;\
								chmod 754 $@



# Local Makefile Targets
#-----------------------------------------------------------------------------

test/stability/callInterface/all: $(WCIPERFTEST_SUPPORT:.in.sh=.sh) \
							      wcistabilityTest.sh

test/stability/callInterface/clean: clean
