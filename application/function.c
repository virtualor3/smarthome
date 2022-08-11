#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <stdint.h>

#include "ui.h"
#include "smartapp.h"
#include "ioprotocol.h"

void do_led(int fd)
{
    int ret;
    char buf[8];
    printf("\ec"LED_MENU"input(1~3)>>");
    fflush(stdout);
    do {
        fgets(buf, sizeof(buf), stdin);
        if (buf[6] != '\n') {
            while (getchar() != '\n');
        }
        if ((ret = buf[0] - '0') < 4 && ret > 0) break;
        printf("input(1~3)>>");
        fflush(stdout);
    } while (1);
    if (ret == 1) ioctl()
}