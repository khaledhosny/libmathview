#!/bin/sh

run ()
{
    echo "running: $*"
    eval $*

    if test $? != 0 ; then
	echo "error: while running '$*'"
	exit 1
    fi
}

run libtoolize --force --copy
run aclocal -I ac-helpers
run autoheader
run automake --add-missing --copy --foreign
run autoconf
