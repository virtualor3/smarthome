#include <linux/init.h>
#include <linux/of.h>
#include <linux/irq.h>
#include <linux/timer.h>

#include "smartdriver.h"
#include "ioprotocol.h"

struct timer_list timer;
struct work_struct work;
uint32_t temperature;

#define TIME_NSEC 200

void work_handler(struct work_struct* work)
{
    if ((temperature = read_temperature()) > get_temp_threshold()) {
        fan_start();
        buzz_on();
    } else {
        fan_stop();
        buzz_off();
    }
}

void timer_handler(struct timer_list* timer)
{
    schedule_work(&work);
    mod_timer(timer, jiffies + TIME_NSEC / HZ);
}

void timer_init(void)
{
    INIT_WORK(&work, work_handler);
    timer.expires = jiffies + TIME_NSEC / HZ;
    timer_setup(&timer, timer_handler, 0);
    add_timer(&timer);
}