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
#   Checks for the presence of log4cpp
#

# Check for log4cpp
# Usage: WDB_LOG4CPP_CHECK([required_version_number])
AC_DEFUN([WDB_LOG4CPP_CHECK],
[
required_log4cpp_version=ifelse([$1], [], [1.0], [$1])

PKG_CHECK_MODULES(
	[log4cpp],
	[log4cpp >= $required_log4cpp_version],
	[
		AC_SUBST(log4cpp_CFLAGS)
		AC_SUBST(log4cpp_LDFLAGS)
		AC_SUBST(log4cpp_LIBS)
				
		CPPFLAGS="$CPPFLAGS $log4cpp_CFLAGS"
		LDFLAGS="$LDFLAGS $log4cpp_LDFLAGS"
		LIBS="$LIBS $log4cpp_LIBS"
	],
	[
		AC_MSG_ERROR([
-------------------------------------------------------------------------
    Could not find log4cpp version $required_version_number using pkg-config. Verify that log4cpp 
    is correctly installed, and that pkg-config has access to the the 
    appropriate .pc file.
-------------------------------------------------------------------------
		])
	])

	# Header files
    AC_LANG_PUSH(C++)
	AC_CHECK_HEADER([log4cpp/Appender.hh],,
					[AC_MSG_ERROR([
-------------------------------------------------------------------------
    Could not locate log4cpp/Appender.hh 
    This indicates that log4cpp may be missing the development files for
    log4cpp or that it has not been installed correctly.
    
    CPPFLAGS=$CPPFLAGS
-------------------------------------------------------------------------
])
	])
	AC_CHECK_HEADER([log4cpp/Category.hh],,
					[AC_MSG_ERROR([
-------------------------------------------------------------------------
    Could not locate log4cpp/Category.hh 
    This indicates that log4cpp may be missing the development files for
    log4cpp or that it has not been installed correctly.
    
    CPPFLAGS=$CPPFLAGS
-------------------------------------------------------------------------
])
	])
	AC_CHECK_HEADER([log4cpp/CategoryStream.hh],,
					[AC_MSG_ERROR([
-------------------------------------------------------------------------
    Could not locate log4cpp/CategoryStream.hh 
    This indicates that log4cpp may be missing the development files for
    log4cpp or that it has not been installed correctly.
    
    CPPFLAGS=$CPPFLAGS
-------------------------------------------------------------------------
])
	])
	AC_CHECK_HEADER([log4cpp/PatternLayout.hh],,
					[AC_MSG_ERROR([
-------------------------------------------------------------------------
    Could not locate log4cpp/PatternLayout.hh 
    This indicates that log4cpp may be missing the development files for
    log4cpp or that it has not been installed correctly.
    
    CPPFLAGS=$CPPFLAGS
-------------------------------------------------------------------------
])
	])
	AC_CHECK_HEADER([log4cpp/Priority.hh],,
					[AC_MSG_ERROR([
-------------------------------------------------------------------------
    Could not locate log4cpp/Priority.hh 
    This indicates that log4cpp may be missing the development files for
    log4cpp or that it has not been installed correctly.
    
    CPPFLAGS=$CPPFLAGS
-------------------------------------------------------------------------
])
	])
	AC_CHECK_HEADER([log4cpp/OstreamAppender.hh],,
					[AC_MSG_ERROR([
-------------------------------------------------------------------------
    Could not locate log4cpp/OstreamAppender.hh 
    This indicates that log4cpp may be missing the development files for
    log4cpp or that it has not been installed correctly.
    
    CPPFLAGS=$CPPFLAGS
-------------------------------------------------------------------------
])
	])
	AC_CHECK_HEADER([log4cpp/RollingFileAppender.hh],,
					[AC_MSG_ERROR([
-------------------------------------------------------------------------
    Could not locate log4cpp/RollingFileAppender.hh 
    This indicates that log4cpp may be missing the development files for
    log4cpp or that it has not been installed correctly.
    
    CPPFLAGS=$CPPFLAGS
-------------------------------------------------------------------------
])
	])
	AC_LANG_POP(C++)


])

