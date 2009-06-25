
#-----------------------------------------------------------------------------
# wdbProjection
#-----------------------------------------------------------------------------

# Library
#-----------------------------------------------------------------------------

lib_LIBRARIES +=			libwdbProj.a

libwdbProj_a_SOURCES =		src/common/projection/WdbProjection.cpp \
						    src/common/projection/GridGeometry.cpp
							
include_HEADERS += 			src/common/projection/WdbProjection.h \
						    src/common/projection/PlaceSpecification.h \
						    src/common/projection/wdbProjectionCInterface.h \
							src/common/projection/GridGeometry.h \
							src/common/projection/gridRotate.h
 
						    
libwdbProj_a_LIBADD = 		libwdbExcept.a

libwdbProj_a_CPPFLAGS = 	$(AM_CPPFLAGS) -fPIC

EXTRA_DIST +=				src/common/projection/wdb.mk \
							src/common/projection/Makefile.am \
							src/common/projection/Makefile.in

DISTCLEANFILES +=			src/common/projection/Makefile


# Local Makefile Targets
#-----------------------------------------------------------------------------

src/common/projection/all: libwdbProj.a

src/common/projection/clean: clean

src/common/projection/install: install
