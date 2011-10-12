cleansql_DATA += src/cleaningProgram/remove_old_referencetimes/remove_old_referencetimes.sql

pkglib_SCRIPTS += src/cleaningProgram/remove_old_referencetimes/install_remove_old_referencetimes.sh

src/cleaningProgram/remove_old_referencetimes/install_remove_old_referencetimes.sh: src/cleaningProgram/remove_old_referencetimes/install_remove_old_referencetimes.in.sh
	mkdir -p `dirname $@`
	$(SED_SUBSTITUTION)
	chmod 774 $@

EXTRA_DIST += src/cleaningProgram/remove_old_referencetimes/install_remove_old_referencetimes.in.sh
CLEANFILES += src/cleaningProgram/remove_old_referencetimes/install_remove_old_referencetimes.sh
