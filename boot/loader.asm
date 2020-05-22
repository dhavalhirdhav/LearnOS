org 0x1000
format ELF ;instruct assembler to produce ELF (Executable and Linkable Format) file.

extrn loadkernel ;tell assembler that main is the external function so ignore the assembler / compiler if main is not found in code.

start:
section '.text'
    call loadkernel ;call external main function.
    jmp $