
#-----------------------------------------------------------------------------
# WDB Call Interface Utility Functions
#-----------------------------------------------------------------------------

wdbCall_la_SOURCES += 	src/callInterface/util/conversion.h \
						src/callInterface/util/conversion.c \
						src/callInterface/util/tuple.h \
						src/callInterface/util/tuple.c
	
EXTRA_DIST +=			src/callInterface/util/wdb.mk \
						src/callInterface/util/Makefile.am \
						src/callInterface/util/Makefile.in 

DISTCLEANFILES +=		src/callInterface/util/Makefile


