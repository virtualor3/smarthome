/**
 * @file ioprotocol.h
 * @author your name (you@domain.com)
 * @brief 开发板ioctl功能码，应用application和驱动driver公有
 * @version 0.1
 * @date 2022-08-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

/**
 * @brief ioctl系统调用功能码.
 * @note ioprotocol.h需要同时放入驱动及应用源码目录。
 */
enum iocmd {
    IO_LED_ON,                  //led开启
    IO_LED_OFF,                 //led关闭
    IO_LED_TOGGLE,              //led状态反转
    IO_MOTOR_START,             //电机启动
    IO_MOTOR_STOP,              //电机停止
    IO_BUZZ_ON,                 //蜂鸣器开启
    IO_BUZZ_OFF,                //蜂鸣器关闭
    IO_FAN_START,               //风扇启动
    IO_FAN_STOP,                //风扇停止
    IO_SET_TMP_UP_THRESHOLD,    //设置温度阈值上限
    IO_GET_TMP_UP_THRESHOLD,    //获取温度阈值上限
    IO_SET_TMP_DOWN_THRESHOLD,  //设置温度阈值下限
    IO_GET_TMP_DOWN_THRESHOLD,  //设置温度阈值下限
    IO_GET_TMP,                 //获取温度
    IO_GET_HUM,                 //获取湿度
    IO_GET_TMP_AND_HUM,         //获取温湿度
    IO_SET_DIGITUBE,            //设置数码管显示数值
};

#include <linux/ioctl.h>
#define IOW(nr) _IOW('s', (nr), uint32_t)

/**
 * 设置ioctl传参,IO_SET_DIGITUBE
 * @neg: bool - 符号位 (1:负数 0:正数)
 * @num: uint - 数码管将要显示的数字显示的数字
 * @decimal: uint - 小数点后小数位数
**/
#define SET_DIGITNUM(neg, num, decimal)  (((neg) << 31) | ((num) & 0xffffff) | (((decimal) & 0x7f) << 24))
#define GET_DIGITNUM(x)                  ((x) & 0xffffff)                          //从ioctl参数arg获取显示的数字
#define GET_DIGITDEC(x)                  (((x) >> 24) & 0x7f)                      //从ioctl参数arg获取小数位数
#define GET_DIGITNEG(x)                  ((x) >> 31)

#define SET_TEMPHUM(temp, hum)  ((hum << 16) | (temp & 0xffff))    //设置ioctl传参,IO_GET_TMP_AND_HUM
#define GET_TEMP(x)             (x & 0xffff)    //从ioctl返回值获取温度
#define GET_HUM(x)              (x >> 16)       //从ioctl返回值获取湿度

#define LED_OFF         0
#define LED_ON          1

#define MOTOR_STOP      0
#define MOTOR_START     1

#define BUZZ_OFF        0
#define BUZZ_ON         1

#define FAN_STOP        0
#define FAN_START       1