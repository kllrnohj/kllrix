#!/bin/sh

nasm -f elf -o loader.o loader.s
gcc -o kernel.o -c kernel.c -Wall -Wextra -nostdlib -nostartfiles -nodefaultlibs -fleading-underscore
ld -T linker.ld -o kernel.bin loader.o kernel.o
cat stage1 stage2 pad kernel.bin > floppy.img
cp floppy.img /mnt/shared/
