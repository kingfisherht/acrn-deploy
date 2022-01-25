#include <linux/module.h>
#include <linux/init.h>  
#include <linux/kernel.h> 
#include <linux/types.h> 
    
#include "./include/vcpu.h"
#include "./include/cpu_caps.h"
MODULE_LICENSE("GPL");

void init_pcpu_pre(void){
    //uint16_t pcpu_id = BSP_CPU_ID;
	init_pcpu_capabilities();
    init_pcpu_model_name();
    load_pcpu_state_data();
}