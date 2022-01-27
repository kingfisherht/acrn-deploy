#include "./include/gdt.h"
#include <linux/types.h>
typedef __uint8_t uint8_t;
typedef __uint16_t uint16_t;
typedef __uint32_t uint32_t;
typedef __uint64_t uint64_t;

#define BSP_CPU_ID             0U
#define MAX_PCPU_NUM                  4U
/* CPUID source operands */
#define CPUID_VENDORSTRING      0U
#define CPUID_FEATURES          1U
#define CPUID_TLB               2U
#define CPUID_SERIALNUM         3U
#define CPUID_EXTEND_FEATURE    7U
#define CPUID_XSAVE_FEATURES   0xDU
#define CPUID_RDT_ALLOCATION   0x10U
#define CPUID_MAX_EXTENDED_FUNCTION  0x80000000U
#define CPUID_EXTEND_FUNCTION_1      0x80000001U
#define CPUID_EXTEND_FUNCTION_2      0x80000002U
#define CPUID_EXTEND_FUNCTION_3      0x80000003U
#define CPUID_EXTEND_FUNCTION_4      0x80000004U
#define CPUID_EXTEND_INVA_TSC        0x80000007U
#define CPUID_EXTEND_ADDRESS_SIZE    0x80000008U

#define PAGE_SHIFT	12U
#define PAGE_SIZE	(1U << PAGE_SHIFT)


struct per_cpu_region {
	struct host_gdt gdt;
	struct tss_64 tss;
	uint8_t mc_stack[CONFIG_STACK_SIZE] __aligned(16);
	uint8_t df_stack[CONFIG_STACK_SIZE] __aligned(16);
	uint8_t sf_stack[CONFIG_STACK_SIZE] __aligned(16);
	uint8_t stack[CONFIG_STACK_SIZE] __aligned(16);

} __aligned(PAGE_SIZE); /* per_cpu_region size aligned with PAGE_SIZE */



static inline void cpuid_subleaf(uint32_t leaf, uint32_t subleaf,
				uint32_t *eax, uint32_t *ebx,
				uint32_t *ecx, uint32_t *edx)
{
	/* Execute CPUID instruction and save results */
	asm volatile("cpuid":"=a"(*eax), "=b"(*ebx),
			"=c"(*ecx), "=d"(*edx)
			: "a" (leaf), "c" (subleaf)
			: "memory");
}
extern struct per_cpu_region per_cpu_data[MAX_PCPU_NUM];
/*
 * get percpu data for pcpu_id.
 */
#define per_cpu(name, pcpu_id)	\
	(per_cpu_data[(pcpu_id)].name)

/* get percpu data for current pcpu */
#define get_cpu_var(name)	per_cpu(name, get_pcpu_id())


void init_pcpu_pre(void);
void init_pcpu_post(uint16_t pcpu_id);
void load_gdtr_and_tr(void);