-- Organizations
SELECT wci.addOrganization( 'WDB Project' 'international organization', 'WDB', '2006-08-31', '2999-12-31', 'The WDB (Weather and Water Database) open-source project' );
SELECT wci.addOrganization( 'World Meteorological Organization', 'international organization', 'WMO', '2000-01-01', '2999-12-31', 'WMO is a special department within the United Nations. WMO currently has 187 member states.' );
SELECT wci.addOrganization( 'Det Norske Meteorologiske Institutt', 'government organization', 'met.no', '2000-01-01', '2999-12-31', 'The Norwegian Meteorological Institute' );

-- Persons
SELECT wci.addPerson( 'Michael', 'Akinde', 'Dr', 'Dear Michael' 'MiA', NULL, 'male', NULL, NULL, '2000-01-01', '2999-12-31', NULL );

-- Software
SELECT wci.addSoftwareVersion( 'WDB', '__WDB_VERSION__', '2000-01-01', '2999-12-31', 'The WDB System' );
