#!/bin/sh
set -e
KERNEL_BIN="$1"; shift
GRUB_CFG="$1"; shift
ISO_FN="$1"; shift
MODULES="$@"

grub-file --is-x86-multiboot "$KERNEL_BIN"
if [[ $? -ne 0 ]]; then
	echo "The file \"$KERNEL_BIN\" is not a multiboot compliant kernel."
	echo "It will not be possible to create a bootable ISO image."
    exit 1
fi

ISODIR="$(mktemp -d)"

mkdir -p "$ISODIR/boot/grub"
cp "$KERNEL_BIN" "$ISODIR/boot"
MODULES_ARG=$(printf " %s" "$@")
erb modules="$MODULES_ARG" -- "$GRUB_CFG" > "$ISODIR/boot/grub/grub.cfg"

mkdir -p "$ISODIR/modules"
for THIS_MODULE in $@; do
cp "$THIS_MODULE" "$ISODIR/modules"
done

grub-mkrescue -o "$ISO_FN" "$ISODIR"

rm -rf "$ISODIR"
