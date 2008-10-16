
#-----------------------------------------------------------------------------
# WDB GribLoad Component
#-----------------------------------------------------------------------------

noinst_PROGRAMS +=			gribWrite

gribWrite_SOURCES =		test/utility/gribWrite/main.cpp \
				test/utility/gribWrite/commandLineParser.cpp \
				test/utility/gribWrite/commandLineParser.h \
				test/utility/gribWrite/gribWriter.cpp \
				test/utility/gribWrite/gribWriter.h \
				test/utility/gribWrite/gribPoint.cpp \
				test/utility/gribWrite/gribPoint.h 

EXTRA_DIST +=			test/utility/gribWrite/wdb.mk \
				test/utility/gribWrite/Makefile.am \
				test/utility/gribWrite/Makefile.in

DISTCLEANFILES +=		test/utility/gribWrite/Makefile


# Local Makefile Targets
#-----------------------------------------------------------------------------

test/utility/gribWrite/all: gribWrite

test/utility/gribWrite/clean: clean
