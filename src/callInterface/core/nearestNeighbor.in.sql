CREATE OR REPLACE FUNCTION __WCI_SCHEMA__.expandoverlap_metric(a geometry,
															   b geometry,
															   maxe double precision,
															   maxslice double precision)
RETURNS integer AS
$BODY$
BEGIN
    FOR i IN 0..maxslice LOOP
        IF st_expand(a,maxe*i/maxslice) && b THEN
            RETURN i;
        END IF;
    END LOOP; 
    RETURN 99999999;
END;
$BODY$
LANGUAGE 'plpgsql' IMMUTABLE;

DROP TYPE IF EXISTS __WCI_SCHEMA__.pgis_nn CASCADE;
CREATE TYPE __WCI_SCHEMA__.pgis_nn AS (nn_gid integer, nn_dist numeric(16,5));
   
CREATE OR REPLACE FUNCTION __WCI_SCHEMA__._pgis_fn_nn(geom1 geometry,
													  distguess double precision,
													  numnn integer,
													  maxslices integer,
													  lookupset varchar(150),
													  swhere varchar(5000),
													  sgid2field varchar(100),
													  sgeom2field varchar(100))
RETURNS SETOF __WCI_SCHEMA__.pgis_nn AS
$BODY$
DECLARE
    strsql text;
    rec __WCI_SCHEMA__.pgis_nn;
    ncollected integer;
    it integer;
--NOTE: it: the iteration we are currently at 
--start at the bounding box of the object (expand 0) and move up until it has collected more objects than we need or it = maxslices whichever event happens first
BEGIN
	IF geom1 IS NULL THEN
		RETURN;
	END IF;	
    ncollected := 0; it := 0;
    WHILE ncollected < numnn AND it <= maxslices LOOP
        strsql := 'SELECT currentit.' || sgid2field || ', st_distance(ref.geom, currentit.' || sgeom2field || ') as dist FROM ' || lookupset || '  as currentit, (SELECT geometry(''' || CAST(geom1 As text) || ''') As geom) As ref WHERE ' || swhere || ' AND st_distance(ref.geom, currentit.' || sgeom2field || ') <= ' || CAST(distguess As varchar(200)) || ' AND st_expand(ref.geom, ' || CAST(distguess*it/maxslices As varchar(100)) ||  ') && currentit.' || sgeom2field || ' AND __WCI_SCHEMA__.expandoverlap_metric(ref.geom, currentit.' || sgeom2field || ', ' || CAST(distguess As varchar(200)) || ', ' || CAST(maxslices As varchar(200)) || ') = ' || CAST(it As varchar(100)) || ' ORDER BY st_distance(ref.geom, currentit.' || sgeom2field || ') LIMIT ' || 
        CAST((numnn - ncollected) As varchar(200));
        -- RAISE NOTICE 'sql: %', strsql;
        FOR rec in EXECUTE (strsql) LOOP
            IF ncollected < numnn THEN
                ncollected := ncollected + 1;
                RETURN NEXT rec;
            ELSE
                EXIT;
            END IF;
        END LOOP;
        it := it + 1;
    END LOOP;
END
$BODY$
LANGUAGE 'plpgsql' STABLE;

CREATE OR REPLACE FUNCTION __WCI_SCHEMA__.nearestneighbor(geom1 		geometry,
													 	  distguess 	double precision,
													 	  numnn 		integer,
													 	  maxslices 	integer,
													 	  lookuptable 	varchar(150),
													 	  swhere 		varchar(5000),
													 	  sgid2field 	varchar(100),
													 	  sgeom2field 	varchar(100))
RETURNS SETOF __WCI_SCHEMA__.pgis_nn AS
$BODY$
    SELECT * FROM __WCI_SCHEMA__._pgis_fn_nn($1,$2, $3, $4, $5, $6, $7, $8);
$BODY$
  LANGUAGE 'sql' STABLE;
