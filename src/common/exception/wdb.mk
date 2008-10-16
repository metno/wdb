
#-----------------------------------------------------------------------------
# wdbException
#-----------------------------------------------------------------------------

lib_LIBRARIES +=			libwdbException.a

libwdbException_a_SOURCES =	src/common/exception/wdbException.cpp \
							src/common/exception/wdbEmptyResultException.cpp \
							src/common/exception/wdbDoNotLoadException.cpp
							
include_HEADERS += \
							src/common/exception/wdbException.h \
							src/common/exception/wdbEmptyResultException.h \
							src/common/exception/wdbDoNotLoadException.h
 

libwdbException_a_CPPFLAGS = -fPIC

EXTRA_DIST +=				src/common/exception/wdb.mk \
							src/common/exception/Makefile.am \
							src/common/exception/Makefile.in

DISTCLEANFILES +=			src/common/exception/Makefile


# Local Makefile Targets
#-----------------------------------------------------------------------------

src/common/exception/all: libwdbException.la

src/common/exception/clean: clean
