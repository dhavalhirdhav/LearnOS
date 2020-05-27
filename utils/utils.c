#include "utils.h"

void memcopy(uint8_t *source, uint8_t *destination, int total_bytes)
{
    int i;
    for(i = 0; i < total_bytes; i++)
    {
        *(destination + i) = *(source + i);
    }
}