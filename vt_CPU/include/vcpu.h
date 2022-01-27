#define BSP_CPU_ID             0U
#define MAX_PCPU_NUM                  4U
#define CONFIG_STACK_SIZE 0x2000U
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


void init_pcpu_pre(void);
void init_pcpu_post(void);
