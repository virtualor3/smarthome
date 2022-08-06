ARCH ?= arm
MODENAME ?= 
TARGETDIR ?= ~/nfs/rootfs/devtest/

ifneq ($(KERNELRELEASE),)
#驱动多文件编译
	obj-m := $(MODENAME).o
	$(MODENAME)-obj := $(SOURCES)
else
	ifeq ($(ARCH),arm)
 		KERNELDIR := /home/virtualor3/linux-5.10
		CROSS_COMPILE ?= arm-linux-gnueabihf-
	else
 		KERNELDIR := /lib/modules/$(shell uname -r)/build
 		CROSS_COMPILE ?=
	endif

	PWD := $(shell pwd)
	CC := $(CROSS_COMPILE)gcc

all:
	make -C $(KERNELDIR)  M=$(PWD)   modules
	-@$(CC) test.c  -o test

install:
	cp *.ko  $(TARGETDIR)
	cp test  $(TARGETDIR)

clean:
	make -C $(KERNELDIR)  M=$(PWD)   clean
	-@rm test

help:
	@echo "make ARCH=<arm or x86> MODENAME=<drivers file name>"

endif