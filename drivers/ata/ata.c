#include "ata.h"
#include "../pci/pci.h"
#include "../ports.h"

void InitializeATA()
{
    int sleep = 0;
    printf("\r\n");
    GetIDE();
    printf("\r\n");
    GetSATA();
}

void GetIDE()
{
    printf("Searching for IDE Drive...");
    
    uint16_t vendor;
    uint32_t bus, device, function;
    uint16_t ide_primary_io_base, ide_primary_ctrl, ide_secondary_io_base, ide_secondary_ctrl, ide_bus_master_base;

    pci_find(0x01, 0x00, &bus, &device, &function, &vendor); /*IDE*/
    if(vendor == 0x00)
    {
        printf("No IDE Drive found..\r\n");
    }
    else
    {
        printf("Done...\r\n");
        printf("\tbus: %d, device: %d, function: %d, vendor: %x\r\n", bus, device, function, vendor);
        uint16_t bar0 = pci_read(bus, device, function, PCI_BASE_ADDRESS_0_OFFSET, PCI_BASE_ADDRESS_0_REG);
        uint16_t bar1 = pci_read(bus, device, function, PCI_BASE_ADDRESS_1_OFFSET, PCI_BASE_ADDRESS_1_REG);
        uint16_t bar2 = pci_read(bus, device, function, PCI_BASE_ADDRESS_2_OFFSET, PCI_BASE_ADDRESS_2_REG);
        uint16_t bar3 = pci_read(bus, device, function, PCI_BASE_ADDRESS_3_OFFSET, PCI_BASE_ADDRESS_3_REG);
        uint16_t bar4 = pci_read(bus, device, function, PCI_BASE_ADDRESS_4_OFFSET, PCI_BASE_ADDRESS_4_REG);
        uint16_t bar5 = pci_read(bus, device, function, PCI_BASE_ADDRESS_5_OFFSET, PCI_BASE_ADDRESS_5_REG);

        printf("\t\tbar0: %x, bar1: %x, bar2: %x,\r\n\t\tbar3: %x, bar4: %x, bar5: %x\r\n", bar0, bar1, bar2, bar3, bar4, bar5);

        if(bar0 <= 1)
        {
            ide_primary_io_base = 0x1F0;
        }
        if(bar1 <= 1)
        {
            ide_primary_ctrl = 0x3F6;
        }
        if(bar2 <= 1)
        {
            ide_secondary_io_base = 0x170;
        }
        if(bar3 <= 1)
        {
            ide_secondary_ctrl = 0x376;
        }
        ide_bus_master_base = bar4;
        
        uint8_t i;
        printf("\t\t\tPRI: ");
        for( i = 2; i < 6; i++)
            printf("0x%x ", port_byte_in(ide_primary_io_base + i) );
        printf("\r\n");

        printf("\t\t\tSEC: ");
        for( i = 2; i < 6; i++)
            printf("0x%x ", port_byte_in(ide_secondary_io_base + i) );
        printf("\r\n");

        ide_initialize(bus, device, function, ide_primary_io_base, ide_primary_ctrl, ide_secondary_io_base, ide_secondary_ctrl, bar4);
    }
}

void GetSATA()
{
    printf("Searching for SATA Drive...");
    
    uint16_t vendor;
    uint32_t bus, device, function;
    uint16_t ide_primary_io_base, ide_primary_ctrl, ide_secondary_io_base, ide_secondary_ctrl, ide_bus_master_base;

    pci_find(0x01, 0x06, &bus, &device, &function, &vendor); /*SATA*/
    if(vendor == 0x00)
    {
        printf("No SATA Drive found..\r\n");
    }
    else
    {
        printf("Done...\r\n");
        printf("\tbus: %d, device: %d, function: %d, vendor: %x\r\n", bus, device, function, vendor);
        uint16_t bar0 = pci_read(bus, device, function, PCI_BASE_ADDRESS_0_OFFSET, PCI_BASE_ADDRESS_0_REG);
        uint16_t bar1 = pci_read(bus, device, function, PCI_BASE_ADDRESS_1_OFFSET, PCI_BASE_ADDRESS_1_REG);
        uint16_t bar2 = pci_read(bus, device, function, PCI_BASE_ADDRESS_2_OFFSET, PCI_BASE_ADDRESS_2_REG);
        uint16_t bar3 = pci_read(bus, device, function, PCI_BASE_ADDRESS_3_OFFSET, PCI_BASE_ADDRESS_3_REG);
        uint16_t bar4 = pci_read(bus, device, function, PCI_BASE_ADDRESS_4_OFFSET, PCI_BASE_ADDRESS_4_REG);
        uint16_t bar5 = pci_read(bus, device, function, PCI_BASE_ADDRESS_5_OFFSET, PCI_BASE_ADDRESS_5_REG);

        printf("\t\tbar0: %x, bar1: %x, bar2: %x,\r\n\t\tbar3: %x, bar4: %x, bar5: %x\r\n", bar0, bar1, bar2, bar3, bar4, bar5);

        if(bar0 <= 1)
        {
            ide_primary_io_base = 0x1F0;
        }
        if(bar1 <= 1)
        {
            ide_primary_ctrl = 0x3F6;
        }
        if(bar2 <= 1)
        {
            ide_secondary_io_base = 0x170;
        }
        if(bar3 <= 1)
        {
            ide_secondary_ctrl = 0x376;
        }
        ide_bus_master_base = bar4;
        
        uint8_t i;
        printf("\t\t\tPRI: ");
        for( i = 2; i < 6; i++)
            printf("0x%x ", port_byte_in(ide_primary_io_base + i) );
        printf("\r\n");

        printf("\t\t\tSEC: ");
        for( i = 2; i < 6; i++)
            printf("0x%x ", port_byte_in(ide_secondary_io_base + i) );
        printf("\r\n");

        ide_initialize(bus, device, function, ide_primary_io_base, ide_primary_ctrl, ide_secondary_io_base, ide_secondary_ctrl, bar4);
    }
}

void ide_write(unsigned char channel, unsigned char reg, unsigned char data) {
   if   (reg > 0x07 && reg < 0x0C) ide_write(channel, ATA_REG_CONTROL, 0x80 | channels[channel].nIEN);
   if   (reg < 0x08) port_byte_out(data, channels[channel].base  + reg - 0x00);
   else if   (reg < 0x0C) port_byte_out(data, channels[channel].base  + reg - 0x06);
   else if   (reg < 0x0E) port_byte_out(data, channels[channel].ctrl  + reg - 0x0A);
   else if   (reg < 0x16) port_byte_out(data, channels[channel].bmide + reg - 0x0E);
   if   (reg > 0x07 && reg < 0x0C) ide_write(channel, ATA_REG_CONTROL, channels[channel].nIEN);
}

unsigned char ide_read(unsigned char channel, unsigned char reg) {
   unsigned char result;
   if   (reg > 0x07 && reg < 0x0C) ide_write(channel, ATA_REG_CONTROL, 0x80 | channels[channel].nIEN);
   if   (reg < 0x08) result = port_byte_in(channels[channel].base  + reg - 0x00);
   else if   (reg < 0x0C) result = port_byte_in(channels[channel].base  + reg - 0x06);
   else if   (reg < 0x0E) result = port_byte_in(channels[channel].ctrl  + reg - 0x0A);
   else if   (reg < 0x16) result = port_byte_in(channels[channel].bmide + reg - 0x0E);
   if   (reg > 0x07 && reg < 0x0C) ide_write(channel, ATA_REG_CONTROL, channels[channel].nIEN);
   return result;
}

void ide_read_buffer(unsigned char channel, unsigned char reg, unsigned int buffer, unsigned int quads) {
   if   (reg > 0x07 && reg < 0x0C) ide_write(channel, ATA_REG_CONTROL, 0x80 | channels[channel].nIEN);
   asm("pushw %es; movw %ds, %ax; movw %ax, %es");
//    if   (reg < 0x08) insl(channels[channel].base  + reg - 0x00, buffer, quads);
//    else if   (reg < 0x0C) insl(channels[channel].base  + reg - 0x06, buffer, quads);
//    else if   (reg < 0x0E) insl(channels[channel].ctrl  + reg - 0x0A, buffer, quads);
//    else if   (reg < 0x16) insl(channels[channel].bmide + reg - 0x0E, buffer, quads);
   asm("popw %es;");
   if   (reg > 0x07 && reg < 0x0C) ide_write(channel, ATA_REG_CONTROL, channels[channel].nIEN);
}

void ide_initialize(uint32_t bus, uint32_t device, uint32_t function, unsigned int BAR0, unsigned int BAR1, unsigned int BAR2, unsigned int BAR3, unsigned int BAR4)
{
    int i, j, k, count = 0;
 
    // 1- Detect I/O Ports which interface IDE Controller:
    channels[ATA_PRIMARY  ].base  = (BAR0 & 0xFFFFFFFC) + 0x1F0 * (!BAR0);
    channels[ATA_PRIMARY  ].ctrl  = (BAR1 & 0xFFFFFFFC) + 0x3F6 * (!BAR1);
    channels[ATA_SECONDARY].base  = (BAR2 & 0xFFFFFFFC) + 0x170 * (!BAR2);
    channels[ATA_SECONDARY].ctrl  = (BAR3 & 0xFFFFFFFC) + 0x376 * (!BAR3);
    channels[ATA_PRIMARY  ].bmide = (BAR4 & 0xFFFFFFFC) + 0; // Bus Master IDE
    channels[ATA_SECONDARY].bmide = (BAR4 & 0xFFFFFFFC) + 8; // Bus Master IDE

    // 2- Disable IRQs:
    ide_write(ATA_PRIMARY  , ATA_REG_CONTROL, 2);
    ide_write(ATA_SECONDARY, ATA_REG_CONTROL, 2);

    // 3- Detect ATA Devices:
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 2; j++)
        {
            unsigned char err = 0, type = IDE_ATA, status;
            ide_devices[count].reserved = 0;

            /*Select Drive*/
            ide_write(i, ATA_REG_HDDEVSEL, 0xA0 | (j << 4));

            /*Identify ATA*/
            ide_write(i, ATA_REG_COMMAND, ATA_CMD_IDENTIFY);

            /*IDE Polling*/
            if (ide_read(i, ATA_REG_STATUS) == 0) continue;
            printf("polling");

            while(1) {
                status = ide_read(i, ATA_REG_STATUS);
                if ((status & ATA_SR_ERR)) {err = 1; break;} // If Err, Device is not ATA.
                if (!(status & ATA_SR_BSY) && (status & ATA_SR_DRQ)) break; // Everything is right.
            }

            printf("identified");

            /*Read Identification Space of a Device*/
            ide_read_buffer(i, ATA_REG_DATA, (unsigned int) ide_buf, 128);

            // Read Device Parameters:
            ide_devices[count].reserved     = 1;
            ide_devices[count].type         = type;
            ide_devices[count].channel      = i;
            ide_devices[count].drive        = j;
            ide_devices[count].sign    = *((unsigned short *)(ide_buf + ATA_IDENT_DEVICETYPE));
            ide_devices[count].capabilities = *((unsigned short *)(ide_buf + ATA_IDENT_CAPABILITIES));
            ide_devices[count].commandsets  = *((unsigned int *)(ide_buf + ATA_IDENT_COMMANDSETS));

            /*Get Size*/
            if (ide_devices[count].commandsets & (1 << 26))
                // Device uses 48-Bit Addressing:
                ide_devices[count].size   = *((unsigned int *)(ide_buf + ATA_IDENT_MAX_LBA_EXT));
            else
                // Device uses CHS or 28-bit Addressing:
                ide_devices[count].size   = *((unsigned int *)(ide_buf + ATA_IDENT_MAX_LBA));

            /*Print Device Info*/
            for (i = 0; i < 4; i++)
                if (ide_devices[i].reserved == 1) {
                    printf(" Found %s Drive %dGB - %s\n",
                        (const char *[]){"ATA", "ATAPI"}[ide_devices[i].type],         /* Type */
                        ide_devices[i].size / 1024 / 1024 / 2,               /* Size */
                        ide_devices[i].model);
                }
        }
    }


    // port_long_out((1 << 31) | (bus << 16) | (device << 11) | (function << 8) | 8, 0xCF8);
    // uint32_t class = port_long_in(0xCFC);
    // if(class != 0xFFFF)
    // {
    //     port_long_out((1 << 31) | (bus << 16) | (device << 11) | (function << 8) | 0x3C, 0xCF8);
    //     port_byte_out(0xFE, 0xCFC); // Change the IRQ field to 0xFE
    //     port_long_out((1 << 31) | (bus << 16) | (device << 11) | (function << 8) | 0x3C, 0xCF8);
    //     uint32_t IRQAssignReq = port_long_in(0xCFC);
    //     if(IRQAssignReq & 0xFF == 0xFE)
    //     {
    //         // This device needs an IRQ assignment.
    //     }
    //     else
    //     {
    //         // if (class == 0x01 && subclass == 0x01 && (ProgIF == 0x8A || ProgIF == 0x80)) {
    //         //     // This is a Parallel IDE Controller which uses IRQs 14 and 15.
    //         // }
    //     }
    // }
}