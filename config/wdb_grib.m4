#
#   wdb - weather and water data storage
#
#   Copyright (C) 2007 met.no
#   
#   Contact information:
#   Norwegian Meteorological Institute
#   Box 43 Blindern
#   0313 OSLO
#   NORWAY
#   E-mail: wdb@met.no
# 
#   This program is free software; you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation; either version 2 of the License, or
#   (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program; if not, write to the Free Software
#   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, 
#   MA  02110-1301, USA
#

# Checks for the Presence of the GRIB API

AC_DEFUN([WDB_GRIB_CHECK],
[
# Set Minimum GRIB version
min_grib_version=ifelse([$1],[],[1.2.0],[$1])

# Set up option
AC_ARG_WITH([gribapi],
	     AS_HELP_STRING([--with-gribapi=PATH], [Specify the directory in which grib_api is installed (by default, configure uses the environment variable LDFLAGS). If set, configure will add PATH/include to CPPFLAGS and PATH/lib to LDFLAGS]),
	    [ac_grib_path="$withval"],
            [])

# Add path if given
if test "$ac_grib_path" != ""; then
    GRIB_CPPFLAGS="-I$ac_grib_path/include"
	GRIB_LDFLAGS="-L$ac_grib_path/lib"
fi

AM_CONDITIONAL(SPECIAL_GRIB_API_PATH, [ test -n $ac_grib_path ]) 
AC_SUBST(GRIB_PATH, $ac_grib_path)

# Check for the grib_info program
PATH="$ac_grib_path/bin:$prefix/bin:$prefix/usr/bin:$PATH"

AC_PATH_PROG(GRIB_CONFIG, grib_info, no, $PATH)

# Test Version >= "1.2.0"
if test "$GRIB_CONFIG" != "no" ; then
        AC_MSG_CHECKING(for grib_api version >= $min_grib_version)

        grib_version="`$GRIB_CONFIG -v`"

        # Convert versions to an integer e.g. 1.1.7 becomes 117
        min_vers=`echo $min_grib_version | sed 's/\.//g'`
        grib_vers=`echo $grib_version | sed 's/\.//g'`

        if test -n "$grib_vers" -a "$grib_vers" -ge $min_vers; then
                # All is good. Set up environment
                AC_MSG_RESULT(yes)
		CPPFLAGS="$CPPFLAGS $GRIB_CPPFLAGS"
		LDFLAGS="$LDFLAGS $GRIB_LDFLAGS"
        else
                # Old version of grib_api
                AC_MSG_RESULT(no)
		AC_MSG_ERROR([
-------------------------------------------------------------------------
    The installed version of the Grib API is too old. Please upgrade 
    to at least version $min_grib_version!
-------------------------------------------------------------------------
])
        fi
else
        # grib_info could not be found. Grib API must be installed
	AC_MSG_ERROR([
-------------------------------------------------------------------------
    Could not locate the grib_info utility. This indicates that the 
    Grib API is not installed correctly. If it is installed, use the 
    --with-gribapi=PATH option to specify the install location.
-------------------------------------------------------------------------
])
fi

# Check for the header file
AC_CHECK_HEADER([grib_api.h],,
	[AC_MSG_ERROR([
-------------------------------------------------------------------------
    Could not locate the grib_api.h file. This indicates that the 
    Grib API is not installed correctly. If it is installed, use the 
    --with-gribapi=PATH option to specify the install location and 
    verify that grib_api.h is located in GRIBAPI_PATH/include
-------------------------------------------------------------------------
])
	]
	)

AC_LANG_PUSH([C++])

# Search for Jasper
# Automatically add -ljasper if it is available.
# Jasper is required by the grib_api. If the grib_api is compiled with
# jasper and the latter is not available, the configure will fail.
AC_SEARCH_LIBS(jas_stream_memopen,
				[jasper],,,[-lm -ljpeg])

# Search for the grib_api library
# automatically adds -lgrib to the LIBS variable
AC_SEARCH_LIBS(grib_multi_support_on,
		[grib_api grib],
		[		
		],
		[AC_MSG_ERROR([
-------------------------------------------------------------------------
    Could not link with the grib_api library (or linking resulted in
    unresolved symbols). Unresolved symbols could be caused by not being
    able to link to jasper (verify with check on jas_stream_memopen 
    above). If the library is installed, verify that 
    -L(GRIBAPI_PATH)/lib is in your LDFLAGS variable or use the 
    --with-gribapi=PATH option to specify the GRIBAPI_PATH.

    LDFLAGS: $LDFLAGS
-------------------------------------------------------------------------
])
		]
		)
		
AC_LANG_POP([C++])

])

#
