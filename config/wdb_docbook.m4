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
#   Checks for the presence of docbook
#

AC_DEFUN([WDB_DOCBOOK_CHECK],
[
# Set up option
AC_ARG_WITH([docbook],
	     	AS_HELP_STRING([--with-docbook=DOCBOOK_PATH], 
			[Specify the directory in which xmlto is installed (by default, configure checks your PATH). If an installation path is given using --with-docbook, configure will also check DOCBOOK_PATH/bin]),
	    	[ac_docbook_path="$withval"],
            [])

# Add path if given
PATH="$ac_docbook_path/bin:$PATH"

# Find xmlto
AC_PATH_PROG(DOCBOOK, xmlto, no, $PATH)

if test "$DOCBOOK" = "no" ; then 
	AC_MSG_WARN([
-------------------------------------------------------------------------
    Unable to find xmlto. xmlto is required in order to build the WDB 
    documentation.
-------------------------------------------------------------------------
])
fi
AM_CONDITIONAL([HAS_DOCBOOK], [test "$DOCBOOK" != "no"])
])
