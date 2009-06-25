
#-----------------------------------------------------------------------------
# wdbMath
#-----------------------------------------------------------------------------

# Library
#-----------------------------------------------------------------------------

lib_LIBRARIES +=			libwdbMath.a

libwdbMath_a_SOURCES =		src/common/math/wdbMath.h \
							src/common/math/wdbMath.cpp \
							src/common/math/wdbMathC.h \
							src/common/math/wdbMathC.cpp

include_HEADERS += 			src/common/math/wdbMath.h \
							src/common/math/wdbMathC.h
							
libwdbMath_a_CPPFLAGS = 	$(AM_CPPFLAGS) -fPIC

EXTRA_DIST +=				src/common/math/wdb.mk \
							src/common/math/Makefile.am \
							src/common/math/Makefile.in

DISTCLEANFILES +=			src/common/math/Makefile


# Local Makefile Targets
#-----------------------------------------------------------------------------

src/common/math/all: libwdbMath.a

src/common/math/clean: clean

src/common/math/install: install
