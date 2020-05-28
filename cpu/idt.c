#include <stdint.h>
#include "idt.h"
#include "../drivers/screen.h"

void install_ir(uint32_t index,uint16_t flags, uint16_t sel, uint32_t *handler_address)
{
	if (index >=MAX_INTERRUPTS) return;

	_idt[index].baseLo = (uint32_t)handler_address & 0xffff;
	_idt[index].baseHi = ((uint32_t)handler_address >> 16) & 0xffff;
	_idt[index].reserved = 0;
	_idt[index].flags = flags;
	_idt[index].sel = sel;
}

void init_pic()
{
	//ICW 1  Expect IC4|single?|0|level?|init?|000
	port_byte_out(0x20,0x11);
	port_byte_out(0xA0,0x11);
	
	//ICW 2  Remapping the IRQs
	port_byte_out(0x21,0x20);
	port_byte_out(0xA1,0x28);

	// Send ICW 3 to primary PIC
	// 0x4 = 0100 Second bit (IR Line 2)
	port_byte_out(0x21,0x04);

	// Send ICW 3 to secondary PIC
	// 010=> IR line 2
	// write to data register of secondary PIC
	port_byte_out(0xA1,0x02);

	// Send ICW 4 - Set x86 mode --------------------------------
	// bit 0 enables 80x86 mode
	port_byte_out(0x21,0x01);
	port_byte_out(0xA1,0x01);
 
	
	// Setting the IMR - All interrupts enabled
	port_byte_out(0x21,0);
	port_byte_out(0xA1,0);
}

__attribute__ ((interrupt))
void default_handler(struct interrupt_frame *frame)
{
    printf("Default handler called - %x\r\n", frame);
    port_byte_out(0x20, 0x20);
}

void setupIDT() {
    _idtr.base = (uint32_t)&_idt[0];
	_idtr.limit = (sizeof (idt_descriptor_t) * MAX_INTERRUPTS) -1;

    int i = 0;
    //clear all interrupts
    for (i=0;i<MAX_INTERRUPTS;i++)
	{
		_idt[i].baseLo = 0;
		_idt[i].sel = 0;
		_idt[i].reserved = 0;
		_idt[i].flags = 0;
		_idt[i].baseHi = 0;
	}

    for (i=0;i<MAX_INTERRUPTS;i++)
        install_ir(i,IDT_DESC_BIT32 | IDT_DESC_PRESENT, 0x08, default_handler);

    init_pic();
    __asm__ __volatile__("lidt %0" :: "m"(_idtr));
}