CC = gcc
LD = ld
LDFLAGS = -T linker.ld
CFLAGS = -Wall -Wextra -nostdlib -nostartfiles -nodefaultlibs -fleading-underscore
AS = nasm
ASFLAGS = -f elf

CSRC = *.c
ASRC = *.s

OBJS = kernel.o loader.o

kernel: $(OBJS)
	$(LD) $(LDFLAGS) -o kernel.bin $(OBJS)
	cat stage1 stage2 pad kernel.bin > floppy.img
	cp floppy.img /mnt/shared/

$(CSRC):
	$(CC) $(CFLAGS) -c $*.c

$(ASRC):
	$(AS) $(ASFLAGS) -o $@.o $?

clean:
	-rm kernel.bin floppy.img *.o
