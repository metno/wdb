
#-----------------------------------------------------------------------------
# wdbLogHandler
#-----------------------------------------------------------------------------

# Library
#-----------------------------------------------------------------------------

lib_LIBRARIES += 			libwdbLog.a

libwdbLog_a_SOURCES = 		src/common/logHandler/wdbLogHandler.cpp
include_HEADERS += 			src/common/logHandler/wdbLogHandler.h

libwdbLog_a_CPPFLAGS = 		$(AM_CPPFLAGS) -fPIC

EXTRA_DIST +=				src/common/logHandler/wdb.mk \
							src/common/logHandler/Makefile.am \
							src/common/logHandler/Makefile.in

DISTCLEANFILES +=			src/common/logHandler/Makefile


# Local Makefile Targets
#-----------------------------------------------------------------------------

src/common/logHandler/all: libwdbLog.a

src/common/logHandler/clean: clean