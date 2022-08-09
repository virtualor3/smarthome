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
//温度阈值
extern void set_temp_threshold(uint32_t);
extern uint32_t get_temp_threshold(void);
//LED
extern void led_on(void);
extern void led_off(void);
extern void led_toggle(void);
extern uint32_t led_status(void);
//电机
extern void motor_start(void);
extern void motor_stop(void);
extern uint32_t motor_status(void);
//蜂鸣器
extern void buzz_on(void);
extern void buzz_off(void);
extern uint32_t buzz_status(void);
//风扇
extern void fan_start(void);
extern void fan_stop(void);
extern uint32_t fan_status(void);
//数码管显示
extern void digitube_display(uint32_t);

//初始化
extern void temphum_init(void);
extern void pwm_init(void);
extern void digitube_init(void);

//卸载delinit
extern void temphum_delinit(void);
extern void pwm_delinit(void);
extern void digitube_delinit(void);