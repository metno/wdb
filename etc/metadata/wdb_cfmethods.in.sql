-- CF Methods
select wci.addcfmethods('maximum within days', 'Maximum', 'max' );
select wci.addcfmethods('minimum within days', 'Minimum', 'min' );
select wci.addcfmethods('mean within days', 'Mean', 'mean' );
select wci.addcfmethods('10th percentile', '10th percentile value aggregated over data series (data version)', '10th percentile of' );
select wci.addcfmethods('25th percentile', '25th percentile value aggregated over data series (data version)', '25th percentile of' );
select wci.addcfmethods('50th percentile', '50th percentile value aggregated over data series (data version)', '50th percentile of' );
select wci.addcfmethods('75th percentile', '75th percentile value aggregated over data series (data version)', '75th percentile of' );
select wci.addcfmethods('90th percentile', '90th percentile value aggregated over data series (data version)', '90th percentile of' );
select wci.addcfmethods('local 20th percentile', '20th percentile value aggregated over the local area', 'local 20th percentile of' );
select wci.addcfmethods('local 50th percentile', '50th percentile value aggregated over the local area', 'local 50th percentile of' );
select wci.addcfmethods('local 80th percentile', '80th percentile value aggregated over the local area', 'local 80th percentile of' );

