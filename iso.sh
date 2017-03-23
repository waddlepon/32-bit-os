#!/bin/sh
set -e
. ./build.sh

mkdir -p iso
mkdir -p iso/boot
mkdir -p iso/boot/grub

cp sysroot/boot/asbestos.kernel iso/boot/asbestos.kernel
cat > iso/boot/grub/grub.cfg << EOF
menuentry "asbestos" {
    multiboot /boot/asbestos.kernel
}
EOF
grub-mkrescue -o asbestos.iso iso
