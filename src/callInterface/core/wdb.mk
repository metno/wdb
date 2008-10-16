
#-----------------------------------------------------------------------------
# WDB Call Interface Core Implementation
#-----------------------------------------------------------------------------

wci_la_SOURCES +=		src/callInterface/core/binaryInterpretation.cpp \
						src/callInterface/core/wciGetGridPoint.cpp \
						src/callInterface/core/psqlTupleInterface.h \
						src/callInterface/core/psqlTupleInterface.c \
						src/callInterface/core/wciSession.h \
						src/callInterface/core/wciSession.cpp \
						src/callInterface/core/wciSetup.cpp \
						src/callInterface/core/getDataProvider.h \
						src/callInterface/core/getDataProvider.c \
						src/callInterface/core/readWhereClause.h \
						src/callInterface/core/readWhereClause.cpp \
						src/callInterface/core/readQuery.cpp \
						src/callInterface/core/interpolation.c \
						src/callInterface/core/placePointInternal.c \
						src/callInterface/core/placePointInternal.h \
						src/callInterface/core/ProjectionCache.cpp \
						src/callInterface/core/ProjectionCache.h \
						$(libwciCoreNoPostgres_la_SOURCES)

check_LTLIBRARIES +=	libwciCoreNoPostgres.la

libwciCoreNoPostgres_la_SOURCES =\
						src/callInterface/core/projTransform.h \
						src/callInterface/core/projTransform.cpp

WCICORE_SOURCES =		src/callInterface/core/wciExtractGridData.in.sql \
						src/callInterface/core/wciSession.in.sql \
						src/callInterface/core/readQuery.in.sql \
						src/callInterface/core/readWhereClause.in.sql \
						src/callInterface/core/readDataProvider.in.sql \
						src/callInterface/core/wciWriteInternals.in.sql \
						src/callInterface/core/wciInterpolation.in.sql

wcicoredir = 			$(wcidir)/core

wcicore_DATA = 			$(WCICORE_SOURCES:.in.sql=.sql)

CLEANFILES += 			$(WCICORE_SOURCES:.in.sql=.sql)

EXTRA_DIST +=			$(WCICORE_SOURCES) \
						src/callInterface/core/wdb.mk \
						src/callInterface/core/Makefile.am \
						src/callInterface/core/Makefile.in

DISTCLEANFILES +=		src/callInterface/core/Makefile


# Local Makefile Targets
#-----------------------------------------------------------------------------

src/callInterface/core/all: libwciCoreNoPostgres.a $(WCICORE_SOURCES:.in.sql=.sql)

src/callInterface/core/clean: clean
