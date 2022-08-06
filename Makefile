ARCH ?= arm
#驱动模块目标文件名<modename>.ko
MODENAME ?= smarthome
#开发板应用程序目标文件名<filename>.out
APPNAME ?= smarthome
#目标安装目录
INSTALLDIR ?= ~/nfs/rootfs/

#驱动.o文件
DRIVERSOURCES ?= $(wildcard drivers/*.c)
#开发板应用程序.o文件
APPSOURCES ?= $(wildcard application/*.c)

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

#全部编译
all:driver app
#只编译驱动
driver:$(MODENAME)
#只编译开发板应用
app:$(APPNAME)

$(MODENAME):
	make -C $(KERNELDIR)  M=$(PWD)  modules

$(APPNAME):$(APPSOURCES)
	CC $^ -I./ -o $@

install:
	-cp $(MODENAME).ko	$(INSTALLDIR)
	-cp $(APPNAME)	$(INSTALLDIR)

clean:
	make -C $(KERNELDIR)  M=$(PWD)  clean
	rm $(APPNAME)

help:
	@echo "make ARCH=<arm or x86> MODENAME=<drivers filename> driver"
	@echo "make ARCH=<arm or x86> APPNAME=<app filename> app"
	@echo "make ARCH=<arm or x86> APPNAME=<app filename> MODENAME=<driver filename> all"

endif