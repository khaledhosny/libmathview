#!/bin/sh

automake --version | perl -ne 'if (/\(GNU automake\) ([0-9].[0-9])/) {print;  if ($1 < 1.7) {exit 1;}}'

if [ $? -ne 0 ]; then
    echo "Error: you need automake 1.7 or later.  Please upgrade."
    exit 1
fi

aclocal
libtoolize --force
autoheader
automake -a
autoconf
