#include <stdint.h>

#define PCI_CONFIG  0xCF8
#define PCI_DATA    0xCFC

#define PCI_VENDOR_DEVICE_REG   0x80000000
#define PCI_CLASS_SUBCLASS_REG  0x80000002
#define PCI_BASE_ADDRESS_0  0x04
#define PCI_BASE_ADDERSS_1  0x05

#define PCI_VENDOR_DEVICE 0x00
#define PCI_CLASS_SUBCLASS 0x08
#define PCI_BASE_ADDERSS_0_OFFSET   0x10
#define PCI_BASE_ADDRESS_1_OFFSET   0x14

void pci_test();
void pci_find(uint8_t _class, uint8_t subclass, uint32_t* bus, uint32_t* device, uint32_t* function, uint16_t* vendor);