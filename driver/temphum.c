/**
 * @file temphum.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-08-10
 *
 * @copyright Copyright (c) 2022
 */


 //温湿度

 //不要用注册file_operation
 /**
  * @brief: 不要使用module_xxx_driver
  * @example: 如果使用probe，可以用下面的形式
  * void xxx_init(void)
  * {
  *      platform_register_driver(...);
  * }
  */
  //只要实现smartdriver.h里的功能函数就行
  //头文件可以自行添加，自行添加的头文件是temphum.c的私有头文件
  //
#include <linux/i2c.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include "temphum.h"
#include "smartdriver.h"

#define CNAME "si7006"

// &i2c1{
//     pinctrl-names = "default", "sleep";
//     pinctrl-0 = <&i2c1_pins_b>;
//     pinctrl-1 = <&i2c1_sleep_pins_b>;
//     i2c-scl-rising-time-ns = <100>;
//     i2c-scl-falling-time-ns = <7>;
//     status = "okay";                                                                                              
//     /delete-property/dmas;      //删除属性
//     /delete-property/dma-names;

//     si7006@40{
//         compatible = "yanyan,si7006";
//         reg = <0x40>;
//     };
// };

static struct i2c_client* gclient;
static spinlock_t lock;

static int temp_up_threshold = 65535;
static int temp_down_threshold = 0;
uint32_t temperature;   //温度
uint32_t humidity;      //湿度

void set_temp_up_threshold(uint32_t buf)
{
    temp_up_threshold = buf;
}

void set_temp_down_threshold(uint32_t buf)
{
    temp_down_threshold = buf;
}

uint32_t get_temp_up_threshold(void)
{
    return temp_up_threshold;
}

uint32_t get_temp_down_threshold(void)
{
    return temp_down_threshold;
}

uint32_t get_temperature(void)
{
    return temperature;
}

uint32_t get_humidity(void)
{
    return humidity;
}

uint32_t read_temperature(void)
{
    int ret;
    unsigned char r_buf[] = { TMP_ADDR };
    unsigned short data;

    // 1.封装消息
    struct i2c_msg r_msg[] = {
        [0] = {
            .addr = gclient->addr,
            .flags = 0,
            .len = 1,
            .buf = r_buf,
        },
        [1] = {
            .addr = gclient->addr,
            .flags = 1,
            .len = 2,
            .buf = (__u8*)&data,
        },
    };

    // 2发送消息
    ret = i2c_transfer(gclient->adapter, r_msg, ARRAY_SIZE(r_msg));
    if (ret != ARRAY_SIZE(r_msg)) {
        printk("i2c read serial or firmware error\n");
        return -EAGAIN;
    }
    data = data >> 8 | data << 8;
    return data;
}

uint32_t read_temperature_humidity(void)
{
    int ret;
    unsigned char r_buf[] = { TMP_ADDR };
    unsigned short data[2];

    // 1.封装消息
    struct i2c_msg r_msg[] = {
        [0] = {
            .addr = gclient->addr,
            .flags = 0,
            .len = 1,
            .buf = r_buf,
        },
        [1] = {
            .addr = gclient->addr,
            .flags = 1,
            .len = 4,
            .buf = (__u8*)data,
        },
    };

    // 2发送消息
    ret = i2c_transfer(gclient->adapter, r_msg, ARRAY_SIZE(r_msg));
    if (ret != ARRAY_SIZE(r_msg)) {
        printk("i2c read serial or firmware error\n");
        return -EAGAIN;
    }
    data[0] = (data[0] << 8) | (data[0] >> 8);
    data[1] = (data[1] << 8) | (data[1] >> 8);
    return *(uint32_t*)data;
}

uint32_t read_humidity(void)
{
    int ret;
    unsigned char r_buf[] = { HUM_ADDR };
    unsigned short data;
    // 1.封装消息
    struct i2c_msg r_msg[] = {
        [0] = {
            .addr = gclient->addr,
            .flags = 0,
            .len = 1,
            .buf = r_buf,
        },
        [1] = {
            .addr = gclient->addr,
            .flags = 1,
            .len = 2,
            .buf = (__u8*)&data,
        },
    };

    // 2发送消息
    ret = i2c_transfer(gclient->adapter, r_msg, ARRAY_SIZE(r_msg));
    if (ret != ARRAY_SIZE(r_msg)) {
        printk("i2c read serial or firmware error\n");
        return -EAGAIN;
    }
    data = data >> 8 | data << 8;
    return data;
}

int si7006_probe(struct i2c_client* client, const struct i2c_device_id* id)
{
    spin_lock_init(&lock);
    gclient = client;
    return 0;
}

int si7006_remove(struct i2c_client* client)
{
    return 0;
}

const struct of_device_id oftable[] = {
    {
        .compatible = "yanyan,si7006",
    },
    { /*end*/ }
};

MODULE_DEVICE_TABLE(of, oftable);

struct i2c_driver si7006 = {
    .probe = si7006_probe,
    .remove = si7006_remove,
    .driver = {
        .name = "yanyan",
        .of_match_table = oftable,
    }
};

void temphum_init(void)
{
    i2c_add_driver(&si7006);
}

void temphum_delinit(void)
{
    i2c_del_driver(&si7006);
}

MODULE_LICENSE("GPL");