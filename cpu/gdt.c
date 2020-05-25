#include <stdint.h>
#include "gdt.h"

void gdt_set_descriptor(uint16_t i, uint32_t base, uint32_t limit, uint8_t access, uint8_t grand)
{
	if (i > GDT_DESCRIPTOR_COUNT)
		return;

	//! set limit and base addresses
	_gdt[i].baseLo	= (uint16_t)(base & 0xffff);
	_gdt[i].baseMid	= (uint8_t)((base >> 16) & 0xff);
	_gdt[i].baseHi	= (uint8_t)((base >> 24) & 0xff);
	_gdt[i].limit	= (uint16_t)(limit & 0xffff);

	//! set flags and grandularity bytes
	_gdt[i].flags = access;
	_gdt[i].grand = (uint8_t)((limit >> 16) & 0x0f);
	_gdt[i].grand |= grand & 0xf0;
}

void setupGDT() {
    _gdtr.limit = (sizeof (struct gdt_descriptor) * GDT_DESCRIPTOR_COUNT)-1;
    _gdtr.base = (uint32_t)&_gdt[0];

	gdt_set_descriptor(0, 0, 0, 0, 0);

    gdt_set_descriptor (1,0,0xffffffff,
		I86_GDT_DESC_READWRITE|I86_GDT_DESC_EXEC_CODE|I86_GDT_DESC_CODEDATA|I86_GDT_DESC_MEMORY,
		I86_GDT_GRAND_4K | I86_GDT_GRAND_32BIT | I86_GDT_GRAND_LIMITHI_MASK);

	gdt_set_descriptor (2,0,0xffffffff,
		I86_GDT_DESC_READWRITE|I86_GDT_DESC_CODEDATA|I86_GDT_DESC_MEMORY,
		I86_GDT_GRAND_4K | I86_GDT_GRAND_32BIT | I86_GDT_GRAND_LIMITHI_MASK);

	gdt_set_descriptor (3,0,0xffffffff,
		I86_GDT_DESC_READWRITE|I86_GDT_DESC_EXEC_CODE|I86_GDT_DESC_CODEDATA|I86_GDT_DESC_MEMORY|I86_GDT_DESC_DPL,
		I86_GDT_GRAND_4K | I86_GDT_GRAND_32BIT | I86_GDT_GRAND_LIMITHI_MASK);

	gdt_set_descriptor (4,0,0xffffffff,
		I86_GDT_DESC_READWRITE|I86_GDT_DESC_CODEDATA|I86_GDT_DESC_MEMORY|I86_GDT_DESC_DPL,
		I86_GDT_GRAND_4K | I86_GDT_GRAND_32BIT | I86_GDT_GRAND_LIMITHI_MASK);

    __asm__ __volatile__("lgdt %0" :: "m"(_gdtr));
}