
#-----------------------------------------------------------------------------
# WDB FeltLoad Component
#-----------------------------------------------------------------------------

pkglib_PROGRAMS += feltLoad

feltLoad_SOURCES =  \
	src/loadingProgram/feltLoad/main.cpp \
	src/loadingProgram/feltLoad/feltConstants.h \
	src/loadingProgram/feltLoad/feltTypeConversion.h \
	src/loadingProgram/feltLoad/feltTypeConversion.cpp \
	src/loadingProgram/feltLoad/FeltFile.cpp \
	src/loadingProgram/feltLoad/FeltFile.h \
	src/loadingProgram/feltLoad/FeltField.cpp \
	src/loadingProgram/feltLoad/FeltField.h \
	src/loadingProgram/feltLoad/FeltLoader.cpp \
	src/loadingProgram/feltLoad/FeltLoader.h \
	src/loadingProgram/feltLoad/FeltGridDefinition.h \
	src/loadingProgram/feltLoad/FeltGridDefinition.cpp \
	src/loadingProgram/feltLoad/FeltDatabaseInterface.h \
	src/loadingProgram/feltLoad/FeltDatabaseConnection.cpp \
	src/loadingProgram/feltLoad/FeltDatabaseConnection.h \
	src/loadingProgram/feltLoad/transactors/ReadDataProvider.h \
	src/loadingProgram/feltLoad/transactors/ReadValueParameter.h \
	src/loadingProgram/feltLoad/transactors/ReadAdditionalLevels.h \
	src/loadingProgram/feltLoad/transactors/ReadLevelParameter.h \
	src/loadingProgram/feltLoad/transactors/ReadValidDuration.h

	
#	src/loadingProgram/feltLoad/transactors/feltTransactorWci.h \
#	src/loadingProgram/feltLoad/transactors/feltTransactorValue.h \
#	src/loadingProgram/feltLoad/transactors/feltTransactorPlaceDefinition.h \
#	src/loadingProgram/feltLoad/transactors/feltTransactorSrid.h 
	

feltLoad_CPPFLAGS = $(milib_CFLAGS) -I$(top_srcdir)/src/loadingProgram/loaderBase
feltLoad_LDADD = $(milib_LIBS) \
				libwdbLoaderBase.la -lwdbConfiguration -lwdbProjection -lwdbException -lwdbLogHandler

EXTRA_DIST +=				src/loadingProgram/feltLoad/wdb.mk \
							src/loadingProgram/feltLoad/Makefile.am \
							src/loadingProgram/feltLoad/Makefile.in

DISTCLEANFILES +=			src/loadingProgram/feltLoad/Makefile


# Local Makefile Targets
#-----------------------------------------------------------------------------

src/loadingProgram/feltLoad/all: feltLoad

src/loadingProgram/feltLoad/clean: clean
