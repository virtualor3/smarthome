#include <linux/init.h>
#include <linux/of.h>
#include <linux/workqueue.h>
#include <linux/timer.h>

#include "smartdriver.h"
#include "ioprotocol.h"
#include "temphum.h"

#define TIME_NSEC 100

uint32_t temperature;   //温度
uint32_t humidity;      //湿度
static struct timer_list timer;
static struct work_struct alarm_work;

static void alarm_work_handler(struct work_struct* work)
{
    static int count = 0;
    static uint8_t status = 0;
    uint32_t tmp = read_temperature_humidity();
    temperature = GET_TEMP(tmp);
    humidity = GET_HUM(tmp);

    if (temperature > get_temp_up_threshold()) {
        if(!status) {
            fan_start();
            buzz_on();
        }
        goto Return;
    }
    if (temperature < get_temp_down_threshold()) {
        if (status) {
            fan_stop();
            buzz_off();
            led_off();
        }
        status = 0;
        goto Return;
    }
Return:
    if (status) {
        count %= 5;
        if (count == 0) led_toggle();
        count++;
    }
}

static void timer_handler(struct timer_list* timer)
{
    schedule_work(&alarm_work);
    mod_timer(timer, jiffies + HZ * TIME_NSEC / 1000);
}

void timer_init(void)
{
    INIT_WORK(&alarm_work, alarm_work_handler);
    timer.expires = jiffies + HZ * TIME_NSEC / 1000;
    timer_setup(&timer, timer_handler, 0);
    add_timer(&timer);
}

void timer_delinit(void)
{
    del_timer(&timer);
}