#include <linux/init.h>
#include <linux/of.h>
#include <linux/workqueue.h>
#include <linux/timer.h>

#include "smartdriver.h"
#include "ioprotocol.h"
#include "digitube.h"
#include "temphum.h"

#define TIME_NSEC 500

static struct timer_list timer;
static struct work_struct alarm_work;

static void alarm_work_handler(struct work_struct* work)
{
    int hum, temp;
    static int count = 0;
    static uint8_t status = 0;
    temperature = read_temperature();
    humidity = read_humidity();

    switch (digitube_info.status) {
    case DISPLAY_TEMP:
        digitube_display(SET_DIGITNUM(0, 17572 * temperature / 65536 - 4685, 2));
        break;
    case DISPLAY_HUM:
        digitube_display(SET_DIGITNUM(0, 12500 * humidity / 65536 - 600, 2));
        break;
    case DISPLAY_TEMP_AND_HUM:
        hum = 125 * humidity / 65536 - 6;
        temp = 17572 * temperature / 65536 - 4685;
        digitube_display(SET_DIGITNUM(0, temp / 100 * 100 + hum, 0));
        break;
    default:
        break;
    }
    if (temperature > get_temp_up_threshold()) {
        if (!status) {
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
    mod_timer(timer, jiffies + msecs_to_jiffies(TIME_NSEC));
}

void timer_init(void)
{
    INIT_WORK(&alarm_work, alarm_work_handler);
    timer.expires = jiffies + msecs_to_jiffies(TIME_NSEC);
    timer_setup(&timer, timer_handler, 0);
    add_timer(&timer);
}

void timer_delinit(void)
{
    del_timer(&timer);
}