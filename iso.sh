#!/bin/sh
set -e
. ./build.sh

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp /proot/sysroot/boot/potatos.kernel isodir/boot/potatos.kernel
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "potatos" {
	multiboot /boot/potatos.kernel
}
EOF
grub-mkrescue -o potatos.iso isodir
