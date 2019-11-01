mov ah, 0eh ;tele prompt
mov al, 57h ;W
int 10h ;print
mov al, 65h ;e
int 10h
mov al, 6ch ;l
int 10h
mov al, 63h ;c
int 10h
mov al, 6fh ;o
int 10h
mov al, 6dh ;m
int 10h
mov al, 65h ;e
int 10h
mov al, 20h ;{space}
int 10h
mov al, 74h ;t
int 10h
mov al, 6fh ;o
int 10h
mov al, 20h ;{space}
int 10h
mov al, 4Ch ;L
int 10h
mov al, 65h ;e
int 10h
mov al, 61h ;a
int 10h
mov al, 72h ;r
int 10h
mov al, 6Eh ;n
int 10h
mov al, 4Fh ;O
int 10h
mov al, 53h ;S
int 10h

times 510-$ db 0
dw 0xAA55