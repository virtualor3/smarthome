#pragma once

#include <stdint.h>

//温湿度读取
// return - uint32_t: 
extern uint32_t read_temperature_humidity(void);
extern uint32_t read_temperature(void);
extern uint32_t read_humidity(void);