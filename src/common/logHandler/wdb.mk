
#-----------------------------------------------------------------------------
# wdbLogHandler
#-----------------------------------------------------------------------------

# Library
#-----------------------------------------------------------------------------

#noinst_LIBRARIES +=			libwdbLogHandler.a
lib_LIBRARIES += libwdbLogHandler.a

libwdbLogHandler_a_SOURCES = src/common/logHandler/wdbLogHandler.cpp
include_HEADERS += src/common/logHandler/wdbLogHandler.h

libwdbLogHandler_a_CPPFLAGS =\
							-DSYSCONFDIR=\"$(sysconfdir)\" -fPIC

EXTRA_DIST +=				src/common/logHandler/wdb.mk \
							src/common/logHandler/Makefile.am \
							src/common/logHandler/Makefile.in

DISTCLEANFILES +=			src/common/logHandler/Makefile


# Local Makefile Targets
#-----------------------------------------------------------------------------

src/common/logHandler/all: libwdbLogHandler.a

src/common/logHandler/clean: clean