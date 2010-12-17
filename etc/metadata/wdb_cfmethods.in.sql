-- CF Methods
select wci.addcfmethods('maximum within days', 'Maximum', 'max' );
select wci.addcfmethods('minimum within days', 'Minimum', 'min' );
select wci.addcfmethods('10th percentile', 'Minimum', 'min' );
select wci.addcfmethods('local 20th percentile', '20th percentile value aggregated over the local area', 'local 20th percentile of' );
select wci.addcfmethods('local 50th percentile', '50th percentile value aggregated over the local area', 'local 50th percentile of' );
select wci.addcfmethods('local 80th percentile', '80th percentile value aggregated over the local area', 'local 80th percentile of' );