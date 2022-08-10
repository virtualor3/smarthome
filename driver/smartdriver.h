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
#include "ioprotocol.h"
// typedef unsigned int uint32_t;
//温湿度读取
// return - uint32_t: 温度低16位、湿度高16位
extern uint32_t read_temperature_humidity(void);
//温度读取
extern uint32_t read_temperature(void);
//湿度读取
extern uint32_t read_humidity(void);
//温度阈值
extern void inline set_temp_threshold(uint32_t);
extern uint32_t inline get_temp_threshold(void);
//LED
extern void inline led_on(void);
extern void inline led_off(void);
extern void inline led_toggle(void);
extern uint32_t inline led_status(void);
//电机
extern void inline motor_start(void);
extern void inline motor_stop(void);
extern uint32_t motor_status(void); //获取motor状态，状态宏在ioprotocol.h中
//蜂鸣器
extern void inline buzz_on(void);
extern void inline buzz_off(void);
extern uint32_t inline buzz_status(void);  //获取buzz状态
//风扇
extern void inline fan_start(void);
extern void inline fan_stop(void);
extern uint32_t inline fan_status(void);   //获取fan状态
//数码管显示
extern void inline digitube_display(uint32_t num);

//初始化
extern void inline timer_init(void);
extern void inline temphum_init(void);
extern void inline pwm_init(void);
extern void inline digitube_init(void);

//卸载delinit
extern void inline timer_delinit(void);
extern void inline temphum_delinit(void);
extern void inline pwm_delinit(void);
extern void inline digitube_delinit(void);
extern void inline irq_delinit(void);

uint32_t temperature;   //温度
uint32_t humidity;      //湿度