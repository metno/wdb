
#-----------------------------------------------------------------------------
# WDB CleanDB Install Tests
#-----------------------------------------------------------------------------

CLEANDBTEST_SOURCES =	test/install/cleaningProgram/remove_old_referencetimes/caseF001.in.test \
						test/install/cleaningProgram/remove_old_referencetimes/caseF003.in.test \
						test/install/cleaningProgram/remove_old_referencetimes/caseF004.in.test \
						test/install/cleaningProgram/remove_old_referencetimes/caseF005.in.test \
						test/install/cleaningProgram/remove_old_referencetimes/caseF006.in.test 

# Useless test removed:
# test/install/cleaningProgram/remove_old_referencetimes/caseF002.in.test


CLEANDBTEST_SUPPORT =	test/install/cleaningProgram/remove_old_referencetimes/tearDown.in.sh \
						test/install/cleaningProgram/remove_old_referencetimes/testEnvironment.in.sh


INSTALLTESTS +=			cleanDbInstallTest.sh

noinst_SCRIPTS +=		cleanDbInstallTest.sh \
						$(CLEANDBTEST_SOURCES:.in.test=.test) \
						$(CLEANDBTEST_SUPPORT:.in.sh=.sh)

CLEANFILES +=			cleanDbInstallTest.sh \
						$(CLEANDBTEST_SOURCES:.in.test=.test) \
						$(CLEANDBTEST_SUPPORT:.in.sh=.sh)

EXTRA_DIST +=			test/install/cleaningProgram/remove_old_referencetimes/cleanDbInstallTest.in.sh \
						$(CLEANDBTEST_SOURCES) \
						$(CLEANDBTEST_SUPPORT)

cleanDbInstallTest.sh:	test/install/cleaningProgram/remove_old_referencetimes/cleanDbInstallTest.in.sh test/install/cleaningProgram/remove_old_referencetimes/
						mkdir -p test/install/cleaningProgram/remove_old_referencetimes/
						$(SED_SUBSTITUTION);\
						chmod 754 $@
