#! /usr/bin/perl

@FILES = (`find . -name "*.cc"`, `find . -name "*.hh"`);

sub apply_header ($$$) {
    my $prefix  = shift;
    my $infile  = shift;
    my $outfile = shift;

    open(OUTFILE, ">$outfile") || die "Could not open $outfile for writing\n";
    
    print OUTFILE "$prefix$infile$prefix\n";

    close OUTFILE;
}

sub cut_licence ($$) {
    my $infile  = shift;
    my $outfile = shift;

    open(INFILE, "<$infile") || die "Could not open $infile for reading\n";
    open(OUTFILE, ">>$outfile") || die "Could not open $outfile for writing\n";

    my $licence = 1;

    print $infile;

    while (<INFILE>) {
	if ($licence &&
	    (/^\/\// ||
	     /^\x20\*/ ||
	     /^\/\*/)) {
	} else {
	    $licence = 0;
	    print OUTFILE "$_";
	}
    }
}

foreach $f (@FILES) {
    apply_header "// ", $f, "/tmp/f";
    `cat doc/LICENCEcc >>/tmp/f`;
    cut_licence $f, "/tmp/f";
    exit 0;
}

__DATA__

for i in $FILES
do
	cp doc/LICENCEcc /tmp/f
	$1/cutlicence.pl <$i >>/tmp/f
	mv /tmp/f $i
done

FILES=`find . -name "*.hh"`
for i in $FILES
do
	cp doc/LICENCEcc /tmp/f
	$1/cutlicence.pl <$i >>/tmp/f
	mv /tmp/f $i
done

FILES=`find . -name "*.c"`
for i in $FILES
do
	cp doc/LICENCEc /tmp/f
	$1/cutlicence.pl <$i >>/tmp/f
	mv /tmp/f $i
done

FILES=`find . -name "*.h"`
for i in $FILES
do
	cp doc/LICENCEc /tmp/f
	$1/cutlicence.pl <$i >>/tmp/f
	mv /tmp/f $i
done

