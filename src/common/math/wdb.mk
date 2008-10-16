
#-----------------------------------------------------------------------------
# wdbMath
#-----------------------------------------------------------------------------

# Library
#-----------------------------------------------------------------------------

noinst_LIBRARIES +=			libwdbMath.a

libwdbMath_a_SOURCES =		src/common/math/wdbMath.h \
							src/common/math/wdbMath.cpp \
							src/common/math/wdbMathC.h \
							src/common/math/wdbMathC.cpp
							
libwdbMath_a_CPPFLAGS = 	-fPIC

EXTRA_DIST +=				src/common/math/wdb.mk \
							src/common/math/Makefile.am \
							src/common/math/Makefile.in

DISTCLEANFILES +=			src/common/math/Makefile


# Local Makefile Targets
#-----------------------------------------------------------------------------

src/common/math/all: libwdbMath.a

src/common/math/clean: clean

src/common/math/install: install
