
#-----------------------------------------------------------------------------
# wdbException
#-----------------------------------------------------------------------------

lib_LIBRARIES +=			libwdbExcept.a

libwdbExcept_a_SOURCES =	src/common/exception/wdbException.h
							
include_HEADERS += 			src/common/exception/wdbException.h 

libwdbExcept_a_CPPFLAGS = 	-fPIC

EXTRA_DIST +=				src/common/exception/wdb.mk \
							src/common/exception/Makefile.am \
							src/common/exception/Makefile.in

DISTCLEANFILES +=			src/common/exception/Makefile


# Local Makefile Targets
#-----------------------------------------------------------------------------

src/common/exception/all: libwdbExcept.la

src/common/exception/clean: clean
