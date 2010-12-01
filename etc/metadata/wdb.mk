
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
						etc/metadata/wdb_measure.in.csv \
						etc/metadata/wdb_configuration.in.csv \
						etc/metadata/wdb_defaultnamespace.in.csv \
						etc/metadata/wdb_placeindeterminatetype.in.csv \
						etc/metadata/wdb_levelindeterminatetype.in.csv \
						etc/metadata/wdb_timeindeterminatetype.in.csv \
						etc/metadata/wdb_dataprovider.in.csv \
						etc/metadata/wdb_dataprovidercomment.in.csv \
						etc/metadata/wdb_wciuserdataprovider.in.csv \
						etc/metadata/wdb_dataprovidername.in.csv \
						etc/metadata/wdb_placedefinition.in.csv \
						etc/metadata/wdb_placeregulargrid.in.csv \
						etc/metadata/wdb_placename.in.csv \
						etc/metadata/wdb_parameterfunctiontype.in.csv \
						etc/metadata/wdb_physicalphenomenon.in.csv \
						etc/metadata/wdb_unit.in.csv \
						etc/metadata/wdb_siunitconversion.in.csv \
						etc/metadata/wdb_valueparameterusage.in.csv \
						etc/metadata/wdb_levelparameterusage.in.csv \
						etc/metadata/wdb_valueparameter.in.csv \
						etc/metadata/wdb_valuemeasureparameter.in.csv \
						etc/metadata/wdb_valuedimensionlessparameter.in.csv \
						etc/metadata/wdb_valuecodeparameter.in.csv \
						etc/metadata/wdb_valuefunctionparameter.in.csv \
						etc/metadata/wdb_levelparameter.in.csv \
						etc/metadata/wdb_levelmeasureparameter.in.csv \
						etc/metadata/wdb_levelcodeparameter.in.csv \
						etc/metadata/wdb_qualityconfidencecode.in.csv \
						etc/metadata/wdb_qualitycontrolstage.in.csv \
						etc/metadata/wdb_qualityprocess.in.csv \
						etc/metadata/wdb_spatial_ref_sys.in.csv

METADATA_SOURCES =		etc/metadata/wdb_spatial_ref_sys.in.sql \
						etc/metadata/wdb_party.in.sql \
						etc/metadata/wdb_dataprovider.in.sql \
						etc/metadata/wdb_placepoint.in.sql \
						etc/metadata/wdb_placeregulargrid.in.sql \
						etc/metadata/wdb_unit.in.sql \
						etc/metadata/wdb_cfsurface.in.sql \
						etc/metadata/wdb_parameter.in.sql \
						etc/metadata/wdb_namespace.in.sql \
						etc/metadata/wdbMetadata.in.sql

sql_DATA +=		 		$(METADATA_SOURCES:.in.sql=.sql) \
						$(CORE_METADATA:.in.csv=.csv)

CLEANFILES +=		 	$(METADATA_SOURCES:.in.sql=.sql) \
						$(CORE_METADATA:.in.csv=.csv)

EXTRA_DIST +=			$(CORE_METADATA) \
						$(METADATA_SOURCES) \
						etc/metadata/wdb.mk \
						etc/metadata/Makefile.am \
						etc/metadata/Makefile.in

DISTCLEANFILES +=		etc/metadata/Makefile


# Local Makefile Targets
#-----------------------------------------------------------------------------

etc/metadata/all: $(METADATA_SOURCES:.in.sql=.sql) $(CORE_METADATA:.in.csv=.csv)

etc/metadata/clean: clean
