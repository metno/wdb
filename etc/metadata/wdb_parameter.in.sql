-- Value Parameters
SELECT wci.addparameter( 'altitude', null, null, null, null, null, null, 'm' );
SELECT wci.addparameter( 'air potential temperature', null, null, null, null, null, null, 'K' );
SELECT wci.addparameter( 'air pressure', null, null, null, null, null, null, 'Pa' );
SELECT wci.addparameter( 'air pressure', 'sea level', null, null, null, null, null, 'Pa' );
SELECT wci.addparameter( 'air temperature', null, null, null, null, null, null, 'K' );
SELECT wci.addparameter( 'air temperature', null, null, null, null, null, 'maximum within days', 'K' );
SELECT wci.addparameter( 'air temperature', null, null, null, null, null, 'minimum within days', 'K' );
SELECT wci.addparameter( 'air velocity', null, 'upward', null, null, null, null, 'm/s' );
SELECT wci.addparameter( 'cloud area fraction', null, null, null, null, null, null, '%' );
SELECT wci.addparameter( 'cloud area fraction', null, null, 'atmosphere layer', null, null, null, '%' );
SELECT wci.addparameter( 'cloud liquid water content of atmosphere layer', null, null, null, null, null, null, 'ratio' );
SELECT wci.addparameter( 'convective precipitation amount', null, null, null, null, null, null, 'kg/m2' );
SELECT wci.addparameter( 'convective snowfall amount', null, null, null, null, null, null, 'kg/m2' );
SELECT wci.addparameter( 'dew point temperature', null, null, null, null, null, null, 'K' );
SELECT wci.addparameter( 'geopotential', null, null, null, null, null, null, 'm2/s2' );
SELECT wci.addparameter( 'geopotential height', null, null, null, null, null, null, 'm' );
SELECT wci.addparameter( 'lagrangian tendency of surface air pressure', null, null, null, null, null, null, 'Pa/s' );
SELECT wci.addparameter( 'land area fraction', null, null, null, null, null, null, '%' );
SELECT wci.addparameter( 'large scale precipitation amount', null, null, null, null, null, null, 'kg/m2' );
SELECT wci.addparameter( 'large scale snowfall amount', null, null, null, null, null, null, 'kg/m2' );
SELECT wci.addparameter( 'latent heat flux', 'surface', 'upward', null, null, null, null, 'W/m2' );
SELECT wci.addparameter( 'lwe precipitation rate', null, null, null, null, null, null, 'm/s' );
SELECT wci.addparameter( 'lwe thickness of convective precipitation amount', null, null, null, null, null, null, 'm' );
SELECT wci.addparameter( 'lwe thickness of convective snowfall amount', null, null, null, null, null, null, 'm' );
SELECT wci.addparameter( 'lwe thickness of large scale precipitation amount', null, null, null, null, null, null, 'm' );
SELECT wci.addparameter( 'lwe thickness of large scale snowfall amount', null, null, null, null, null, null, 'm' );
SELECT wci.addparameter( 'lwe thickness of precipitation amount', null, null, null, null, null, null, 'm' );
SELECT wci.addparameter( 'lwe thickness of snowfall amount', null, null, null, null, null, null, 'm' );
SELECT wci.addparameter( 'lwe thickness of surface snow amount', null, null, null, null, null, null, 'm' );
SELECT wci.addparameter( 'moisture content of soil layer', null, null, null, null, null, null, 'kg/m2' );
SELECT wci.addparameter( 'precipitation amount', null, null, null, null, null, null, 'kg/m2' );
SELECT wci.addparameter( 'snowfall amount', null, null, null, null, null, null, 'kg/m2' );
SELECT wci.addparameter( 'relative humidity', null, null, null, null, null, null, '%' );
SELECT wci.addparameter( 'roughness length', 'surface', null, null, null, null, null, 'm' );
SELECT wci.addparameter( 'sea floor depth below geoid', null, null, null, null, null, null, 'm' );
SELECT wci.addparameter( 'sea ice area fraction', null, null, null, null, null, null, '%' );
SELECT wci.addparameter( 'sea surface temperature', null, null, null, null, null, null, 'K' );
SELECT wci.addparameter( 'sea surface wave significant height', null, null, null, null, null, null, 'm' );
SELECT wci.addparameter( 'sensible heat flux', 'surface', 'upward', null, null, null, null, 'W/m2' );
SELECT wci.addparameter( 'shortwave flux', 'surface', 'downwelling', 'air', null, null, null, 'W/m2' );
SELECT wci.addparameter( 'soil moisture content', null, null, null, null, null, null, 'kg/m2' );
SELECT wci.addparameter( 'specific humidity', null, null, null, null, null, null, 'none' );
SELECT wci.addparameter( 'tendency of surface air pressure', null, null, null, null, null, null, 'Pa/s' );
SELECT wci.addparameter( 'thunderstorm probability', null, null, null, null, null, null, '%' );
SELECT wci.addparameter( 'visibility', null, null, 'air', null, null, null, 'm' );
SELECT wci.addparameter( 'wind', null, 'x', null, null, null, null, 'm/s' );
SELECT wci.addparameter( 'wind', null, 'y', null, null, null, null, 'm/s' );


-- Level Parameters
SELECT wci.addparameter( 'atmosphere sigma coordinate', null, null, null, null, null, null, 'none' );
SELECT wci.addparameter( 'height above ground', null, null, null, null, null, null, 'm' );
SELECT wci.addparameter( 'height above reference ellipsoid', null, null, null, null, null, null, 'm' );
SELECT wci.addparameter( 'isobaric surface', null, null, null, null, null, null, 'Pa' );


-- Create the test namespace
SELECT wci.begin( 'wdb', 999, 999, 999 );
SELECT wci.copyparameternamespace( 0 );


-- 
-- Alternatives
--
-- ALT: SELECT wci.addparameter( 'tendency of surface air pressure', null, null, null, null, null, null, 'Pa/s' );
-- GRIB4|air|K.m2/(kg.s)|scalar
-- GRIB5|icao standard atmospheric reference|m|scalar
--SELECT wci.addparameter( 'geometric height', null, null, null, null, null, null, 'm' );
-- GRIB9: standard deviation of height
--SELECT wci.addparameter( 'equivalent thickness at stp of atmosphere o3 content', null, null, null, null, null, null, 'm' );
--SELECT wci.addparameter( 'virtual temperature', null, null, null, null, null, null, 'K' );
--SELECT wci.addparameter( 'pseudo equivalent potential temperature', null, null, null, null, null, null, 'K' );
--SELECT wci.addparameter( 'dew point temperature', null, null, null, null, null, null, 'K' );
--SELECT wci.addparameter( 'dew point depression', null, null, null, null, null, null, 'K' );
--SELECT wci.addparameter( 'air temperature lapse rate', null, null, null, null, null, null, 'K/m' );
--21|radar spectra 1|WMO Code Table 2
--22|radar spectra 2|WMO Code Table 2
--23|radar spectra 3|WMO Code Table 2
--24|parcel lifted index|K|scalar
--SELECT wci.addparameter( 'air temperature anomaly', null, null, null, null, null, null, 'K' );
--SELECT wci.addparameter( 'air pressure anomaly', null, null, null, null, null, null, 'Pa' );
--SELECT wci.addparameter( 'geopotential height anomaly', null, null, null, null, null, null, 'm' );
--28|wave spectra 1|WMO Code Table 2
--29|Wave Spectra 3|WMO Code Table 2
--30|Wave Spectra 3|WMO Code Table 2
--SELECT wci.addparameter( 'wind from direction', null, null, null, null, null, null, 'rad' );
--SELECT wci.addparameter( 'wind speed', null, null, null, null, null, null, 'm/s' );
--35|stream function|m2/s|scalar
--36|potential air|m2/s|scalar
--37|montgomery stream function|m2/s2|scalar
--38|sigma coordinate vertical|/s|scalar
-- ALT: SELECT wci.addparameter( 'vertical air velocity expressed as tendency of pressure', null, null, null, null, null, null, 'Pa/s' );
--41|absolute vorticity|/s|scalar
--42|absolute divergence|/s|scalar
--43|relative vorticity|/s|scalar
--44|relative divergence|/s|scalar
--45|vertical shear|/s|u vector
--46|vertical shear|/s|v vector
--SELECT wci.addparameter( 'direction of sea water velocity', null, null, null, null, null, null, 'rad' );
--SELECT wci.addparameter( 'sea water speed', null, null, null, null, null, null, 'm/s' );
--SELECT wci.addparameter( 'sea water velocity', null, 'eastward', null, null, null, null, 'm/s' );
--SELECT wci.addparameter( 'sea water velocity', null, 'northward', null, null, null, null, 'm/s' );
--53|mixing ratio humidity|Mixing ratio humidity
--54|precipitable water|kg/m2|scalar
--55|vapour|Pa|scalar
--56|saturation deficit|Pa|scalar
--57|evaporation|kg/m2|scalar
--58|cloud ice|kg/m2|scalar
--SELECT wci.addparameter( 'precipitation flux', null, null, null, null, null, null, 'kg/(m2.s)' );
--SELECT wci.addparameter( 'snowfall flux', null, null, null, null, null, null, 'kg/(m2.s)' );
--SELECT wci.addparameter( 'snow amount', 'surface', null, null, null, null, null, 'kg/m2' );
--SELECT wci.addparameter( 'snow thickness', 'surface', null, null, null, null, null, 'm' );
--67|mixed layer depth|m|scalar
--68|transient thermocline depth|m|scalar
--69|main thermocline depth|m|scalar
--70|main thermocline anomaly|m|scalar
--SELECT wci.addparameter( 'convective cloud area fraction', null, null, null, null, null, null, '%' );
--76|cloud water|kg/m2|scalar
--77|best lifted index|K|scalar
--80|precipitable water|K|scalar
--82|deviation from mean sea level|m|scalar
--SELECT wci.addparameter( 'albedo', 'surface', null, null, null, null, null, 'm' );
--85|soil|K|scalar
--87|land vegetation|NA
--SELECT wci.addparameter( 'sea water salinity', null, null, null, null, null, null, 'none' );
--89|water|kg/m3|scalar
--90|water run-off|kg/m2|scalar
--92|ice thickness|m|scalar
--93|ice drift direction|NA
--94|ice drift|m/s|scalar
--95|ice drift|m/s|u vector
--96|ice drift|m/s|v vector
--97|ice growth rate|m/s|scalar
--98|ice divergence|/s|scalar
--99|snow melt|kg/m2|scalar
--ALT: SELECT wci.addparameter( 'significant height of wind and swell waves', null, null, null, null, null, null, 'm' );
--102|significant wind waves height|m|scalar
--105|significant swell waves height|m|scalar
--115|long-wave|W/m2|scalar
--116|short-wave|W/m2|scalar
--117|global|W/m2|scalar
--118|brightness|K|scalar
--119|wave number|W/(m.sr)|u vector
--120|wave number|W/(m.sr)|v vector
--123|boundary layer dissipation|W/m2|scalar
--124|momentum flux|N/m2|u vector
--125|momentum flux|N/m2|v vector
--126|wind mixing|J|scalar
--SELECT wci.addparameter( 'surface temperature', null, null, null, null, null, null, 'K' );
--189|sunshine|s|scalar
--228|rain|kg/m2|scalar
--SELECT wci.addparameter( 'undefined', null, null, null, null, null, null, 'none' );
--351|geometrical sea depth|m|scalar
--6100|total precipitation|mm|scalar
--10351|sea bottom topography|m|scalar
--10291|stokes drift|m/s|u vector
--10292|stokes drift|m/s|v vector
--10299|wind|rad|scalar
--10301|sea surface|m|scalar
--10302|sea current|m/s|u vector
--10303|sea current|m/s|v vector
--10309|surface stress|m2/s2|u vector
--10310|surface stress|m2/s2|v vector
--10314|horizontal eddy viscosity|m2/s|scalar
--10340|sea ice concentration|%|scalar
--10341|sea ice depth|m|scalar
--10342|sea ice drift|m/s|u vector
--10343|sea ice drift|m/s|v vector
--10371|inorganic nitrate|kg/m3|scalar
--10372|phosphate|kg/m3|scalar
--10373|silicate|kg/m3|scalar
--10374|detritus|kg/m3|scalar
--10375|diatomer|kg/m3|scalar
--10376|flagellates|kg/m3|scalar
--10377|oxygen|kg/m3|scalar
--127|image data|WMO Code Table 2
--800|yr weather symbol|www.yr.no
--10293|ice presence|Boolean value
--10295|normalized stress|Hav og kyst
--10296|drag coefficient|Hav og kyst
--10297|friction velocity|Hav og kyst
--10298|mpp processor map|Hav og kyst
--10307|salinity|Practical salinity scale
--0|null parameter|Used for reference in cross-reference parameters to NULL
--101|wind waves direction|NA
--104|swell waves direction|NA
--107|primary waves direction|NA
--109|secondary waves direction|NA
--184|sea albedo|NA
--719|probability of precipitation|Probability of precipitation
--720|probability of precipitation >0.1mm|Probability of precipitation exceeding 0.1 mm
--721|probability of precipitation >0.2mm|Probability of precipitation exceeding 0.2 mm
--722|probability of precipitation >0.5mm|Probability of precipitation exceeding 0.5 mm
--723|probability of precipitation >1.0mm|Probability of precipitation exceeding 1.0 mm
--724|probability of precipitation >2.0mm|Probability of precipitation exceeding 2.0 mm
--725|probability of precipitation >5.0mm|Probability of precipitation exceeding 5.0 mm
--730|probability of air temperature +/-1.5|Probability of yr temperature +/-1.5 degree Kelvin
--731|probability of air temperature +/-2.5|Probability of yr temperature +/-2.5 degree Kelvin
--801|probability of yr weather symbol|Probability of the correctness of the yr weather symbol
--802|probability of wind strength|Probability of the correctness of yr wind parameters
--1104|probability of fog|NA
--1106|vessel icing index|PROFF
--10349|wtsurf|Hav and kyst
--10352|alfaF|Hav and kyst
--10353|alfaU|Hav and kyst
--10354|alfaV|Hav and kyst
--10355|river map 1|Hav and kyst
--10356|river map 2|Hav and kyst
--10357|mpp processor map|Hav and kyst
--103|mean|wind waves|s|scalar
--106|mean|swell waves|s|scalar
--108|mean|primary waves|s|scalar
--110|mean|secondary waves|s|scalar
--112|accumulated|long wave|W/m2|scalar
--113|accumulated|top atmosphere short-wave |W/m2|scalar
--114|accumulated|top atmosphere long-wave |W/m2|scalar
--SELECT wci.addparameter( 'longwave radiation', 'surface', null, null, null, null, null, 'W.s/m2' );
--142|accumulated|large scale precipitation|m|scalar
--143|accumulated|convective precipitation|m|scalar
--151|mean|sea level|Pa|scalar
--189|accumulated|sunshine|s|scalar
--700|10th percentile|air|K|scalar
--701|25th percentile|air|K|scalar
--702|50th percentile|air|K|scalar
--703|75th percentile|air|K|scalar
--704|90th percentile|air|K|scalar
--710|10th percentile accumulated|precipitation|kg/m2|scalar
--711|25th percentile accumulated|precipitation|kg/m2|scalar
--712|50th percentile accumulated|precipitation|kg/m2|scalar
--713|75th percentile accumulated|precipitation|kg/m2|scalar
--714|90th percentile accumulated|precipitation|kg/m2|scalar
--10201|peak|wave|s|scalar
--10202|significant|wave|s|scalar
--10203|peak|wave|rad|scalar
--10204|mean|wave|rad|scalar
--10210|significant|wind wave|m|scalar
--10211|peak|wind wave|s|scalar
--10212|peak|wind wave|rad|scalar
--10214|mean|wind wave|rad|scalar
--10220|significant|swell wave|m|scalar
--10221|peak|swell wave|s|scalar
--10222|peak|swell wave|rad|scalar
--10223|mean|swell wave|s|scalar
--10224|mean|swell wave|rad|scalar
