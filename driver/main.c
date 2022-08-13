#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>

#include "smartdriver.h"
#include "ioprotocol.h"
#include "digitube.h"

struct cdev* cdev;
struct class* cls;
struct device* device;
dev_t devno = MKDEV(0, 0);

#define CNAME "smarthome"

int smarthome_open(struct inode* inode, struct file* filp)
{
    return 0;
}

int smarthome_close(struct inode* inode, struct file* filp)
{
    return 0;
}

long smarthome_ioctl(struct file* filp, unsigned int cmd, unsigned long arg)
{
    uint32_t ret = 0;
    typedef void(*pfunc_t)(void);
    static const pfunc_t callzu[] = {
        [IO_LED_ON] = led_on,
        [IO_LED_OFF] = led_off,
        [IO_LED_TOGGLE] = led_toggle,
        [IO_MOTOR_START] = motor_start,
        [IO_MOTOR_STOP] = motor_stop,
        [IO_BUZZ_ON] = buzz_on,
        [IO_BUZZ_OFF] = buzz_off,
        [IO_FAN_START] = fan_start,
        [IO_FAN_STOP] = fan_stop,
    };
    if (_IOC_NR(cmd) < ARRAY_SIZE(callzu)) {
        callzu[_IOC_NR(cmd)]();
        return 0;
    }
    switch (_IOC_NR(cmd)) {
        case IO_SET_TMP_UP_THRESHOLD:   set_temp_up_threshold((uint32_t)arg); break;
        case IO_GET_TMP_UP_THRESHOLD:   ret = get_temp_up_threshold(); break;
        case IO_SET_TMP_DOWN_THRESHOLD: set_temp_down_threshold((uint32_t)arg); break;
        case IO_GET_TMP_DOWN_THRESHOLD: ret = get_temp_down_threshold(); break;
        case IO_GET_TMP:                ret = get_temperature(); break;
        case IO_GET_HUM:                ret = get_humidity(); break;
        case IO_GET_TMP_AND_HUM:        ret = SET_TEMPHUM(get_temperature(), get_humidity()); break;
        case IO_DIGITUBE_TEMP:          set_digitube(DISPLAY_TEMP); break;
        case IO_DIGITUBE_HUM:           set_digitube(DISPLAY_HUM); break;
        case IO_DIGITUBE_TEMP_AND_HUM:  set_digitube(DISPLAY_TEMP_AND_HUM); break;
        case IO_SET_DIGITUBE:           set_digitube(DISPLAY_NUMBER); digitube_display((uint32_t)arg); break;
        default:                        printk("IOCTL cmdcode err!\n"); ret = -1;
    }
    return ret;
}

ssize_t smarthome_read(struct file* filp, char __user* mem, size_t size, loff_t* off)
{
    return size;
}

ssize_t smarthome_write(struct file* filp, const char __user* mem, size_t size, loff_t* off)
{
    return size;
}
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
    led_init();
    pwm_init();
    temphum_init();
    digitube_init();
    timer_init();

    cdev = cdev_alloc();
    cdev_init(cdev, &fops);
    alloc_chrdev_region(&devno, MINOR(devno), 1, CNAME);
    cdev_add(cdev, devno, 1);

    cls = class_create(THIS_MODULE, CNAME);
    if (IS_ERR(cls)) {
        printk("create class failed\n");
        return PTR_ERR(cls);
    }
    device = device_create(cls, NULL, devno, NULL, CNAME);
    if (IS_ERR(device)) {
        printk("create device failed\n");
        return PTR_ERR(device);
    }
    return 0;
}

static void __exit smarthome_exit(void)
{
    led_delinit();
    timer_delinit();
    pwm_delinit();
    temphum_delinit();
    digitube_delinit();

    device_destroy(cls, devno);
    class_destroy(cls);
    cdev_del(cdev);
    unregister_chrdev_region(devno, 1);
    kfree(cdev);
}

module_init(smarthome_init);
module_exit(smarthome_exit);
MODULE_LICENSE("GPL");