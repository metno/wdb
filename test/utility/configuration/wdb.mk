#-----------------------------------------------------------------------------
# testConfiguration
#-----------------------------------------------------------------------------

# Library
#-----------------------------------------------------------------------------

pkglib_LIBRARIES = 					libtestConfiguration.a
libtestConfiguration_a_SOURCES = 	test/utility/configuration/testConfiguration.cpp
pkginclude_HEADERS += 				test/utility/configuration/testConfiguration.h
libtestConfiguration_a_CPPFLAGS =	-DSYSCONFDIR=\"$(sysconfdir)\" -fPIC

EXTRA_DIST +=						test/utility/configuration/wdb.mk \
									test/utility/configuration/Makefile.am \
									test/utility/configuration/Makefile.in

DISTCLEANFILES +=					test/utility/configuration/Makefile


# Local Makefile Targets
#-----------------------------------------------------------------------------

test/utility/configuration/all: 	libtestConfiguration.a

test/utility/configuration/clean:	clean
