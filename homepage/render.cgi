#!/usr/bin/perl

use strict;
use CGI;

my $q = new CGI;

my $source = $q->param('source');
my $fontsize = $q->param('fontsize');
my $tmp = `tempfile --suffix=_mathml | tr -d '\n'`;

open (OUT, ">$tmp");
print OUT $source;
close(OUT);

my $res = `mathmlsvg --font-size=$fontsize --verbose=3 --config=/projects/helm/public_html/software/mml-widget/gtkmathview-online.conf.xml --cut-filename=no $tmp 2>&1`;
unlink $tmp;
my $png_res = `rsvg $tmp.svg $tmp.png 2>&1`;
unlink "$tmp.svg";

open (MAIL, '|mail -s "GtkMathView Online" lpadovan@cs.unibo.it');
print MAIL "Source MathML:\n";
print MAIL "==============\n";
print MAIL $source;
print MAIL "\n\nmathmlsvg:\n";
print MAIL "==============\n";
print MAIL $res;
print MAIL "\n\nrsvg:\n";
print MAIL $png_res;
close(MAIL);

print <<EOF;
Content-type: image/png

EOF

my $png = `cat $tmp.png`;
print $png;
unlink "$tmp.png";

