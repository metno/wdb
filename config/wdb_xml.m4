#
#   wdb - weather and water data storage
#
#   Copyright (C) 2008 met.no
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
#   Checks for the presence of libxml
#

# Check for libxml2
# Usage: WDB_LIBXML_CHECK([required_version_number])
AC_DEFUN([WDB_LIBXML_CHECK],
[
	required_libxml_version=ifelse([$1], [], [2.6.0], [$1])
	AC_PATH_PROG(PKGCONFIG, pkg-config)
	if test -f "${PKGCONFIG}"; then
		libxml_CFLAGS="`${PKGCONFIG} libxml-2.0 --cflags`"
		libxml_LDFLAGS="`${PKGCONFIG} libxml-2.0 --libs-only-L`"
		libxml_LIBS="`${PKGCONFIG} libxml-2.0 --libs-only-l`"
		libxml_LIBS="${libxml_LIBS} `${PKGCONFIG} libxml-2.0 --libs-only-other`"
	else
		AC_MSG_ERROR([
-------------------------------------------------------------------------
   Unable to find pkg_config. If pkg-config is installed, make sure 
   pkg_config is in your PATH. ${PKGCONFIG}
-------------------------------------------------------------------------
		])
	fi
	AC_MSG_CHECKING([if libxml-2.0 version is >= $required_libxml_version])
	if test -z "`${PKGCONFIG} libxml-2.0 --atleast-version=${required_libxml_version}`"; then
		AC_MSG_RESULT([yes])
	else
		AC_MSG_RESULT([no])
		AC_MSG_ERROR([
-------------------------------------------------------------------------
    Could not find libxml2 version $required_version_number using pkg-config. 
    Verify that libxml2 is correctly installed, and that pkg-config has 
    access to the the appropriate .pc file.
-------------------------------------------------------------------------
		])
	fi	

	AC_SUBST(libxml_CFLAGS)
	AC_SUBST(libxml_LDFLAGS)
	AC_SUBST(libxml_LIBS)
		
	CPPFLAGS="$CPPFLAGS $libxml_CFLAGS"
	LDFLAGS="$LDFLAGS $libxml_LDFLAGS"
	LIBS="$LIBS $libxml_LIBS"
	# Header files
    AC_LANG_PUSH(C++)
	AC_LANG_POP(C++)

])