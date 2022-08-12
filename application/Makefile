cflags := -Wall -O2 -I$(shell pwd)/..
#开发板应用程序.o文件
APPSOURCES := $(wildcard *.c)

all:
	$(APPCC) $(cflags) $(APPSOURCES) -o $(APPNAME)

clean:
	-@rm *.o $(APPNAME) 2> /dev/null