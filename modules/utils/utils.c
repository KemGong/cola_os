#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include <ctype.h>

int get_num(char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    return -1;
}

void put_be_val(uint32_t val, uint8_t * p, int bytes)
{
    while (bytes-- > 0)
    {
        *(p + bytes) = val & 0xFF;
        val >>= 8;
    }
}

uint32_t get_be_val(const uint8_t * p, int bytes)
{
    uint32_t ret = 0;
    while (bytes-- > 0)
    {
        ret <<= 8;
        ret |= *p++;
    }

    return ret;
}
