/**
 * @file ioprotocol.h
 * @author your name (you@domain.com)
 * @brief 开发板ioctl功能码
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
    IO_LED_ON,              //led开启
    IO_LED_OFF,             //led关闭
    IO_MOTOR_START,         //电机启动
    IO_MOTOR_STOP,          //电机停止
    IO_BUZZ_ON,             //蜂鸣器开启
    IO_BUZZ_OFF,            //蜂鸣器关闭
    IO_FAN_START,           //风扇启动
    IO_FAN_STOP,            //风扇停止
    IO_SET_TMP_THRESHOLD,   //设置温度阈值
    IO_GET_TMP_THRESHOLD,   //获取温度阈值
    IO_GET_TMP,             //获取温度
    IO_GET_HUM,             //获取湿度
    IO_GET_TMP_AND_HUM,     //获取温湿度
    IO_SET_DIGITUBE,        //设置数码管显示数值
};