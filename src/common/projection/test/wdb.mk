
#-----------------------------------------------------------------------------
# WCI Types Unit Test Framework
#
# See: <top>/test/unit
#-----------------------------------------------------------------------------

if HAS_CPPUNIT
wdbUnitTest_SOURCES += 		src/common/projection/test/WdbProjectionTest.cpp \
    						src/common/projection/test/WdbProjectionTest.h \
    						src/common/projection/test/GridGeometryTest.cpp \
							src/common/projection/test/GridGeometryTest.h
    						
    						
wdbUnitTest_LDFLAGS += 		-lwdbProjection
# HAS_CPPUNIT
endif

EXTRA_DIST +=				src/common/projection/test/wdb.mk \
							src/common/projection/test/Makefile.am \
							src/common/projection/test/Makefile.in

DISTCLEANFILES +=			src/common/projection/test/Makefile


# Local Makefile Targets
#-----------------------------------------------------------------------------

src/common/projection/test/all: wdbUnitTest

src/common/projection/test/clean: clean
