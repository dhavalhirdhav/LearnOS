#include <stdint.h>

#define PCI_CONFIG  0xCF8
#define PCI_DATA    0xCFC

#define PCI_VENDOR_DEVICE_REG   0x80000000
#define PCI_CLASS_SUBCLASS_REG  0x80000002

#define PCI_BASE_ADDRESS_0_REG  0x80000004
#define PCI_BASE_ADDRESS_1_REG  0x80000005
#define PCI_BASE_ADDRESS_2_REG  0x80000006
#define PCI_BASE_ADDRESS_3_REG  0x80000007
#define PCI_BASE_ADDRESS_4_REG  0x80000008
#define PCI_BASE_ADDRESS_5_REG  0x80000009

#define PCI_VENDOR_DEVICE 0x00
#define PCI_CLASS_SUBCLASS 0x08

#define PCI_BASE_ADDRESS_0_OFFSET   0x10
#define PCI_BASE_ADDRESS_1_OFFSET   0x14
#define PCI_BASE_ADDRESS_2_OFFSET   0x18
#define PCI_BASE_ADDRESS_3_OFFSET   0x1C
#define PCI_BASE_ADDRESS_4_OFFSET   0x20
#define PCI_BASE_ADDRESS_5_OFFSET   0x24

void pci_test();
void pci_find(uint8_t _class, uint8_t subclass, uint32_t* bus, uint32_t* device, uint32_t* function, uint16_t* vendor);