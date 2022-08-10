ARCH ?= arm
#驱动模块目标文件名<modename>.ko
export MODENAME ?= smarthome
#开发板应用程序目标文件名<filename>.out
APPNAME ?= smarthomeapp
#目标安装目录
INSTALLDIR ?= ~/nfs/rootfs/
#开发板应用程序.o文件
APPSOURCES ?= $(wildcard application/*.c)

ifeq ($(ARCH),arm)
 	KERNELDIR := /home/virtualor3/linux-5.4.31
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
	make -C $(KERNELDIR)  M=$(PWD)/driver  modules

$(APPNAME):$(APPSOURCES)
	CC application/$^ -I./ -o $@

install:
	-cp driver/$(MODENAME).ko	$(INSTALLDIR)
	-cp application/$(APPNAME)	$(INSTALLDIR)

clean:
	make -C $(KERNELDIR)  M=$(PWD)/driver  clean
	rm application/$(APPNAME)

help:
	@echo "make ARCH=<arm or x86> MODENAME=<drivers filename> driver"
	@echo "make ARCH=<arm or x86> APPNAME=<app filename> app"
	@echo "make ARCH=<arm or x86> APPNAME=<app filename> MODENAME=<driver filename> all"
