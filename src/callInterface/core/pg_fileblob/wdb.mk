
#-----------------------------------------------------------------------------
# WDB Call Interface Core Implementation
#-----------------------------------------------------------------------------

wci_la_SOURCES +=		src/callInterface/core/pg_fileblob/fileblob.cpp \
						src/callInterface/core/pg_fileblob/fileblobimpl_psql.cpp \
						src/callInterface/core/pg_fileblob/fileblobimpl_psql.h \
						$(libwciCoreFileBlobNoPostgres_la_SOURCES)

check_LTLIBRARIES +=	libwciCoreFileBlobNoPostgres.la

libwciCoreFileBlobNoPostgres_la_SOURCES =\
						src/callInterface/core/pg_fileblob/bstream.h \
						src/callInterface/core/pg_fileblob/database_query.c \
						src/callInterface/core/pg_fileblob/database_query.h \
						src/callInterface/core/pg_fileblob/fileblobImpl.cpp \
						src/callInterface/core/pg_fileblob/fileblobImpl.h \
						src/callInterface/core/pg_fileblob/fileblob.in.sql \
						src/callInterface/core/pg_fileblob/PathGenerator.cpp \
						src/callInterface/core/pg_fileblob/PathGenerator.h
						
						

PG_FILEBLOB_SOURCES =	src/callInterface/core/pg_fileblob/fileblob.in.sql

pgfileblobdir = 		$(wcidir)/core/pg_fileblob

pgfileblob_DATA = 		$(PG_FILEBLOB_SOURCES:.in.sql=.sql)

CLEANFILES += 			$(PG_FILEBLOB_SOURCES:.in.sql=.sql)

EXTRA_DIST +=			$(PG_FILEBLOB_SOURCES) \
						src/callInterface/core/pg_fileblob/wdb.mk \
						src/callInterface/core/pg_fileblob/Makefile.am \
						src/callInterface/core/pg_fileblob/Makefile.in

DISTCLEANFILES +=		src/callInterface/core/pg_fileblob/Makefile
