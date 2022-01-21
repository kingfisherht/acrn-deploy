#include <linux/module.h>
#include <linux/init.h>  
#include <linux/kernel.h> 


#include "./include/vmcs.h"
#include "./include/cpudata.h"
#include "./include/macro.h"



MODULE_LICENSE("GPL");
#define exec_vmwrite exec_vmwrite64;
void exec_vmwrite64(uint32_t field_full, uint64_t value)
{
	asm volatile (
		"vmwrite %%rax, %%rdx "
		: : "a" (value), "d"(field_full)
		: "cc");
}

void exec_vmwrite32(uint32_t field, uint32_t value)
{
	exec_vmwrite64(field, (uint64_t)value);
}

void exec_vmwrite16(uint32_t field, uint16_t value)
{
	exec_vmwrite64(field, (uint64_t)value);
}

void exec_vmclear(void *addr)
{

	/* pre-conditions can avoid VMfail
	 * here no need check RFLAGS since it will generate #GP or #UD
	 * except VMsuccess. SDM 30.3
	 */
	asm volatile (
		"vmclear (%%rax)\n"
		:
		: "a"(addr)
		: "cc", "memory");
}


void exec_vmptrld(void *addr)
{
	/* pre-conditions can avoid VMfail
	 * here no need check RFLAGS since it will generate #GP or #UD
	 * except VMsuccess. SDM 30.3
	 */
	asm volatile (
		"vmptrld (%%rax)\n"
		:
		: "a"(addr)
		: "cc", "memory");
}
void init_host_state(void) {
    uint16_t value16;
	uint64_t value64;
	uint64_t value;
	uint64_t tss_addr;
	uint64_t gdt_base;
	uint64_t idt_base;

    printk("Initialize host state\n");

    CPU_SEG_READ(es, &value16);
	exec_vmwrite16(VMX_HOST_ES_SEL, value16);
	printk("VMX_HOST_ES_SEL: 0x%hx ", value16);

    CPU_SEG_READ(cs, &value16);
	exec_vmwrite16(VMX_HOST_CS_SEL, value16);
    printk("VMX_HOST_CS_SEL: 0x%hx ", value16);

 	CPU_SEG_READ(ss, &value16);
	exec_vmwrite16(VMX_HOST_SS_SEL, value16);
	printk("VMX_HOST_SS_SEL: 0x%hx ", value16);

	CPU_SEG_READ(ds, &value16);
	exec_vmwrite16(VMX_HOST_DS_SEL, value16);
	printk("VMX_HOST_DS_SEL: 0x%hx ", value16);

	CPU_SEG_READ(fs, &value16);
	exec_vmwrite16(VMX_HOST_FS_SEL, value16);
	printk("VMX_HOST_FS_SEL: 0x%hx ", value16);

	CPU_SEG_READ(gs, &value16);
	exec_vmwrite16(VMX_HOST_GS_SEL, value16);
	printk("VMX_HOST_GS_SEL: 0x%hx ", value16);

    gdt_base = sgdt();
	if (((gdt_base >> 47U) & 0x1UL) != 0UL) {
	        gdt_base |= 0xffff000000000000UL;
	}
    exec_vmwrite(VMX_HOST_GDTR_BASE, gdt_base);
	printk("VMX_HOST_GDTR_BASE: 0x%x ", gdt_base);

}
void SetupVMCS(void){

	uint64_t vmx_rev_id;
    printk("Initializing VMCS\n");
    vmx_rev_id = msr_read(MSR_IA32_VMX_BASIC);
    void *vmxon_region_va = (void *)g_VMXCPU.pVMCSRegion;
	(void)memcpy_s(vmxon_region_va, 4U, (void *)&vmx_rev_id, 4U);
    uint64_t vmcs_pa = g_VMXCPU.pVMCSRegion_PA;
    printk("debug 1 \n");
    exec_vmclear((void*)&vmcs_pa);
    printk("debug 2 \n");
    exec_vmptrld((void*)&vmcs_pa);
    init_host_state();
}