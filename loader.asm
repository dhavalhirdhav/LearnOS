format ELF ;instruct assembler to produce ELF (Executable and Linkable Format) file.

extrn main ;tell assembler that main is the external function so ignore the assembler / compiler if main is not found in code.

public _start

_start:
  call main ;call external main function.
  jmp $