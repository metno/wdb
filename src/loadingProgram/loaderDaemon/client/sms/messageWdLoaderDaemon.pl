#!/usr/bin/perl

use strict;
use warnings;


# modelname/hour
my $loadPath = 'hirlam12/00';

my $daemonAddress = 'prologdev1';
my $daemonPort = 7894;
my $daemon = $daemonAddress.':'.$daemonPort;

my $url = 'http://'.$daemon.'/'.$loadPath;

print "Contacting wdb loaderDaemon $url\n";

my $result = `wget -O- $url 2>/dev/null`;
print "$result\n";
if ( $result =~ /^ok/ ) {
	print "$url successfully loaded by wdb loaderDaemon\n";
}
else {
	print "$url: Unable to connect to server, or server was unable to find file\n";
}
