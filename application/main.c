#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdint.h>

#include "smartapp.h"
#include "ui.h"

#define DEVNAME "/dev/smarthome"

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
    int fd = open(DEVNAME, O_RDWR);
    while (1) {
        printf("\ec"MAIN_MENU);
        ret = get_inputi(1, 8, "input(1~8)>>");
        zufunc[ret - 1](fd);
    }
    return 0;
}
