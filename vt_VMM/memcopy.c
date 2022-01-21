#include<stdio.h>
#include <bits/types.h>
typedef __uint8_t uint8_t;
typedef __uint16_t uint16_t;
typedef __uint32_t uint32_t;
typedef __uint64_t uint64_t;

#define MSR_IA32_VMX_BASIC			0x00000480U
/* Read MSR */
static inline uint64_t cpu_msr_read(uint32_t reg)
{
    // msr can not read in usrspace 
    // if want read plsease write /dev/cpu/%i/msr
	uint32_t  msrl, msrh;

	asm volatile (" rdmsr ":"=a"(msrl), "=d"(msrh) : "c" (reg));
	return (((uint64_t)msrh << 32U) | msrl);
}

static inline int move_func(int m,int n){
    asm volatile("movl %1,%0"
    :"=r"(n)
    : "r"(m)
    );
    return n;
}

static inline void memcpy_erms(void *d, const void *s, size_t slen)
{
	asm volatile ("rep; movsb"
		: "=&D"(d), "=&S"(s)
		: "c"(slen), "0" (d), "1" (s)
		: "memory");
}


int main(int argc, char const *argv[])
{
    char a[100];
    char *src="hello world";
    memcpy_erms(a,src,20);
    printf("%s\n",a);
    /*
    int m=2,n=0;
    int ret = move_func(m,n);
    printf("%d\n",ret);
    */
    if (iopl(3) == -1) {
        printf("Cannot get I/O permissions (being root helps)\n");
        return -1;
    }
    uint32_t tmp32 = (uint32_t)cpu_msr_read(MSR_IA32_VMX_BASIC);
    printf("%x\n",tmp32);
    return 0;
}
