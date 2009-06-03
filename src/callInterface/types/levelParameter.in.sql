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

CREATE TYPE __WCI_SCHEMA__.levelParameterTuple AS
(
    physicalPhenomena VARCHAR(80),
    levelDomain VARCHAR(80)
);

CREATE OR REPLACE FUNCTION __WCI_SCHEMA__.normalizeLevelParameter( param TEXT )
RETURNS text
AS '__WDB_LIBDIR__/__WCI_LIB__', 'normalizeWciLevelParameter'
LANGUAGE C IMMUTABLE STRICT;


CREATE OR REPLACE FUNCTION __WCI_SCHEMA__.getLevelParameterName( data __WCI_SCHEMA__.gridvalue )
RETURNS text AS
$BODY$
DECLARE
	ret text;
BEGIN
	ret := data.levelparametername;
	RETURN ret;
END;
$BODY$
LANGUAGE 'plpgsql' IMMUTABLE STRICT;


CREATE OR REPLACE FUNCTION __WCI_SCHEMA__.getLevelParameterName( data __WCI_SCHEMA__.levelParameterTuple )
RETURNS text AS
$BODY$
DECLARE
	ret text;
BEGIN
	ret := data.physicalPhenomena || ' ' || data.levelDomain;
	RETURN ret;
END;
$BODY$
LANGUAGE 'plpgsql' IMMUTABLE STRICT;



CREATE OR REPLACE FUNCTION
__WCI_SCHEMA__.getLevelParameterId(
        parameter text
)
RETURNS integer AS
$BODY$
DECLARE
        session __WCI_SCHEMA__.sessionData;
        ret integer;
BEGIN
    session := __WCI_SCHEMA__.getsessiondata();
	SELECT levelparameterid INTO ret
	FROM __WCI_SCHEMA__.levelparameter_mv
	WHERE
		levelparametername = parameter AND
		parameternamespaceid = session.parameterNameSpaceId;

	IF NOT FOUND THEN
		RAISE EXCEPTION 'Failed to identify levelparameter in namespace: %', parameter;
	END IF;

	RETURN ret;
END;
$BODY$
LANGUAGE 'plpgsql';


CREATE OR REPLACE FUNCTION
__WCI_SCHEMA__.getLevelParameterId(
        parameter text,
        unit text
)
RETURNS int AS
$BODY$
DECLARE
        session __WCI_SCHEMA__.sessionData;
        lparameter __WCI_SCHEMA__.levelparameter;
BEGIN
    session := __WCI_SCHEMA__.getsessiondata();

	SELECT * INTO lparameter FROM __WCI_SCHEMA__.levelparameter
	WHERE
		levelparametername = parameter AND
		levelunitname = unit AND
		levelparametercodespaceid = session.parameterNameSpaceId;

	IF NOT FOUND THEN
		DECLARE
			paramid int := nextval('__WDB_SCHEMA__.levelparameter_levelparameterid_seq');
			explodedParameter __WCI_SCHEMA__.levelParameterTuple;
		BEGIN
			RAISE DEBUG 'Inserting new level parameter into database: %', parameter;

			SELECT * INTO explodedParameter FROM __WCI_SCHEMA__.getLevelParameter(parameter);

			PERFORM __WCI_SCHEMA__.assertUnitIsPhenomenon(unit, explodedParameter.physicalPhenomena);

			BEGIN
				INSERT INTO __WDB_SCHEMA__.levelparameter 
					(levelparameterid, levelparametercodespaceid, levelparametertype) 
				VALUES 
					(paramid, session.parameternameSpaceId,'standard parameter');
	
				INSERT INTO __WDB_SCHEMA__.levelstandardparameter 
					(levelparameterid, levelparametercodespaceid, levelparametertype, levelparameterunitname, levelparameterusage)
				VALUES
					(paramid,session.parameternameSpaceId,'standard parameter', unit, explodedParameter.levelDomain);
	
				RAISE DEBUG 'Level parameter <%> got internal value %.', parameter, paramid;

			EXCEPTION WHEN unique_violation THEN
				RAISE DEBUG 'Insertion failed. This is probably because a concurrent process has added the same parameter. Attempting to read level parameter again.';
            	SELECT levelparameterid INTO paramid FROM __WCI_SCHEMA__.levelparameter
				WHERE
					levelparametername = parameter AND
					levelunitname = unit AND
					levelparametercodespaceid = session.parameterNameSpaceId;

				IF NOT FOUND THEN
					RAISE EXCEPTION 'Unable to allocate internal value for level parameter <%>', parameter;
				END IF;
	        END;

			RETURN paramid;
		END;
	END IF;

	RETURN lparameter.levelparameterid;
END;
$BODY$
LANGUAGE 'plpgsql';


CREATE OR REPLACE FUNCTION __WCI_SCHEMA__.lp_matches( val __WCI_SCHEMA__.gridvalue, param text[] )
RETURNS boolean AS
$BODY$
DECLARE
	currentParam __WCI_SCHEMA__.levelParameterTuple;
	item text;
	i int;
BEGIN
	--<<find_match>>
	--FOR i IN 1..array_upper( param, 1 )
	--LOOP
	--	currentParam := __WCI_SCHEMA__.getLevelParameter( param[i] );
	--	IF	( currentParam.physicalPhenomena = val.physicalphenomenon OR
	--		  currentParam.physicalPhenomena = '*' ) AND
	--		( currentParam.levelDomain = val.levelDomain OR
	--		  currentParam.levelDomain = '*' )
	--	THEN
	--		RETURN true;
	--	END IF;
	--END LOOP;
	
	RETURN false;
END;
$BODY$
LANGUAGE 'plpgsql' IMMUTABLE;

