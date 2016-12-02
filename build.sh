#!/bin/sh
AS=i386-elf-as
GCC=i386-elf-gcc

$AS boot.s -o boot.o
$GCC -c kernel.c -o kernel.o -std=gnu11 -ffreestanding -O2 -Wall -Wextra -Werror
$GCC -T linker.ld -o kernel.bin -ffreestanding -O2 -nostdlib boot.o kernel.o -lgcc

mkdir -p isodir/boot/grub
cp kernel.bin isodir/boot/kernel.bin
cp grub.cfg isodir/boot/grub/grub.cfg
grub-mkrescue -o FlapjackOS.iso isodir
