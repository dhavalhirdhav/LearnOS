org     0x7c00

mov     ah, 0Eh

mov     al, "1"
int        10h
mov     al, [data_text]
int        10h

jmp     $

data_text:
     db     "0"

times     510-($-$$)     db     0
dw     0xAA55