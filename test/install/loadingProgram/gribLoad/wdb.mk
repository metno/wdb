
#-----------------------------------------------------------------------------
# WDB GribLoad Install Tests
#-----------------------------------------------------------------------------

GRIBLOADTEST_SOURCES =		test/install/loadingProgram/gribLoad/caseF001_01.in.test \
				test/install/loadingProgram/gribLoad/caseF001_02.in.test \
				test/install/loadingProgram/gribLoad/caseF001_03.in.test \
				test/install/loadingProgram/gribLoad/caseF002_02.in.test \
				test/install/loadingProgram/gribLoad/caseF003_01.in.test \
				test/install/loadingProgram/gribLoad/caseF004_01.in.test \
				test/install/loadingProgram/gribLoad/caseF004_03.in.test \
				test/install/loadingProgram/gribLoad/caseF004_04.in.test \
				test/install/loadingProgram/gribLoad/caseF005_01.in.test \
				test/install/loadingProgram/gribLoad/caseF005_02.in.test \
				test/install/loadingProgram/gribLoad/caseF006_01.in.test \
				test/install/loadingProgram/gribLoad/caseF006_02.in.test \
				test/install/loadingProgram/gribLoad/caseF006_03.in.test \
				test/install/loadingProgram/gribLoad/caseF007_01.in.test 

GRIBLOADTEST_SUPPORT =		test/install/loadingProgram/gribLoad/tearDown.in.sh \
				test/install/loadingProgram/gribLoad/testEnvironment.in.sh

INSTALLTESTS +=			gribLoadInstallTest.sh

noinst_SCRIPTS +=		gribLoadInstallTest.sh \
				$(GRIBLOADTEST_SOURCES:.in.test=.test) \
				$(GRIBLOADTEST_SUPPORT:.in.sh=.sh) 

CLEANFILES +=			gribLoadInstallTest.sh \
				$(GRIBLOADTEST_SOURCES:.in.test=.test) \
				$(GRIBLOADTEST_SUPPORT:.in.sh=.sh) 

EXTRA_DIST +=			test/install/loadingProgram/gribLoad/gribLoadInstallTest.in.sh \
				$(GRIBLOADTEST_SOURCES) \
				$(GRIBLOADTEST_SUPPORT) \
				test/install/loadingProgram/gribLoad/caseF003_01.grib \
				test/install/loadingProgram/gribLoad/wdb.mk \
				test/install/loadingProgram/gribLoad/Makefile.am \
				test/install/loadingProgram/gribLoad/Makefile.in


DISTCLEANFILES +=		test/install/loadingProgram/gribLoad/Makefile

gribLoadInstallTest.sh:		test/install/loadingProgram/gribLoad/gribLoadInstallTest.in.sh
				$(SED_SUBSTITUTION);\
				chmod 754 $@




# Local Makefile Targets
#-----------------------------------------------------------------------------

test/install/loadingProgram/gribLoad/all: $(GRIBLOADTEST_SOURCES:.in.test=.test) $(GRIBLOADTEST_SUPPORT:.in.sh=.sh)

test/install/loadingProgram/gribLoad/clean: clean
