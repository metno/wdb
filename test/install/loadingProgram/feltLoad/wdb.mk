
#-----------------------------------------------------------------------------
# WDB FeltLoad Install Tests
#-----------------------------------------------------------------------------

FELTLOADTEST_SOURCES =	test/install/loadingProgram/feltLoad/caseF001_01.in.test \
						test/install/loadingProgram/feltLoad/caseF001_02.in.test \
						test/install/loadingProgram/feltLoad/caseF001_03.in.test \
						test/install/loadingProgram/feltLoad/caseF002_02.in.test \
						test/install/loadingProgram/feltLoad/caseF003_01.in.test \
						test/install/loadingProgram/feltLoad/caseF004_01.in.test \
						test/install/loadingProgram/feltLoad/caseF004_03.in.test \
						test/install/loadingProgram/feltLoad/caseF004_04.in.test \
						test/install/loadingProgram/feltLoad/caseF005_01.in.test \
						test/install/loadingProgram/feltLoad/caseF005_02.in.test \
						test/install/loadingProgram/feltLoad/caseF006_01.in.test \
						test/install/loadingProgram/feltLoad/caseF006_02.in.test \
						test/install/loadingProgram/feltLoad/caseF006_03.in.test \
						test/install/loadingProgram/feltLoad/caseF007_01.in.test
												
FELTLOADTEST_SUPPORT =	test/install/loadingProgram/feltLoad/tearDown.in.sh \
						test/install/loadingProgram/feltLoad/testEnvironment.in.sh

INSTALLTESTS +=			feltLoadInstallTest.sh

noinst_SCRIPTS +=		feltLoadInstallTest.sh \
						$(FELTLOADTEST_SOURCES:.in.test=.test) \
						$(FELTLOADTEST_SUPPORT:.in.sh=.sh) 

CLEANFILES +=			feltLoadInstallTest.sh \
						$(FELTLOADTEST_SOURCES:.in.test=.test) \
						$(FELTLOADTEST_SUPPORT:.in.sh=.sh) 

EXTRA_DIST +=			test/install/loadingProgram/feltLoad/feltLoadInstallTest.in.sh \
						$(FELTLOADTEST_SOURCES) \
						$(FELTLOADTEST_SUPPORT) \
						test/install/loadingProgram/feltLoad/wdb.mk \
						test/install/loadingProgram/feltLoad/Makefile.am \
						test/install/loadingProgram/feltLoad/Makefile.in


DISTCLEANFILES +=		test/install/loadingProgram/feltLoad/Makefile

feltLoadInstallTest.sh: test/install/loadingProgram/feltLoad/feltLoadInstallTest.in.sh
						$(SED_SUBSTITUTION);\
						chmod 754 $@




# Local Makefile Targets
#-----------------------------------------------------------------------------

test/install/loadingProgram/feltLoad/all: $(FELTLOADTEST_SOURCES:.in.test=.test) $(FELTLOADTEST_SUPPORT:.in.sh=.sh)

test/install/loadingProgram/feltLoad/clean: clean
