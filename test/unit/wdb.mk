#-----------------------------------------------------------------------------
# Unit Tests
#-----------------------------------------------------------------------------

if HAS_CPPUNIT
TESTS +=   					wdbUnitTest

check_PROGRAMS +=       	wdbUnitTest

wdbUnitTest_SOURCES = 		test/unit/wdbUnitTest.cpp

wdbUnitTest_CPPFLAGS = 		$(cppunit_CFLAGS) -I$(srcdir)/test/utility/configuration

wdbUnitTest_LDFLAGS = 		$(AM_LDFLAGS) 

wdbUnitTest_LDADD = 		$(cppunit_LIBS) -ltestConfiguration -lwdbConfiguration -lwdbProjection
# HAS_CPPUNIT
endif

EXTRA_DIST +=   			test/unit/wdb.mk \
							test/unit/Makefile.am \
							test/unit/Makefile.in

DISTCLEANFILES +=       	test/unit/Makefile

