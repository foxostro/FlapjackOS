#!/bin/sh
AS=i386-elf-as
GCC=i386-elf-gcc

rm kernel.bin boot.o kernel.o

$AS boot.s -o boot.o
$GCC -c kernel.c -o kernel.o -std=gnu11 -ffreestanding -O2 -Wall -Wextra -Werror
$GCC -T linker.ld -o kernel.bin -ffreestanding -O2 -nostdlib boot.o kernel.o -lgcc

# Verify that a bootable kernel was built.
grub-file --is-x86-multiboot kernel.bin
exit $?
