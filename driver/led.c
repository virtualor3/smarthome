/**
 * @file led.c
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
#include <linux/of_irq.h>
#include <linux/interrupt.h>
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
#define CNAME "led"
#define PWM_PATH_NAME "/myplatform"
int status = 0;         // 电平切换
int irqno;              //中断号
char gpioname = "led1"; //设备树的键
struct device_node *node;
struct gpio_desc *desc;
wait_queue_head_t wq;

//按键中断
irqreturn_t pwm_irq(int irq, void *dev)
{
    //唤醒
    wake_up_interruptible(&wq);
    led_toggle();
    return IRQ_HANDLED;
}
// LED
void led_on(void)
{
    gpiod_set_value(desc, LED_ON);
}
void led_off(void)
{
    gpiod_set_value(desc, LED_OFF);
}
void led_toggle(void)
{
    status = gpiod_get_value(desc);
    status = !status;
    gpiod_set_value(desc, status);
}
uint32_t led_status(void)
{
    uint32_t ret = gpiod_get_value(desc);
    return ret;
}

//初始化
void led_init(void)
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

    desc = gpiod_get_from_of_node(node, gpioname, 0, GPIOD_OUT_LOW, NULL);
    if (IS_ERR(desc))
    {
        printk("get led error\n");
        ret = PTR_ERR(desc);
        goto ERR2;
    }
    gpiod_set_value(desc, 0); // 保持低电平

    // 3.获取软中断号、注册中断
    irqno = irq_of_parse_and_map(node, 0);
    if (irqno == 0)
    {
        printk("parse irqno error\n");
        return -EAGAIN;
        goto ERR3;
    }
    ret = request_irq(irqno, pwm_irq, IRQF_TRIGGER_FALLING, CNAME, NULL);
    if (ret)
    {
        printk("request irq error\n");
        goto ERR4;
    }

    // 4.初始化等待队列头
    init_waitqueue_head(&wq);
ERR4:
    free_irq(irqno, NULL);
ERR3:
    gpiod_put(desc);
ERR2:
    gpiod_put(desc);
ERR1:
    return;
}
//卸载delinit
void led_delinit(void)
{
    free_irq(irqno, NULL);
    gpiod_put(desc);
    gpiod_put(desc);
}