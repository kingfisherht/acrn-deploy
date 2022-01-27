#include "./include/vcpu.h"
#define CONFIG_STACK_SIZE 0x2000U
void load_gdtr_and_tr(void){
    struct host_gdt *gdt = &get_cpu_var(gdt);
    struct tss_64 *tss = &get_cpu_var(tss);
    gdt->rsvd = 0xAAAAAAAAAAAAAAAAUL;
    gdt->code_segment_descriptor = 0x00Af9b000000ffffUL;
	gdt->data_segment_descriptor = 0x00cf93000000ffffUL;

	tss->ist1 = (uint64_t)get_cpu_var(mc_stack) + CONFIG_STACK_SIZE;
	tss->ist2 = (uint64_t)get_cpu_var(df_stack) + CONFIG_STACK_SIZE;
	tss->ist3 = (uint64_t)get_cpu_var(sf_stack) + CONFIG_STACK_SIZE;
	tss->ist4 = 0UL;


    struct host_gdt_descriptor gdtr;

}
void init_pcpu_post(uint16_t pcpu_id){
    load_gdtr_and_tr();


}