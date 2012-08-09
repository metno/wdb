select wci.begin('wdb', 999, 999, 999 );
select wci.addorupdateplacepolygon('test polygon 0',st_geomfromtext('POLYGON((11.34 60.75, 12.34 60.75, 12.34 61.25, 11.34 61.25, 11.34 60.75 ))',4030), '-infinity', 'infinity' );
