#include "./msr.h"

/* 16-bit host-state fields */
#define VMX_HOST_ES_SEL     0x00000c00U
#define VMX_HOST_CS_SEL     0x00000c02U
#define VMX_HOST_SS_SEL     0x00000c04U
#define VMX_HOST_DS_SEL     0x00000c06U
#define VMX_HOST_FS_SEL     0x00000c08U
#define VMX_HOST_GS_SEL     0x00000c0aU
#define VMX_HOST_TR_SEL     0x00000c0cU
#define VMX_HOST_GDTR_BASE    0x00006c0cU
#define VMX_HOST_IDTR_BASE    0x00006c0eU

struct descriptor_table {
	uint16_t limit;
	uint64_t base;
} __packed;

void SetupVMCS(void);
void exec_vmptrld(void *addr);
void exec_vmclear(void *addr);

#define CPU_SEG_READ(seg, result_ptr)						\
{										\
	asm volatile ("mov %%" STRINGIFY(seg) ", %0": "=r" (*(result_ptr)));	\
}

static inline uint64_t sgdt(void)
{
	struct descriptor_table gdtb = {0U, 0UL};
	asm volatile ("sgdt %0":"=m"(gdtb)::"memory");
	return gdtb.base;
}