#ifndef __SMARTDRIVER_H__
#define __SMARTDRIVER_H__

#include <stdint.h>

extern uint32_t read_temperature_humidity(void);
extern uint32_t read_temperature(void);
extern uint32_t read_humidity(void);

#endif