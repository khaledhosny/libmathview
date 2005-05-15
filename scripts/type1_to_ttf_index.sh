#!/bin/sh
ttf2afm -c $1 -o /dev/null $1.ttf
ttf2afm -e $1.e31 $1.ttf >$1.ttf_afm
awk -f afmdiff.awk /tmp/cmpsfont/afm/$1.afm $1.ttf_afm | sort -k 2 -n
