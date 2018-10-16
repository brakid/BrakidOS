; Boot loader that calls the main method in main.cpp
section .boot
bits 16
global boot
boot:
    mov ax, 0x2401
    int 0x15

    mov ax, 0x3
    int 0x10

    mov [disk],dl

    mov ah, 0x2    ;read sectors
    mov al, 65     ;sectors to read
    mov ch, 0      ;cylinder idx
    mov dh, 0      ;head idx
    mov cl, 2      ;sector idx
    mov dl, [disk] ;disk idx
    mov bx, copy_target;target pointer
    int 0x13
    cli
    lgdt [gdt_pointer]
    mov eax, cr0
    or eax,0x1
    mov cr0, eax
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp CODE_SEG:boot2

gdt_start:
    dq 0x0
gdt_code:
    dw 0xFFFF
    dw 0x0
    db 0x0
    db 10011010b
    db 11001111b
    db 0x0
gdt_data:
    dw 0xFFFF
    dw 0x0
    db 0x0
    db 10010010b
    db 11001111b
    db 0x0
gdt_end:
gdt_pointer:
    dw gdt_end - gdt_start
    dd gdt_start
disk:
    db 0x0
CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

times 510 - ($-$$) db 0
dw 0xaa55
copy_target:
bits 32
boot2:
    mov esp, kernel_stack_top
    extern main
    call main
    jmp $ ;endless loop

%macro isr_noerr 1
global isr%1
isr%1:
    cli
    push 0
    push %1
    jmp isr_common_stub
%endmacro

%macro isr_err 1
global isr%1
isr%1:
    cli
    push %1
    jmp isr_common_stub
%endmacro

;; Interupt Service Routines: http://www.osdever.net/bkerndev/Docs/isrs.htm
isr_noerr 0
isr_noerr 1
isr_noerr 2
isr_noerr 3
isr_noerr 4
isr_noerr 5
isr_noerr 6
isr_noerr 7
isr_err 8
isr_noerr 9
isr_err 10
isr_err 11
isr_err 12
isr_err 13
isr_err 14
isr_noerr 15
isr_noerr 16
isr_noerr 17
isr_noerr 18
isr_noerr 19
isr_noerr 20
isr_noerr 21
isr_noerr 22
isr_noerr 23
isr_noerr 24
isr_noerr 25
isr_noerr 26
isr_noerr 27
isr_noerr 28
isr_noerr 29
isr_noerr 30
isr_noerr 31

extern faultHandler
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
    mov eax, faultHandler
    call eax       ; A special call, preserves the 'eip' register
    pop eax
    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8     ; Cleans up the pushed error code and pushed ISR number
    iret           ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP!

%macro irqs 2
global irq%1
irq%1:
    cli
    push 0
    push %2
    jmp irq_common_stub
%endmacro

extern timerHandler
global irq0
irq0:
    cli
    jmp timerHandler

irqs 1, 33
irqs 2, 34
irqs 3, 35
irqs 4, 36
irqs 5, 37
irqs 6, 38
irqs 7, 39
irqs 8, 40
irqs 9, 41
irqs 10, 42
irqs 11, 43
irqs 12, 44
irqs 13, 45
irqs 14, 46
irqs 15, 47

extern irqHandler

irq_common_stub:
    push ds
    push es
    push fs
    push gs
    push eax
    push ebx
    push ecx
    push edx
    push ebp
    push esi
    push edi
    ; push esp
    mov eax, esp
    push eax
    mov eax, esp
    push eax
    
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov eax, irqHandler
    call eax
    
    ; pop esp
    pop eax
    pop eax
    pop edi
    pop esi
    pop ebp
    pop edx
    pop ecx
    pop ebx
    pop eax
    pop gs
    pop fs
    pop es
    pop ds
    add esp, 8
    iret

section .bss
align 4
kernel_stack_bottom: equ $
    resb 32768 ; 32 KB
kernel_stack_top: