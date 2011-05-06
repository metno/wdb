#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
#
#-----------------------------------------------------------------------------

#-----------------------------------------------------------------------------
# Pre-Conf
#-----------------------------------------------------------------------------

AC_PREREQ(2.59)
AC_INIT(wdb, 1.0.2, [wdb@met.no])
AC_SUBST( [DEBIAN_VERSION],["1.0"] )
AC_SUBST( [SCHEMA_VERSION],["1"] )
# A previous schema version
# - This is used for test purposes; e.g., the upgrade test
AC_SUBST( [OLD_MAJOR_VERSION], ["1"] )
AC_SUBST( [OLD_MINOR_VERSION], ["0"] )
AC_SUBST( [OLD_MICRO_VERSION], ["1"] )

# Config
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADER([config.h])
AC_CONFIG_MACRO_DIR([m4])

#-----------------------------------------------------------------------------
# Source & Build Directories
#-----------------------------------------------------------------------------

# Source Directories
# - All directories that need to be linked into AM_FLAGS
src_subdirs="			src/blob \
						src/callInterface/api \
			     		src/callInterface/core \
			     		src/callInterface/core/extractGridData \
				     	src/callInterface/core/test \
				     	src/callInterface/types \
				     	src/callInterface/types/test \
				     	src/callInterface/util \
				     	src/cleaningProgram \
				     	src/common/configuration \
				     	src/common/exception \
				     	src/common/logHandler \
				     	src/common/math \
				     	src/common/projection \
				     	src/common/projection/test \
				    	src/database \
				    	src/admin \
				    	src/admin/operations \
				    	src/admin/ui/cmdLine \
						test/utility/testWrite \
						test/unit \
						examples \ 
						examples/C++
						examples/sql "

# Build Directories
# - All directories that building can occur in
build_subdirs="			doc \
						etc/metadata \
						etc/wdb \
						install \
						src/blob \
						src/callInterface/api \
						src/callInterface/core \
						src/callInterface/core/test \
						src/callInterface/types \
						src/callInterface/types/test \
						src/callInterface/util \
						src/common/configuration \
						src/common/exception \
						src/common/logHandler \
				     	src/common/math \
						src/common/projection \
						src/common/projection/test \
						src/database \
						src/cleaningProgram \
			    		src/admin \
			    		src/admin/operations \
		    			src/admin/ui/cmdLine \
						test/install/callInterface \
						test/install/cleaningProgram \
						test/performance/callInterface \
						test/stability/callInterface \
						test/upgrade \
						test/utility/configuration \
						test/utility/testWrite \
						test/unit \
						examples \
						examples/C++
						examples/sql "


#-----------------------------------------------------------------------------
# Programs
#-----------------------------------------------------------------------------

AC_PROG_CXX
AC_PROG_CC
AC_PROG_CPP
AC_PROG_AWK
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AC_PROG_LN_S
AC_PROG_LIBTOOL

AM_PATH_GLIB_2_0

#-----------------------------------------------------------------------------
# Checks
#-----------------------------------------------------------------------------

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdio.h stdlib.h sys/time.h unistd.h wordexp.h ctype.h])
AC_CHECK_HEADERS([string vector iterator iosfwd iostream sstream fstream])
AC_CHECK_HEADERS([cmath algorithm memory new stdexcept cstdlib exception])

# string exception sstream 

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_MEMCMP

# Check for libraries.
AC_CXX_REQUIRE_STL
VL_LIB_READLINE

#-----------------------------------------------------------------------------
# Module Checks
#-----------------------------------------------------------------------------

# POSTGRES
WDB_POSTGRES_CHECK(8.4.x)
WDB_POSTGIS_CHECK(1.4.x)
WDB_PQXX_CHECK(2.6.8)

WDB_GEOS_C

# BOOST
WDB_BOOST_CHECK(1.33.1)

# LOG4CPP
WDB_LOG4CPP_CHECK(1.0)

# PROJ
WDB_PROJ_CHECK

# CppUnit
WDB_CPPUNIT

AM_PATH_PYTHON(2.4,, [:])
AM_CONDITIONAL([HAVE_PYTHON], [test "$PYTHON" != :])

# DOCBOOK
WDB_DOCBOOK_CHECK

# Set Default Projection
AC_DEFINE(DEFAULT_PROJECTION,["+proj=longlat +ellps=WGS84 +no_defs"],[The default projection used in the database])

# Set Database Install
WDB_DATABASE_NAME
WDB_DATABASE_INSTALL


#-----------------------------------------------------------------------------
# Setup CPP linking
#-----------------------------------------------------------------------------

# Link to Source Directories (header files)
for srcdir_tmp in $src_subdirs; do
	CPPFLAGS="$CPPFLAGS -I$srcdir/$srcdir_tmp"
done


#-----------------------------------------------------------------------------
# Create Config File
#-----------------------------------------------------------------------------

# Create PkgConfig File
AC_CREATE_GENERIC_CONFIG(wdb)


#-----------------------------------------------------------------------------
# Automake
#-----------------------------------------------------------------------------

AM_INIT_AUTOMAKE(-Wall -Werror subdir-objects)
AM_PROG_CC_C_O


#-----------------------------------------------------------------------------
# Configure and Exit
#-----------------------------------------------------------------------------

# Configuration Files
AC_CONFIG_FILES([Makefile
				 etc/metadata/Makefile
				 etc/wdb/Makefile
				 install/Makefile
				 src/blob/Makefile
				 src/common/configuration/Makefile
				 src/common/exception/Makefile
				 src/common/logHandler/Makefile
				 src/common/math/Makefile
				 src/common/projection/Makefile
				 src/common/projection/test/Makefile
				 src/callInterface/api/Makefile
				 src/callInterface/core/Makefile
				 src/callInterface/core/test/Makefile
				 src/callInterface/types/Makefile
				 src/callInterface/types/test/Makefile
				 src/callInterface/util/Makefile
				 src/cleaningProgram/Makefile
				 src/database/Makefile
				 src/admin/Makefile
		   	 	 src/admin/operations/Makefile
		   	 	 src/admin/ui/cmdLine/Makefile
				 test/install/callInterface/Makefile
				 test/install/cleaningProgram/Makefile 
				 test/performance/callInterface/Makefile
				 test/stability/callInterface/Makefile
				 test/upgrade/Makefile
				 test/utility/configuration/Makefile
				 test/utility/testWrite/Makefile
		 		 test/unit/Makefile
			     examples/Makefile
		 		 examples/C++/Makefile
		 		 examples/sql/Makefile
		 		 doc/doxygen/Doxyfile
		])
AC_OUTPUT

# Print Out Summary of Configuration
AC_MSG_RESULT([ ])
AC_MSG_RESULT([$PACKAGE_NAME $PACKAGE_VERSION])
AC_MSG_RESULT([Configuration:])
AC_MSG_RESULT([-------------------------------------------------------------------------])
AC_MSG_RESULT([ ])
AC_MSG_RESULT([ Database Name:	  $DATABASE_NAME])
AC_MSG_RESULT([ Source Code:      $srcdir])
AC_MSG_RESULT([ Host System:      $host])
AC_MSG_RESULT([  ])
AC_MSG_RESULT([ Prefix:           $prefix])
AC_MSG_RESULT([ Binaries:         $exec_prefix])
AC_MSG_RESULT([ Manuals:          $mandir])
AC_MSG_RESULT([ Data:             $datadir])
AC_MSG_RESULT([ System Config:    $sysconfdir])
AC_MSG_RESULT([ ])
AC_MSG_RESULT([ CPPFLAGS:         $CPPFLAGS])
AC_MSG_RESULT([ LDFLAGS:          $LDFLAGS])
AC_MSG_RESULT([ LIBS:             $LIBS])
AC_MSG_RESULT([  ])
AC_MSG_RESULT([-------------------------------------------------------------------------])
AC_MSG_RESULT([ ])
