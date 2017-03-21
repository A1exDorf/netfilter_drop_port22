KERNELDIR ?= /lib/modules/$(shell uname -r)/build

obj-m := drop22.o

PWD := $(shell pwd)

all:

	make -C $(KERNELDIR) M=$(PWD) modules

clean:

	rm *.ko *.mod.c *.order *.symvers *.o
