#!/bin/sh
set -e
. ./clean.sh
. ./build.sh
echo ---STARTING ISO BUILD---

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp ./sysroot/boot/potatos.kernel isodir/boot/potatos.kernel
cat > isodir/boot/grub/grub.cfg << EOF
set timeout=15
set default=0 # Set the default menu entry

menuentry "PotatOS" {
	multiboot /boot/potatos.kernel
	boot
}
EOF
grub-mkrescue -o potatos.iso isodir
