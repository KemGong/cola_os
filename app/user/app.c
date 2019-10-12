#include <string.h>
#include "app.h"
#include "config.h"
#include "cola_device.h"
#include "cola_os.h"


static task_t timer_500ms;
static task_t timer_sleep;
static cola_device_t *app_led_dev;

static cola_device_t *app_sleep_dev;

//led每500ms状态改变一次
static void timer_500ms_cb(uint32_t event)
{
    cola_device_ctrl(app_led_dev,LED_TOGGLE,0);
}

static void timer_sleep_cb(uint32_t event)
{
    uint16_t time = 10;
    cola_device_ctrl(app_led_dev,LED_OFF,0);
    cola_device_ctrl(app_sleep_dev,DEV_SLEEP,&time);
}

void app_init(void)
{
    app_led_dev = cola_device_find("led");
    assert(app_led_dev);
    app_sleep_dev = cola_device_find("sleep");
    assert(app_sleep_dev);
    cola_timer_create(&timer_500ms,timer_500ms_cb);
    cola_timer_start(&timer_500ms,TIMER_ALWAYS,500);
    
    cola_timer_create(&timer_sleep,timer_sleep_cb);
    cola_timer_start(&timer_sleep,TIMER_ALWAYS,10000);
}
