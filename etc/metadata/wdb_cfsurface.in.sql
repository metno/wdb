--select wci.addcfstandardname( 'air pressure', 'Pressure' );
--select wci.addcfstandardname( 'tendency of air pressure', 'Pressure tendency' );
--select wci.addcfstandardname( 'geopotential', 'Geopotential' );
--select wci.addcfstandardname( 'geopotential height', 'Geopotential height' );
--select wci.addcfstandardname( 'geometric height', 'Altitude' );
--select wci.addcfstandardname( 'equivalent_thickness_at_stp_of_atmosphere_o3_content', 'Total ozone' );
--select wci.addcfstandardname( 'air temperature', 'Temperature' );
--select wci.addcfstandardname( 'virtual temperature', 'Virtual temperature' );
--select wci.addcfstandardname( 'air potential temperature', 'Potential temperature' );
--select wci.addcfstandardname( 'pseudo equivalent potential temperature', 'Pseudo-adiabatic or equivalent potential temperature' );
--select wci.addcfstandardname( 'dew point temperature', 'Dew point temperature' );
--select wci.addcfstandardname( 'dew point depression', 'Dew point depression or deficit' );
--select wci.addcfstandardname( 'air temperature lapse rate', 'Lapse rate' );
--select wci.addcfstandardname( 'visibility', 'Visibility' );
--select wci.addcfstandardname( 'air temperature anomaly', 'Temperature anomaly' );
--select wci.addcfstandardname( 'air pressure anomaly', 'Pressure anomaly' );
--select wci.addcfstandardname( 'geopotential height anomaly', 'Geopotential heigh anomaly' );
--select wci.addcfstandardname( 'wind from direction', 'Wind direction (from which it is blowing)' );
--select wci.addcfstandardname( 'winds speed', 'Wind Speed' );
--select wci.addcfstandardname( 'wind', 'Wind' );
--select wci.addcfstandardname( 'direction of sea water velocity', 'Direction of current' );
--select wci.addcfstandardname( 'sea speed', 'Speed of current' );
--select wci.addcfstandardname( 'sea velocity', 'Component of current' );
--select wci.addcfstandardname( 'specific humidity', 'Specific humidity' );
--select wci.addcfstandardname( 'precipitation flux', 'Precipitation rate' );
--select wci.addcfstandardname( 'thunderstorm probability', 'Thunderstorm probability' );
--select wci.addcfstandardname( 'precipitation amount', 'Total precipitation' );
--select wci.addcfstandardname( 'large scale precipitation amount', 'Large scale (non-convective) precipitation' );
--select wci.addcfstandardname( 'convective precipitation amount', 'Convective precipitation' );
--SELECT wci.addcfstandardname( 'snowfall flux', 'Snowfall rate water equivalent');
--SELECT wci.addcfstandardname( 'snow amount', 'Water equivalent of accumulated snow depth');
--SELECT wci.addcfstandardname( 'snow thickness', 'Snow depth');
--SELECT wci.addcfstandardname( 'cloud area fraction', 'Total cloud cover');
--SELECT wci.addcfstandardname( 'convective cloud area fraction', 'Convective cloud cover');
--SELECT wci.addcfstandardname( 'low cloud area fraction', 'Low cloud cover');
--SELECT wci.addcfstandardname( 'medium cloud area fraction', 'Medium cloud cover');
--SELECT wci.addcfstandardname( 'high cloud area fraction', 'High cloud cover');
--SELECT wci.addcfstandardname( 'sea water salinity', 'Salinity');

select wci.addcfsurface('sea level', 'MSL');
select wci.addcfsurface('surface', 'Lower boundary of the atmosphere');

select wci.addcfcomponent('eastward', 'U-component of vector');
select wci.addcfcomponent('northward', 'V-component of vector');
select wci.addcfcomponent('upward', 'Upward component');
select wci.addcfcomponent('downward', 'Upward component');
select wci.addcfcomponent('downwelling', 'Downwelling');

select wci.addcfmedium('air', 'Air');
select wci.addcfmedium('atmosphere layer', 'Atmosphere layer');


select wci.addcfmethods('maximum within days', 'Maximum', 'max' );
select wci.addcfmethods('minimum within days', 'Minimum', 'min' );
select wci.addcfmethods('10th percentile', 'Minimum', 'min' );
select wci.addcfmethods('local 20th percentile', '20th percentile value aggregated over the local area', 'local 20th percentile of' );
select wci.addcfmethods('local 50th percentile', '50th percentile value aggregated over the local area', 'local 50th percentile of' );
select wci.addcfmethods('local 80th percentile', '80th percentile value aggregated over the local area', 'local 80th percentile of' );