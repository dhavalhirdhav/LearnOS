#include "idt.h"
#include "timer.h"
#include "../drivers/screen.h"
#include "../drivers/ports.h"

unsigned long timer_ticks = 0;

void initTimer(uint16_t count)
{
    int divisor = 1193180 / count;

    /*IRQ0*/
    install_ir(32, IDT_DESC_BIT32 | IDT_DESC_PRESENT, 0x08, timer_handler);

    port_byte_out(TIMER_CONTROL_PORT, 0x36);
    port_byte_out(TIMER_DATA_PORT, divisor & 0xFF);
    port_byte_out(TIMER_DATA_PORT, (divisor >> 8) & 0xFF);
}

__attribute__ ((interrupt)) void timer_handler(struct interrupt_frame *frame)
{
    timer_ticks++;
    port_byte_out(0x20, 0x20);

    /*later on we can use this space to switch task -- multi tasking*/
}

void sleep(int milliseconds) {
  while(timer_ticks < milliseconds);
  timer_ticks = 0;
}