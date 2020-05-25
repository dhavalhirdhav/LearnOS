#include "common.h"

void enable_interrupt() {
    __asm__ __volatile__ ("sti");
}

void disable_interrupt() {
    __asm__ __volatile__ ("cli");
}
