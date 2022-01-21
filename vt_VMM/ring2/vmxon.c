#include <linux/module.h>
#include <linux/init.h>  
#include <linux/kernel.h> 

#include <asm/page.h> 
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/types.h>

#include "./include/msr.h"
#include "./include/cpudata.h"
#include "./include/vmxon.h"
#include "./include/macro.h"

MODULE_LICENSE("GPL");

VMX_CPU g_VMXCPU;

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

int IsVTenable(void){

    _CPUID_ECX ucpuid; 
    uint32_t eax, ebx,ecx,edx;
    //cpuid
    cpuid_subleaf(CPUID_FEATURES, 0x0U, &eax, &ebx, &ecx, &edx);
    *((uint32_t*)&ucpuid) = ecx;
    if(ucpuid.VMX !=1 ){
        printk("vmx is not ON\n");
        return -1;
    }
  // cr0
    uint64_t val64=0;
    CPU_CR_READ(cr0, &val64);
    _CR0 uCr0;
    *((uint32_t*)&uCr0) = val64;
    if (uCr0.PE != 1 || uCr0.PG!=1 || uCr0.NE!=1)
    {
        printk("ERROR:cr0 CPU DONOT SUPPORT VT!\n");
        return -1;
    }
    //cr4
    val64=0;
    CPU_CR_READ(cr4, &val64);
    _CR4 uCr4;
     *((uint32_t*)&uCr0) = val64;
         if (uCr4.VMXE == 1)
    {
        printk("ERROR:cr4 DONOT SUPPORT VT! \n");
        return -1;
    }
    //msr
    uint32_t tmp32 = (uint32_t)msr_read(MSR_IA32_FEATURE_CONTROL);
     IA32_FEATURE_CONTROL_MSR msr;
     *((uint32_t*)&msr) = tmp32;
         if (msr.Lock!=1)
    {
        printk("ERROR:msr.Lock is LOCK!\n");
        return -1;
    }

    return 0;
}

VMMSTATE AllocateVMXRegion(void){
    static struct page* page_vmon;
    page_vmon = alloc_pages(GFP_KERNEL,0);
    if(page_vmon == NULL){
      printk("ERROR page_vmon alloc_pages\n");
      return STATUS_VMMALLOC_PAGE_ERROR;
    }
    void *pvmon = page_address(page_vmon);
    memset(pvmon,0,4096);

    static struct page* page_vmcs;
    page_vmcs = alloc_pages(GFP_KERNEL,0);
    if(page_vmcs == NULL){
      printk("ERROR page_vmcs alloc_pages\n");
      return STATUS_VMMALLOC_PAGE_ERROR;
    }
    void *pvmcs = page_address(page_vmcs);
    memset(pvmcs,0,4096);

    static struct page* page_stck;
    page_stck = alloc_pages(GFP_KERNEL,0);
    if(page_stck == NULL){
      printk("ERROR page_stck alloc_pages\n");
      return STATUS_VMMALLOC_PAGE_ERROR;
    }
    void *pvmstck = page_address(page_stck);
    memset(pvmstck,0,4096);

    g_VMXCPU.pVMXONRegion = (uint64_t)pvmon;
    g_VMXCPU.pVMXONRegion_PA =  (uint64_t)page_vmon;
    g_VMXCPU.pVMCSRegion = (uint64_t)pvmcs;
    g_VMXCPU.pVMCSRegion_PA = (uint64_t)page_vmcs;
    g_VMXCPU.pStack = (uint64_t)pvmstck;

    return STATUS_VMMOK;
}
void SetupVMXRegion(void) {
    uint64_t tmp64;
	uint32_t tmp32;
    _EFLAGS uEflags;
    uint64_t vmxon_region_pa;
    void *vmxon_region_va = (void *)g_VMXCPU.pVMXONRegion;
    tmp32 = (uint32_t)msr_read(MSR_IA32_VMX_BASIC);
	(void)memcpy_s(vmxon_region_va, 4U, (void *)&tmp32, 4U);

	CPU_CR_READ(cr0, &tmp64);
	CPU_CR_WRITE(cr0, tmp64 | CR0_NE);

    CPU_CR_READ(cr4, &tmp64);
    CPU_CR_WRITE(cr4, tmp64 | CR4_VMXE);
	/* Read Feature ControL MSR */
	tmp64 = msr_read(MSR_IA32_FEATURE_CONTROL);

	/* Check if feature control is locked */
	if ((tmp64 & MSR_IA32_FEATURE_CONTROL_LOCK) == 0U) {
		/* Lock and enable VMX support */
		tmp64 |= (MSR_IA32_FEATURE_CONTROL_LOCK |
			  MSR_IA32_FEATURE_CONTROL_VMX_NO_SMX);
		msr_write(MSR_IA32_FEATURE_CONTROL, tmp64);
	}

    vmxon_region_pa = (void *)g_VMXCPU.pVMXONRegion_PA;
    exec_vmxon(vmxon_region_pa);
    *((uint32_t*)&uEflags) = reg_read();
    if (uEflags.CF != 0)
    {
        printk("ERROR:VMXONָ is not ON!\n");
    }

}

void StartVirtualTechnology(void){
    int ret = IsVTenable();
    if(ret != 0){
        printk("VT is not enable\n");
    }

 }
