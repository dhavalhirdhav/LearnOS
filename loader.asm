format ELF
;entry _start

extrn _main

section '.text'
;_start:
  call _main
  jmp $

;[bits 32]
;global start
;[extern _main] ; Define calling point. Must have same name as kernel.c 'main' function
;start:
;call _main ; Calls the C function. The linker will know where it is placed in memory
;jmp $
