OUTPUT_FORMAT(elf32-i386)
ENTRY(loadkernel)
SECTIONS
{
    . = 0x01000; /*0x001000*/
    .text BLOCK(2K) : ALIGN(2K)
    {
        *(.text)
    }

    /*Read-only data*/
    .rodata BLOCK(2K) : ALIGN(2K)
    {
        *(.rodata)
    }

    /*Read-write data (initialized)*/
    .data BLOCK(2K) : ALIGN(2K)
    {
        *(.data)
    }

    /*Read-write data (uninitialized) and stack*/
    .bss BLOCK(2K) : ALIGN(2K)
    {
        *(.bss)
    }
}