#include "cola_os.h"
#include "app.h"
#include <string.h>
#include "bsp.h"
#include "led.h"
#include "config.h"

task_t timer_500ms,timer_1s;


//led每500ms状态改变一次
static void timer_500ms_cb(uint32_t event)
{
    led_toggle();
}
//led每500ms状态改变一次
static void timer_1s_cb(uint32_t event)
{
    os_log("timer 1s......\r\n");
}


void app_init(void)
{
    cola_timer_create(&timer_500ms,timer_500ms_cb);
    cola_timer_start(&timer_500ms,TIMER_ALWAYS,500);
    cola_timer_create(&timer_1s,timer_1s_cb);
    cola_timer_start(&timer_1s,TIMER_ALWAYS,1000);
}
