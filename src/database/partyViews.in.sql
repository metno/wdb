-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- 
-- wdb - weather and water data storage
--
-- Copyright (C) 2007 - 2012 met.no
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
SET SESSION client_min_messages TO 'warning';

CREATE VIEW __WCI_SCHEMA__.person_v AS
SELECT
    py.partyid,
    py.partytype,
    py.partyvalidfrom,
    py.partyvalidto,
    pn.firstname,
    pn.lastname,
    pn.title,
    pn.salutation,
    pn.initials,
    pn.signum,
    pn.gender,
    pn.namesuffix,
    pn.maritalstatus,
	pc.partycomment,
    pc.partycommentstoretime
FROM
	( __WDB_SCHEMA__.party AS py JOIN
	  __WDB_SCHEMA__.person AS pn
	  ON py.partyid = pn.partyid )  
	LEFT OUTER JOIN __WDB_SCHEMA__.partycomment AS pc
	ON py.partyid = pc.partyid;

REVOKE ALL ON __WCI_SCHEMA__.person_v FROM PUBLIC;
GRANT ALL ON __WCI_SCHEMA__.person_v TO wdb_admin;
GRANT SELECT ON __WCI_SCHEMA__.person_v TO wdb_read, wdb_write;



CREATE VIEW __WCI_SCHEMA__.organization_v AS
SELECT
    py.partyid,
    py.partytype,
    py.partyvalidfrom,
    py.partyvalidto,
    po.organizationname,
    po.organizationalias,
    po.organizationtype,
	pc.partycomment,
    pc.partycommentstoretime
FROM
	( __WDB_SCHEMA__.party AS py JOIN
	  __WDB_SCHEMA__.organization AS po
	  ON
	  py.partyid = po.partyid ) 
	LEFT OUTER JOIN
	__WDB_SCHEMA__.partycomment AS pc
	ON py.partyid = pc.partyid;

REVOKE ALL ON __WCI_SCHEMA__.organization_v FROM PUBLIC;
GRANT ALL ON __WCI_SCHEMA__.organization_v TO wdb_admin;
GRANT SELECT ON __WCI_SCHEMA__.organization_v TO wdb_read, wdb_write;


CREATE VIEW __WCI_SCHEMA__.softwareversion_v AS
SELECT
    py.partyid,
    py.partytype,
    py.partyvalidfrom,
    py.partyvalidto,
	ps.softwarename,
	ps.softwareversioncode,
	pc.partycomment,
    pc.partycommentstoretime
FROM
	( __WDB_SCHEMA__.party AS py JOIN
	  __WDB_SCHEMA__.softwareversion AS ps
	  ON py.partyid = ps.partyid )
	LEFT OUTER JOIN
	__WDB_SCHEMA__.partycomment AS pc
	ON py.partyid = pc.partyid;

REVOKE ALL ON __WCI_SCHEMA__.softwareversion_v FROM PUBLIC;
GRANT ALL ON __WCI_SCHEMA__.softwareversion_v TO wdb_admin;
GRANT SELECT ON __WCI_SCHEMA__.softwareversion_v TO wdb_read, wdb_write;
