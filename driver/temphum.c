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
//
//test
//virtualor3 test.hahaha
#include <linux/i2c.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include "temphum.h"
#include "smartdriver.h"

#define CNAME "si7006"

static struct i2c_client* gclient;

static int temp;
static spinlock_t lock;

void set_temp_threshold(uint32_t buf)
{
    //临界区
    spin_lock(&lock);
    temp = buf;
    spin_unlock(&lock);
}

uint32_t get_temp_threshold(void)
{
    return temp;
}

uint32_t read_temperature(void)
{
    int ret;
    unsigned char r_buf[] = { 0xE3 };
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
            .buf =  (__u8 *)&data,
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
    unsigned char r_buf[] = { 0xE3 };
    unsigned char data[4];
    unsigned int  data_return = 0;

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
            .buf =  (__u8 *)data,
        },
    };

    // 2发送消息
    ret = i2c_transfer(gclient->adapter, r_msg, ARRAY_SIZE(r_msg));
    if (ret != ARRAY_SIZE(r_msg)) {
        printk("i2c read serial or firmware error\n");
        return -EAGAIN;
    }
    data_return = 
        (data[2] << 24) | (data[3] << 16) | 
        (data[0] << 8 ) | (data[1] << 0 );  

    return data_return;
}

uint32_t read_humidity(void)
{
    int ret;
    unsigned char r_buf[] = { 0xE5 };
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
            .buf =  (__u8 *)&data,
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