OUTPUT_FORMAT(elf32-i386)
ENTRY(start)
SECTIONS
{
    . = 0x01000; /*0x001000*/
    .text BLOCK(4K) : ALIGN(4K)
    {
        *(.text)
    }

    /*Read-only data*/
    .rodata BLOCK(4K) : ALIGN(4K)
    {
        *(.rodata)
    }

    /*Read-write data (initialized)*/
    .data BLOCK(4K) : ALIGN(4K)
    {
        *(.data)
    }

    /*Read-write data (uninitialized) and stack*/
    .bss BLOCK(4K) : ALIGN(4K)
    {
        *(.bss)
    }
}