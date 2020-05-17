#include <stdint.h>
#include "pci.h"
#include "../screen.h"
#include "../ports.h"
#include "../../utils/utils.h"

uint32_t pci_read(uint32_t bus, uint32_t device, uint32_t function, uint32_t vendor)
{
    uint32_t reg = 0x80000000;

    reg |= (bus & 0xFF) << 16;
    reg |= (device & 0x1F) << 11;
    reg |= (function & 0x7) << 8;
    reg |= (vendor & 0xFF) & 0xFC;

    port_byte_out(PCI_CONFIG, reg );

    return port_word_in(PCI_DATA);
}

void pci_test()
{
    uint32_t bus, device, function, vendor;
    printf("PCI:\r\n");
    for(bus = 0; bus < 256; bus++)
    {
        for(device = 0; device < 32; device++)
        {
            for(function = 0; function < 8; function++)
            {
                vendor = pci_read(bus, device, function, PCI_VENDOR_DEVICE);
                if(vendor != 0xFFFFFFFF)
                {
                    char bus_str[255], device_str[255], function_str[255], vendor_str[255], vendor_str2[255];
                    int_to_ascii(bus, bus_str);
                    int_to_ascii(device, device_str);
                    int_to_ascii(function, function_str);
                    int_to_ascii(vendor & 0xFFFF, vendor_str);
                    int_to_ascii(vendor >> 16, vendor_str2);
                    
                    printf(bus_str);
                    printf(":");
                    printf(device_str);
                    printf(":");
                    printf(function_str);
                    printf("-");
                    printf(vendor_str);
                    printf(":");
                    printf(vendor_str2);
                    printf("\r\n");
                }
            }
        }
    }
}