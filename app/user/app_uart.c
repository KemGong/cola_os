#include <string.h>
#include "app.h"
#include "config.h"
#include "cola_device.h"
#include "cola_os.h"
#include "utils.h"
#include "app_uart.h"

static uint8_t g_buf[512] = {0};

static task_t uart_task;
static cola_device_t *uart_dev;

static void uart_cb(void *arg,uint32_t event)
{
    int ret;
    if(event & SIG_DATA)
    {
        ret = cola_device_read(uart_dev,0,g_buf,sizeof(g_buf));
        if(ret)
        {
            cola_device_write(uart_dev,0,g_buf,ret);
        }
    }
}

static void uart_dev_init(void)
{
    uart_dev = cola_device_find("uart1");
    assert(uart_dev);
    
    cola_device_set_owner(uart_dev,&uart_task);
    struct serial_configure cfg;
    cfg.baud_rate = 115200;
    cola_device_cfg(uart_dev,&cfg,0);
}

void app_uart_init(void)
{
    uart_dev_init();
    
    cola_task_create(&uart_task,uart_cb,NULL);
}
