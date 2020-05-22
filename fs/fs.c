#include <stdint.h>
#include "fs.h"
#include "../drivers/ata/ata.h"
#include "../drivers/screen.h"

static fs_header_t *_fsheader;

void InitializeFS()
{
    _fsheader->header = 0x53464644;
    _fsheader->version = 0x30302E31;
    _fsheader->fs_table_start_address = 0x0000001F;
    _fsheader->fs_table_last_entry_address = 0x0000001F;

    write_sectors_ATA_PIO(0x1E, 1, _fsheader);
    WriteFile("Test.txt", "Test");
}

fs_file_structure_t WriteFile(char* fileName, char* bytes)
{
    fs_file_structure_t fs = FindFile(fileName);
    if(fs.sectoraddress == 0x00000000)
    {
        CreateFile(fileName, bytes);
    }
}

fs_file_structure_t FindFile(char* fileName)
{
    read_fs_header();

    fs_file_structure_t fs;
    // fs->name = fileName;
    // fs->artifact_type = 0;
    // fs->size = 0;
    fs.sectoraddress = 0x00000000;

    return fs;
}

fs_file_structure_t CreateFile(char* fileName, char* bytes)
{
    read_fs_header();
    printf("Creating new file entry at: %x", _fsheader->fs_table_last_entry_address);
    
    fs_file_structure_t *fs;
    fs->name = "Test";
    fs->artifact_type = 0;
    fs->size = 0;
    fs->sectoraddress = _fsheader->fs_table_last_entry_address;

    write_sectors_ATA_PIO(_fsheader->fs_table_last_entry_address, 1, fs);
}

void read_fs_header()
{
    uint32_t* target;
    read_sectors_ATA_PIO(target, FS_START_ADDRESS, 1);

    _fsheader->header = target[0];
    _fsheader->version = target[1];
    _fsheader->fs_table_start_address = target[2];
    _fsheader->fs_table_last_entry_address = target[3];
}