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

#
# Checks for the use of Informix
# 
# Todo: Needs to be updated with latest requirements.

AC_DEFUN([WDB_INFORMIX_CHECK],
[
# Set up option
AC_ARG_WITH([informix],
	    AS_HELP_STRING([--with-informix], 
			   [Set this option to specify that WDB's components should be compiled to be able to run on an Informix database. At the moment, this option only functions
for the GribLoad component]),
	    [
		AC_DEFINE(USE_INFORMIX,,
		          [define if the Informix files are to be used instead of postgres])
	     	AC_SUBST(INFORMIX_OPT,[informix])
		# Check the Informix Libraries
		# FIXME: Replace `main' with a function in `-lifcli':
		#AC_CHECK_LIB([ifcli], [main])
		# FIXME: Replace `main' with a function in `-lifdmi':
		#AC_CHECK_LIB([ifdmi], [main])
		# FIXME: Replace `main' with a function in `-lifgen':
		#AC_CHECK_LIB([ifgen], [main])
		# FIXME: Replace `main' with a function in `-lifgls':
		#AC_CHECK_LIB([ifgls], [main])
		# FIXME: Replace `main' with a function in `-lifglx':
		#AC_CHECK_LIB([ifglx], [main])
		# FIXME: Replace `main' with a function in `-lifos':
		#AC_CHECK_LIB([ifos], [main])
		# FIXME: Replace `main' with a function in `-lifsql':
		#AC_CHECK_LIB([ifsql], [main])
		# FIXME: Replace `main' with a function in `-lifxa':
		#AC_CHECK_LIB([ifxa], [main])
	    ]
	)

])
