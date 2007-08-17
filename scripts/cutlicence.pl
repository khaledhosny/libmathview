#!/usr/bin/perl

$licence = 1;
while (<STDIN>) {
    if ($licence &&
	(/^\/\// ||
	 /^\x20\*/ ||
	 /^\/\*/)) {
    } else {
	$licence = 0;
	print "$_";
    }
}

