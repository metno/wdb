
#-----------------------------------------------------------------------------
# WCI Types Unit Test Framework
#
# See: <top>/test/unit
#-----------------------------------------------------------------------------

if HAS_CPPUNIT
#GRIBLOAD_LINK_IN_SOURCES = \
# 						src/loadingProgram/gribLoad/gribGridDefinition.cpp \
#						src/loadingProgram/gribLoad/gribGridDefinition.h \
#						src/loadingProgram/gribLoad/GribHandleReader.cpp \
#						src/loadingProgram/gribLoad/GribHandleReader.h \
#						src/loadingProgram/gribLoad/wdbSetup.cpp \
#						src/loadingProgram/gribLoad/wdbSetup.h

#wdbUnitTest_SOURCES += 		src/loadingProgram/gribLoad/test/GribGridDefinitionTest.cpp \
#							src/loadingProgram/gribLoad/test/GribGridDefinitionTest.h \
#							src/loadingProgram/gribLoad/test/FakeGribDatabaseInterface.h \
#							$(GRIBLOAD_LINK_IN_SOURCES)
							
    						
wdbUnitTest_LDFLAGS += 		-lwdbProjection
# HAS_CPPUNIT
endif

EXTRA_DIST +=				src/loadingProgram/gribLoad/test/wdb.mk \
							src/loadingProgram/gribLoad/test/Makefile.am \
							src/loadingProgram/gribLoad/test/Makefile.in

DISTCLEANFILES +=			src/loadingProgram/gribLoad/test/Makefile


# Local Makefile Targets
#-----------------------------------------------------------------------------

src/loadingProgram/gribLoad/test/clean: clean
