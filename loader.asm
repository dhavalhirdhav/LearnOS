format ELF

extrn _main

section '.text'
  call _main
  jmp $