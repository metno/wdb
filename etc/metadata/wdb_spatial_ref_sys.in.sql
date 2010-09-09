select wci.begin( 'wdb', 0, 0, 0 );
select wci.addsrid( 'ECWMF Spherical Earth', '+proj=longlat +a=6367470.0 +towgs84=0,0,0 +no_defs' );
select wci.addsrid( 'Hirlam 10 SRID', '+proj=ob_tran +o_proj=longlat +lon_0=-40 +o_lat_p=22 +a=6367470.0 +no_defs' );
select wci.addsrid( 'Hirlam 20 SRID', '+proj=ob_tran +o_proj=longlat +lon_0=0 +o_lat_p=25 +a=6367470.0 +no_defs' );
select wci.addsrid( 'PROFET SRID', '+proj=ob_tran +o_proj=longlat +lon_0=-24 +o_lat_p=23.5 +a=6367470.0 +no_defs' );
select wci.addsrid( 'MetNo Polar Stereographic', '+proj=stere +lat_0=90 +lon_0=58 +lat_ts=60 +a=6371000 +units=m +no_defs' );
select wci.addsrid( 'Oslo Polar Stereographic', '+proj=stere +lat_0=90 +lon_0=24 +lat_ts=60 +a=6371000 +units=m +no_defs' );
select wci.addsrid( 'SeNorge UTM', '+proj=utm +lon_0=15 +datum=WGS84 +units=m +no_defs' );
select wci.addsrid( 'MetNo Longitude Latitude', '+proj=longlat +a=6367470.0 +no_defs' );