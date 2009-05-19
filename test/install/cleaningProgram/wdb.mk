
#-----------------------------------------------------------------------------
# WDB CleanDB Install Tests
#-----------------------------------------------------------------------------

CLEANDBTEST_SOURCES =	test/install/cleaningProgram/caseF001.in.test \
						test/install/cleaningProgram/caseF002.in.test \
						test/install/cleaningProgram/caseF003.in.test \
						test/install/cleaningProgram/caseF004.in.test \
						test/install/cleaningProgram/caseF005.in.test \
						test/install/cleaningProgram/caseF006.in.test 

CLEANDBTEST_SUPPORT =	test/install/cleaningProgram/tearDown.in.sh \
						test/install/cleaningProgram/testEnvironment.in.sh


INSTALLTESTS +=			cleanDbInstallTest.sh

noinst_SCRIPTS +=		cleanDbInstallTest.sh \
						$(CLEANDBTEST_SOURCES:.in.test=.test) \
						$(CLEANDBTEST_SUPPORT:.in.sh=.sh)

CLEANFILES +=			cleanDbInstallTest.sh \
						$(CLEANDBTEST_SOURCES:.in.test=.test) \
						$(CLEANDBTEST_SUPPORT:.in.sh=.sh)

EXTRA_DIST +=			test/install/cleaningProgram/cleanDbInstallTest.in.sh \
						$(CLEANDBTEST_SOURCES) \
						$(CLEANDBTEST_SUPPORT) \
						test/install/cleaningProgram/wdb.mk \
						test/install/cleaningProgram/Makefile.am \
						test/install/cleaningProgram/Makefile.in

DISTCLEANFILES +=		test/install/cleaningProgram/Makefile

cleanDbInstallTest.sh:	test/install/cleaningProgram/cleanDbInstallTest.in.sh
						$(SED_SUBSTITUTION);\
						chmod 754 $@



# Local Makefile Targets
#-----------------------------------------------------------------------------

test/install/cleaningProgram/all: $(CLEANDBTEST_SOURCES:.in.test=.test) $(CLEANDBTEST_SUPPORT:.in.sh=.sh)

test/install/cleaningProgram/clean: clean
