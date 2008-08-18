nasm -f elf -o loader.o Boot/loader.s
nasm -f elf -o page.o Memory/page.s
echo compiling C files...
gcc -o kernel.o -c Kernel/kernel.c -nostdlib -nostartfiles -nodefaultlibs
gcc -o video.o -c Video/video.c -nostdlib -nostartfiles -nodefaultlibs
gcc -o port.o -c Kernel/port.c -nostdlib -nostartfiles -nodefaultlibs
gcc -o spaceinvaders.o -c Console/SpaceInvaders.c -nostdlib -nostartfiles -nodefaultlibs
gcc -o gdt.o -c Boot/gdt.c -nostdlib -nostartfiles -nodefaultlibs
gcc -o idt.o -c Boot/idt.c -nostdlib -nostartfiles -nodefaultlibs
gcc -o isrs.o -c Boot/isrs.c -nostdlib -nostartfiles -nodefaultlibs
gcc -o irq.o -c Boot/irq.c -nostdlib -nostartfiles -nodefaultlibs
gcc -o timer.o -c Kernel/timer.c -nostdlib -nostartfiles -nodefaultlibs
gcc -o kb.o -c Keyboard/kb.c -nostdlib -nostartfiles -nodefaultlibs
gcc -o pagec.o -c Memory/page.c -nostdlib -nostartfiles -nodefaultlibs
gcc -o console.o -c Console/console.c -nostdlib -nostartfiles -nodefaultlibs
gcc -o consoleFunctions.o -c Console/consoleFunctions.c -nostdlib -nostartfiles -nodefaultlibs
gcc -o rand.o -c Kernel/rand.c -nostdlib -nostartfiles -nodefaultlibs
echo linking object files...
ld -T linker.ld -o kernel.bin loader.o page.o kernel.o video.o port.o spaceinvaders.o gdt.o idt.o isrs.o irq.o timer.o kb.o pagec.o console.o consoleFunctions.o rand.o

echo copying over to shared folder...
sudo mount -t vboxsf Shared /mnt/
cat stage1 stage2 pad kernel.bin > floppy.img
cp floppy.img /mnt/
