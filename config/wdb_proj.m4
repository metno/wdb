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
#   Checks for the Presence of PROJ.4
#

AC_DEFUN([WDB_PROJ_CHECK],
[
	# Set up option
	AC_ARG_WITH([proj],
				AS_HELP_STRING([--with-proj=PATH], [Specify the directory in which proj is installed (by default, configure uses the environment variable LDFLAGS). If set, configure will add PATH/include to CPPFLAGS and PATH/lib to LDFLAGS]),
	    		[ac_proj_path="$withval"],
            	[])

	# Add path if given
	if test "$ac_proj_path" != ""; then
        PROJ_CPPFLAGS="-I$ac_proj_path/include"
		PROJ_LDFLAGS="-L$ac_proj_path/lib"
	fi

	# Set up environment
	CPPFLAGS="$CPPFLAGS $PROJ_CPPFLAGS"
	LDFLAGS="$LDFLAGS $PROJ_LDFLAGS"

	# Check for header file
	AC_CHECK_HEADER([proj_api.h],
					,
					[AC_MSG_ERROR([
-------------------------------------------------------------------------
	Could not locate the proj_api.h header file. This indicates that
	PROJ.4 may not be installed correclty on this server. WDB requires
	PROJ.4 to perform cartographic reprojections.
-------------------------------------------------------------------------
])
					])

	# Search for the Library
	# automatically adds -lproj to the LIBS variable
	AC_SEARCH_LIBS(pj_init_plus, 
				   [proj],, 
				   [AC_MSG_ERROR([
-------------------------------------------------------------------------
    Unable to link with libproj. If the library is installed, make sure 
    -L(PROJ_PATH) is in your LDFLAGS, or specify the path in which proj 
    is installed with --with-proj=PATH.

    LDFLAGS: $LDFLAGS
-------------------------------------------------------------------------
])]
				   )

])
