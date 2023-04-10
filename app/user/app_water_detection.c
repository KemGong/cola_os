#include <string.h>
#include "config.h"
#include "cola_device.h"
#include "cola_os.h"
#include "utils.h"
#include "app_sensor.h"

static task_t timer_1ms;
static cola_device_t *adc_detection_dev;
static cola_device_t *output_dev;

static void timer_1ms_cb(void *arg,uint32_t event)
{
    uint16_t adc_value[5] = {0};
    int ret = cola_device_read(adc_detection_dev, 0, adc_value, sizeof(adc_value));
    /*for(uint8_t i = 0; i < 5; i++)
    {
        os_log("adc value[%d]:%d,\r\n", i, adc_value[i]);
    }*/


#ifdef USING_DEBUG
    float voltage = 0;
    float percent = 0;
    static uint32_t test_cnt = 0;

    test_cnt++;
    if (test_cnt % 1000 == 0) {
        os_log("\r\n");
        for (uint8_t i = 0; i < 5; i++) {
            voltage = (float)(adc_value[i]*3.3/4095);
            percent = (float)(adc_value[i]*100.0f/4095.0f);
            //os_log("sysTickUptime = %d\r\n", sysTickUptime);
            os_log("ADC value[%d] = %d-->Voltage%d = %1.3fv-->Percent%d = %3.1f%%\r\n",
            i+3, adc_value[i], i+3, voltage, i+3, percent);
        }
    }
#endif
    if (adc_value[0] < 3500 //|| //adc检测阈值 2.82v->3500
        /*adc_value[1] < 3500 || 
        adc_value[2] < 3500 ||
        adc_value[3] < 3500 ||
        adc_value[4] < 3500*/)
    {
        cola_device_ctrl(output_dev, STATE_OUTPUT_HIGH, 0);
        //set_out_signal(true);
#ifdef USING_DEBUG
    if (test_cnt % 1000 == 0) {
        os_log("out signal high\r\n");
    }
#endif
    }
    else
    {
        cola_device_ctrl(output_dev, STATE_OUTPUT_LOW, 0);
        //set_out_signal(false);
#ifdef USING_DEBUG
    if (test_cnt % 1000 == 0) {
        os_log("out signal low\r\n");
    }
#endif
    }
}

void water_detection_task(void)
{
    adc_detection_dev = cola_device_find("adc");
    assert(adc_detection_dev);
    output_dev = cola_device_find("output");
    assert(output_dev);
    cola_timer_create(&timer_1ms,timer_1ms_cb,NULL);
    cola_timer_start(&timer_1ms,TIMER_ALWAYS,1);
}

