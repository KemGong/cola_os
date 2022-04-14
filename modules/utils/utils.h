#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdint.h>
#define bcd2bin(val) (((val) & 0x0f) + ((val) >> 4) * 10)
#define bin2bcd(val) ((((val) / 10) << 4) + (val) % 10)
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) < (b) ? (b) : (a))
#define IS_POWER_OF_TWO(A) ( ((A) != 0) && ((((A) - 1) & (A)) == 0) )
#define get_bits(val,x1,x2) (((val)>>(x1))&((1<<((x2)-(x1)+1))-1))
#define array_size(array) (sizeof(array)/sizeof(*array))
#ifndef min
#define min(a, b) ((a)<(b) ? (a):(b))
#endif
#ifndef max
#define max(a, b) ((a)>(b) ? (a):(b))
#endif

#ifndef offset_of
# define offset_of(TYPE, MEMBER) ((unsigned long) &((TYPE *)0)->MEMBER)
#endif

#define ABS(x)   ((x)>0?(x):-(x))

void put_be_val(uint32_t val, uint8_t * p, int bytes);
uint32_t get_be_val(const uint8_t * p, int bytes);
#endif // _UTILS_H_
