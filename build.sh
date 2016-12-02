#!/bin/sh
rm FlapjackOS.iso kernel.bin boot.o kernel.o
rm -rf isodir

i386-elf-gcc -c boot.S -o boot.o
i386-elf-gcc -c kernel.c -o kernel.o -std=gnu11 -ffreestanding -O2 -Wall -Wextra -Werror
i386-elf-gcc -T linker.ld -o kernel.bin -ffreestanding -O2 -nostdlib boot.o kernel.o -lgcc

mkdir -p isodir/boot/grub
cp kernel.bin isodir/boot/kernel.bin
cp grub.cfg isodir/boot/grub/grub.cfg
grub-mkrescue -o FlapjackOS.iso isodir
