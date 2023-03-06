#!/bin/sh
set -e
. ./config.sh
echo ---BUILDING HEADERS---

mkdir -p "$SYSROOT"

for PROJECT in $SYSTEM_HEADER_PROJECTS; do
  (cd $PROJECT && DESTDIR="$SYSROOT" $MAKE install-headers)
done
