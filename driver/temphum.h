#ifndef __TEMPHUM__
#define __TEMPHUM__

#define TMP_ADDR 0xe3
#define HUM_ADDR 0xe5

extern uint32_t temperature;   //温度
extern uint32_t humidity;      //湿度

// typedef unsigned int uint32_t;
//温湿度读取
// return - uint32_t: 温度低16位、湿度高16位
extern uint32_t read_temperature_humidity(void);
//温度读取
extern uint32_t read_temperature(void);
//湿度读取
extern uint32_t read_humidity(void);
#endif