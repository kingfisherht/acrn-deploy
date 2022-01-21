#include <stdio.h>
#include <stdint.h>

/** Replaces 'x' by the string "x". */
#define STRINGIFY(x) #x
#define CPU_SEG_READ(seg, result_ptr)						\
{										\
	asm volatile ("mov %%" STRINGIFY(seg) ", %0": "=r" (*(result_ptr)));	\
}

int main(int argc, char const *argv[])
{
    uint16_t value16;
    CPU_SEG_READ(es, &value16);
    printf("es:%x\n",value16);

    return 0;
}
