
#-----------------------------------------------------------------------------
# WDB Call Interface Core Implementation
#-----------------------------------------------------------------------------

libwciCoreNoPostgres_la_SOURCES = \
						src/callInterface/core/wdb_geos.h \
						src/callInterface/core/wdb_geos.c

include src/callInterface/core/wciRead/wdb.mk

foo:
	@echo $(wdb_la_SOURCES)

wdb_la_SOURCES +=		src/callInterface/core/psqlTupleInterface.h \
						src/callInterface/core/psqlTupleInterface.c \
						src/callInterface/core/wciSession.h \
						src/callInterface/core/wciSession.cpp \
						src/callInterface/core/wciSetup.cpp \
						src/callInterface/core/getDataProvider.h \
						src/callInterface/core/getDataProvider.c \
						src/callInterface/core/readWhereClause.h \
						src/callInterface/core/readWhereClause.cpp \
						src/callInterface/core/readQuery.cpp \
						$(libwciCoreNoPostgres_la_SOURCES)


# This contains all files that may be (relatively) meaningfully used outside of a postgresql server
check_LTLIBRARIES +=	libwciCoreNoPostgres.la

libwciCoreNoPostgres_la_SOURCES +=\
						src/callInterface/core/projTransform.h \
						src/callInterface/core/projTransform.cpp
						
WCICORE_SOURCES =		src/callInterface/core/wciSession.in.sql \
						src/callInterface/core/readQuery.in.sql \
						src/callInterface/core/readWhereClause.in.sql \
						src/callInterface/core/readDataProvider.in.sql \
						src/callInterface/core/getDataProvider.in.sql \
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
