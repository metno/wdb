#!/bin/sh

PSQL="psql `__WDB_BINDIR__/wdbConfiguration --psqlArgs`"

case "$1" in
	install)
		$PSQL < __WDB_DATADIR__/sql/cleaning/remove_old_referencetimes.sql >> /dev/null 2>&1 
		$PSQL < __WDB_DATADIR__/sql/cleaning/default_cleaning_setup.sql >> /dev/null
	;;
	uninstall)
		$PSQL -c "DELETE FROM clean.strategies WHERE function = 'clean_referencetimes'"  >> /dev/null
	;;
	purge)
		$PSQL -c "DELETE FROM clean.strategies WHERE function = 'clean_referencetimes'; DROP TABLE clean.referencetime_lifetime; DROP FUNCTION clean.clean_referencetimes()"  >> /dev/null
	;;
	--help)
		echo "Installs reference time based cleaning into the wdb database."
		echo
		echo "Usage: $0 install|uninstall|purge"
		echo
		echo "Uninstall will only cause the function to not run during cleaning. Purge will"
		echo "also delete any configuration changes made to the reference time cleaner."  
	;;
	--version)
		echo "$0 (wdb) __WDB_VERSION__"	
	;;
	*)
		echo Invalid option: $1. Only install, uninstall and purge is allowed.
		exit 1
	;;
esac
 