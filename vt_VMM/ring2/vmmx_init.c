#include <linux/init.h>  
#include <linux/kernel.h> 
#include <linux/module.h>  

MODULE_LICENSE("GPL");
MODULE_AUTHOR("qianyi,zhang");

#include "./include/vmxon.h"
#include "./include/vmcs.h"

static void vmmx_exit(void){
    printk(KERN_INFO "vmmx_exit\n");
}

static int vmmx_init(void){

    printk(KERN_INFO "driver init:vmmx_init\n");
    StartVirtualTechnology();
    VMMSTATE status = AllocateVMXRegion();
    if (status)
    {
        printk("ERROR:AllocateVMXRegion\n");
        return STATUS_VMMOK;
    }
    SetupVMXRegion();
    SetupVMCS();
    return 0;
}

module_init(vmmx_init);
module_exit(vmmx_exit);
