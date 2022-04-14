#include <string.h>
#include "app.h"
#include "config.h"
#include "cola_device.h"
#include "cola_os.h"
#include "app_uart.h"
#include "app_sensor.h"

static task_t timer_500ms;
static cola_device_t *app_led_dev;

//led每500ms状态改变一次
static void timer_500ms_cb(void *arg,uint32_t event)
{
    cola_device_ctrl(app_led_dev,LED_TOGGLE,0);
}

void app_init(void)
{
    app_led_dev = cola_device_find("led_green");
    assert(app_led_dev);
    cola_timer_create(&timer_500ms,timer_500ms_cb,NULL);
    cola_timer_start(&timer_500ms,TIMER_ALWAYS,500);
    app_uart_init();
    app_sensor();
}
