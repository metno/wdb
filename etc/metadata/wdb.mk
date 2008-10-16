
#-----------------------------------------------------------------------------
# System Metadata
#-----------------------------------------------------------------------------

CORE_METADATA =			etc/metadata/wdb_party.in.csv \
						etc/metadata/wdb_organization.in.csv \
						etc/metadata/wdb_organizationtype.in.csv \
						etc/metadata/wdb_organizationalias.in.csv \
						etc/metadata/wdb_person.in.csv \
						etc/metadata/wdb_softwareversion.in.csv \
						etc/metadata/wdb_namespace.in.csv \
						etc/metadata/wdb_currentconfiguration.in.csv \
						etc/metadata/wdb_defaultCodespace.in.csv \
						etc/metadata/wdb_placeindeterminatetype.in.csv \
						etc/metadata/wdb_levelindeterminatetype.in.csv \
						etc/metadata/wdb_timeindeterminatetype.in.csv \
						etc/metadata/wdb_dataprovider.in.csv \
						etc/metadata/wdb_dataprovidercomment.in.csv \
						etc/metadata/wdb_gribgeneratingprocess.in.csv \
						etc/metadata/wdb_feltgeneratingprocess.in.csv \
						etc/metadata/wdb_wciuserdataprovider.in.csv \
						etc/metadata/wdb_dataprovidername.in.csv \
						etc/metadata/wdb_placedefinition.in.csv \
						etc/metadata/wdb_placeregulargrid.in.csv \
						etc/metadata/wdb_placename.in.csv \
						etc/metadata/wdb_statisticstype.in.csv \
						etc/metadata/wdb_physicalphenomenon.in.csv \
						etc/metadata/wdb_unit.in.csv \
						etc/metadata/wdb_siunitconversion.in.csv \
						etc/metadata/wdb_valueparameterusage.in.csv \
						etc/metadata/wdb_levelparameterusage.in.csv \
						etc/metadata/wdb_valueparameter.in.csv \
						etc/metadata/wdb_valuephysicalparameter.in.csv \
						etc/metadata/wdb_valuedimensionlessparameter.in.csv \
						etc/metadata/wdb_valuecodeparameter.in.csv \
						etc/metadata/wdb_valuestatisticsparameter.in.csv \
						etc/metadata/wdb_levelparameter.in.csv \
						etc/metadata/wdb_levelphysicalparameter.in.csv \
						etc/metadata/wdb_levelcodeparameter.in.csv \
						etc/metadata/wdb_qualityconfidencecode.in.csv \
						etc/metadata/wdb_qualitycontrolstage.in.csv \
						etc/metadata/wdb_qualityprocess.in.csv \
						etc/metadata/wdb_spatial_ref_sys.in.csv

GRIBLOAD_METADATA =		etc/metadata/wdb_griblevelparameterxref.in.csv \
						etc/metadata/wdb_gribparametertolevelxref.in.csv \
						etc/metadata/wdb_gribparameterxref.in.csv 

FELTLOAD_METADATA =		etc/metadata/wdb_feltlevelparameterxref.in.csv \
						etc/metadata/wdb_feltparametertolevelxref.in.csv \
						etc/metadata/wdb_feltparameterxref.in.csv \
						etc/metadata/wdb_feltparametertovaliddurationxref.in.csv

XMLLOAD_METADATA =		etc/metadata/wdb_xmlparameterxref.in.csv

KVALOBSLOAD_METADATA =	etc/metadata/wdb_kvalobsparameterxref.in.csv \
						etc/metadata/wdb_kvalobsvalidtimexref.in.csv
 

METADATA_SOURCES =		etc/metadata/wdbMetadataLoad.in.sql

sql_DATA +=		 		$(METADATA_SOURCES:.in.sql=.sql) \
						$(CORE_METADATA:.in.csv=.csv) \
						$(GRIBLOAD_METADATA:.in.csv=.csv) \
						$(FELTLOAD_METADATA:.in.csv=.csv) \
						$(XMLLOAD_METADATA:.in.csv=.csv) \
						$(KVALOBSLOAD_METADATA:.in.csv=.csv)

CLEANFILES +=		 	$(METADATA_SOURCES:.in.sql=.sql) \
						$(CORE_METADATA:.in.csv=.csv) \
						$(GRIBLOAD_METADATA:.in.csv=.csv) \
						$(FELTLOAD_METADATA:.in.csv=.csv) \
						$(XMLLOAD_METADATA:.in.csv=.csv) \
						$(KVALOBSLOAD_METADATA:.in.csv=.csv)

EXTRA_DIST +=			$(CORE_METADATA) \
						$(GRIBLOAD_METADATA) \
						$(FELTLOAD_METADATA) \
						$(XMLLOAD_METADATA) \
						$(KVALOBSLOAD_METADATA) \
						$(METADATA_SOURCES) \
						etc/metadata/wdb.mk \
						etc/metadata/Makefile.am \
						etc/metadata/Makefile.in

DISTCLEANFILES +=		etc/metadata/Makefile


# Local Makefile Targets
#-----------------------------------------------------------------------------

etc/metadata/all: $(METADATA_SOURCES:.in.sql=.sql) $(CORE_METADATA:.in.csv=.csv) $(GRIBLOAD_METADATA:.in.csv=.csv) $(FELTLOAD_METADATA:.in.csv=.csv) $(XMLLOAD_METADATA:.in.csv=.csv) $(KVALOBSLOAD_METADATA:.in.csv=.csv)

etc/metadata/clean: clean
