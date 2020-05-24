#include "../drivers/screen.h"
#include "../utils/utils.h"
#include "../drivers/ata/ata.h"
#include "../utils/mem.h"

void loadkernel(void) {
    clear(); /* clear the screen. */
    
    printf("Welcome to Learn OS.\r\n");
    printf("Memory address of loadkernel in: 0x%x\r\n\r\n", loadkernel); /* print memory address of loadkernel method */

    uint32_t phy_addr;
    uint32_t page = malloc(1000, 1, &phy_addr); /* allocate memory */
    printf("page: %x\r\n", page);
    printf("phy addr: %x\r\n", phy_addr);

    printf("loading kernel into memory at 1M (0x%x)...\r\n", page);

    read_sectors_ATA_PIO(page, 0x12, 38); /* read 38 sectors (size of our kernel) from 12th sector (That's where kernel will be stored.). */

    uint32_t* kernelFile = page;
    printf("\r\n\r\n");

    if(kernelFile[0] == 0x464C457F) /*read first 4 bits of file to validate that it is a ELF file */
    {
        printf("Kernel is ELF File:\r\n");
    }
    int bits = (kernelFile[1] >> 0) & 0xFF; /* 5th bit is for 32-bit or 64-bit */
    if(bits == 1)
    {
        printf("\t32-bit\r\n");
    }
    int endian = (kernelFile[1] >> 8) & 0xFF; /* 6th bit is for little endian or big endian */
    if(endian == 1)
    {
        printf("\tlittle endian\r\n");
    }
    printf("\tentry position : 0x%x \r\n", page + kernelFile[6]); /* get the entry point of a kernel */
    printf("\texecuting kernel from: 0x%x....\r\n\r\n", page + kernelFile[6]); /* add entry point of a kernel to base address of requested memory */

    void (*func)(void) = page + kernelFile[6]; /* create a pointer to a function */
    func(); /* execute it / pass control to kernel */
}