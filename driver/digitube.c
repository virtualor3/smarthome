#include <linux/init.h>
#include <linux/module.h>
#include <linux/spi/spi.h>
#include <linux/timer.h>
#include <linux/spinlock.h>
#include <linux/workqueue.h>

#include "smartdriver.h"
// &spi4{
//     pinctrl-names = "default", "sleep";
//     pinctrl-0 = <&spi4_pins_b>;
//     pinctrl-1 = <&spi4_sleep_pins_b>;
//     cs-gpios = <&gpioe 11 0>;
//     status = "okay";

//     m74hc595@0{
//         compatible = "hqyj,spi_m74hc595";
//         reg = <0>;
//         spi-max-frequency = <10000000>; //10Mhz
//     };
// };

#define DIGIt_drv_NAME   "digit_drv"
#define TIME_NSEC        10

static struct spi_device* digit_dev;
static struct work_struct digitube_work;
static struct timer_list timer;
static spinlock_t spin;

static const u8 code[] = {
 0x3f, //0
 0x06, //1
 0x5b, //2
 0x4f, //3
 0x66, //4
 0x6d, //5
 0x7d, //6
 0x07, //7
 0x7f, //8
 0x6f, //9
 0x77, //A
 0x7c, //b
 0x39, //c
 0x5e, //d
 0x79, //e
 0x71, //f
 0x40, //-
};

static const u8 which[] = {
 0x1, //sg0
 0x2, //sg1
 0x4, //sg2
 0x8, //sg3
};

static struct digitubeinfo
{
    uint8_t digitube_char[4];  //4位数码管数值
    uint8_t decimal_places;    //数码管小数点位置(0~3)
    uint8_t neg;               //符号位 
} digitube_info = { 0 };

void digitube_display(uint32_t num)
{
    int i;
    spin_lock(&spin);
    digitube_info.decimal_places = 3 - (GET_DIGITDEC(num) & 0x3);
    digitube_info.neg = GET_DIGITNEG(num);

    num = GET_DIGITNUM(num);
    for (i = 0; i < 4; i++) {
        digitube_info.digitube_char[3 - i] = num % 10;
        num /= 10;
    }
    spin_unlock(&spin);
}

static void digitube_work_handler(struct work_struct* work)
{
    uint8_t buf[2];
    static uint32_t i = 0;
    spin_lock(&spin);
    buf[0] = which[i];
    if (i == 0 && digitube_info.neg != 0) {
        buf[1] = code[16];
    } else {
        buf[1] = code[digitube_info.digitube_char[i]];
    }
    if (i != 3 && digitube_info.decimal_places == i) buf[1] |= 0x80;
    spin_unlock(&spin);
    spi_write(digit_dev, buf, sizeof(buf));
    i++;
    i &= 0x3;       //限制i的值在0~3
}

static void timer_handler(struct timer_list* timer)
{
    schedule_work(&digitube_work);
    mod_timer(timer, jiffies + HZ * TIME_NSEC / 1000);
}

static int inline digit_drv_probe(struct spi_device* spi)
{
    spin_lock_init(&spin);
    INIT_WORK(&digitube_work, digitube_work_handler);
    timer.expires = jiffies + HZ * TIME_NSEC / 1000;
    timer_setup(&timer, timer_handler, 0);
    add_timer(&timer);
    digit_dev = spi;
    return 0;
}

static int inline digit_drv_remove(struct spi_device* spi)
{
    del_timer(&timer);
    return 0;
}

static struct of_device_id of_table[] = {
    [0] = {.compatible = "virtualor3,spi_m74hc595" },
    { /*END*/ },
};
MODULE_DEVICE_TABLE(of, of_table);

static struct spi_driver digit_drv = {
    .probe = digit_drv_probe,
    .remove = digit_drv_remove,
    .driver = {
        .name = DIGIt_drv_NAME,
        .of_match_table = of_table,
    },
};

void inline digitube_init(void)
{
    spi_register_driver(&digit_drv);
}

void inline digitube_delinit(void)
{
    spi_unregister_driver(&digit_drv);
}