#pragma once

#include <stdint.h>

//温湿度读取
// return - uint32_t: 温度低16位、湿度高16位
extern uint32_t read_temperature_humidity(void);
//从read_temperature_humidity返回值获取温度
#define GET_TEMP(x) (x & 0xffff)    //可重定义
//从read_temperature_humidity返回值获取湿度
#define GET_HUM(x)  (x >> 16)       //可重定义
//温度读取
extern uint32_t read_temperature(void);
//湿度读取
extern uint32_t read_humidity(void);