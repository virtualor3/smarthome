#pragma once

enum digit_stat {
    DISPLAY_TEMP,
    DISPLAY_HUM,
    DISPLAY_TEMP_AND_HUM,
    DISPLAY_NUMBER,
};

#include <linux/types.h>

struct digitube_info
{
    uint8_t digitube_char[4];  //4位数码管数值
    uint8_t decimal_places;    //数码管小数点位置(0~3)
    uint8_t neg;               //符号位
    enum digit_stat status;     //数码管显示状态
};

extern struct digitube_info digitube_info;
