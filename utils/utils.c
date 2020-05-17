#include "utils.h"

void memcopy(uint8_t *source, uint8_t *destination, int total_bytes)
{
    int i;
    for(i = 0; i < total_bytes; i++)
    {
        *(destination + i) = *(source + i);
    }
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
 }

/*TODO: Works partially and needs massive improvement.*/
 void byte_to_hex(int b, char str[])
 {
     int quotient, remainder, i;

    if(b < 1)
    {
        char zero[1];
        int_to_ascii(0, zero);
        str[i++] = zero[0];
    }
    else
    {
        quotient = b;
        i = 0;
        while(quotient != 0)
        {
            remainder = quotient % 16;
            char newstr[1];
            switch (remainder)
            {
                case 10:
                    str[i++] = 'A';
                    break;
                case 11:
                    str[i++] = 'B';
                    break;
                case 12:
                    str[i++] = 'C';
                    break;
                case 13:
                    str[i++] = 'D';
                    break;
                case 14:
                    str[i++] = 'E';
                    break;
                case 15:
                    str[i++] = 'F';
                    break;
                default:
                    int_to_ascii(remainder, newstr);
                    str[i++] = newstr[0];
                    break;
            }

            quotient = quotient / 16;
        }
    }
 }