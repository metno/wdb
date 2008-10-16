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
#   Enable/Disable database installation
#
AC_DEFUN([WDB_DATABASE_INSTALL],
[
	AC_ARG_ENABLE(database_install,
     	AC_HELP_STRING( [--disable-database-install], [Do not install database as part of running make install (you may instead run make install-database, or install/install_database.sh after having run make install).] ),
     			[case "${enableval}" in
       				yes) database_install=true ;;
       				no)  database_install=false ;;
       				*) AC_MSG_ERROR(bad value ${enableval} for --enable-database-install) ;;
     			 esac],
     			 [database_install=true])
	AC_SUBST( [DO_INSTALL_DATABASE], [$database_install] )
])

#
# Set Database name for installation/uninstall
#
AC_DEFUN([WDB_DATABASE_NAME],
[
	AC_ARG_WITH([database_name],
		  	    AS_HELP_STRING([--with-database-name=DBNAME], 
	     					   [Specify the name of the database to be installed (default is wdb)]),
	    		[database_name="$withval"],
            	[database_name="wdb"])
	AC_SUBST( [DATABASE_NAME], [$database_name] )
	# Database Schema
	version_major=`expr $PACKAGE_VERSION : '\([[0-9]]*\)'`
    version_minor=`expr $PACKAGE_VERSION : '[[0-9]]*\.\([[0-9]]*\)'`
    version_micro=`expr $PACKAGE_VERSION : '[[0-9]]*\.[[0-9]]*\.\([[0-9]]*\)'`
	database_version="${version_major}_${version_minor}_${version_micro}"
	AC_SUBST( [DATABASE_SCHEMA], [$database_version] )
	# Define Database Schemas
	AC_DEFINE_UNQUOTED(WDB_SCHEMA,
			  		   ["wdb_${database_version}"],
			  		   [The default schema of WDB])
	AC_DEFINE_UNQUOTED(WCI_SCHEMA,
			  		   ["wci_${database_version}"],
			  		   [The default schema of WCI])
])
