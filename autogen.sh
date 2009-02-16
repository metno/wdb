#!/bin/sh
#
# autogen.sh is utilized for the eclipse autotools plugin and in the
# continuous build bots
#
# Parse command line
FLAG=""
while test -n "$1"; do
	case $1 in
    --help) 
    	echo "Usage: $0 [OPTION]

autogen.sh is utilized for the eclipse autotools plugin and in the
continous build bots.

Options:
--help             display this help and exit
--force            use --force in the local autoreconf (replaces files)
"; exit 0;;
    *)
    	FLAG="$FLAG $1"
    	shift
    	continue;; 
	esac
done

autoreconf -i $FLAG

