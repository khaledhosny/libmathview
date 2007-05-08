#!/bin/sh

if test `uname` == "Darwin"; then
  BUILD_PATH=~/Projects/Builds/gtkmathview_svg
  SOURCE_PATH=~/Projects/gtkmathview
else
  BUILD_PATH=/localdisk/luca/Builds/gtkmathview_gmetadom_full
  SOURCE_PATH=~/Projects/gtkmathview
fi

MATHMLSVG="$BUILD_PATH/mathmlsvg/mathmlsvg --config=$SOURCE_PATH/config/gtkmathview.conf.xml.debug"
exec $MATHMLSVG $*

