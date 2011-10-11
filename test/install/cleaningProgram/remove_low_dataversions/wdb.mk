INSTALLTESTS += remove_low_dataversions_test.sh

noinst_SCRIPTS += remove_low_dataversions_test.sh

remove_low_dataversions_test.sh: test/install/cleaningProgram/remove_low_dataversions/remove_low_dataversions_test.in.sh
	$(SED_SUBSTITUTION)
	chmod 754 $@

CLEANFILES += remove_low_dataversions_test.sh

EXTRA_DIST += test/install/cleaningProgram/remove_low_dataversions/test.sql \
	test/install/cleaningProgram/remove_low_dataversions/remove_low_dataversions_test.in.sh
	
	