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
#   Checks for the presence of postgres
#

# Check for PostgreSQL
# Usage: WDB_POSTGRES_CHECK([required_version_number])
AC_DEFUN([WDB_POSTGRES_CHECK],
[
# Set up option
AC_ARG_WITH(
	[pgsql],
	AS_HELP_STRING([--with-pgsql=PATH], 
	[Specify the directory in which postgresql is installed (by default, configure searches your PATH). If set, configure will search PATH/bin for pg_config]),
	[PGCONFIG="${with_pgsql}/bin/pg_config"]
)

# PG_CONFIG
AC_PATH_PROG(PGCONFIG,pg_config)
if test -f "${PGCONFIG}"; then
	pgsql_CPPFLAGS="-I`${PGCONFIG} --includedir` -I`${PGCONFIG} --includedir-server`"
	pgsql_LDFLAGS="-L`${PGCONFIG} --libdir`"
	pgsql_LIBS=""
	pgsql_VERSION="`${PGCONFIG} --version | sed -e 's#PostgreSQL ##'`"
else
	AC_MSG_ERROR([
-------------------------------------------------------------------------
   Unable to find pg_config. If Postgres is installed, make sure 
   pg_config is in your PATH, or specify the path in which postgres 
   is installed with --with-pgsql=PATH
-------------------------------------------------------------------------
	])
fi

# Check version of PostgreSQL
required_pgsql_version=ifelse([$1], [], [], [$1])

if test -n "$required_pgsql_version"; then
	AC_MSG_CHECKING([if PostgreSQL version is >= $required_pgsql_version])

	# Deconstruct version string (required)
	required_pgsql_version_major=`expr $required_pgsql_version : '\([[0-9]]*\)'`
    required_pgsql_version_minor=`expr $required_pgsql_version : '[[0-9]]*\.\([[0-9]]*\)'`
    required_pgsql_version_micro=`expr $required_pgsql_version : '[[0-9]]*\.[[0-9]]*\.\([[0-9]]*\)'`
	if test "x$required_pgsql_version_micro" = "x"; then
    	required_pgsql_version_micro="0"
    fi

	required_pgsql_version_number=`expr $required_pgsql_version_major \* 1000000 \
                                   \+ $required_pgsql_version_minor \* 1000 \
                                   \+ $required_pgsql_version_micro`

	# Deconstruct actual version string
	pgsql_version_major=`expr $pgsql_VERSION : '\([[0-9]]*\)'`
	pgsql_version_minor=`expr $pgsql_VERSION : '[[0-9]]*\.\([[0-9]]*\)'`
	pgsql_version_micro=`expr $pgsql_VERSION : '[[0-9]]*\.[[0-9]]*\.\([[0-9]]*\)'`
	if test "x$pgsql_version_micro" = "x"; then
		pgsql_version_micro="0"
	fi

	pgsql_version_number=`expr $pgsql_version_major \* 1000000 \
                          \+ $pgsql_version_minor \* 1000 \
                          \+ $pgsql_version_micro`

	pgsql_version_check=`expr $pgsql_version_number \>\= $required_pgsql_version_number`
	if test "$pgsql_version_check" = "1"; then
		AC_MSG_RESULT([yes])
	else
		AC_MSG_RESULT([no])
	fi
fi

CPPFLAGS="$CPPFLAGS $pgsql_CPPFLAGS"
LDFLAGSS="$LDFLAGS $pgsql_LDFLAGS"

# Check PQ Libraries
# Search for the libpq Library
# automatically adds -lpq to the LIBS variable
AC_SEARCH_LIBS(PQexec, 
	       	[pq],	 
	       	[pgsql_LIBS="$pgsql_LIBS -lpq"],
			[
		 	AC_MSG_ERROR([
-------------------------------------------------------------------------
    Unable to link with libpq. If the library is installed, make sure 
    -L(PGSQL_PATH)/lib is in your LDFLAGS, or specify the path in which 
    postgres is installed with --with-pgsql=PATH
-------------------------------------------------------------------------
			])
			]
)

AC_SUBST(pgsql_CFLAGS)
AC_SUBST(pgsql_LDFLAGS)
AC_SUBST(pgsql_LIBS)

	# Header files
    #AC_LANG_PUSH(C++)
	AC_CHECK_HEADER([postgres.h],,
					[AC_MSG_ERROR([
-------------------------------------------------------------------------
    Could not locate postgres.h 
    This indicates that Postgres may not be correctly installed for WDB
    on your site. WDB requires the development files for server-side
    programming of PostgreSQL to be installed.
    
    CPPFLAGS=$CPPFLAGS
-------------------------------------------------------------------------
])
	])
	AC_CHECK_HEADER([fmgr.h],,
					[AC_MSG_ERROR([
-------------------------------------------------------------------------
    Could not locate fmgr.h 
    This indicates that Postgres may not be correctly installed for WDB
    on your site. WDB requires the development files for server-side
    programming of PostgreSQL to be installed.
    
    CPPFLAGS=$CPPFLAGS
-------------------------------------------------------------------------
])
	])
	AC_CHECK_HEADER([funcapi.h],,
					[AC_MSG_ERROR([
-------------------------------------------------------------------------
    Could not locate funcapi.h 
    This indicates that Postgres may not be correctly installed for WDB
    on your site. WDB requires the development files for server-side
    programming of PostgreSQL to be installed.
    
    CPPFLAGS=$CPPFLAGS
-------------------------------------------------------------------------
])
	])
	AC_CHECK_HEADER([access/htup.h],,
					[AC_MSG_ERROR([
-------------------------------------------------------------------------
    Could not locate access/htup.h 
    This indicates that Postgres may not be correctly installed for WDB
    on your site. WDB requires the development files for server-side
    programming of PostgreSQL to be installed.
    
    CPPFLAGS=$CPPFLAGS
-------------------------------------------------------------------------
])
	])
	AC_CHECK_HEADER([utils/array.h],,
					[AC_MSG_ERROR([
-------------------------------------------------------------------------
    Could not locate utils/array.h
    This indicates that Postgres may not be correctly installed for WDB
    on your site. WDB requires the development files for server-side
    programming of PostgreSQL to be installed.
    
    CPPFLAGS=$CPPFLAGS
-------------------------------------------------------------------------
])
	])
	AC_CHECK_HEADER([utils/timestamp.h],,
					[AC_MSG_ERROR([
-------------------------------------------------------------------------
    Could not locate utils/timestamp.h
    This indicates that Postgres may not be correctly installed for WDB
    on your site. WDB requires the development files for server-side
    programming of PostgreSQL to be installed.
    
    CPPFLAGS=$CPPFLAGS
-------------------------------------------------------------------------
])
	])
	AC_CHECK_HEADER([executor/executor.h],,
					[AC_MSG_ERROR([
-------------------------------------------------------------------------
    Could not locate executor/executor.h
    This indicates that Postgres may not be correctly installed for WDB
    on your site. WDB requires the development files for server-side
    programming of PostgreSQL to be installed.
    
    CPPFLAGS=$CPPFLAGS
-------------------------------------------------------------------------
])
	])
	AC_CHECK_HEADER([executor/spi.h],,
					[AC_MSG_ERROR([
-------------------------------------------------------------------------
    Could not locate executor/spi.h
    This indicates that Postgres may not be correctly installed for WDB
    on your site. WDB requires the development files for server-side
    programming of PostgreSQL to be installed.
    
    CPPFLAGS=$CPPFLAGS
-------------------------------------------------------------------------
])
	])
	#AC_LANG_POP(C++)

])



# Check for libpqxx
# Usage: WDB_PQXX_CHECK([required_version_number])
AC_DEFUN([WDB_PQXX_CHECK],
[

required_pqxx_version=ifelse([$1], [], [2.6.8], [$1])

PKG_CHECK_MODULES([libpqxx], [libpqxx >= $required_pqxx_version])

AC_SUBST(libpqxx_CFLAGS)
AC_SUBST(libpqxx_LDFLAGS)
AC_SUBST(libpqxx_LIBS)

CPPFLAGS="$CPPFLAGS $libpqxx_CFLAGS"
LDFLAGS="$LDFLAGS $libpqxx_LDFLAGS"
LIBS="$LIBS $libpqxx_LIBS"

])



# Check for PostGIS
# Usage: WDB_PQOSTGIS_CHECK([required_version_number])
AC_DEFUN([WDB_POSTGIS_CHECK],
[
# Set up Option
AC_ARG_WITH([postgis],
	    AS_HELP_STRING([--with-postgis=PATH], 
	    [Specify the directory in which postgis is installed (default is the PostgreSQL contrib directory)]),
	    [postgis_SQL="${with_postgis}"],
	    [postgis_SQL=`${PGCONFIG} --sharedir`]
)

required_postgis_version=ifelse([$1], [], [1.1.x], [$1])
# no obvious way to check for PostGIS version

AC_MSG_CHECKING(for postgis)
if test ! -f ${postgis_SQL}/lwpostgis.sql; then
	postgis_SQL=${postgis_SQL}/../
	if test ! -f ${postgis_SQL}/lwpostgis.sql; then
		postgis_SQL=${postgis_SQL}/../postgresql-8.1-postgis
		if test ! -f ${postgis_SQL}/lwpostgis.sql; then
			AC_MSG_RESULT(no)
			AC_MSG_ERROR([
-------------------------------------------------------------------------
    Cannot find postgis SQL files. Ensure that these files are 
    installed in the share directory of your PostgreSQL installation, 
    or explicitly specify its location using the --with-postgis=PATH 
    option.
-------------------------------------------------------------------------
			])
		fi
	fi
fi
AC_MSG_RESULT(yes)

AC_SUBST(postgis_SQL)

# Specify default SRID 
AC_ARG_WITH(
	[default-srid],
	AS_HELP_STRING([--with-default-srid=SRID], 
	[Specify the default spatial reference ID (SRID) to be used in geographical queries on the database; this is also the projection that will be used internally in the database. The default SRID is 4030 (+proj=longlat +ellps=WGS84)]),
	[srid="${with_default-srid}"],
	[srid="4030"]
)
AC_MSG_CHECKING([for the SRID to be used in the database])
AC_MSG_RESULT([$srid])
AC_DEFINE_UNQUOTED([DEFAULT_SRID], $srid, [The default SRID to be used in the database])
AC_SUBST([DEFAULT_SRID], $srid)

])
