section .text
; This will set up our new segment registers. We need to do
; something special in order to set CS. We do what is called a
; far jump. A jump that includes a segment as well as an offset.
; This is declared in C as 'extern void gdt_flush();'
global gdt_flush     ; Allows the C code to link to this
extern gp            ; Says that '_gp' is in another file
gdt_flush:
    lgdt [gp]        ; Load the GDT with our '_gp' which is a special pointer
    mov ax, 0x10      ; 0x10 is the offset in the GDT to our data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:flush2   ; 0x08 is the offset to our code segment: Far jump!
flush2:
    ret               ; Returns back to the C code!

; Loads the IDT defined in '_idtp' into the processor.
; This is declared in C as 'extern void idt_load();'
global idt_load
extern idtp
idt_load:
    lidt [idtp]
    ret

; In just a few pages in this tutorial, we will add our Interrupt
; Service Routines (ISRs) right here!
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

;  0: Divide By Zero Exception
isr0:
    cli
    push byte 0    ; A normal ISR stub that pops a dummy error code to keep a
                   ; uniform stack frame
    push byte 0
    jmp isr_common_stub

;  1: Debug Exception
isr1:
    cli
    push byte 0
    push byte 1
    jmp isr_common_stub

;  2: Non Maskable Interrupt Exception
isr2:
	cli
	push byte 0
	push byte 2
	jmp isr_common_stub

;  3: Breakpoint Exception
isr3:
	cli
	push byte 0
	push byte 3
	jmp isr_common_stub

;  4: Into Detected Overflow Exception
isr4:
	cli
	push byte 0
	push byte 4
	jmp isr_common_stub

;  5: Out of Bounds Exception
isr5:
	cli
	push byte 0
	push byte 5
	jmp isr_common_stub

;  6: Invalid Opcode Exception
isr6:
	cli
	push byte 0
	push byte 6
	jmp isr_common_stub

;  7: No Coprocessor Exception
isr7:
	cli
	push byte 0
	push byte 7
	jmp isr_common_stub

;  8: Double Fault Exception (With Error Code!)
isr8:
    cli
    push byte 8        ; Note that we DON'T push a value on the stack in this one!
                   ; It pushes one already! Use this type of stub for exceptions
                   ; that pop error codes!
    jmp isr_common_stub

;  9: Coprocessor Segment Overrun Exception
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

; 17: Alignment Check Exception (486+)
isr17:
	cli
	push byte 0
	push byte 17
	jmp isr_common_stub

; 18: Machine Check Exception (Pentium/586+)
isr18:
	cli
	push byte 0
	push byte 18
	jmp isr_common_stub

; 19: Reserved Exceptions
isr19:
	cli
	push byte 0
	push byte 19
	jmp isr_common_stub

; 20: Reserved Exceptions
isr20:
	cli
	push byte 0
	push byte 20
	jmp isr_common_stub

; 21: Reserved Exceptions
isr21:
	cli
	push byte 0
	push byte 21
	jmp isr_common_stub

; 22: Reserved Exceptions
isr22:
	cli
	push byte 0
	push byte 22
	jmp isr_common_stub

; 23: Reserved Exceptions
isr23:
	cli
	push byte 0
	push byte 23
	jmp isr_common_stub

; 24: Reserved Exceptions
isr24:
	cli
	push byte 0
	push byte 24
	jmp isr_common_stub

; 25: Reserved Exceptions
isr25:
	cli
	push byte 0
	push byte 25
	jmp isr_common_stub

; 26: Reserved Exceptions
isr26:
	cli
	push byte 0
	push byte 26
	jmp isr_common_stub

; 27: Reserved Exceptions
isr27:
	cli
	push byte 0
	push byte 27
	jmp isr_common_stub

; 28: Reserved Exceptions
isr28:
	cli
	push byte 0
	push byte 28
	jmp isr_common_stub

; 29: Reserved Exceptions
isr29:
	cli
	push byte 0
	push byte 29
	jmp isr_common_stub

; 30: Reserved Exceptions
isr30:
	cli
	push byte 0
	push byte 30
	jmp isr_common_stub

; 31: Reserved Exceptions
isr31:
	cli
	push byte 0
	push byte 31
	jmp isr_common_stub

; We call a C function in here. We need to let the assembler know
; that '_fault_handler' exists in another file
extern fault_handler

; This is our common ISR stub. It saves the processor state, sets
; up for kernel mode segments, calls the C-level fault handler,
; and finally restores the stack frame.
isr_common_stub:
    pusha
    push ds
    push es
    push fs
    push gs
    mov ax, 0x10   ; Load the Kernel Data Segment descriptor!
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov eax, esp   ; Push us the stack
    push eax
    mov eax, fault_handler
    call eax       ; A special call, preserves the 'eip' register
    pop eax
    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8     ; Cleans up the pushed error code and pushed ISR number
    iret           ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP!

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
    push byte 0    ; Note that these don't push an error code on the stack:
                   ; We need to push a dummy error code
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

; This is a stub that we have created for IRQ based ISRs. This calls
; 'irq_handler' in our C code. We need to create this in an 'irq.c'
irq_common_stub:
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
