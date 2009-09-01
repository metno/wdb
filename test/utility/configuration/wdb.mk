#-----------------------------------------------------------------------------
# libwdbtest
#-----------------------------------------------------------------------------

# Library
#-----------------------------------------------------------------------------

noinst_LTLIBRARIES += 		libwdbTest.la

libwdbTest_la_SOURCES = test/utility/configuration/testConfiguration.cpp

pkginclude_HEADERS += 	test/utility/configuration/testConfiguration.h

libwdbTest_la_CPPFLAGS = \
						$(AM_CPPFLAGS) -fPIC

EXTRA_DIST +=			test/utility/configuration/wdb.mk \
						test/utility/configuration/Makefile.am \
						test/utility/configuration/Makefile.in

DISTCLEANFILES +=		test/utility/configuration/Makefile


# Local Makefile Targets
#-----------------------------------------------------------------------------

test/utility/configuration/all: libwdbTest.la

test/utility/configuration/clean: clean
