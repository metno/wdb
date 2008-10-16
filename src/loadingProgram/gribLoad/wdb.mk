
#-----------------------------------------------------------------------------
# WDB GribLoad Component
#-----------------------------------------------------------------------------

pkglib_PROGRAMS +=		gribLoad

gribLoad_SOURCES =		src/loadingProgram/gribLoad/gribLoad.cpp \
						src/loadingProgram/gribLoad/GribDatabaseInterface.h \
						src/loadingProgram/gribLoad/gribDatabaseConnection.cpp \
						src/loadingProgram/gribLoad/gribDatabaseConnection.h \
						src/loadingProgram/gribLoad/gribField.cpp \
						src/loadingProgram/gribLoad/gribField.h \
						src/loadingProgram/gribLoad/gribGridDefinition.cpp \
						src/loadingProgram/gribLoad/gribGridDefinition.h \
						src/loadingProgram/gribLoad/GribHandleReader.cpp \
						src/loadingProgram/gribLoad/GribHandleReader.h \
						src/loadingProgram/gribLoad/transactors/gribTransactorDataProvider.h \
						src/loadingProgram/gribLoad/transactors/gribTransactorValueParameter.h \
						src/loadingProgram/gribLoad/transactors/gribTransactorLevelParameter.h \
						src/loadingProgram/gribLoad/wmoCodeTables.h
					
gribLoad_CPPFLAGS = 	-I$(top_srcdir)/src/loadingProgram/loaderBase
						
gribLoad_LDADD = 		libwdbLoaderBase.la \
						-lwdbConfiguration -lwdbProjection -lwdbException -lwdbLogHandler \
						-lboost_filesystem

EXTRA_DIST +=			src/loadingProgram/gribLoad/wdb.mk \
						src/loadingProgram/gribLoad/Makefile.am \
						src/loadingProgram/gribLoad/Makefile.in

DISTCLEANFILES +=		src/loadingProgram/gribLoad/Makefile


# Local Makefile Targets
#-----------------------------------------------------------------------------

src/loadingProgram/gribLoad/all: gribLoad

src/loadingProgram/gribLoad/clean: clean
