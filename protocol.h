/**
 * @file protocol.h
 * @author none (none@domain.com)
 * @brief 开发板及pc机网络应用层通信协议
 * @version 0.1
 * @date 2022-08-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#pragma once

#include "ioprotocol.h"

struct msg {
    uint32_t size;
    enum iocmd cmdcode;
    uint32_t data;
};