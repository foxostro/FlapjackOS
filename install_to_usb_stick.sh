#!/bin/sh
# On a Mac, install boot disk image onto a specified USB disk drive.
set -o errexit

echo "This tool will install the boot image onto a bootable USB storage device."
echo "Enter the desired device: "
read OUTPUT_DEVICE
if [ -z "$OUTPUT_DEVICE" ]; then
	echo "No input, exiting."
	exit 2
fi

T=`hdid -nomount FlapjackOS.iso`
BOOTFD_DEVICE=`perl -e 'print $ARGV[$1]' $T`
MOUNT_POINT="/Volumes/bootfd"

# First, do a block copy onto the desired device.
echo "Please enter admin password to allow raw block copy to the output device \"$OUTPUT_DEVICE\"."
sudo -k
sudo dd if="$BOOTFD_DEVICE" of="$OUTPUT_DEVICE" bs=512 conv=notrunc,noerror

# Now mount the image as a disk.
mkdir -p "$MOUNT_POINT"
sudo mount -t msdos "$OUTPUT_DEVICE" "$MOUNT_POINT"
open "$MOUNT_POINT"
