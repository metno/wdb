-- Organizations
SELECT wci.addOrganization( 'WDB Project', 'WDB', 'international organization', '2006-08-31', '2999-12-31', 'The WDB (Weather and Water Database) open-source project' );
SELECT wci.addOrganization( 'World Meteorological Organization', 'WMO', 'international organization', '2000-01-01', '2999-12-31', 'WMO is a special department within the United Nations. WMO currently has 187 member states.' );
SELECT wci.addOrganization( 'Det Norske Meteorologiske Institutt', 'met.no', 'government organization', '2000-01-01', '2999-12-31', 'The Norwegian Meteorological Institute' );

-- Persons
SELECT wci.addPerson( 'Michael', 'Akinde', 'Dr', 'Hi Michael,', 'MiA', NULL, 'male', NULL, NULL, '2000-01-01', '2999-12-31', NULL );

-- Software
SELECT wci.addSoftwareVersion( 'WDB', '__WDB_VERSION__', '2000-01-01', '2999-12-31', 'The WDB System' );
