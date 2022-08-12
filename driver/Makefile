#驱动.o文件
DRIVERSOURCES ?= $(notdir $(wildcard $(src)/*.c))
#驱动多文件编译
obj-m := $(MODENAME).o
$(MODENAME)-y := $(patsubst %.c,%.o,$(DRIVERSOURCES))
#指定自定义头文件路径
ccflags-y += -I$(src)/../