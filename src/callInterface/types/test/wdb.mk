
#-----------------------------------------------------------------------------
# WCI Types Unit Test Framework
#
# See: <top>/test/unit
#-----------------------------------------------------------------------------

if HAS_CPPUNIT
wdbUnitTest_SOURCES += 		src/callInterface/types/test/wciNamedIntegerTest.cpp \
							src/callInterface/types/test/wciNamedIntegerTest.h \
							src/callInterface/types/test/ValueParameterTypeTest.cpp \
							src/callInterface/types/test/ValueParameterTypeTest.h \
							src/callInterface/types/test/LevelParameterTypeTest.cpp \
							src/callInterface/types/test/LevelParameterTypeTest.h

#src/callInterface/types/test/locationTest.cpp 
#src/callInterface/types/test/locationTest.h 
# HAS_CPPUNIT
endif
							
EXTRA_DIST +=				src/callInterface/types/test/wdb.mk \
							src/callInterface/types/test/Makefile.am \
							src/callInterface/types/test/Makefile.in

DISTCLEANFILES +=			src/callInterface/types/test/Makefile
