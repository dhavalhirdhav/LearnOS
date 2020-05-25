#include <stdint.h>

#define MAX_INTERRUPTS 256
#define IDT_DESC_TRAP 0x01	//00000001
#define IDT_DESC_BIT16 0x06	//00000110
#define IDT_DESC_BIT32 0x0E	//00001110
#define IDT_DESC_RING1 0x40	//01000000
#define IDT_DESC_RING2 0x20	//00100000
#define IDT_DESC_RING3 0x60	//01100000
#define IDT_DESC_PRESENT 0x80//10000000

typedef struct __attribute__ ((__packed__)) idtr {
    uint16_t    limit;
    uint32_t    base;
} idtr_t;

typedef struct __attribute__ ((__packed__)) idt_descriptor {
    uint16_t		baseLo;
    uint16_t		sel;
    uint8_t			reserved;
    uint8_t			flags;
    uint16_t		baseHi;
} idt_descriptor_t;

typedef struct __attribute__ ((__packed__)) registers {
   uint32_t ds; /* Data segment selector */
   uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; /* Pushed by pusha. */
   uint32_t int_no, err_code; /* Interrupt number and error code (if applicable) */
   uint32_t eip, cs, eflags, useresp, ss; /* Pushed by the processor automatically */
} registers_t;

struct interrupt_frame;

//Global variables in this routine
static idt_descriptor_t _idt[MAX_INTERRUPTS];
static idtr_t _idtr;

void setupIDT();
void install_ir(uint32_t index,uint16_t flags, uint16_t sel, uint32_t *handler_address);