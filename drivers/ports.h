#include <stdint.h>

unsigned char port_byte_in (unsigned short port);
void port_byte_out (unsigned short port, unsigned char data);
unsigned char port_word_in (unsigned short port);
void port_word_out (unsigned short port, unsigned short data);

uint32_t port_long_in(uint32_t port);
void port_long_out(uint32_t port, uint32_t value);