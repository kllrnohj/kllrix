CC = gcc
LD = ld
LDFLAGS = -T linker.ld -m elf_i386
CFLAGS = -Wall -Wextra -nostdlib -nostartfiles -nodefaultlibs -fno-leading-underscore -march=i386 -m32 -fstrength-reduce -fomit-frame-pointer -finline-functions -fno-builtin
AS = nasm
ASFLAGS = -f elf

OBJS = Boot/gdt.o Boot/idt.o Boot/irq.o Boot/isrs.o Boot/loader.o Console/console.o Console/consoleFunctions.o Console/SpaceInvaders.o Kernel/kernel.o Kernel/port.o Kernel/rand.o Kernel/timer.o Keyboard/kb.o Memory/page.o Memory/pageasm.o Video/video.o

all: $(OBJS)
	$(LD) $(LDFLAGS) -o kernel.bin $(OBJS)
	cat stage1 stage2 pad kernel.bin > BFfloppy.img

clean:
	-rm kernel.bin BFfloppy.img *.o Boot/*.o Console/*.o Kernel/*.o Keyboard/*.o Memory/*.o Video/*.o
