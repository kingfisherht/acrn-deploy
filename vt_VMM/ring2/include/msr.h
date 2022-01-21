#include <linux/types.h> 

int32_t memcpy_s(void *d, size_t dmax, const void *s, size_t slen);
#define CPUID_FEATURES          1U
#define CR4_VMXE                (1UL<<13U)	/* VMX enable */


/** Replaces 'x' by the string "x". */
#define STRINGIFY(x) #x
/* Read control register */
#define CPU_CR_READ(cr, result_ptr)				\
{								\
	asm volatile ("mov %%" STRINGIFY(cr) ", %0"		\
			: "=r"(*(result_ptr)));			\
}

/* Write control register */
#define CPU_CR_WRITE(cr, value)					\
{								\
	asm volatile ("mov %0, %%" STRINGIFY(cr)		\
			: /* No output */			\
			: "r"(value));				\
}

/* Read MSR */
static inline uint64_t cpu_msr_read(uint32_t reg)
{
	uint32_t  msrl, msrh;

	asm volatile (" rdmsr ":"=a"(msrl), "=d"(msrh) : "c" (reg));
	return (((uint64_t)msrh << 32U) | msrl);
}

static inline uint64_t msr_read(uint32_t reg_num)
{
	return cpu_msr_read(reg_num);
}
/* Write MSR */
static inline void cpu_msr_write(uint32_t reg, uint64_t msr_val)
{
	asm volatile (" wrmsr " : : "c" (reg), "a" ((uint32_t)msr_val), "d" ((uint32_t)(msr_val >> 32U)));
}

static inline void msr_write(uint32_t reg_num, uint64_t value64)
{
	cpu_msr_write(reg_num, value64);
}

static inline void exec_vmxon(void *addr)
{
	/* Turn VMX on, pre-conditions can avoid VMfailInvalid
	 * here no need check RFLAGS since it will generate #GP or #UD
	 * except VMsuccess. SDM 30.3
	 */
	asm volatile (
			"vmxon (%%rax)\n"
			:
			: "a"(addr)
			: "cc", "memory");

}

static inline uint32_t reg_read(void)
{

        uint32_t dst;
        asm("pushfw; popw %%ax"
        :"=a"(dst)
        :  
        );

	return dst >> 16;
}

