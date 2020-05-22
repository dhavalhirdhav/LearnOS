#include <stdint.h>

#define FS_START_ADDRESS    0x1E /*30th Sector (30 x 512bytes = 15,360 (0x3c00))*/
#define SECTOR_SIZE         0x200 /*512 bytes*/

typedef struct __attribute__ ((__packed__)) fs_header {
    uint32_t header;
    uint32_t version;
    uint32_t fs_table_start_address;
    uint32_t fs_table_last_entry_address;
} fs_header_t;

typedef struct __attribute__ ((__packed__)) fs_file_structure {
    char* name;
    char    artifact_type; /*0 = file, 1 = folder*/
    uint32_t size;
    uint32_t createdate;
    uint32_t createtime;
    uint32_t updatedate;
    uint32_t updatetime;
    uint32_t sectorno;
    uint32_t sectoraddress;
} fs_file_structure_t;

void InitializeFS();

void read_fs_header();
fs_file_structure_t CreateFile(char* fileName, char* bytes);
fs_file_structure_t FindFile(char* fileName);
fs_file_structure_t WriteFile(char* fileName, char* bytes);