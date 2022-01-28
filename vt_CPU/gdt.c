#include <linux/init.h>  
#include <linux/kernel.h> 
#include <linux/module.h>

#include "./include/gdt.h"

MODULE_LICENSE("GPL");

struct per_vcpu_region per_vcpu_data[MAX_PCPU_NUM] __aligned(PAGE_SIZE);

void load_gdtr_and_tr(void){

    struct host_gdt *gdt = &get_vcpu_var(gdt);
    struct tss_64 *tss = &get_vcpu_var(tss);
    gdt->rsvd = 0xAAAAAAAAAAAAAAAAUL;
    gdt->code_segment_descriptor = 0x00Af9b000000ffffUL;
	gdt->data_segment_descriptor = 0x00cf93000000ffffUL;

	tss->ist1 = (uint64_t)get_vcpu_var(mc_stack) + CONFIG_STACK_SIZE;
	tss->ist2 = (uint64_t)get_vcpu_var(df_stack) + CONFIG_STACK_SIZE;
	tss->ist3 = (uint64_t)get_vcpu_var(sf_stack) + CONFIG_STACK_SIZE;
	tss->ist4 = 0UL;
}

void init_pcpu_post(void){

	uint16_t pcpu_id;
	pcpu_id = get_pcpu_id();
    printk("pcpu_id=%d\n",pcpu_id);

    load_gdtr_and_tr();

    if (pcpu_id == BSP_CPU_ID) {
        printk("pcpu_id = BSP_CPU_ID\n");
    }

}