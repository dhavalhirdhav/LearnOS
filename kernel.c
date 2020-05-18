#include "drivers/screen.h"
#include "utils/utils.h"
#include "drivers/pci/pci.h"
#include "drivers/ata/ata.h"

void kmain(void) {
    clear();
    
    printf("Welcome to Learn OS.\r\n");
    printf("kmain located at: 0x%x\r\n\r\n", kmain);

    pci_test();
    InitializeATA();
}