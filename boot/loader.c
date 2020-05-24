#include "../drivers/screen.h"
#include "../utils/utils.h"
#include "../drivers/ata/ata.h"
#include "../utils/mem.h"

void loadkernel(void) {
    clear();
    
    printf("Welcome to Learn OS.\r\n");
    printf("Memory address of loadkernel in: 0x%x\r\n\r\n", loadkernel);

    uint32_t phy_addr;
    uint32_t page = malloc(1000, 1, &phy_addr);
    printf("page: %x\r\n", page);
    printf("phy addr: %x\r\n", phy_addr);

    printf("loading kernel into memory at 1M (0x%x)...\r\n", page);

    read_sectors_ATA_PIO(page, 0x12, 38);

    uint32_t* aa = page;
    printf("\r\n\r\n");

    if(aa[0] == 0x464C457F)
    {
        printf("Kernel is ELF File:\r\n");
    }
    int bits = (aa[1] >> 0) & 0xFF; //5
    if(bits == 1)
    {
        printf("\t32-bit\r\n");
    }
    int endian = (aa[1] >> 8) & 0xFF; //6
    if(endian == 1)
    {
        printf("\tlittle endian\r\n");
    }
    printf("\tentry position : 0x%x \r\n", page + aa[6]);
    printf("\texecuting kernel from: 0x%x....\r\n\r\n", page + aa[6]);

    void (*func)(void) = page + aa[6]; func();
}