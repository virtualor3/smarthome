#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>

#include "smartdriver.h"
#include "../ioprotocol.h"

int smarthome_open(struct inode* inode, struct file* filp)
{}

int smarthome_close(struct inode* inode, struct file* filp)
{}

long smarthome_ioctl(struct file* filp, unsigned int cmd, unsigned long arg)
{
    typedef void(*pfunc_t)(void);
    static pfunc_t callzu[] = {
        [IO_LED_ON] = led_on,
        [IO_LED_OFF] = led_off,
        [IO_MOTOR_START] = motor_start,
        [IO_MOTOR_STOP] = motor_stop,
        [IO_BUZZ_ON] = buzz_on,
        [IO_BUZZ_OFF] = buzz_off,
        [IO_FAN_START] = fan_start,
        [IO_FAN_STOP] = fan_stop,
    };
    if(_IOC_NR(cmd) < ARRAY_SIZE(callzu)) callzu[_IOC_NR(cmd)]();
}

ssize_t smarthome_read(struct file* filp, char __user* mem, size_t size, loff_t* off)
{}

ssize_t smarthome_write(struct file* filp, const char __user* mem, size_t size, loff_t* off)
{}
// 定义操作方法结构体的变量，并对成员进行初始化
struct file_operations fops = {
    .open = smarthome_open,
    .release = smarthome_close,
    .unlocked_ioctl = smarthome_ioctl,
    .read = smarthome_read,
    .write = smarthome_write,
};

static int __init smarthome_init(void)
{
    pwm_init();
    temphum_init();
    digitube_init();
}

static int __exit smarthome_exit(void)
{
    pwm_delinit();
    temphum_delinit();
    digitube_delinit();
}

module_init(smarthome_init);
module_exit(smarthome_exit);
MODULE_LICENSE("GPL");