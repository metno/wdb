#-----------------------------------------------------------------------------
# gribWrite unit tests
#-----------------------------------------------------------------------------

if HAS_CPPUNIT
wdbUnitTest_SOURCES += test/utility/gribWrite/test/gribPointTest.h \
                test/utility/gribWrite/test/gribPointTest.cpp \
                test/utility/gribWrite/gribPoint.cpp \
                test/utility/gribWrite/gribPoint.h
wdbUnitTest_LDFLAGS += $(gribWrite_LDADD)
# HAS_CPPUNIT
endif

DISTCLEANFILES += 		test/utility/gribWrite/test/Makefile

EXTRA_DIST +=           test/utility/gribWrite/test/wdb.mk \
                test/utility/gribWrite/test/Makefile.am \
                test/utility/gribWrite/test/Makefile.in
