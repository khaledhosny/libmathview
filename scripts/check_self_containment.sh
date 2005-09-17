#!/bin/sh

rm -f /tmp/log
for i in $HOME/Local/include/gtkmathview/MathView/*.hh;
do
  echo "************************ $i" >&2
  echo "#include \"$i\"" >t.cc
  g++ `pkg-config libxml-2.0 gdome2-cpp-smart --cflags` -c -D GMV_HAVE_EXT_HASH_MAP -I $HOME/Local/include/gtkmathview/MathView t.cc
done &>/tmp/log

