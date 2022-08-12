#pragma once

#include <stdio.h>

#define ign_return(func)  {if(func){}}

/**
 * 从终端获取整数值
 * @min: int - 从终端获取整数值的范围最小值
 * @max: int - 从终端获取整数值的范围最大值
 * @format: char* - 输入提示格式化字符串
 * @arg: ... - 格式字符串format可变参数
**/
#define get_inputi(min, max, format, args...)    ({  \
    int ret;    \
    char buf[16] = { 0 };    \
    do { \
        printf(format, ##args); \
        fflush(stdout); \
        ign_return(fgets(buf, sizeof(buf), stdin)); \
        if (buf[sizeof(buf) - 2] != 0 && buf[sizeof(buf) - 2] != '\n') { \
            while (getchar() != '\n'); \
            buf[sizeof(buf) -2] = 0;   \
        }   \
    } while ((sscanf(buf, "%d", &ret) != 1) || (ret > max && ret < min));    \
    ret;    \
})

#define get_inputf(format, args...)     ({  \
    float ret;  \
    char buf[16] = { 0 };   \
    do {    \
        printf(format, ##args); \
        fflush(stdout); \
        ign_return(fgets(buf, sizeof(buf), stdin)); \
        if (buf[sizeof(buf) - 2] != 0 && buf[sizeof(buf) - 2] != '\n') { \
            while (getchar() != '\n'); \
            buf[sizeof(buf) -2] = 0;   \
        }   \
    } while (sscanf(buf, "%f", &ret) != 1); \
    ret;    \
})

uint32_t get_digit(char* buf, uint32_t digits);

void do_led(int fd);
void do_buzz(int fd);
void do_fan(int fd);
void do_temphum(int fd);
void do_motor(int fd);
void do_threshold(int fd);
void do_digitube(int fd);
void do_exit(int fd);