global loader           ; making entry point visible to linker
extern _main            ; kmain is defined elsewhere

; setting up the Multiboot header - see GRUB docs for details
MODULEALIGN equ  1<<0                   			; align loaded modules on page boundaries
MEMINFO     equ  1<<1                   			; provide memory map
VIDEOMODE	equ  1<<2								; set video mode
FLAGS       equ  MODULEALIGN | MEMINFO | VIDEOMODE	; this is the Multiboot 'flag' field
MAGIC       equ    0x1BADB002           			; 'magic number' lets bootloader find the header
CHECKSUM    equ -(MAGIC + FLAGS)        			; checksum required

; These are only valid if MULTIBOOT_AOUT_KLUDGE is set.
HEADER_ADDR		equ 0
LOAD_ADDR		equ 0
LOAD_END_ADDR	equ 0
BSS_END_ADDR	equ 0
ENTRY_ADDR		equ 0

; These are only valid if MULTIBOOT_VIDEO_MODE is set.
MODE_TYPE	equ 0
WIDTH		equ 800
HEIGHT		equ 600
DEPTH		equ 24

; This is the virtual base address of kernel space. It must be used to convert virtual
; addresses into physical addresses until paging is enabled. Note that this is not
; the virtual address where the kernel image itself is loaded -- just the amount that must
; be subtracted from a virtual address to get a physical address.
KERNEL_VIRTUAL_BASE equ 0xC0000000                  ; 3GB
KERNEL_PAGE_NUMBER equ (KERNEL_VIRTUAL_BASE >> 22)  ; Page directory index of kernel's 4MB PTE.

section .data
align 0x1000
BootPageDirectory:
    ; This page directory entry identity-maps the first 4MB of the 32-bit physical address space.
    ; All bits are clear except the following:
    ; bit 7: PS The kernel page is 4MB.
    ; bit 1: RW The kernel page is read/write.
    ; bit 0: P  The kernel page is present.
    ; This entry must be here -- otherwise the kernel will crash immediately after paging is
    ; enabled because it can't fetch the next instruction! It's ok to unmap this page later.
    dd 0x00000083
    times (KERNEL_PAGE_NUMBER - 1) dd 0                 ; Pages before kernel space.
    ; This page directory entry defines a 4MB page containing the kernel.
    dd 0x00000083
    times (1024 - KERNEL_PAGE_NUMBER - 1) dd 0  ; Pages after the kernel image.


section .text
align 4
MultiBootHeader:
   dd MAGIC
   dd FLAGS
   dd CHECKSUM
   dd HEADER_ADDR
   dd LOAD_ADDR
   dd LOAD_END_ADDR
   dd BSS_END_ADDR
   dd ENTRY_ADDR
   dd MODE_TYPE
   dd WIDTH
   dd HEIGHT
   dd DEPTH

; reserve initial kernel stack space
STACKSIZE equ 0x4000                  ; that's 16k.

extern start_ctors, end_ctors, start_dtors, end_dtors

loader:
    ; NOTE: Until paging is set up, the code must be position-independent and use physical
    ; addresses, not virtual ones!
    mov ecx, (BootPageDirectory - KERNEL_VIRTUAL_BASE)
    mov cr3, ecx                                        ; Load Page Directory Base Register.

    mov ecx, cr4
    or ecx, 0x00000010                          ; Set PSE bit in CR4 to enable 4MB pages.
    mov cr4, ecx

    mov ecx, cr0
    or ecx, 0x80000000                          ; Set PG bit in CR0 to enable paging.
    mov cr0, ecx

    ; Start fetching instructions in kernel space.
    lea ecx, [StartInHigherHalf]
    jmp ecx                                                     ; NOTE: Must be absolute jump!

StartInHigherHalf:
    ; Unmap the identity-mapped first 4MB of physical address space. It should not be needed
    ; anymore.
    mov dword [BootPageDirectory], 0
    invlpg [0]

    ; NOTE: From now on, paging should be enabled. The first 4MB of physical address space is
    ; mapped starting at KERNEL_VIRTUAL_BASE. Everything is linked to this address, so no more
    ; position-independent code or funny business with virtual-to-physical address translation
    ; should be necessary. We now have a higher-half kernel.
    mov esp, stack+STACKSIZE           ; set up the stack

    ; hopefully this works
    mov ecx, BootPageDirectory
    push ecx
    push eax                           ; pass Multiboot magic number

    ; pass Multiboot info structure -- WARNING: This is a physical address and may not be
    ; in the first 4MB!
    push ebx

static_ctors_loop:
   mov ebx, start_ctors
   jmp .test
.body:
   call [ebx]
   add ebx,4
.test:
   cmp ebx, end_ctors
   jb .body

   call _main                      ; call kernel proper

static_dtors_loop:
   mov ebx, start_dtors
   jmp .test
.body:
   call [ebx]
   add ebx,4
.test:
   cmp ebx, end_dtors
   jb .body

   hlt                             ; halt machine should kernel return

section .bss
align 32
stack:
   resb STACKSIZE                     ; reserve 16k stack on a quadword boundary
