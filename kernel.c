#include "drivers/screen.h"
#include "utils/utils.h"
#include "drivers/ata/ata.h"
#include "utils/mem.h"
//#include "fs/fs.h"

void kmain(void) {
    clear();
    
    printf("Welcome to Learn OS.\r\n");
    printf("Memory address of kmain in: 0x%x\r\n\r\n", kmain);

    uint32_t phy_addr;
    uint32_t page = malloc(1000, 1, &phy_addr);
    printf("page: %x\r\n", page);
    printf("phy addr: %x\r\n", phy_addr);

    printf("loading kernel into memory at 1M (0x10000)...\r\n");

    read_sectors_ATA_PIO(page, 0x16, 42);

    uint32_t* aa = page;
    printf("\r\n\r\n");

    if(aa[0] == 0x464C457F)
    {
        printf("ELF File.\r\n");
    }
    int bits = (aa[1] >> 0) & 0xFF; //5
    int endian = (aa[1] >> 8) & 0xFF; //6
    if(bits == 1)
    {
        printf("32-bit\r\n");
    }
    if(endian == 1)
    {
        printf("little endian\r\n");
    }
    int exec = aa[4] >> 0 & 0xFFFF; //16-17
    if(exec == 3)
    {
        printf("shared\r\n");
    }
    printf("entry position : 0x%x \r\n", 0x11000 + aa[6]);

    printf("\r\n");
    printf("executing kernel from: 0x%x....\r\n", 0x11000 + aa[6]);

    void (*func)(void) = 0x11000 + aa[6]; func();

    // //printf("Initializing file system...\r\n");
    // //InitializeFS();
    // // printf("DFFS v1.00 File System installed at: 0x3c00\r\n");
}