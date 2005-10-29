#!/bin/sh

if test `uname` == "Darwin"; then
  BUILD_PATH=~/Projects/Builds/gtkmathview_svg
  SOURCE_PATH=~/Projects/gtkmathview
else
  BUILD_PATH=~/projects/BUILDS/gtkmathview_svg
  SOURCE_PATH=~/projects/gtkmathview
fi

MATHMLSVG="$BUILD_PATH/mathmlsvg/mathmlsvg --config=$SOURCE_PATH/config/gtkmathview.conf.xml.debug"
exec $MATHMLSVG $*

