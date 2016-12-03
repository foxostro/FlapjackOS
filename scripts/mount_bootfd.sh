#!/bin/sh
# On a Mac, mounts the bootfd image as a disk drive.
set -o errexit
T=`hdid -nomount bootfd.img`
BOOTFD_DEVICE=`perl -e 'print $ARGV[$1]' $T`
MOUNT_POINT="/Volumes/bootfd"
mkdir -p "$MOUNT_POINT"
echo "Please enter admin password to allow image to be mounted."
sudo -k
sudo mount -t msdos "$BOOTFD_DEVICE" "$MOUNT_POINT"
open "$MOUNT_POINT"
