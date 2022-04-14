#ifndef _COLA_DEVICE_H_
#define _COLA_DEVICE_H_


#include "cola_init.h"
#include <stdint.h>

#define I2C_REG_8BIT     0x08               
#define I2C_REG_16BIT    0x10           

struct i2c_addr
{
    uint32_t addr:8;
    uint32_t reg_width:8;
    uint32_t reserve:16;
};

union i2c_msg
{
    uint32_t addr_msg;
    struct i2c_addr addr;
};


enum OUTPUT_state
{
	STATE_OUTPUT_LOW  = 0x00,
    STATE_OUTPUT_HIGH = 0x01,

};

enum LED_state
{
    LED_OFF,
    LED_ON,
    LED_TOGGLE,

};
enum DEV_sleep
{
	DEV_SLEEP= 0x00,
    DEV_DEEP_SLEEP ,
    DEV_STOP,
    DEV_STANDBY,
	
};

struct serial_configure
{
    uint32_t baud_rate;
};



typedef struct cola_device  cola_device_t;

struct cola_device_ops
{
    int  (*init)   (cola_device_t *dev);
    int  (*open)   (cola_device_t *dev, int oflag);
    int  (*close)  (cola_device_t *dev);
    int  (*read)   (cola_device_t *dev, int pos, void *buffer, int size);
    int  (*write)  (cola_device_t *dev, int pos, const void *buffer, int size);
    int  (*control)(cola_device_t *dev, int cmd, void *args);
	int  (*config) (cola_device_t *dev, void *args, void *var);
};

struct cola_device
{
    const char * name;
    const struct cola_device_ops *dops;
    void   *owner;
    void   *argv;
    int    data;
    struct cola_device *next;
};

/*
    驱动注册
*/
int cola_device_register(cola_device_t *dev);
/*
    驱动查找
*/
cola_device_t *cola_device_find(const char *name);
/*
    驱动读
*/
int cola_device_read(cola_device_t *dev,  int pos, void *buffer, int size);
/*
    驱动写
*/
int cola_device_write(cola_device_t *dev, int pos, const void *buffer, int size);
/*
    驱动控制
*/
int cola_device_ctrl(cola_device_t *dev,  int cmd, void *arg);
/*
    驱动配置
*/
int cola_device_cfg(cola_device_t *dev, void *args, void *var);
/*
    设置驱动属于哪个任务
*/
void cola_device_set_owner(cola_device_t *dev, const void *owner);
#endif 
