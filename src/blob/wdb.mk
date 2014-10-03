
#-----------------------------------------------------------------------------
# WDB Blob Implementation
#-----------------------------------------------------------------------------

wdb_la_SOURCES +=		src/blob/fileblob.cpp \
						src/blob/fileblobimpl_psql.cpp \
						src/blob/fileblobimpl_psql.h \
						$(libwdbBlobNoPostgres_la_SOURCES)

check_LTLIBRARIES +=	libwdbBlobNoPostgres.la

libwdbBlobNoPostgres_la_SOURCES = \
						src/blob/FileId.h \
						src/blob/bstream.cpp \
						src/blob/bstream.h \
						src/blob/database_query.c \
						src/blob/database_query.h \
						src/blob/fileblobImpl.cpp \
						src/blob/fileblobImpl.h \
						src/blob/fileblob.in.sql \
						src/blob/PathGenerator.cpp \
						src/blob/PathGenerator.h \
						src/blob/LRUList.h

PG_FILEBLOB_SOURCES =	src/blob/fileblob.in.sql

sql_DATA += 			$(PG_FILEBLOB_SOURCES:.in.sql=.sql)

if HAS_CPPUNIT
wdbUnitTest_SOURCES += \
	src/blob/test/LRUFileListTest.cpp \
	src/blob/test/LRUFileListTest.h
	
wdbUnitTest_LDFLAGS += $(check_LTLIBRARIES)
endif

CLEANFILES += 			$(PG_FILEBLOB_SOURCES:.in.sql=.sql)

EXTRA_DIST +=			$(PG_FILEBLOB_SOURCES) \
						src/blob/wdb.mk \
						src/blob/Makefile.am \
						src/blob/Makefile.in

DISTCLEANFILES +=		src/blob/Makefile
