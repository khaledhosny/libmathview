#!/usr/bin/perl

use strict;
use CGI;
use MIME::Base64 qw(encode_base64);
use Time::HiRes qw(gettimeofday tv_interval);

my $q = new CGI;

my $source = $q->param('source');
my $fontsize = $q->param('fontsize');
my $tmp = `tempfile --suffix=_mathml | tr -d '\n'`;

open (OUT, ">$tmp");
print OUT $source;
close(OUT);

my $mathmlsvg_version = `mathmlsvg --version`;

my $t_start = [gettimeofday];

my $res = `mathmlsvg --font-size=$fontsize --verbose=3 --config=/projects/helm/public_html/software/mml-widget/gtkmathview-online.conf.xml --cut-filename=no $tmp 2>&1`;
unlink $tmp;
my $t_mathmlsvg = [gettimeofday];

my $png_res = `rsvg $tmp.svg $tmp.png 2>&1`;
#my $png_res = `unset DISPLAY; /usr/bin/sodipodi -z -w 320 -e $tmp.png $tmp.svg 2>&1`;
unlink "$tmp.svg";
my $t_svgpng = [gettimeofday];

open (MAIL, '|mail -s "GtkMathView Online" lpadovan@cs.unibo.it');
print MAIL "Remote host: ", $q->remote_host(), "\n";
print MAIL "Source MathML:\n";
print MAIL "==============\n";
print MAIL $source;
print MAIL "\n\nmathmlsvg:\n";
print MAIL "==============\n";
print MAIL $res;
print MAIL "\n\nrsvg:\n";
print MAIL $png_res;
close(MAIL);

my $elapsed_mathmlsvg = tv_interval ($t_start, $t_mathmlsvg);
my $elapsed_svgpng = tv_interval ($t_mathmlsvg, $t_svgpng);

print <<EOF;
Content-type: text/html

<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN" "http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd">

<html xmlns="http://www.w3.org/1999/xhtml">
<head>
  <meta content="text/html; charset=ISO-8859-1" http-equiv="content-type">
  <link href="http://helm.cs.unibo.it/software/mml-widget/gtkmathview.css" rel="stylesheet" type="text/css"/>
  <title>GtkMathView Rendering Report</title>
</head>

<body>
  <h3>GtkMathView rendering report</h3>

<pre>$mathmlsvg_version</pre>

  <p>
    Conversion from MathML to SVG (<tt>mathmlsvg</tt>) took $elapsed_mathmlsvg seconds <br/>
    Conversion from SVG to PNG (<tt>rsvg</tt>) took $elapsed_svgpng seconds
  </p>

  <img src="data:image/png;base64,
EOF

open (PNG, "$tmp.png");
my $buf;
while (read(PNG, $buf, 60 * 57)) {
  print encode_base64($buf);
}
close (PNG);
unlink "$tmp.png";

print <<EOF;
" alt="Base64 encoded image"/>

<h3>Output from <tt>mathmlsvg</tt></h3>
<pre>
$res
</pre>

<h3>Output from <tt>rsvg</tt></h3>
<pre>
$png_res
</pre>

</body>
</html>
EOF

