format COFF
;entry _start

;use16
extrn loader_main

;_start:
  call loader_main
  jmp $