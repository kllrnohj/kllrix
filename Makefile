CC = gcc
LD = ld
LDFLAGS = -T linker.ld -m elf_i386
CFLAGS = -Wall -Wextra -nostdlib -nostartfiles -nodefaultlibs -fleading-underscore -march=i386 -m32 -fstrength-reduce -fomit-frame-pointer -finline-functions -fno-builtin
AS = nasm
ASFLAGS = -f elf

OBJS = kernel.o loader.o kmem.o console.o misc_s.o gdt.o idt.o isrs.o irq.o timer.o kbd.o

all: $(OBJS)
	$(LD) $(LDFLAGS) -o kernel.bin $(OBJS)
	cat stage1 stage2 pad kernel.bin > floppy.img

clean:
	-rm kernel.bin floppy.img *.o
