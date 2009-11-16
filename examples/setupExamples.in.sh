#!/bin/sh

TEST_WRITE=./testWrite

# accumulated values
$TEST_WRITE --dataprovider='test wci 5' --placename='hirlam 20' --reftime='2009-11-13 00:00:00+00' --validtimefrom='2009-11-13 00:00:00+00' --validtimeto='2009-11-13 03:00:00+00' --valueparameter='accumulated precipitation surface density'
$TEST_WRITE --dataprovider='test wci 5' --placename='hirlam 20' --reftime='2009-11-13 00:00:00+00' --validtimefrom='2009-11-13 00:00:00+00' --validtimeto='2009-11-13 06:00:00+00' --valueparameter='accumulated precipitation surface density'
$TEST_WRITE --dataprovider='test wci 5' --placename='hirlam 20' --reftime='2009-11-13 00:00:00+00' --validtimefrom='2009-11-13 00:00:00+00' --validtimeto='2009-11-13 09:00:00+00' --valueparameter='accumulated precipitation surface density'
$TEST_WRITE --dataprovider='test wci 5' --placename='hirlam 20' --reftime='2009-11-13 00:00:00+00' --validtimefrom='2009-11-13 00:00:00+00' --validtimeto='2009-11-13 12:00:00+00' --valueparameter='accumulated precipitation surface density'

# instant values
$TEST_WRITE --dataprovider='test wci 5' --placename='hirlam 20' --reftime='2009-11-13 00:00:00+00' --validtimefrom='2009-11-13 18:00:00+00' --validtimeto='2009-11-13 18:00:00+00' --valueparameter='air temperature' --levelfrom=2
$TEST_WRITE --dataprovider='test wci 5' --placename='hirlam 20' --reftime='2009-11-13 00:00:00+00' --validtimefrom='2009-11-13 18:00:00+00' --validtimeto='2009-11-13 18:00:00+00' --valueparameter='air pressure'
