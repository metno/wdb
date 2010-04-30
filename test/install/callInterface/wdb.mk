if HAS_CPPUNIT

noinst_LTLIBRARIES += libwciInstallTestBase.la

libwciInstallTestBase_la_SOURCES = \
	test/install/callInterface/AbstractWciTestFixture.cpp \
	test/install/callInterface/AbstractWdbTestFixture.cpp \
	test/install/callInterface/wciInstallTester.cpp

pkginclude_HEADERS += \
	test/install/callInterface/AbstractWciTestFixture.h \
	test/install/callInterface/AbstractWdbTestFixture.h \
	test/install/callInterface/wciInstallTester.h


libwciInstallTestBase_la_CPPFLAGS = $(AM_CPPFLAGS) \
	$(cppunit_CFLAGS) -I$(srcdir)/test/utility/configuration \
	-I$(srcdir)/test/install/callInterface
	
libwciInstallTestBase_la_LIBS = \
	-lwdbTest -lwdbConfig
	

#-----------------------------------------------------------------------------
# WDB callInterface Install Tests
#-----------------------------------------------------------------------------

wciInstallTester_SOURCES =	test/install/callInterface/SqlInjectionTest.h \
							test/install/callInterface/SqlInjectionTest.cpp \
							test/install/callInterface/DataProviderTest.h \
							test/install/callInterface/DataProviderTest.cpp \
							test/install/callInterface/PlaceGeometryTest.h \
							test/install/callInterface/PlaceGeometryTest.cpp \
							test/install/callInterface/PlaceNameTest.h \
							test/install/callInterface/PlaceNameTest.cpp \
							test/install/callInterface/SurroundInterpolationTest.h \
							test/install/callInterface/SurroundInterpolationTest.cpp \
							test/install/callInterface/ReferenceTimeTest.h \
							test/install/callInterface/ReferenceTimeTest.cpp \
							test/install/callInterface/ValidTimeTest.h \
							test/install/callInterface/ValidTimeTest.cpp \
							test/install/callInterface/ValueParameterTest.h \
							test/install/callInterface/ValueParameterTest.cpp \
							test/install/callInterface/LevelTest.h \
							test/install/callInterface/LevelTest.cpp \
							test/install/callInterface/DataVersionTest.h \
							test/install/callInterface/DataVersionTest.cpp \
							test/install/callInterface/WciBeginTest.h \
							test/install/callInterface/WciBeginTest.cpp \
							test/install/callInterface/WciEndTest.h \
							test/install/callInterface/WciEndTest.cpp \
							test/install/callInterface/WciConfigurationTest.h \
							test/install/callInterface/WciConfigurationTest.cpp \
							test/install/callInterface/WciFetchTest.h \
							test/install/callInterface/WciFetchTest.cpp \
							test/install/callInterface/WciWriteTest.h \
							test/install/callInterface/WciWriteTest.cpp \
							test/install/callInterface/WciBrowseTest.h \
							test/install/callInterface/WciBrowseTest.cpp \
							test/install/callInterface/wciOutputTest.h \
							test/install/callInterface/wciOutputTest.cpp \
							test/install/callInterface/TransactionCorrectnessTest.h \
							test/install/callInterface/TransactionCorrectnessTest.cpp \
							test/install/callInterface/main.cpp

wciInstallTester_CPPFLAGS =	$(AM_CPPFLAGS) $(CPPUNIT_CFLAGS) -I$(srcdir)/test/utility/configuration -I$(srcdir)/test/install/callInterface

wciInstallTester_LDFLAGS =	$(AM_LDFLAGS) $(top_builddir)/libwciInstallTestBase.la \
							-lwciTypesNoPostgres -lwdbTest -lwdbConfig \
							$(cppunit_LIBS) 

wciInstallTester_LDADD =	-lwdbProj -lwdbExcept -lwdbMath

WCIINSTALLTEST_SUPPORT =	test/install/callInterface/buildUp.in.sh \
							test/install/callInterface/tearDown.in.sh
							
WCIINSTALLTEST_XML = 		test/install/callInterface/wciInstallTest1.xml \
							test/install/callInterface/wciInstallTest2.xml \
							test/install/callInterface/wciInstallTest3.xml

if HAS_CPPUNIT

INSTALLTESTS += 			wciInstallTest.sh

noinst_PROGRAMS += 			wciInstallTester

noinst_SCRIPTS += 			wciInstallTest.sh \
							$(WCIINSTALLTEST_SUPPORT:.in.sh=.sh)

endif

CLEANFILES += 				wciInstallTest.sh \
							$(WCIINSTALLTEST_SUPPORT:.in.sh=.sh)

EXTRA_DIST +=				test/install/callInterface/wciInstallTest.in.sh \
							$(WCIINSTALLTEST_SUPPORT) \
							$(WCIINSTALLTEST_XML) \
							test/install/callInterface/wdb.mk \
							test/install/callInterface/Makefile.am \
							test/install/callInterface/Makefile.in

DISTCLEANFILES += 			test/install/callInterface/Makefile

wciInstallTest.sh:			test/install/callInterface/wciInstallTest.in.sh
				$(SED_SUBSTITUTION);\
				chmod 754 $@



# Local Makefile Targets
#-----------------------------------------------------------------------------

test/install/callInterface/all: wciInstallTest

test/install/callInterface/clean: clean

test/install/callInterface/install: install

# HAS_CPPUNIT
endif
