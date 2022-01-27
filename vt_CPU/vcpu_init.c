#include <linux/init.h>  
#include <linux/kernel.h> 
#include <linux/module.h>  

MODULE_LICENSE("GPL");
MODULE_AUTHOR("qianyi,zhang");

#include "./include/vcpu.h"

static void vmmx_cpu_exit(void){
    printk(KERN_INFO "vmmx_cpu_exit\n");
}


static int vmmx_cpu_init(void){

    printk(KERN_INFO "driver init:vmmx_cpu_init\n");


    init_pcpu_post();
    
    init_pcpu_pre();

    return 0;
}

module_init(vmmx_cpu_init);
module_exit(vmmx_cpu_exit);