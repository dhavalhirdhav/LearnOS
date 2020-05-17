#include "drivers/screen.h"
#include "utils/utils.h"

void kmain(void) {
    clear();
    
    printf("Welcome to Learn OS.\r\n");
    printf("Memory address of kmain in: 0x%x\r\n", kmain);
}