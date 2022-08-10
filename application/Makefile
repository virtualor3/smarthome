cflags := -wall -O2
#开发板应用程序.o文件
APPSOURCES := $($(wildcard *.c):%.c=%.o)

all:
	$(CC) $(cflags) $(SOURCES) -o $(APPNAME)

%.o:%.c

clean:
	-@rm *.o $(APPNAME) 2> /dev/null