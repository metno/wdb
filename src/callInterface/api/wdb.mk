
#-----------------------------------------------------------------------------
# WDB Call Interface API
#-----------------------------------------------------------------------------

WCIAPI_SOURCES = 		src/callInterface/api/wciWrite.in.sql \
						src/callInterface/api/wciRead.in.sql \
						src/callInterface/api/wciFetch.in.sql \
						src/callInterface/api/wciInfo.in.sql \
						src/callInterface/api/wciBrowse.in.sql \
						src/callInterface/api/wciBegin.in.sql \
						src/callInterface/api/wciEnd.in.sql \
						src/callInterface/api/dataprovider.in.sql \
						src/callInterface/api/placeDefinition.in.sql \
						src/callInterface/api/placegeo.in.sql \
						src/callInterface/api/regulargrid.in.sql \
						src/callInterface/api/parameterunit.in.sql \
						src/callInterface/api/listmetadata.in.sql \
						src/callInterface/api/wciVersion.in.sql \
						src/callInterface/api/loadPlaceDefinition.in.sql

wciapidir=				$(wcidir)/api

wciapi_DATA = 			$(WCIAPI_SOURCES:.in.sql=.sql)

CLEANFILES +=			$(WCIAPI_SOURCES:.in.sql=.sql)

EXTRA_DIST +=			$(WCIAPI_SOURCES) \
						src/callInterface/api/wdb.mk \
						src/callInterface/api/Makefile.am \
						src/callInterface/api/Makefile.in

DISTCLEANFILES +=		src/callInterface/api/Makefile


# Local Makefile Targets
#-----------------------------------------------------------------------------

src/callInterface/api/all: $(WCIAPI_SOURCES:.in.sql=.sql)

src/callInterface/api/clean: clean
