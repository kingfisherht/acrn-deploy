#include<stdio.h>


static inline uint16_t get_pcpu_id(void)
{
	uint32_t tsl, tsh, cpu_id;

	asm volatile ("rdtscp":"=a" (tsl), "=d"(tsh), "=c"(cpu_id)::);
	return (uint16_t)cpu_id;
}

int main(int argc, char const *argv[])
{
    uint16_t pcpu_id;

	pcpu_id = get_pcpu_id();

    printf("pcpu_id=%d\n",pcpu_id);




    return 0;
}
