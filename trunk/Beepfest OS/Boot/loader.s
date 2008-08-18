global _loader			; visible to linker
global gdt_flush
extern kmain			; kernel main, linked to from kernel.c
extern gp

; set up IRQs
global irq0
global irq1
global irq2
global irq3
global irq4
global irq5
global irq6
global irq7
global irq8
global irq9
global irq10
global irq11
global irq12
global irq13
global irq14
global irq15

; 32: IRQ0
irq0:
	cli
	push byte 0
	push byte 32
	jmp irq_common_stub

; 33: IRQ1
irq1:
	cli
	push byte 0
	push byte 33
	jmp irq_common_stub

; 34: IRQ2
irq2:
	cli
	push byte 0
	push byte 34
	jmp irq_common_stub

; 35: IRQ3
irq3:
	cli
	push byte 0
	push byte 35
	jmp irq_common_stub

; 36: IRQ4
irq4:
	cli
	push byte 0
	push byte 36
	jmp irq_common_stub

; 37: IRQ5
irq5:
	cli
	push byte 0
	push byte 37
	jmp irq_common_stub

; 38: IRQ6
irq6:
	cli
	push byte 0
	push byte 38
	jmp irq_common_stub

; 39: IRQ7
irq7:
	cli
	push byte 0
	push byte 39
	jmp irq_common_stub

; 40: IRQ8
irq8:
	cli
	push byte 0
	push byte 40
	jmp irq_common_stub

; 41: IRQ9
irq9:
	cli
	push byte 0
	push byte 41
	jmp irq_common_stub

; 42: IRQ10
irq10:
	cli
	push byte 0
	push byte 42
	jmp irq_common_stub

; 43: IRQ11
irq11:
	cli
	push byte 0
	push byte 43
	jmp irq_common_stub

; 44: IRQ12
irq12:
	cli
	push byte 0
	push byte 44
	jmp irq_common_stub

; 45: IRQ13
irq13:
	cli
	push byte 0
	push byte 45
	jmp irq_common_stub

; 46: IRQ14
irq14:
	cli
	push byte 0
	push byte 46
	jmp irq_common_stub

; 47: IRQ15
irq15:
	cli
	push byte 0
	push byte 47
	jmp irq_common_stub

extern irq_handler

irq_common_stub
	pusha
	push ds
	push es
	push fs
	push gs
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov eax, esp
	push eax
	mov eax, irq_handler
	call eax
	pop eax
	pop gs
	pop fs
	pop es
	pop ds
	popa
	add esp, 8
	iret

; set up ISRs
global isr0
global isr1
global isr2
global isr3
global isr4
global isr5
global isr6
global isr7
global isr8
global isr9
global isr10
global isr11
global isr12
global isr13
global isr14
global isr15
global isr16
global isr17
global isr18
global isr19
global isr20
global isr21
global isr22
global isr23
global isr24
global isr25
global isr26
global isr27
global isr28
global isr29
global isr30
global isr31

; 0: divide by zero exception
isr0:
	cli
	push byte 0
	push byte 0
	jmp isr_common_stub
	
; 1: Debug exception
isr1:
	cli
	push byte 0
	push byte 1
	jmp isr_common_stub

; 2: Non Maskable interrupt exception
isr2:
	cli
	push byte 0
	push byte 2
	jmp isr_common_stub
	
; 3: Breakpoint exception
isr3:
	cli
	push byte 0
	push byte 3
	jmp isr_common_stub
	
; 4: Into detected overflow exception
isr4:
	cli
	push byte 0
	push byte 4
	jmp isr_common_stub

; 5: Out of Bounds Exception
isr5:
	cli
	push byte 0
	push byte 5
	jmp isr_common_stub
	
; 6: Invalid Opcode Exception
isr6:
	cli
	push byte 0
	push byte 6
	jmp isr_common_stub
	
; 7: No coprocessor Exception
isr7:
	cli
	push byte 0
	push byte 7
	jmp isr_common_stub
	
; 8: Double Fault Exception
isr8:
	cli
	push byte 8
	jmp isr_common_stub
	
; 9: Coprocessor Segment overrun exception
isr9:
	cli
	push byte 0
	push byte 9
	jmp isr_common_stub

; 10: Bad TSS Exception
isr10:
	cli
	push byte 10
	jmp isr_common_stub
	
; 11: Segment Not Present Exception
isr11:
	cli
	push byte 11
	jmp isr_common_stub
	
; 12: Stack Fault Exception
isr12:
	cli
	push byte 12
	jmp isr_common_stub
	
; 13: General Protection Fault Exception
isr13:
	cli
	push byte 13
	jmp isr_common_stub
	
; 14: Page Fault Exception
isr14:
	cli
	push byte 14
	jmp isr_common_stub
	
; 15: Unknown Interrupt Exception
isr15:
	cli
	push byte 0
	push byte 15
	jmp isr_common_stub
	
; 16: Coprocessor Fault Exception
isr16:
	cli
	push byte 0
	push byte 16
	jmp isr_common_stub
	
; 17: Alignment Check Exception
isr17:
	cli
	push byte 0
	push byte 17
	jmp isr_common_stub
	
; 18: Machine Check Exception
isr18:
	cli
	push byte 0
	push byte 18
	jmp isr_common_stub
	
; 19: Reserved
isr19:
	cli
	push byte 0
	push byte 19
	jmp isr_common_stub

; 20: Reserved
isr20:
	cli
	push byte 0
	push byte 20
	jmp isr_common_stub

; 21: Reserved
isr21:
	cli
	push byte 0
	push byte 21
	jmp isr_common_stub
	
; 22: Reserved
isr22:
	cli
	push byte 0
	push byte 22
	jmp isr_common_stub
	
; 23: Reserved
isr23:
	cli
	push byte 0
	push byte 23
	jmp isr_common_stub
	
; 24: Reserved
isr24:
	cli
	push byte 0
	push byte 24
	jmp isr_common_stub
	
; 25: Reserved
isr25:
	cli
	push byte 0
	push byte 25
	jmp isr_common_stub
	
; 26: Reserved
isr26:
	cli
	push byte 0
	push byte 26
	jmp isr_common_stub
	
; 27: Reserved
isr27:
	cli
	push byte 0
	push byte 27
	jmp isr_common_stub
	
; 28: Reserved
isr28:
	cli
	push byte 0
	push byte 28
	jmp isr_common_stub
	
; 29: Reserved
isr29:
	cli
	push byte 0
	push byte 29
	jmp isr_common_stub
	
; 30: Reserved
isr30:
	cli
	push byte 0
	push byte 30
	jmp isr_common_stub
	
; 31: Reserved
isr31:
	cli
	push byte 0
	push byte 31
	jmp isr_common_stub
	
extern fault_handler

isr_common_stub:
	pusha
	push ds
	push es
	push fs
	push gs
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov eax, esp
	push eax
	mov eax, fault_handler
	call eax
	pop eax
	pop gs
	pop fs
	pop es
	pop ds
	popa
	add esp, 8
	iret
	

; set up multiboot header - GRUB docs explain
MODULEALIGN equ 1<<0				; align to page boundaries
MEMINFO equ 1<<1				; provide memory map
FLAGS equ MODULEALIGN | MEMINFO			; multiboot flag field
MAGIC equ 0x1BADB002				; magic number, for header
CHECKSUM equ -(MAGIC + FLAGS)			; checksum required

section .text
align 4
MultiBootHeader:
	dd MAGIC
	dd FLAGS
	dd CHECKSUM

; reserve initial kernel stack space
STACKSIZE equ 0x4000				; 16k

_loader:
	mov esp, stack+STACKSIZE		; set up stack
	push eax				; push Multiboot magic to main
	push ebx				; push Multiboot struct to main
	
	call kmain				; call kernel main
	hlt					; halt, kernel shouldn't return
	
gdt_flush:
	lgdt [gp]				; load GDT from C pointer
	mov ax, 0x10				; 0x10 is offset to GDT data seg
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	jmp 0x08:flush2				; 0x08 is offset to code segment, far jump to flush
flush2:
	ret
global idt_load
extern idtp
idt_load:
	lidt [idtp]
	ret

section .bss
align 32
stack:
	resb STACKSIZE				; reserve 16k stack on 32 bit align
