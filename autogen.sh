#!/bin/sh

AUTOMAKE=automake
ACLOCAL=aclocal
$AUTOMAKE --version | perl -ne 'if (/\(GNU $AUTOMAKE\) ([0-9].[0-9])/) {print; if ($1 < 1.7) {exit 1;}}'
#AUTOMAKE=automake-1.7
#ACLOCAL=aclocal-1.7
if [ $? -ne 0 ]; then
$AUTOMAKE --version | perl -ne 'if (/\(GNU $AUTOMAKE\) ([0-9].[0-9])/) {print; if ($1 < 1.7) {exit 1;}}'

if [ $? -ne 0 ]; then
    echo "Error: you need automake 1.7 or later.  Please upgrade."
    exit 1
fi
fi

echo "libtoolize --force --copy"
libtoolize --force --copy || {
    echo "libtoolize failed! Unable to continue."
    exit 1
}

# Produce aclocal.m4, so autoconf gets the automake macros it needs
# 
echo "Creating aclocal.m4: $ACLOCAL $ACLOCAL_FLAGS"

ACLOCAL_FLAGS="-I ac-helpers $ACLOCAL_FLAGS"
$ACLOCAL $ACLOCAL_FLAGS || {
    echo "$ACLOCAL failed! Unable to continue."
    exit 1
}

if test -f autom4te.cache/requests; then
    echo "Checking for PKG_CHECK_MODULES in aclocal.m4 or autom4te.cache/requests ..."
    pkgcheckdef=`grep PKG_CHECK_MODULES autom4te.cache/requests``grep PKG_CHECK_MODULES aclocal.m4 | grep AC_DEFUN`
else
    echo "Checking for PKG_CHECK_MODULES in aclocal.m4 ..."
    pkgcheckdef=`grep PKG_CHECK_MODULES aclocal.m4 | grep AC_DEFUN`
fi

if test "x$pkgcheckdef" = "x"; then
  echo "Running $ACLOCAL -I ac-helpers/pkg-config $ACLOCAL_FLAGS"
  $ACLOCAL -I ac-helpers/pkg-config $ACLOCAL_FLAGS || {
    echo "$ACLOCAL failed! Unable to continue."
    exit 1
  }
  if test -f autom4te.cache/requests; then
      echo "Checking for PKG_CHECK_MODULES in aclocal.m4 or autom4te.cache/requests ..."
      pkgcheckdef=`grep PKG_CHECK_MODULES autom4te.cache/requests``grep PKG_CHECK_MODULES aclocal.m4 | grep AC_DEFUN`
  else
      echo "Checking for PKG_CHECK_MODULES in aclocal.m4 ..."
      pkgcheckdef=`grep PKG_CHECK_MODULES aclocal.m4 | grep AC_DEFUN`
  fi
  if test "x$pkgcheckdef" = "x"; then
    echo ""
    echo "error: PKG_CHECK_MODULES isn't defined"
    echo ""
    echo "   Either pkg.m4 wasn't in aclocal's search path or pkgconfig"
    echo "   (or pkgconfig-devel?) isn't installed."
    echo ""
    echo "   If pkg-config is installed in <prefix> then re-run autogen.sh:"
    echo ""
    echo "       ACLOCAL_FLAGS=\"-I <prefix>/share/aclocal\" ./autogen.sh"
    echo ""
    exit
  fi
fi

autoheader

# Produce all the `GNUmakefile.in's and create neat missing things
# like `install-sh', etc.
# 
echo "$AUTOMAKE --add-missing --copy --foreign"

$AUTOMAKE --add-missing --copy --foreign || {
    echo ""
    echo "* * * warning: possible errors while running automake"
    echo ""
}

echo "autoconf"
autoconf || {
    echo "autoconf failed! Unable to continue."
    exit 1
}
