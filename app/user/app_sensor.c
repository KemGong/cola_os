#include <string.h>
#include "config.h"
#include "cola_device.h"
#include "cola_os.h"
#include "utils.h"
#include "app_sensor.h"

static task_t timer_500ms;
static cola_device_t *sensordev;

//ledÿ500ms״̬�ı�һ��
static void timer_500ms_cb(void *arg,uint32_t event)
{
    uint8_t rx[8] = {0};
    int ret = cola_device_read(sensordev,0,rx,8);
    os_log("temp:%d,humi:%d\r\n",get_be_val(rx,2),get_be_val(&rx[2],2));
}

void app_sensor(void)
{
    sensordev = cola_device_find("aht20");
    assert(sensordev);
    cola_timer_create(&timer_500ms,timer_500ms_cb,NULL);
    cola_timer_start(&timer_500ms,TIMER_ALWAYS,3000);
}
