/**
 * @file smartdriver.h
 * @author your name (you@domain.com)
 * @brief 驱动的公共头文件
 * @version 0.1
 * @date 2022-08-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#pragma once
#include <linux/types.h>
#include "../ioprotocol.h"
// typedef unsigned int uint32_t;
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
extern uint32_t motor_status(void); //获取motor状态，状态宏在ioprotocol.h中
//蜂鸣器
extern void buzz_on(void);
extern void buzz_off(void);
extern uint32_t buzz_status(void);  //获取buzz状态
//风扇
extern void fan_start(void);
extern void fan_stop(void);
extern uint32_t fan_status(void);   //获取fan状态
//数码管显示
extern void digitube_display(uint32_t);

//初始化
extern void timer_init(void);
extern void temphum_init(void);
extern void pwm_init(void);
extern void digitube_init(void);

//卸载delinit
extern void timer_delinit(void);
extern void temphum_delinit(void);
extern void pwm_delinit(void);
extern void digitube_delinit(void);
extern void irq_delinit(void);

uint32_t temperature;   //温度
uint32_t humidity;      //湿度