lib_LTLIBRARIES += libwdbLoaderBase.la

libwdbLoaderBase_la_SOURCES = \
	src/loadingProgram/loaderBase/LoaderConfiguration.cpp \
	src/loadingProgram/loaderBase/LoaderDatabaseConnection.cpp \
	src/loadingProgram/loaderBase/wdbSetup.cpp \
	src/loadingProgram/loaderBase/transactors/loaderTransactorPlaceDefinition.h \
	src/loadingProgram/loaderBase/transactors/loaderTransactorSrid.h \
	src/loadingProgram/loaderBase/transactors/loaderTransactorValue.h \
	src/loadingProgram/loaderBase/transactors/loaderTransactorUnit.h \
	src/loadingProgram/loaderBase/transactors/loaderTransactorWci.h
	
pkginclude_HEADERS += \
	src/loadingProgram/loaderBase/LoaderDatabaseInterface.h \
	src/loadingProgram/loaderBase/LoaderConfiguration.h \
	src/loadingProgram/loaderBase/LoaderDatabaseConnection.h \
	src/loadingProgram/loaderBase/wdbSetup.h \
	src/loadingProgram/loaderBase/WdbLevel.h
	

libwdbLoaderBase_la_CPPFLAGS = $(milib_CFLAGS)
libwdbLoaderBase_la_LIBADD = $(milib_LIBS) \
				-lwdbProjection \
				-lwdbException \
				-lwdbConfiguration \
				-lwdbLogHandler 

EXTRA_DIST +=				src/loadingProgram/loaderBase/wdb.mk \
							src/loadingProgram/loaderBase/Makefile.am \
							src/loadingProgram/loaderBase/Makefile.in

DISTCLEANFILES +=			src/loadingProgram/loaderBase/Makefile


# Local Makefile Targets
#-----------------------------------------------------------------------------

src/loadingProgram/loaderBase/all: libwdbLoaderBase.la

src/loadingProgram/loaderBase/clean: clean
