#!/usr/bin/perl

if ($#ARGV != 0 || ($ARGV[0] ne "c" && $ARGV[0] ne "cc" && $ARGV[0] ne "Perl" && $ARGV[0] ne "Makefile")) {
    print "Usage: mklicence.pl { c | cc | Perl | Makefile } <licence-file >output-file\n";
    exit 0;
}

$lang     = $ARGV[0];
$filename = $ARGV[1];

if ($lang eq "c") {
    print "/*\n";
    while (<STDIN>) { print " * $_"; }
    print " */\n";
} elsif ($lang eq "Perl" || $lang eq "Makefile") {
    if ($lang eq "Perl") { print "#! /usr/bin/perl\n"; }
    while (<STDIN>) { print "# $_"; }
} else {
    while (<STDIN>) { print "// $_"; }
}
