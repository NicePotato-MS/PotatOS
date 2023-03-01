#!/bin/sh
set -e
. ./iso.sh

qemu-system-$(./target-triplet-to-arch.sh $HOST) -cdrom myos.iso -drive file=../qemu/HDD.img,index=0,media=disk,format=raw -device intel-hda -device hda-duplex
