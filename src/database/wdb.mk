
#-----------------------------------------------------------------------------
# Database Source
#-----------------------------------------------------------------------------

lib_LTLIBRARIES +=		wdb.la
wdb_la_LDFLAGS = 		-module  $(LIBTOOL_VERSION_FLAG)
wdb_la_CPPFLAGS = 		-I`pg_config --includedir-server`
wdb_la_LIBADD =			libwdbProjection.a libwdbMath.a libwdbException.a
wdb_la_SOURCES =		src/database/wdbSetup.c \
						src/database/databaseProjection.cpp


DATABASE_SOURCES =		src/database/wdbConstraintDefinitions.in.sql \
						src/database/wdbSchemaDefinitions.in.sql \
						src/database/wdbBaseTables.in.sql \
						src/database/wdbDataProviderTables.in.sql \
						src/database/wdbPlaceDefinitionTables.in.sql \
						src/database/wdbParameterTables.in.sql \
						src/database/wdbValueTables.in.sql \
						src/database/wdbGribDefinitions.in.sql \
						src/database/wdbFeltDefinitions.in.sql \
						src/database/wdbXmlDefinitions.in.sql \
						src/database/wdbIndexDefinitions.in.sql \
						src/database/wdbMaterializedView.in.sql \
						src/database/wdbTestDefinitions.in.sql \
						src/database/wdbTriggerDefinitions.in.sql \
						src/database/wdbUserDefinitions.in.sql \
						src/database/wdbViewDefinitions.in.sql \
						src/database/wciViewDefinitions.in.sql \
						src/database/wdbLoaderBaseDefinitions.in.sql

sql_DATA += 			$(DATABASE_SOURCES:.in.sql=.sql)

CLEANFILES +=			$(DATABASE_SOURCES:.in.sql=.sql)

EXTRA_DIST += 			$(DATABASE_SOURCES) \
						src/database/wdb.mk \
						src/database/Makefile.am \
						src/database/Makefile.in

DISTCLEANFILES +=		src/database/Makefile


# Local Makefile Targets
#-----------------------------------------------------------------------------

src/database/all: $(DATABASE_SOURCES:.in.sql=.sql)

src/database/clean: clean
