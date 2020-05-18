/*https://forum.osdev.org/viewtopic.php?f=1&p=167798#p167798*/
#include <stdint.h>

#define      IDE_ATA         0x00
#define      IDE_ATAPI       0x01

#define      ATA_MASTER      0x00
#define      ATA_SLAVE       0x01

// Channels:
#define      ATA_PRIMARY      0x00
#define      ATA_SECONDARY      0x01

// Directions:
#define      ATA_READ      0x00
#define      ATA_WRITE      0x01

#define    ATA_SR_BSY       0x80
#define    ATA_SR_DRDY      0x40
#define    ATA_SR_DF        0x20
#define    ATA_SR_DSC       0x10
#define    ATA_SR_DRQ       0x08
#define    ATA_SR_CORR      0x04
#define    ATA_SR_IDX       0x02
#define    ATA_SR_ERR       0x01

#define    ATA_ER_BBK      0x80
#define    ATA_ER_UNC      0x40
#define    ATA_ER_MC       0x20
#define    ATA_ER_IDNF     0x10
#define    ATA_ER_MCR      0x08
#define    ATA_ER_ABRT     0x04
#define    ATA_ER_TK0NF    0x02
#define    ATA_ER_AMNF     0x01

// ATA-Commands:
#define      ATA_CMD_READ_PIO         0x20
#define      ATA_CMD_READ_PIO_EXT     0x24
#define      ATA_CMD_READ_DMA         0xC8
#define      ATA_CMD_READ_DMA_EXT     0x25
#define      ATA_CMD_WRITE_PIO        0x30
#define      ATA_CMD_WRITE_PIO_EXT    0x34
#define      ATA_CMD_WRITE_DMA        0xCA
#define      ATA_CMD_WRITE_DMA_EXT    0x35
#define      ATA_CMD_CACHE_FLUSH      0xE7
#define      ATA_CMD_CACHE_FLUSH_EXT  0xEA
#define      ATA_CMD_PACKET           0xA0
#define      ATA_CMD_IDENTIFY_PACKET  0xA1
#define      ATA_CMD_IDENTIFY         0xEC

/*ATAPI - We will most likely not implement*/
#define      ATAPI_CMD_READ      0xA8
#define      ATAPI_CMD_EJECT      0x1B

/*The Commands ATA_CMD_IDENTIFY_PACKET, and ATA_CMD_IDENTIFY, returns a buffer of 512 byte,*/
/*the buffer is called Identification space, the following definitions are used to read information from the identification space.*/
#define    ATA_IDENT_DEVICETYPE   0
#define    ATA_IDENT_CYLINDERS   2
#define    ATA_IDENT_HEADS      6
#define    ATA_IDENT_SECTORS      12
#define    ATA_IDENT_SERIAL   20
#define    ATA_IDENT_MODEL      54
#define    ATA_IDENT_CAPABILITIES   98
#define    ATA_IDENT_FIELDVALID   106
#define    ATA_IDENT_MAX_LBA   120
#define    ATA_IDENT_COMMANDSETS   164
#define    ATA_IDENT_MAX_LBA_EXT   200

// ATA-ATAPI Task-File:
#define      ATA_REG_DATA      0x00
#define      ATA_REG_ERROR      0x01
#define      ATA_REG_FEATURES   0x01
#define      ATA_REG_SECCOUNT0   0x02
#define      ATA_REG_LBA0      0x03
#define      ATA_REG_LBA1      0x04
#define      ATA_REG_LBA2      0x05
#define      ATA_REG_HDDEVSEL   0x06
#define      ATA_REG_COMMAND      0x07
#define      ATA_REG_STATUS      0x07
#define      ATA_REG_SECCOUNT1   0x08
#define      ATA_REG_LBA3      0x09
#define      ATA_REG_LBA4      0x0A
#define      ATA_REG_LBA5      0x0B
#define      ATA_REG_CONTROL      0x0C
#define      ATA_REG_ALTSTATUS   0x0C
#define      ATA_REG_DEVADDRESS   0x0D

struct channel {
   unsigned short base;  // I/O Base.
   unsigned short ctrl;  // Control Base
   unsigned short bmide; // Bus Master IDE
   unsigned char  nIEN;  // nIEN (No Interrupt);
} channels[2];

unsigned char ide_buf[2048];
unsigned static char ide_irq_invoked = 0;
unsigned static char atapi_packet[12] = {0xA8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

struct ide_device {
   unsigned char  reserved;    // 0 (Empty) or 1 (This Drive really exists).
   unsigned char  channel;     // 0 (Primary Channel) or 1 (Secondary Channel).
   unsigned char  drive;       // 0 (Master Drive) or 1 (Slave Drive).
   unsigned short type;        // 0: ATA, 1:ATAPI.
   unsigned short sign;       // Drive Signature
   unsigned short capabilities;// Features.
   unsigned int   commandsets; // Command Sets Supported.
   unsigned int   size;       // Size in Sectors.
   unsigned char  model[41];   // Model in string.
} ide_devices[4];

void InitializeATA();
void ide_initialize(uint32_t bus, uint32_t device, uint32_t function, unsigned int BAR0, unsigned int BAR1, unsigned int BAR2, unsigned int BAR3, unsigned int BAR4);