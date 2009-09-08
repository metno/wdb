#!/bin/sh

set -e

PG_VERSION=8.3
PG_CLUSTER=main
PG_CTL="pg_ctlcluster $PG_VERSION $PG_CLUSTER"
#PG_CTL="pg_ctl"

error() {
	echo ERROR: $@
	exit 1
}

move_and_link() {
	
	S_SOURCE_FULL=$1
	C_SOURCE_FULL=`readlink -m $S_SOURCE_FULL`
	DESTINATION_DIRECTORY=`readlink -m $2`
	DESTINATION_FULL=$DESTINATION_DIRECTORY/`basename $C_SOURCE_FULL`
	
	# Silently ignore attempts to move a database to its own location
	if [ $C_SOURCE_FULL = $DESTINATION_FULL ]; then
		return
	fi
	
	# Remove destination location, in case it existed as a symlink
	if [ -L $DESTINATION_FULL ]; then
		rm  $DESTINATION_FULL
	fi
	
	mv $C_SOURCE_FULL $DESTINATION_DIRECTORY

	if [ $DESTINATION_FULL != $S_SOURCE_FULL ]; then
		ln -sf $DESTINATION_FULL $S_SOURCE_FULL
	fi	

	chown postgres:postgres $NEW_DATABASE_DIRECTORY
}

version() {
	echo move_db 1.0.0
}

help() {
	version
	echo
	echo "Usage: move_db DESTINATION"
	echo
	echo "Moves a debian-style postgresql cluster - version <$PG_VERSION> cluster name" 
	echo "<$PG_CLUSTER> - to the given destination."
	echo
	echo "OPTIONS:"
	echo 
	echo "--pg_xlog    In addition to moving the database, move the WAL directory to the" 
	echo "             given location"
	echo
	echo "--help       Show this help message"
	echo "--version    Show version information, and then exit"
}

while [ x`echo $1 | cut -b-2` = "x--" ]; do

	case "$1" in
	--pg_xlog=*)
		PG_XLOG=`echo $1 | sed s/--pg_xlog=// | sed s,/$,,` 
		;;
	--help)
		help
		exit 0
		;;
	--version)
		version
		exit 0
		;;
	*)
		error Unknown argument: $1
		;;
	esac
	shift
done

NEW_DATABASE_DIRECTORY=$1


# Usage check
if [ x = x$NEW_DATABASE_DIRECTORY ]; then
	echo Missing argument
	echo
	help
	exit 1
fi

# Error check: don't overwrite existing files
if [ -f $NEW_DATABASE_DIRECTORY ]; then
	error $NEW_DATABASE_DIRECTORY already existed, and is a file!
fi

# Error check: Parent directory must exist
NEW_DATABASE_PARENT=`readlink -m $NEW_DATABASE_DIRECTORY/..`
if [ ! -d $NEW_DATABASE_PARENT ]; then
	echo $NEW_DATABASE_PARENT does not exist.
	echo Please create it before attempting to move the database
	exit 1
fi


PSQL_DATA_DIRECTORY=`su postgres -c "psql -tc 'show data_directory'"`
#PSQL_DATA_DIRECTORY=`psql postgres -tc 'show data_directory'`

# Create target directory
if [ ! -e $NEW_DATABASE_DIRECTORY ]; then
	mkdir $NEW_DATABASE_DIRECTORY
fi

# Stop database
$PG_CTL stop || error unable to stop database

# Move database directory
move_and_link $PSQL_DATA_DIRECTORY $NEW_DATABASE_DIRECTORY

# Move WAL directory, if the correct option is given.
if [ x != x$PG_XLOG ]; then
	move_and_link $PSQL_DATA_DIRECTORY/pg_xlog $PG_XLOG
fi

# restart database
$PG_CTL start -l /dev/null || error unable to restart database

echo Moved database successfully
