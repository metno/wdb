#-----------------------------------------------------------------------------
# WCI Core Unit Test Framework
#
# See: <top>/test/unit
#-----------------------------------------------------------------------------

if HAS_CPPUNIT
wdbUnitTest_SOURCES += 		src/callInterface/core/test/projTransformTest.cpp \
							src/callInterface/core/test/projTransformTest.h \
							src/callInterface/core/test/BuilderTest.h \
							src/callInterface/core/test/BuilderTest.cpp
#							src/callInterface/core/test/PolygonReaderTest.cpp \
#							src/callInterface/core/test/PolygonReaderTest.h
# HAS_CPPUNIT
endif


EXTRA_DIST +=				src/callInterface/core/test/wdb.mk \
							src/callInterface/core/test/Makefile.am \
							src/callInterface/core/test/Makefile.in

DISTCLEANFILES +=			src/callInterface/core/test/Makefile
