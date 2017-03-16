#!/bin/sh
set -e
. ./build.sh

mkdir -p iso
mkdir -p iso/boot
mkdir -p iso/boot/grub

cp sysroot/boot/myos.kernel iso/boot/myos.kernel
cat > iso/boot/grub/grub.cfg << EOF
menuentry "myos" {
    multiboot /boot/myos.kernel
}
EOF
grub-mkrescue -o myos.iso iso
