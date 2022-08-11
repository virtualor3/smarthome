#include <linux/init.h>
#include <linux/of.h>
#include <linux/workqueue.h>
#include <linux/timer.h>

#include "smartdriver.h"
#include "ioprotocol.h"

uint32_t temperature;   //温度
uint32_t humidity;      //湿度
static struct timer_list timer;
static struct work_struct alarm_work;

#define TIME_NSEC 200

static void alarm_work_handler(struct work_struct* work)
{
    humidity = read_humidity();
    if ((temperature = read_temperature()) > get_temp_threshold()) {
        fan_start();
        buzz_on();
    } else {
        fan_stop();
        buzz_off();
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