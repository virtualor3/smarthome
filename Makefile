ARCH ?= $(shell uname -m)
#当前文件目录
PWD := $(shell pwd)
#驱动模块目标文件名<modename>.ko
export MODENAME ?= smarthome
#开发板应用程序目标文件名<filename>.out
export APPNAME ?= smarthomeapp
#目标安装目录
INSTALLDIR ?= ~/nfs/rootfs/
#内核源码目录
KERNELDIR ?= /home/virtualor3/linux-5.10.61

DTBNAME ?= stm32mp157a-fsmp1a

ifeq ($(ARCH),arm)
	CROSS_COMPILE ?= arm-linux-gnueabihf-
else
 	KERNELDIR := /lib/modules/$(shell uname -r)/build
 	CROSS_COMPILE ?=
endif

CC := $(CROSS_COMPILE)gcc

#全部编译
all:driver app
#只编译驱动
driver:$(MODENAME)
#只编译开发板应用
app:$(APPNAME)

$(MODENAME):
	@make -C $(KERNELDIR)  M=$(PWD)/driver  $(MODENAME).ko

$(APPNAME):
	@make -C $(PWD)/application all

.PHONY = clean install

install:
	cp driver/$(MODENAME).ko	$(INSTALLDIR) 2> /dev/null
	cp application/$(APPNAME)	$(INSTALLDIR) 2> /dev/null

clean:
	@make -C $(KERNELDIR)  M=$(PWD)/driver  clean
	@make -C $(PWD)/application clean 2> /dev/null

help:
	@echo "make ARCH=<arm or x86> MODENAME=<drivers filename> driver"
	@echo "make ARCH=<arm or x86> APPNAME=<app filename> app"
	@echo "make ARCH=<arm or x86> APPNAME=<app filename> MODENAME=<driver filename> all"
