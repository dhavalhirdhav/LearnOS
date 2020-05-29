#include "kernel.h"
#include "../drivers/screen.h"
#include "../cpu/gdt.h"
#include "../cpu/idt.h"
#include "../cpu/common.h"
#include "../cpu/timer.h"

void start()
{
    printf("LearnOS Kernel loading....\r\n\r\n");
    
    printf("Setting up GDT...\t\t\t\t\t\t\t\t");
    setupGDT();
    printf("Done\r\n");

    printf("Setting up IDT...\t\t\t\t\t\t\t\t");
    setupIDT();
    printf("Done\r\n");

    initTimer(100);

    enable_interrupt();

    int i = 0;
    for(i = 0; i < 10; i++)
    {
        printf("this should take 10 seconds to print: %d\r\n", i);
        sleep(100); /*sleep for 1 second*/
    }
}