#include <stdint.h>
#include "pci.h"
#include "../screen.h"
#include "../ports.h"
#include "../../utils/utils.h"

uint32_t pci_read(uint32_t bus, uint32_t device, uint32_t function, uint32_t offset, uint32_t reg)
{
    reg |= (bus & 0xFF) << 16;
    reg |= (device & 0x1F) << 11;
    reg |= (function & 0x7) << 8;
    reg |= (offset & 0xFF) & 0xFC;

    port_long_out(PCI_CONFIG, reg);

    return port_long_in(PCI_DATA);
}

void pci_write(uint32_t bus, uint32_t device, uint32_t function, uint32_t offset, uint32_t reg, uint32_t data)
{
    reg |= (bus & 0xFF) << 16;
    reg |= (device & 0x1F) << 11;
    reg |= (function & 0x7) << 8;
    reg |= offset & 0xFC;

    port_byte_out(PCI_CONFIG, reg);
    port_byte_out(PCI_DATA, data);
}

void pci_find(uint8_t _class, uint8_t subclass, uint32_t* bus, uint32_t* device, uint32_t* function, uint16_t* vendor)
{
    uint32_t tmp_bus, tmp_device, tmp_function, tmp_vendor, tmp_subclass;
    for(tmp_bus = 0; tmp_bus < 256; tmp_bus++)
    {
        for(tmp_device = 0; tmp_device < 32; tmp_device++)
        {
            for(tmp_function = 0; tmp_function < 8; tmp_function++)
            {
                tmp_vendor = pci_read(tmp_bus, tmp_device, tmp_function, PCI_VENDOR_DEVICE, PCI_VENDOR_DEVICE_REG);
                if(tmp_vendor != 0xFFFFFFFF)
                {
                    tmp_subclass = pci_read(tmp_bus, tmp_device, tmp_function, PCI_CLASS_SUBCLASS, PCI_CLASS_SUBCLASS_REG);
                    if(_class == (tmp_subclass & 0x000000FF) && subclass == (tmp_subclass & 0x0000FF00) >> 8)
                    {
                        *bus = tmp_bus;
                        *device = tmp_device;
                        *function = tmp_function;
                        *vendor = tmp_vendor;
                        return;
                    }
                }
            }
        }
    }
}

void pci_test()
{
    uint32_t bus, device, function, vendor, subclass;
    printf("Devices over PCI:\r\n");
    printf("---------------------------------------------------------\r\n");
    printf("| BUS | DEV | FUN | VENDOR ID | DEVICE ID | CLAS | SUBC |\r\n");
    printf("---------------------------------------------------------\r\n");
    for(bus = 0; bus < 256; bus++)
    {
        for(device = 0; device < 32; device++)
        {
            for(function = 0; function < 8; function++)
            {
                vendor = pci_read(bus, device, function, PCI_VENDOR_DEVICE, PCI_VENDOR_DEVICE_REG);
                if(vendor != 0xFFFFFFFF)
                {
                    subclass = pci_read(bus, device, function, PCI_CLASS_SUBCLASS, PCI_CLASS_SUBCLASS_REG);
                    // printf("%x\t", vendor);
                    // printf("%x\t", (vendor & 0xFFFF0000) >> 16); /*device id*/
                    // printf("%x\t", (vendor & 0xFFFF)); /*vendor id*/
                    
                    // printf("%x\t", subclass);
                    // printf("%x\t", (subclass & 0xFF000000) >> 24); /*class*/
                    // printf("%x\t", (subclass & 0xFF0000) >> 16); /*subclass*/
                    // printf("%x\t", (subclass & 0xFF00) >> 8); /*progif*/
                    // printf("%x\t", (subclass & 0xFF)); /*revisionid*/

                    printf("|  %d  |  %d  |  %d  |    %x   |   %x    | %x   |   %x   |\r\n", bus, device, function, (vendor & 0x0000FFFF), (vendor & 0xFFFF0000) >> 16, (subclass & 0x000000FF) , (subclass & 0x0000FF00) >> 8); /*, (subclass & 0x00FF0000) >> 16, (subclass & 0xFF000000) >> 24);*/
                    //uint32_t bar0 = pci_read(bus, device, function, PCI_BASE_ADDRESS_0_OFFSET, PCI_BASE_ADDRESS_0_REG);
                    //printf("Bar0: %x\r\n", bar0);
                }
            }
        }
    }
    printf("---------------------------------------------------------\r\n");
}