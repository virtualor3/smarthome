#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h> 
#include <stdint.h>
#include <stdlib.h>

#include "ui.h"
#include "smartapp.h"
#include "ioprotocol.h"

void do_exit(int fd)
{
    close(fd);
    exit(EXIT_SUCCESS);
}

void do_led(int fd)
{
    int ret;
    printf("\ec"LED_MENU);
    while (1) {
        ret = get_inputi(1, 3, "input(1~3)>>");
        if (ret == 1) {
            if (0 == ioctl(fd, IOW(IO_LED_ON))) {
                printf("LED打开成功!\n");
                continue;
            }
            printf("LED打开失败!\n");
            continue;
        }
        if (ret == 2) {
            if (0 == ioctl(fd, IOW(IO_LED_OFF))) {
                printf("LED关闭成功!\n");
                continue;
            }
            printf("LED关闭失败!\n");
            continue;
        }
        if (ret == 3) break;
    }
}

void do_fan(int fd)
{
    int ret;
    printf("\ec"FAN_MENU);
    while (1) {
        ret = get_inputi(1, 3, "input(1~3)>>");
        if (ret == 1) {
            if (0 == ioctl(fd, IOW(IO_FAN_START))) {
                printf("风扇打开成功!\n");
                continue;
            }
            printf("风扇打开失败!\n");
            continue;
        }
        if (ret == 2) {
            if (0 == ioctl(fd, IOW(IO_FAN_STOP))) {
                printf("风扇关闭成功!\n");
                continue;
            }
            printf("风扇关闭失败!\n");
            continue;
        }
        if (ret == 3) break;
    }
}

void do_buzz(int fd)
{
    int ret;
    printf("\ec"BUZZ_MENU);
    while (1) {
        ret = get_inputi(1, 3, "input(1~3)>>");
        if (ret == 1) {
            if (0 == ioctl(fd, IOW(IO_BUZZ_ON))) {
                printf("蜂鸣器打开成功!\n");
                continue;
            }
            printf("蜂鸣器打开失败!\n");
            continue;
        }
        if (ret == 2) {
            if (0 == ioctl(fd, IOW(IO_BUZZ_OFF))) {
                printf("蜂鸣器关闭成功!\n");
                continue;
            }
            printf("蜂鸣器关闭失败!\n");
            continue;
        }
        if (ret == 3) break;
    }
}

void do_temphum(int fd)
{
    int ret, num;
    printf("\ec"TEMPHUM_MENU);
    while (1) {
        ret = get_inputi(1, 4, "input(1~4)>>");
        if (ret == 1) {
            num = ioctl(fd, IOW(IO_GET_TMP));
            printf("temp = %f\n", 175.72 * GET_TEMP(num) / 65536 - 46.85);
            continue;
        }
        if (ret == 2) {
            num = ioctl(fd, IOW(IO_GET_HUM));
            printf("hum = %f\n", 125.0 * GET_HUM(num) / 65536 - 6);
            continue;
        }
        if (ret == 3) {
            num = ioctl(fd, IOW(IO_GET_TMP_AND_HUM));
            printf("temp = %f\thum = %f\n", 175.72 * GET_TEMP(num) / 65536 - 46.85, 125.0 * GET_HUM(num) / 65536 - 6);
            continue;
        }
        if (ret == 4) break;
    }
}

void do_motor(int fd)
{
    int ret;
    printf("\ec"MOTOR_MENU);
    while (1) {
        ret = get_inputi(1, 3, "input(1~3)>>");
        if (ret == 1) {
            if (0 == ioctl(fd, IOW(IO_MOTOR_START))) {
                printf("电机启动成功!\n");
                continue;
            }
            printf("电机启动失败!\n");
            continue;
        }
        if (ret == 2) {
            if (0 == ioctl(fd, IOW(IO_MOTOR_STOP))) {
                printf("电机关闭成功!\n");
                continue;
            }
            printf("电机关闭失败!\n");
            continue;
        }
        if (ret == 3) break;
    }
}

void do_threshold(int fd)
{
    int ret;
    float up, down;
    up = 175.72 * ioctl(fd, IOW(IO_GET_TMP_UP_THRESHOLD)) / 65536 - 46.85;
    down = 175.72 * ioctl(fd, IOW(IO_GET_TMP_DOWN_THRESHOLD)) / 65536 - 46.85;
    printf("\ec"THRESHOLD_MENU);
    while (1) {
        printf("current temperature threshold: up = %6.3f, down = %6.3f\n", up, down);
        ret = get_inputi(1, 3, "input(1~3)>>");
        if (ret == 1) {
            up = get_inputf("enter temperature up threshold: ");
            ret = ioctl(fd, IOW(IO_SET_TMP_UP_THRESHOLD), (uint32_t)((up + 46.85) * 65536 / 175.72));
            goto Contine;
        }
        if (ret == 2) {
            down = get_inputf("enter temperature up threshold: ");
            ret = ioctl(fd, IOW(IO_SET_TMP_DOWN_THRESHOLD), (uint32_t)((down + 46.85) * 65536 / 175.72));
            goto Contine;
        }
        if (ret == 3) break;
    Contine:
        if (ret == 0) {
            printf("温度阈值设置成功!\n");
        }
    }
}

void do_digitube(int fd)
{
    int ret, num, temp, hum;
    printf("\ec"DIGITUBE_MENU);
    while (1) {
        ret = get_inputi(1, 5, "input(1~5)>>");
        if (ret == 1) {
            num = ioctl(fd, IOW(IO_GET_TMP));
            temp = (uint32_t)(175.72 * 100 * num / 65536 - 46.85 * 100);
            if (0 == ioctl(fd, IOW(IO_SET_DIGITUBE), SET_DIGITNUM(0, temp, 2))) {
                printf("温度显示成功!\n");
                continue;
            }
            printf("温度显示失败!\n");
            continue;
        }
        if (ret == 2) {
            num = ioctl(fd, IOW(IO_GET_HUM));
            hum = (uint32_t)(125 * 100 * num / 65536 - 600);
            if (0 == ioctl(fd, IOW(IO_SET_DIGITUBE), SET_DIGITNUM(0, hum, 2))) {
                printf("湿度显示成功!\n");
                continue;
            }
            printf("湿度显示失败!\n");
            continue;
        }
        if (ret == 3) {
            num = ioctl(fd, IOW(IO_GET_TMP_AND_HUM));
            temp = (uint32_t)(175.72 * GET_TEMP(num) / 65536 - 46.85);
            hum = (uint32_t)(125 * GET_HUM(num) / 65536 - 6);
            if (0 == ioctl(fd, IOW(IO_SET_DIGITUBE), SET_DIGITNUM(0, temp * 100 + hum, 0))) {
                printf("温湿度显示成功!\n");
                continue;
            }
            printf("温湿度显示失败!\n");
            continue;
        }
        if (ret == 4) {
            char buf[16];
            printf("input number>>");
            fflush(stdout);
            ign_return(fgets(buf, sizeof(buf), stdin));
            if (buf[sizeof(buf) - 2] != '\n' && buf[sizeof(buf) - 2] != 0) while (getchar() != '\n');
            if (0 == ioctl(fd, IOW(IO_SET_DIGITUBE), get_digit(buf, 4))) {
                printf("数字显示成功!\n");
                continue;
            }
            printf("数字显示失败!\n");
            continue;
        }
        if (ret == 5) break;
    }
}

uint32_t get_digit(char* buf, uint32_t digits)
{
    int i, neg = 0, num = 0, decimal = 0;
    if (buf[0] == '-') {
        digits--;
        neg = 1;
    } else if (buf[0] == '+') buf++;
    for (i = 0; buf[i] >= '0' && buf[i] <= '9'; i++) {
        num = num * 10 + buf[i] - '0';
    }
    if (buf[i] == '.') {
        for (i++; buf[i] >= '0' && buf[i] <= '9' && i < digits + 1; i++) {
            num = num * 10 + buf[i] - '0';
            decimal++;
        }
    }
    return SET_DIGITNUM(neg, num, decimal);
}