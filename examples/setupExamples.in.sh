#!/bin/sh

TEST_WRITE=./testWrite

# accumulated values
$TEST_WRITE --dataprovider='test wci 5' --placename='hirlam 20 grid' --reftime='2009-11-13 00:00:00+00' --validtimefrom='2009-11-13 00:00:00+00' --validtimeto='2009-11-13 03:00:00+00' --valueparameter='visibility in air'
$TEST_WRITE --dataprovider='test wci 5' --placename='hirlam 20 grid' --reftime='2009-11-13 00:00:00+00' --validtimefrom='2009-11-13 00:00:00+00' --validtimeto='2009-11-13 06:00:00+00' --valueparameter='visibility in air'
$TEST_WRITE --dataprovider='test wci 5' --placename='hirlam 20 grid' --reftime='2009-11-13 00:00:00+00' --validtimefrom='2009-11-13 00:00:00+00' --validtimeto='2009-11-13 09:00:00+00' --valueparameter='visibility in air'
$TEST_WRITE --dataprovider='test wci 5' --placename='hirlam 20 grid' --reftime='2009-11-13 00:00:00+00' --validtimefrom='2009-11-13 00:00:00+00' --validtimeto='2009-11-13 12:00:00+00' --valueparameter='visibility in air'

# instant values
$TEST_WRITE --dataprovider='test wci 5' --placename='hirlam 20 grid' --reftime='2009-11-13 00:00:00+00' --validtimefrom='2009-11-13 18:00:00+00' --validtimeto='2009-11-13 18:00:00+00' --valueparameter='air temperature' --levelfrom=2
$TEST_WRITE --dataprovider='test wci 5' --placename='hirlam 20 grid' --reftime='2009-11-13 00:00:00+00' --validtimefrom='2009-11-13 18:00:00+00' --validtimeto='2009-11-13 18:00:00+00' --valueparameter='air pressure'
