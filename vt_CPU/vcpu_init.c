#include <linux/init.h>  
#include <linux/kernel.h> 
#include <linux/module.h>  

MODULE_LICENSE("GPL");
MODULE_AUTHOR("qianyi,zhang");

#include "./include/vcpu.h"

static void vmmx_cpu_exit(void){
    printk(KERN_INFO "vmmx_cpu_exit\n");
}

static inline uint16_t get_pcpu_id(void)
{
	uint32_t tsl, tsh, cpu_id;

	asm volatile ("rdtscp":"=a" (tsl), "=d"(tsh), "=c"(cpu_id)::);
	return (uint16_t)cpu_id;
}

static int vmmx_cpu_init(void){

    printk(KERN_INFO "driver init:vmmx_cpu_init\n");
	uint16_t pcpu_id;

	pcpu_id = get_pcpu_id();

    printk("pcpu_id=%d\n",pcpu_id);

    init_pcpu_post(pcpu_id);
    
    //init_pcpu_pre();

    return 0;
}

module_init(vmmx_cpu_init);
module_exit(vmmx_cpu_exit);