CONFIG_MODULE_SIG=n
TARGET := vmmx_cpu
obj-m = $(TARGET).o
$(TARGET)-objs := vcpu_init.o vcpu.o cpu_caps.o
KDIR := /lib/modules/$(shell uname -r)/build
all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) modules clean
	rm -f *.ko *.o *.mod.o *.mod.c *.symvers *.order *.mod *.dwo

