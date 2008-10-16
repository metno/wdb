
#-----------------------------------------------------------------------------
# WDB XmlLoad Component
#-----------------------------------------------------------------------------

pkglib_PROGRAMS +=		xmlLoad

xmlLoad_SOURCES =		src/loadingProgram/xmlLoad/xmlLoad.cpp \
						src/loadingProgram/xmlLoad/ProcessXml.cpp \
						src/loadingProgram/xmlLoad/ProcessXml.h \
						src/loadingProgram/xmlLoad/XmlDatabaseConnection.cpp \
						src/loadingProgram/xmlLoad/XmlDatabaseConnection.h \
						src/loadingProgram/xmlLoad/WdbLevel.h \
						src/loadingProgram/xmlLoad/transactors/xmlTransactorWci.h \
						src/loadingProgram/xmlLoad/transactors/ReadXmlDataProvider.h \
						src/loadingProgram/xmlLoad/transactors/ReadXmlPlaceDefinition.h \
						src/loadingProgram/xmlLoad/transactors/WriteXmlPlaceDefinition.h \
						src/loadingProgram/xmlLoad/transactors/ReadXmlValueParameter.h \
						src/loadingProgram/xmlLoad/transactors/ReadXmlLevelParameter.h \
						src/loadingProgram/xmlLoad/transactors/WriteXmlValue.h
					
xmlLoad_LDADD = 		libwdbLoaderBase.la \
						-lwdbConfiguration -lwdbProjection -lwdbException -lwdbLogHandler \
						-lboost_filesystem \
						-lxml2

EXTRA_DIST +=			src/loadingProgram/xmlLoad/wdb.mk \
						src/loadingProgram/xmlLoad/Makefile.am \
						src/loadingProgram/xmlLoad/Makefile.in

DISTCLEANFILES +=		src/loadingProgram/xmlLoad/Makefile


# Local Makefile Targets
#-----------------------------------------------------------------------------

src/loadingProgram/xmlLoad/all: xmlLoad

src/loadingProgram/xmlLoad/clean: clean
