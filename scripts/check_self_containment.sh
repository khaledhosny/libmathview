#!/bin/sh

rm /tmp/log
for i in /tmp/usr/local/include/gtkmathview/*.hh;
do
  echo "************************ $i" >&2
  echo "#include \"$i\"" >t.cc
  g++ `pkg-config libxml-2.0 gdome2-cpp-smart --cflags` -c -I /tmp/usr/local/include/gtkmathview/ t.cc
done &>/tmp/log

