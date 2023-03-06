#!/bin/sh
set -e
. ./headers.sh
echo --BUILDING---

for PROJECT in $PROJECTS; do
  (cd $PROJECT && DESTDIR="$SYSROOT" $MAKE install)
done
