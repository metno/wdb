select wci.begin( 'wdb', 999, 999, 999 );
select wci.addplaceregulargrid('test grid, regular',2,2,0.100,0.100,0.0000,0.0000,'+proj=longlat +a=6367470.0 +towgs84=0,0,0 +no_defs');
select wci.addplaceregulargrid('test grid, rotated',2,2,0.100,0.100,0.0000,0.0000,'+proj=ob_tran +o_proj=longlat +lon_0=-40 +o_lat_p=22 +a=6367470.0 +no_defs');
select wci.addplaceregulargrid('hirlam 10 grid',248,400,0.100,0.100,5.7500,-13.2500,'+proj=ob_tran +o_proj=longlat +lon_0=-40 +o_lat_p=22 +a=6367470.0 +no_defs');
select wci.addplaceregulargrid('hirlam 20 grid',468,378,0.200,0.200,-46.5000,-36.5000,'+proj=ob_tran +o_proj=longlat +lon_0=0 +o_lat_p=25 +a=6367470.0 +no_defs');
select wci.addplaceregulargrid('proff grid',850,1100,0.036,0.036,-6.7390,-16.0390,'+proj=ob_tran +o_proj=longlat +lon_0=-24 +o_lat_p=23.5 +a=6367470.0 +no_defs');
select wci.addplaceregulargrid('ecmwf grid',187,109,0.500,0.500,-21.0000,30.0000,'+proj=longlat +a=6367470.0 +towgs84=0,0,0 +no_defs');
