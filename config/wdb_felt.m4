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


AC_DEFUN([WDB_FELTLOAD],
[

AC_ARG_ENABLE([feltLoad],
	    AS_HELP_STRING([--enable-feltLoad], [Specify whether or not feltLoad should be built. The default is that feltLoad is NOT compiled.]),
	    [case "${enableval}" in
	       yes) fLoad=true ;;
    	   no)  fLoad=false ;;
       		*) AC_MSG_ERROR([bad value ${enableval} for --enable-feltLoad]) ;;
     	esac],[fLoad=false]
)
AM_CONDITIONAL([FELT_LOAD], [test x$fLoad = xtrue])


if test x$fLoad = xtrue ; then
	PKG_CHECK_MODULES([milib], [milib >= 3.0],
   	[
		AC_SUBST(milib_CFLAGS)
		AC_SUBST(milib_LIBS)
	],
	[
		AC_MSG_RESULT(no)
		AC_MSG_ERROR([
-------------------------------------------------------------------------
    Could not find milib through PKG_CONFIG. Will not be able to 
    compile felt related programs or libraries.
-------------------------------------------------------------------------
])
	])

else
	AC_MSG_RESULT(no)
fi


])
