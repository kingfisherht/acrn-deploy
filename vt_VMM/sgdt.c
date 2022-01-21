#include<stdio.h>
typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef signed short int16_t;
typedef unsigned short uint16_t;
typedef signed int int32_t;
typedef unsigned int uint32_t;
typedef unsigned long uint64_t;
typedef signed long int64_t;


struct descriptor_table {
	uint16_t limit;
	uint64_t base;
} __packed;
static inline uint64_t sgdt(void)
{
	struct descriptor_table gdtb = {0U, 0UL};
	asm volatile ("sgdt %0":"=m"(gdtb)::"memory");
	return gdtb.base;
}
int main(int argc, char const *argv[])
{
    uint64_t gdt_base = sgdt();
    printf("sgdt=%ld\n",gdt_base);
    return 0;
}
