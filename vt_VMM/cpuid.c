#include<stdio.h>
#include <bits/types.h>
typedef __uint8_t uint8_t;
typedef __uint16_t uint16_t;
typedef __uint32_t uint32_t;
typedef __uint64_t uint64_t;

static inline uint16_t get_pcpu_id(void)
{
	uint32_t tsl, tsh, cpu_id;

	asm volatile ("rdtscp":"=a" (tsl), "=d"(tsh), "=c"(cpu_id)::);
	return (uint16_t)cpu_id;
}
int main(int argc, char const *argv[])
{
    // uint16_t cpuid = get_pcpu_id();
    // printf("cpuid=%d\n",cpuid);
   uint16_t p;
   asm volatile("rdtscp"
   :"=c"(p)
   :
   :"memory");
   printf("cpuid=%d\n",p);
    return 0;
}
