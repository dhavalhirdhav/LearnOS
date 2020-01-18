org  0x7c00
mov  bp, 0x9000
mov  sp, bp

mov  bx, MSG_REAL_MODE
call print

call switch_to_pm
jmp $     ;infinite loop

print:
     pusha

start:
     mov  al, [bx]
     cmp  al, 0
     je   done

     mov  ah, 0x0e
     int  0x10

     add  bx, 1
     jmp  start

done:
     popa
     ret

print_nl:
    pusha
    
    mov ah, 0x0e
    mov al, 0x0a ; newline char
    int 0x10
    mov al, 0x0d ; carriage return
    int 0x10
    
    popa
    ret

gdt_start:
     dd   0x0  ;4 bytes
     dd   0x0  ;4 bytes

gdt_code:
     dw   0xffff    ;segment length, bits 0-15
     dw   0x0       ;segment base, bits 0-15
     db   0x0       ;segment base, bits 16-23
     db   10011010b ;flags (8 bits)
     db   11001111b ;flags (4 bits) + segment length, bits 16-19
     db   0x0       ;segment base, bits 24-31

gdt_data:
     dw   0xffff
     dw   0x0
     db   0x0
     db   10010010b
     db   11001111b
     db   0x0

gdt_end:

gdt_descriptor:
     dw   gdt_end - gdt_start - 1  ;size (16-bit), always one less of its true size
     dd   gdt_start                ;address (32-bit)

CODE_SEG  equ  gdt_code - gdt_start
DATA_SEG  equ  gdt_data - gdt_start

use32

VIDEO_MEMORY   equ  0xb8000
WHITE_ON_BLACK equ 0x0f ; the color byte for each character

print_string_pm:
     pusha
     mov  edx, VIDEO_MEMORY

print_string_pm_loop:
    mov al, [ebx] ; [ebx] is the address of our character
    mov ah, WHITE_ON_BLACK

    cmp al, 0 ; check if end of string
    je print_string_pm_done

    mov [edx], ax ; store character + attribute in video memory
    add ebx, 1 ; next char
    add edx, 2 ; next video memory position

    jmp print_string_pm_loop

print_string_pm_done:
    popa
    ret

use16
switch_to_pm:
    cli ; 1. disable interrupts
    lgdt [gdt_descriptor] ; 2. load the GDT descriptor
    mov eax, cr0
    or eax, 0x1 ; 3. set 32-bit mode bit in cr0
    mov cr0, eax
    jmp CODE_SEG:init_pm ; 4. far jump by using a different segment

use32
init_pm:
     mov  ax, DATA_SEG
     mov  ds, ax
     mov  ss, ax
     mov  es, ax
     mov  fs, ax
     mov  gs, ax
     
     mov  ebp, 0x90000
     mov  esp, ebp

     call BEGIN_PM

use32
BEGIN_PM:
     mov  ebx, MSG_PROT_MODE
     call print_string_pm
     jmp  $

MSG_REAL_MODE db "Started in 16-bit real mode", 0
MSG_PROT_MODE db "Loaded 32-bit protected mode", 0

times     510-($-$$)     db   0
dw   0xaa55