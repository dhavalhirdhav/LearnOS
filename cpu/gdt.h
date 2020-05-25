#include <stdint.h>

#define GDT_DESCRIPTOR_COUNT 5

#define I86_GDT_DESC_ACCESS		0x0001		//00000001
#define I86_GDT_DESC_READWRITE		0x0002		//00000010
#define I86_GDT_DESC_EXPANSION		0x0004		//00000100
#define I86_GDT_DESC_EXEC_CODE		0x0008		//00001000
#define I86_GDT_DESC_CODEDATA		0x0010		//00010000
#define I86_GDT_DESC_DPL		0x0060		//01100000
#define I86_GDT_DESC_MEMORY		0x0080		//10000000
#define I86_GDT_GRAND_LIMITHI_MASK	0x0f		//00001111
#define I86_GDT_GRAND_OS		0x10		//00010000
#define I86_GDT_GRAND_32BIT		0x40		//01000000
#define I86_GDT_GRAND_4K		0x80		//10000000

typedef struct __attribute__ ((__packed__)) gdtr
{
    uint16_t    limit;
    uint32_t    base;
} gdtr_t;

typedef struct __attribute__ ((__packed__)) gdt_descriptor
{
    uint16_t	limit;
	uint16_t	baseLo;
	uint8_t		baseMid;
	uint8_t		flags;
	uint8_t		grand;
	uint8_t		baseHi;
} gdt_descriptor_t;

//Global variables
static gdt_descriptor_t _gdt[GDT_DESCRIPTOR_COUNT];
static gdtr_t _gdtr;

void setupGDT();