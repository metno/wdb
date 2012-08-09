select wci.begin('wdb', 999, 999, 999 );
select wci.addorupdateplacepoint('oslo', st_geomfromtext('POINT(10.7464 59.9111)',4030), '-infinity', 'infinity' );
select wci.addorupdateplacepoint('sortland',st_geomfromtext('POINT(15.4161 68.6931)',4030), '-infinity', 'infinity' );
select wci.addorupdateplacepoint('test point 0',st_geomfromtext('POINT(-40 68.1332)',4030), '-infinity', 'infinity' );
select wci.addorupdateplacepoint('test point 1',st_geomfromtext('POINT(-25 5.6)',4030), '-infinity', 'infinity' );
select wci.addorupdateplacepoint('test point 2',st_geomfromtext('POINT(-7.1 -11.3)',4030), '-infinity', 'infinity' );
select wci.addorupdateplacepoint('test point 3',st_geomfromtext('POINT(15.2 55.75)',4030), '-infinity', 'infinity' );
select wci.addorupdateplacepoint('test point 4',st_geomfromtext('POINT(15.2 55.41)',4030), '-infinity', 'infinity' );
select wci.addorupdateplacepoint('test point 5',st_geomfromtext('POINT(15 56)',4030), '-infinity', 'infinity' );
select wci.addorupdateplacepoint('test point 6',st_geomfromtext('POINT(17 55.97)',4030), '-infinity', 'infinity' );
select wci.addorupdateplacepoint('test point 7',st_geomfromtext('POINT(5 56)',4030), '-infinity', 'infinity' );
select wci.addorupdateplacepoint('test point 8',st_geomfromtext('POINT(15 60.2)',4030), '-infinity', 'infinity' );
select wci.addorupdateplacepoint('test point 9',st_geomfromtext('POINT(15 60)',4030), '-infinity', 'infinity' );
select wci.addorupdateplacepoint('test point 10',st_geomfromtext('POINT(12.0 61.0)',4030), '-infinity', 'infinity' );
select wci.addorupdateplacepoint('test point 11',st_geomfromtext('POINT(11.5 60.8)',4030), '-infinity', 'infinity' );
select wci.addorupdateplacepoint('test point 12',st_geomfromtext('POINT(11.6 60.9)',4030), '-infinity', 'infinity' );
select wci.addorupdateplacepoint('test point 13',st_geomfromtext('POINT(12.1 61.1)',4030), '-infinity', 'infinity' );
select wci.addorupdateplacepoint('test point 14',st_geomfromtext('POINT(11.9 60.9)',4030), '-infinity', 'infinity' );

select wci.addorupdateplacepoint('test point 15',st_geomfromtext('POINT(13.9 60.4)',4030), '2010-01-01T00:00:00+00', '2011-01-01T00:00:00+00' );
select wci.addorupdateplacepoint('test point 15',st_geomfromtext('POINT(13.8 60.5)',4030), '2011-01-01T00:00:00+00', '2012-01-01T00:00:00+00' );

select wci.addorupdateplacepoint('test point 16',st_geomfromtext('POINT(13.9 60.4)',4030), '2011-07-01T00:00:00+00', 'infinity' );

