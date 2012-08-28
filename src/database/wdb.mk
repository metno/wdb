
#-----------------------------------------------------------------------------
# Database Source
#-----------------------------------------------------------------------------

wdb_la_SOURCES +=		src/database/databaseProjection.cpp


DATABASE_SOURCES =		src/database/schemaDefinitions.in.sql \
						src/database/partyTables.in.sql \
						src/database/configurationTables.in.sql \
						src/database/timeTables.in.sql \
						src/database/qualityTables.in.sql \
						src/database/namespaceTables.in.sql \
						src/database/dataProviderTables.in.sql \
						src/database/placeDefinitionTables.in.sql \
						src/database/cfParameterTables.in.sql \
						src/database/parameterUnitTables.in.sql \
						src/database/parameterTables.in.sql \
						src/database/floatValueTables.in.sql \
						src/database/gridValueTables.in.sql \
						src/database/materializedView.in.sql \
						src/database/dataProviderTriggers.in.sql \
						src/database/parameterTriggers.in.sql \
						src/database/gridValueTriggers.in.sql \
						src/database/wdbUserDefinitions.in.sql \
						src/database/configurationViews.in.sql \
						src/database/partyViews.in.sql \
						src/database/dataProviderViews.in.sql \
						src/database/placeDefinitionViews.in.sql \
						src/database/placeDefinitionTriggers.in.sql \
						src/database/cfParameterViews.in.sql \
						src/database/parameterViews.in.sql \
						src/database/parameterUnitViews.in.sql \
						src/database/floatValueViews.in.sql \
						src/database/gridValueViews.in.sql \
						src/database/adminDefinitions.in.sql \
						src/database/wdbTestDefinitions.in.sql

UPGRADE_SOURCES =		
#						src/database/wdbUpgrade0001.in.sh \
#						src/database/wdbUpgrade0002.in.sh \
#						src/database/wdbUpgrade0003.in.sh

nodist_pkglibexec_SCRIPTS += $(UPGRADE_SOURCES:.in.sh=.sh)

sql_DATA += 			$(DATABASE_SOURCES:.in.sql=.sql)

CLEANFILES +=			$(DATABASE_SOURCES:.in.sql=.sql) \
						$(UPGRADE_SOURCES:.in.sh=.sh)

EXTRA_DIST += 			$(DATABASE_SOURCES) \
						$(UPGRADE_SOURCES) \
						src/database/wdb.mk \
						src/database/Makefile.am \
						src/database/Makefile.in

DISTCLEANFILES +=		src/database/Makefile


# Local Makefile Targets
#-----------------------------------------------------------------------------

src/database/all: $(DATABASE_SOURCES:.in.sql=.sql)

src/database/clean: clean

#include src/database/upgrade0003/wdb.mk
 