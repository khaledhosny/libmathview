#!/usr/bin/perl
#
# Copyright (C) 2000, Luca Padovani <luca.padovani@cs.unibo.it>.
#
# This file is part of GtkMathView, a Gtk widget for MathML.
#
# GtkMathView is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
# 
# GtkMathView is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GtkMathView; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
#
# For details, see the GtkMathView World-Wide-Web page,
# http://cs.unibo.it/~lpadovan, or send a mail to
# <luca.padovani@cs.unibo.it>
#

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
