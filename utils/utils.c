#include "utils.h"

void memcopy(uint8_t *source, uint8_t *destination, int total_bytes)
{
    int i;
    for(i = 0; i < total_bytes; i++)
    {
        *(destination + i) = *(source + i);
    }
}

void reverse_string(char str[])
{
    int c, i, j;
    for (i = 0, j = strlen(str)-1; i < j; i++, j--) {
        c = str[i];
        str[i] = str[j];
        str[j] = c;
    }
}

int strlen(char s[]) {
    int i = 0;
    while (s[i] != '\0') ++i;
    return i;
}

void int_to_ascii(int n, char str[])
{
    int i, sign;
    if ((sign = n) < 0) n = -n;

    i = 0;
    do
    {
        str[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);
    
    if(sign < 0) str[i++] = '-';
    str[i] = '\0';

    reverse_string(str);
 }

void hex_to_ascii(int n, char str[]) {
    append(str, '0');
    append(str, 'x');
    char zeros = 0;

    int32_t tmp;
    int i;
    for (i = 28; i > 0; i -= 4) {
        tmp = (n >> i) & 0xF;
        if (tmp == 0 && zeros == 0) continue;
        zeros = 1;
        if (tmp > 0xA) append(str, tmp - 0xA + 'a');
        else append(str, tmp + '0');
    }

    tmp = n & 0xF;
    if (tmp >= 0xA) append(str, tmp - 0xA + 'a');
    else append(str, tmp + '0');
}

void append(char s[], char n) {
    int len = strlen(s);
    s[len] = n;
    s[len+1] = '\0';
}