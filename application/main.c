#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdint.h>

#include "smartapp.h"
#include "ui.h"

#define DEVNAME "smarthome"

typedef void (*pfunc_t)(int fd);

static pfunc_t zufunc[] = {
    [0] = do_led,
    [1] = do_temphum,
    [2] = do_buzz,
    [3] = do_fan,
    [4] = do_motor,
    [5] = do_threshold,
    [6] = do_digitube,
    [7] = do_exit,
};

int main(int argc, char const* argv[])
{
    int ret;
    char buf[8] = { 0 };

    int fd = open(DEVNAME, O_RDWR);
    printf("\ec"MAIN_MENU"input(1~8)>>");
    fflush(stdout);
    do {
        fgets(buf, sizeof(buf), stdin);
        if (buf[6] != '\n') {
            while (getchar() != '\n');
        }
        if ((ret = buf[0] - '0' - 1) & (~0x7) == 0) break;
        printf("input(1~8)>>");
        fflush(stdout);
    } while (1);
    zufunc[ret](fd);
    return 0;
}
