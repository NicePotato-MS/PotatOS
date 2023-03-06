#!/bin/sh
# useless addition
set -e
echo ---CLEANING---
. ./config.sh

for PROJECT in $PROJECTS; do
  (cd $PROJECT && $MAKE clean)
done

rm -rf sysroot
rm -rf isodir
rm -rf potatos.iso