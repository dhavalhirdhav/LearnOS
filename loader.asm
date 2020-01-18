format COFF
;entry _start

;use16
extrn __main

;_start:
  call __main
  jmp $