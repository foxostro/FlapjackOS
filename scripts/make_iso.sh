#!/bin/sh
KERNEL_BIN="$1"
GRUB_CFG="$2"
ISO_FN="$3"

grub-file --is-x86-multiboot "$KERNEL_BIN"
if [[ $? -ne 0 ]]; then
	echo "The file \"$KERNEL_BIN\" is not a multiboot compliant kernel."
	echo "It will not be possible to create a bootable ISO image."
    exit 1
fi

ISODIR="$(mktemp -d)"
mkdir -p "$ISODIR/boot/grub"
cp "$KERNEL_BIN" "$ISODIR/boot"
cp "$GRUB_CFG" "$ISODIR/boot/grub/grub.cfg"
grub-mkrescue -o "$ISO_FN" "$ISODIR"
rm -rf "$ISODIR"