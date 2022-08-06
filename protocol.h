#pragma once

#include <stdint.h>
#include "board/ioprotocol.h"

struct msg {
    uint32_t size;
    enum iocmd cmdcode;
    uint32_t data;
};