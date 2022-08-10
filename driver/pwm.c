/**
 * @file pwm.c
 * @author zyq (bqzheng310@163.com)
 * @brief
 * @version 0.1
 * @date 2022-08-09
 *
 * @copyright Copyright (c) 2022
 *
 */

//不要用注册file_operation
/**
 * @brief: 不要使用module_platform_driver
 * @example: 如果使用probe，可以用下面的形式
 * void xxx_init(void)
 * {
 *      platform_register_driver(...);
 * }
 */
//只要实现smartdriver.h里的功能函数就行
//头文件可以自行添加，自行添加的头文件是pwm.c的私有头文件
//所有全局变量请放到自行添加的头文件中
//
#include <linux/init.h>
#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/of_gpio.h>
// #include <linux/of_irq.h>
// #include <linux/interrupt.h>
#include <linux/of.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#include "smartdriver.h"
#include "../protocol.h"
//设备树
/*myplatform{
    compatible = "hqyj,myplatform";
    interrupt-parent = <&gpiof>;
    interrupts =<9 0>;
    reg = <0x12345678 0x1E>;
    led1 = <&gpioe 10 0>;
    fan = <&gpioe 9 0>;
    buzz = <&gpiob 6 0>;
    motor = <&gpiof 6 0>;
};*/
#define CNAME "pwm"
#define PWM_PATH_NAME "/myplatform"
// int status = 0;                               // 中断电平切换
// int irqno;                                    //中断号
char *gpioname[3] = {"fan", "buzz", "motor"}; //设备树的键
struct device_node *node;
struct gpio_desc *desc[3]; //[0]fan [1]buzz [2]motor

// //中断
// irqreturn_t pwm_irq(int irq, void *dev)
// {
//     int i = 0;
//     //唤醒
//     wake_up_interruptible(&wq);
//     //按键中断停止运行
//     for (i = 0; i < 3; i++)
//     {
//         status = gpiod_get_value(desc[i]);
//         if (status == 1)
//         {
//             status = !status;
//             gpiod_set_value(desc[i], status);
//         }
//     }
//     return IRQ_HANDLED;
// }

//电机
void motor_start(void)
{
    gpiod_set_value(desc[2], MOTOR_START);
}
void motor_stop(void)
{
    gpiod_set_value(desc[2], MOTOR_STOP);
}
uint32_t motor_status(void) //获取motor状态，状态宏在ioprotocol.h中
{
    uint32_t ret = gpiod_get_value(desc[2]);
    return ret;
}
//蜂鸣器
void buzz_on(void)
{
    gpiod_set_value(desc[1], BUZZ_ON);
}
void buzz_off(void)
{
    gpiod_set_value(desc[1], BUZZ_OFF);
}
uint32_t buzz_status(void) //获取buzz状态
{
    uint32_t ret = gpiod_get_value(desc[1]);
    return ret;
}
//风扇
void fan_start(void)
{
    gpiod_set_value(desc[0], FAN_START);
}
void fan_stop(void)
{
    gpiod_set_value(desc[0], FAN_STOP);
}
uint32_t fan_status(void) //获取fan状态
{
    uint32_t ret = gpiod_get_value(desc[0]);
    return ret;
}
//初始化
void pwm_init(void)
{
    int ret, i = 0;
    printk("%s:%s:%d\n", __FILE__, __func__, __LINE__);
    // 1.获取结点
    node = of_find_node_by_path(PWM_PATH_NAME);
    if (node == NULL)
    {
        printk("get device node error\n");
        ret = -EAGAIN;
        goto ERR1;
    }
    // 2.获取desc(gpiod)
    for (i = 0; i < 3; i++)
    {
        desc[i] = gpiod_get_from_of_node(node, gpioname[i], 0, GPIOD_OUT_LOW, NULL);
        if (IS_ERR(desc[i]))
        {
            printk("get fan error\n");
            ret = PTR_ERR(desc[i]);
            goto ERR2;
        }
        gpiod_set_value(desc[i], 0); // pwm相关保持低电平
    }
//     // 3.获取软中断号、注册中断
//     irqno = irq_of_parse_and_map(node, 0);
//     if (irqno == 0)
//     {
//         printk("parse irqno error\n");
//         return -EAGAIN;
//         goto ERR3;
//     }
//     ret = request_irq(irqno, pwm_irq, IRQF_TRIGGER_FALLING, CNAME, NULL);
//     if (ret)
//     {
//         printk("request irq error\n");
//         goto ERR4;
//     }
    return 0;
// ERR4:
//     free_irq(irqno, NULL);
ERR3:
    gpiod_put(desc);
ERR2:
    for (i = 0; i < 3; i++)
    {
        gpiod_put(desc[i]);
    }
ERR1:
    return;
}
//卸载delinit
void pwm_delinit(void)
{
    int i = 0;
//     free_irq(irqno, NULL);
    gpiod_put(desc);
    for (i = 0; i < 3; i++)
    {
        gpiod_put(desc[i]);
    }
}
