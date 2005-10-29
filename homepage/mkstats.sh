#!/bin/sh

if test `uname` == 'Darwin'; then
  exec ./mkstats_osx.sh
else
  exec ./mkstats_linux.sh
fi

