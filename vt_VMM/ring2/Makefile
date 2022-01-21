CONFIG_MODULE_SIG=n
TARGET := vmmx_test
obj-m = $(TARGET).o
$(TARGET)-objs := vmmx_init.o vmxon.o memcy.o vmcs.o
# obj-m = hello.o
KDIR := /lib/modules/$(shell uname -r)/build
all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) modules clean
	rm -f *.ko *.o *.mod.o *.mod.c *.symvers *.order *.mod *.dwo

