#-----------------------------------------------------------------------------
# testConfiguration
#-----------------------------------------------------------------------------

# Library
#-----------------------------------------------------------------------------

lib_LTLIBRARIES += 					libtestConfiguration.la
libtestConfiguration_la_SOURCES = 	test/utility/configuration/testConfiguration.cpp
pkginclude_HEADERS += 				test/utility/configuration/testConfiguration.h
libtestConfiguration_la_CPPFLAGS =	-DSYSCONFDIR=\"$(sysconfdir)\" -fPIC

EXTRA_DIST +=						test/utility/configuration/wdb.mk \
									test/utility/configuration/Makefile.am \
									test/utility/configuration/Makefile.in

DISTCLEANFILES +=					test/utility/configuration/Makefile


# Local Makefile Targets
#-----------------------------------------------------------------------------

test/utility/configuration/all: 	libtestConfiguration.la

test/utility/configuration/clean:	clean
