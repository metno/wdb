UPGRADE0003_SOURCES =		src/database/upgrade0003/placeDefinition.in.sql \
							src/database/upgrade0003/placeFunction.in.sql \
							src/database/upgrade0003/placeName.in.sql \
							src/database/upgrade0003/placeTrigger.in.sql \
							src/database/upgrade0003/prognosisTime.in.sql \
							src/database/upgrade0003/wciBrowse.in.sql

upgrade0003dir =			$(sqldir)/upgrade0003

upgrade0003_DATA = 			$(UPGRADE0003_SOURCES:.in.sql=.sql)

CLEANFILES +=				$(UPGRADE0003_SOURCES:.in.sql=.sql)

EXTRA_DIST +=				$(UPGRADE0003_SOURCES) \
							src/database/upgrade0003/wdb.mk \
							src/database/upgrade0003/Makefile.am \
							src/database/upgrade0003/Makefile.in

DISTCLEANFILES +=			src/database/upgrade0003/Makefile


# Local Makefile Targets
#-----------------------------------------------------------------------------

src/callInterface/api/all: 	$(UPGRADE0003_SOURCES:.in.sql=.sql)

src/callInterface/api/clean: clean
