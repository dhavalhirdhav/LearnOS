#include "drivers/screen.h"
#include "utils/utils.h"

void kmain(void) {
    clear();
    
    printf("Welcome to Learn OS.\r\n");
    printf("Memory address of kmain in (Dec): ");

    int i;
    i = 0;
    uintptr_t km = (uintptr_t)kmain;
    char kmainaddress[4];
    memcopy(&km, kmainaddress, 4);
    for(i = 0; i < 4; i++)
    {
        char dec[5];
        int_to_ascii(kmainaddress[i], dec);
        printf(dec);
    }
    printf("\r\n");
}