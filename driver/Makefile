#驱动.o文件
# DRIVERSOURCES ?= $(wildcard *.c)
#驱动多文件编译
obj-m := $(MODENAME).o
# $(MODENAME)-objs := $(patsubst %.c,%.o,$(DRIVERSOURCES))
$(MODENAME)-objs := pwm.o main.o temphum.o timer.o