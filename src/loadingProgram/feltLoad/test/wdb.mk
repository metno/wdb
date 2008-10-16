
#-----------------------------------------------------------------------------
# WCI Types Unit Test Framework
#
# See: <top>/test/unit
#-----------------------------------------------------------------------------
							
if HAS_CPPUNIT
feltLoadToTest = src/loadingProgram/feltLoad/FeltGridDefinition.cpp
							
wdbUnitTest_SOURCES += $(feltLoadToTest) \
	src/loadingProgram/feltLoad/test/FeltGridDefinitionTest.cpp \
	src/loadingProgram/feltLoad/test/FeltGridDefinitionTest.h
							
wdbUnitTest_CPPFLAGS +=		$(milib_CFLAGS)
wdbUnitTest_LDFLAGS  +=		$(milib_LIBS)

EXTRA_DIST += 				src/loadingProgram/feltLoad/test/FakeDatabaseConnection.h
# HAS_CPPUNIT
endif
    						
EXTRA_DIST +=				src/loadingProgram/feltLoad/test/wdb.mk \
							src/loadingProgram/feltLoad/test/Makefile.am \
							src/loadingProgram/feltLoad/test/Makefile.in

DISTCLEANFILES +=			src/loadingProgram/feltLoad/test/Makefile


# Local Makefile Targets
#-----------------------------------------------------------------------------

src/loadingProgram/gribLoad/test/clean: clean
