-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- 
-- wdb - weather and water data storage
--
-- Copyright (C) 2007 met.no
--
--  Contact information:
--  Norwegian Meteorological Institute
--  Box 43 Blindern
--  0313 OSLO
--  NORWAY
--  E-mail: wdb@met.no
--
--  This is free software; you can redistribute it and/or modify
--  it under the terms of the GNU General Public License as published by
--  the Free Software Foundation; either version 2 of the License, or
--  (at your option) any later version.
--
-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


CREATE UNIQUE INDEX XAK1Wdb_GribGeneratingprocess ON __WDB_SCHEMA__.gribgeneratingprocess
(
	GeneratingCenterId,
	GeneratingProcessId,
	GeneratingProcessValidFrom
);


CREATE UNIQUE INDEX XAK1Wdb_OidValue ON __WDB_SCHEMA__.OidValue
(
       DataProviderId,
       ReferenceTime,
       PlaceId,
       ValueParameterId,
       LevelParameterId,
       LevelFrom,
       LevelTo,
       LevelIndeterminateCode,
       ValidTimeFrom,
       ValidTimeTo,
       ValidTimeIndeterminateCode,
       DataVersion
);

CREATE INDEX cascadingDeleteOids_OidValue ON __WDB_SCHEMA__.oidvalue
(
	value
);

CREATE UNIQUE INDEX XAK1Wdb_FloatValue ON __WDB_SCHEMA__.FloatValue
(
       DataProviderId,
       ReferenceTime,
       DataVersion,
       PlaceId,
       ValueParameterId,
       LevelParameterId,
       LevelFrom,
       LevelTo,
       LevelIndeterminateCode,
       ValidTimeFrom,
       ValidTimeTo,
       ValidTimeIndeterminateCode
);


CREATE UNIQUE INDEX XAK1Wdb_LevelIndeterminateType ON __WDB_SCHEMA__.LevelIndeterminateType
(
       LevelIndeterminateType
);

CREATE UNIQUE INDEX XAK1Wdb_Organization ON __WDB_SCHEMA__.Organization
(
       OrganizationName
);

CREATE UNIQUE INDEX XAK1Wdb_PhysicalPhenomenon ON __WDB_SCHEMA__.PhysicalPhenomenon
(
       LengthPower,
       MassPower,
       TimePower,
       LuminosityPower,
       ElectricCurrentPower,
       TemperaturePower,
       SubstanceAmountPower
);

CREATE UNIQUE INDEX XAK1Wdb_PlaceIndeterminateType ON __WDB_SCHEMA__.PlaceIndeterminateType
(
       PlaceIndeterminateType
);

CREATE UNIQUE INDEX XAK1SoftwareVersion ON __WDB_SCHEMA__.softwareversion
(
	SoftwareName,
	SoftwareVersionCode
);

CREATE UNIQUE INDEX XAK1Wdb_TimeIndeterminateType ON __WDB_SCHEMA__.TimeIndeterminateType
(
       TimeIndeterminateType
);



CREATE INDEX XIE1Wdb_FloatValue ON __WDB_SCHEMA__.FloatValue
(
       DataProviderId,
       ValidTimeFrom,
       ValueParameterId
);

CREATE INDEX XIE2Wdb_FloatValue ON __WDB_SCHEMA__.FloatValue
(
       PlaceId,
       ValidTimeFrom,
       ValueParameterId
);

CREATE INDEX XIE3Wdb_FloatValue ON __WDB_SCHEMA__.FloatValue
(
       ValidTimeFrom,
       PlaceId,
       Valueparameterid
);

CREATE INDEX XIE4Wdb_FloatValue ON __WDB_SCHEMA__.FloatValue
(
       ValidTimeFrom,
       DataProviderId,
       ValueParameterId
);


CREATE INDEX XIE1Wdb_PlacePoint ON __WDB_SCHEMA__.placepoint
       USING GIST ( location GIST_GEOMETRY_OPS );


CREATE UNIQUE INDEX XAK1Wdb_ValueStandardParameter ON __WDB_SCHEMA__.valuephysicalparameter
(
    valueparameterunitname,
	valueparameterusage
);

CREATE UNIQUE INDEX XAK1Wdb_LevelStandardParameter ON __WDB_SCHEMA__.levelphysicalparameter
(
    levelparameterunitname,
	levelparameterusage
);



CREATE INDEX XIE0Wdb_OidValue ON __WDB_SCHEMA__.OidValue
(
	   DataProviderId,
       PlaceId,
       ValueParameterId,
       ReferenceTime,
       DataVersion,
       LevelParameterId,
       LevelFrom,
       LevelTo,
       LevelIndeterminateCode,
       ValidTimeFrom,
       ValidTimeTo,
       ValidTimeIndeterminateCode,
       ConfidenceCode,
       ValueStoretime,
	   ValueId,
       Value
);
