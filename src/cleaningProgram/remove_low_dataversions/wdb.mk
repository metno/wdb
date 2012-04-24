cleansql_DATA += src/cleaningProgram/remove_low_dataversions/remove_low_dataversions.sql

pkglib_SCRIPTS += src/cleaningProgram/remove_low_dataversions/install_remove_low_dataversions.sh

src/cleaningProgram/remove_low_dataversions/install_remove_low_dataversions.sh: src/cleaningProgram/remove_low_dataversions/install_remove_low_dataversions.in.sh
	mkdir -p `dirname $@`
	$(SED_SUBSTITUTION)
	chmod 774 $@

EXTRA_DIST += src/cleaningProgram/remove_low_dataversions/install_remove_low_dataversions.in.sh
CLEANFILES += src/cleaningProgram/remove_low_dataversions/install_remove_low_dataversions.sh
