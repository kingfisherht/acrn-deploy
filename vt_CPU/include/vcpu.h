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
/*
 * Segment selectors in x86-64 and i386 are the same size, 8 bytes.
 * Local Descriptor Table (LDT) selectors are 16 bytes on x86-64 instead of 8
 * bytes.
 * Task State Segment (TSS) selectors are 16 bytes on x86-64 instead of 8 bytes.
 */
#define X64_SEG_DESC_SIZE (0x8U)	/* In long mode SEG Descriptors are 8 bytes */
#define X64_LDT_DESC_SIZE (0x10U)/* In long mode LDT Descriptors are 16 bytes */
#define X64_TSS_DESC_SIZE (0x10U)/* In long mode TSS Descriptors are 16 bytes */

/* Number of global 8 byte segments descriptor(s) */
#define    HOST_GDT_RING0_SEG_SELECTORS   (0x3U)	/* rsvd, code, data */
/* Offsets of global 8 byte segment descriptors */
#define    HOST_GDT_RING0_RSVD_SEL        (0x0000U)
#define    HOST_GDT_RING0_CODE_SEL        (0x0008U)
#define    HOST_GDT_RING0_DATA_SEL        (0x0010U)
/* Number of global 16 byte LDT descriptor(s) */
#define    HOST_GDT_RING0_TSS_SELECTORS   (0x1U)
/* One for each CPU in the hypervisor. */


/* Offset to start of LDT Descriptors */
#define HOST_GDT_RING0_LDT_SEL		\
	(HOST_GDT_RING0_SEG_SELECTORS * X64_SEG_DESC_SIZE)
/* Offset to start of LDT Descriptors */
#define HOST_GDT_RING0_CPU_TSS_SEL (HOST_GDT_RING0_LDT_SEL)
/* Size of the GDT */
#define HOST_GDT_SIZE							\
	(HOST_GDT_RING0_CPU_TSS_SEL +					\
		(HOST_GDT_RING0_TSS_SELECTORS * X64_TSS_DESC_SIZE))

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
