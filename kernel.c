#include "drivers/screen.h"
#include "utils/utils.h"
#include "drivers/ata/ata.h"

void kmain(void) {
    clear();
    
    printf("Welcome to Learn OS.\r\n");
    printf("Memory address of kmain in: 0x%x\r\n\r\n", kmain);

    printf("reading...\r\n");

    uint32_t* target;

    read_sectors_ATA_PIO(0x0, 0x0, 2, target);
    
    int i;
    i = 0;
    while(i < 256)
    {
        printf("%x ", target[i] & 0xFF);
        printf("%x ", target[i] >> 8);
        i++;
    }

    printf("\r\n");
    printf("writing 0...\r\n");
    write_sectors_ATA_PIO(0x0, 2);


    printf("reading...\r\n");
    read_sectors_ATA_PIO(0x0, 0x0, 2, target);
    
    i = 0;
    while(i < 256)
    {
        printf("%x ", target[i] & 0xFF);
        printf("%x ", target[i] >> 8);
        i++;
    }
}