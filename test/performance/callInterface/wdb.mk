
#-----------------------------------------------------------------------------
# WDB Call Interface Performance Tests
#-----------------------------------------------------------------------------

noinst_PROGRAMS +=		wciPerformanceTester

wciPerformanceTester_SOURCES =\
						test/performance/callInterface/performanceTest.cpp \
						test/performance/callInterface/performanceTestConfiguration.h \
						test/performance/callInterface/performanceTestConfiguration.cpp \
						test/performance/callInterface/wciRowStructures.h \
						test/performance/callInterface/transactors/wciTransactors.h \
						test/performance/callInterface/transactors/getRandomPoint.h \
						test/performance/callInterface/transactors/getComplexPoint.h \
						test/performance/callInterface/transactors/getPolygon.h \
						test/performance/callInterface/transactors/getField.h \
						test/performance/callInterface/transactors/getBilinearPoint.h

wciPerformanceTester_CPPFLAGS = 	$(AM_CPPFLAGS) -I$(srcdir)/test/performance/callInterface

wciPerformanceTester_LDFLAGS =	$(AM_LDFLAGS)	-lwdbConfig \
						-lwdbLog

WCIPERFTEST_SOURCES =	test/performance/callInterface/caseP001_01.in.test \
						test/performance/callInterface/caseP001_02.in.test \
						test/performance/callInterface/caseP001_03.in.test \
						test/performance/callInterface/caseP001_08.in.test \
						test/performance/callInterface/caseP001_11.in.test \
						test/performance/callInterface/caseP001_12.in.test \
						test/performance/callInterface/caseP001_13.in.test \
						test/performance/callInterface/caseP002_01.in.test \
						test/performance/callInterface/caseP002_02.in.test \
						test/performance/callInterface/caseP003_01.in.test \
						test/performance/callInterface/caseP003_02.in.test \
						test/performance/callInterface/caseP003_03.in.test \
						test/performance/callInterface/caseP003_04.in.test \
						test/performance/callInterface/caseP004_01.in.test \
						test/performance/callInterface/caseP004_02.in.test \
						test/performance/callInterface/caseP004_03.in.test

WCIPERFTEST_SUPPORT =	test/performance/callInterface/buildUp.in.sh \
						test/performance/callInterface/tearDown.in.sh \
						test/performance/callInterface/testEnvironment.in.sh \
						test/performance/callInterface/wciRead_time.in.sh


PERFORMANCETESTS +=		wciPerformanceTest.sh

noinst_SCRIPTS +=		wciPerformanceTest.sh \
						$(WCIPERFTEST_SOURCES:.in.test=.test) \
						$(WCIPERFTEST_SUPPORT:.in.sh=.sh)

CLEANFILES +=			wciPerformanceTest.sh \
						$(WCIPERFTEST_SOURCES:.in.test=.test) \
						$(WCIPERFTEST_SUPPORT:.in.sh=.sh)
				
EXTRA_DIST +=			test/performance/callInterface/wciPerformanceTest.in.sh \
						$(WCIPERFTEST_SOURCES) \
						$(WCIPERFTEST_SUPPORT) \
						test/performance/callInterface/wdb.mk \
						test/performance/callInterface/Makefile.am \
						test/performance/callInterface/Makefile.in

DISTCLEANFILES +=		test/performance/callInterface/Makefile

wciPerformanceTest.sh:		test/performance/callInterface/wciPerformanceTest.in.sh
				sed s%__WDB_VERSION__%$(VERSION)% $<\
				| sed s%__WDB_LIB_PATH__%$(LD_LIBRARY_PATH)% \
	 			| sed s%__WDB_BINDIR__%$(bindir)% \
				| sed s%__WDB_BUILDDIR__%"@abs_builddir@"% \
				| sed s%__WDB_SRCDIR__%$(srcdir)% \
	 			| sed s%__WDB_SYSCONFDIR__%$(sysconfdir)% \
	 			| sed s%__WDB_LOCALSTATEDIR__%$(localstatedir)% \
	 			> $@;\
				chmod 754 $@



# Local Makefile Targets
#-----------------------------------------------------------------------------

test/performance/callInterface/all: $(WCIPERFTEST_SOURCES:.in.test=.test) \
				    $(WCIPERFTEST_SUPPORT:.in.sh=.sh) \
				    $(WCIPERFTEST_SQL:.in.sql=.sql) \
				    wciPerformanceTest.sh

test/performance/callInterface/clean: clean
