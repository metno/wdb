#
#   wdb - weather and water data storage
#
#   Copyright (C) 2009 met.no
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
#   Checks for the presence of docbook
#

AC_DEFUN([WDB_GEOS],
[
# Set up option
AC_ARG_WITH([geos],
	     	AS_HELP_STRING([--with-geos=GEOS_PATH], 
			[Specify the directory in which geos is installed (by default, configure checks your PATH).]),
	    	[ac_geos_path="$withval"],
            [])

# Add path if given
PATH="$ac_docbook_path/bin:$PATH"

# Find xmlto
AC_PATH_PROG(GEOS_CONFIG, geos-config, no, $PATH)

if test "$GEOS_CONFIG" = "no" ; then
 
	AC_MSG_ERROR([
-------------------------------------------------------------------------
    Unable to find geos. Geos is required in order to build the WDB 
    documentation.
-------------------------------------------------------------------------
])
fi

geos_CFLAGS=`$GEOS_CONFIG --cflags`
geos_LIBS=`$GEOS_CONFIG --libs`
AC_SUBST(geos_CFLAGS)
AC_SUBST(geos_LIBS)

])

AC_DEFUN([WDB_GEOS_C],
[
AC_ARG_WITH([geos],
	     	AS_HELP_STRING([--with-geos_c=GEOS_PATH], 
			[Specify the directory in which geos c interface is installed (by default, configure checks your PATH).]),
	    	[LDFLAGS="-L$withval/lib $LDFLAGS"
	    	CPPFLAGS="-I$withval/include $CPPFLAGS"])

AC_CHECK_HEADER([geos_c.h])
AC_CHECK_LIB([geos_c], [GEOSGeom_getCoordSeq])
            
])
