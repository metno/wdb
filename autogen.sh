#!/bin/sh
FLAG=""
while test -n "$1"; do
	case $1 in
    --help) 
    	echo "Usage: $0 [OPTIONS]

autogen.sh is utilized for the eclipse autotools plugin and in the
continous build bots.

Options:
--help             display this help and exit

Other options are passed directly to the autoreconf call.
"; exit 0;;
    *)
    	FLAG="$FLAG $1"
    	shift
    	continue;; 
	esac
done

autoreconf -i $FLAG

if test -f "${PWD}/configure"; then
    echo "======================================"
    echo "You are ready to run './configure'"
    echo "======================================"
else
    echo "  Failed to generate ./configure script!"
fi

