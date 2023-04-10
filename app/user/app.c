#include <string.h>
#include "app.h"
#include "config.h"
#include "cola_device.h"
#include "cola_os.h"
#include "app_uart.h"
#include "app_sensor.h"
#include "app_water_detection.h"

static task_t timer_500ms;
static cola_device_t *app_output_dev;

//ledÿ500ms״̬�ı�һ��
static void timer_500ms_cb(void *arg,uint32_t event)
{
    cola_device_ctrl(app_output_dev,STATE_OUTPUT_TOGGLE,0);
    os_log("this is timer_500ms callback!");
}

void app_init(void)
{
    // app_output_dev = cola_device_find("output");
    // assert(app_output_dev);
    // cola_timer_create(&timer_500ms,timer_500ms_cb,NULL);
    // cola_timer_start(&timer_500ms,TIMER_ALWAYS,500);
    // app_uart_init();
    // app_sensor();
    water_detection_task();
}
