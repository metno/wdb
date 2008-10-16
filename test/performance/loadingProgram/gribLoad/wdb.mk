
#-----------------------------------------------------------------------------
# WDB Call Interface Performance Tests
#-----------------------------------------------------------------------------

GRIBLOADPERFTEST_SOURCES =	test/performance/loadingProgram/gribLoad/caseP001_01.in.test \
							test/performance/loadingProgram/gribLoad/caseP002_01.in.test \
							test/performance/loadingProgram/gribLoad/caseP003_01.in.test

GRIBLOADPERFTEST_SUPPORT =	test/performance/loadingProgram/gribLoad/buildUp.in.sh \
							test/performance/loadingProgram/gribLoad/tearDown.in.sh \
							test/performance/loadingProgram/gribLoad/testEnvironment.in.sh \
							test/performance/loadingProgram/gribLoad/gribLoad_time.in.sh


PERFORMANCETESTS +=		gribLoadPerformanceTest.sh

noinst_SCRIPTS +=		gribLoadPerformanceTest.sh \
						$(GRIBLOADPERFTEST_SOURCES:.in.test=.test) \
						$(GRIBLOADPERFTEST_SUPPORT:.in.sh=.sh)

CLEANFILES +=			gribLoadPerformanceTest.sh \
						$(GRIBLOADPERFTEST_SOURCES:.in.test=.test) \
						$(GRIBLOADPERFTEST_SUPPORT:.in.sh=.sh)

EXTRA_DIST +=			test/performance/loadingProgram/gribLoad/gribLoadPerformanceTest.in.sh \
						$(GRIBLOADPERFTEST_SOURCES) \
						$(GRIBLOADPERFTEST_SUPPORT) \
						test/performance/loadingProgram/gribLoad/wdb.mk \
						test/performance/loadingProgram/gribLoad/Makefile.am \
						test/performance/loadingProgram/gribLoad/Makefile.in

DISTCLEANFILES +=		test/performance/loadingProgram/gribLoad/Makefile

gribLoadPerformanceTest.sh:	test/performance/loadingProgram/gribLoad/gribLoadPerformanceTest.in.sh
							$(SED_SUBSTITUTION);\
							chmod 754 $@


# Local Makefile Targets
#-----------------------------------------------------------------------------

test/performance/loadingProgram/gribLoad/all: \
				$(GRIBLOADPERFTEST_SOURCES:.in.test=.test) \
				$(GRIBLOADPERFTEST_SUPPORT:.in.sh=.sh) \
				gribLoadPerformanceTest.sh

test/performance/loadingProgram/gribLoad/clean: clean
