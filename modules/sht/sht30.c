#include <string.h>
#include "config.h"
#include "cola_os.h"
#include "cola_device.h"
#include "bsp.h"
#include "utils.h"
#include "sht30.h"

#define SHT30_ADDR       0x44

struct sht30_data
{
    cola_device_t *i2c_bus;
};

static struct sht30_data sht30_data;
static struct cola_device sht30_dev;

static int crc8_compute(uint8_t *check_data, uint8_t num_of_data)
{
	uint8_t bit;        
	uint8_t crc = 0xFF; 
	uint8_t byteCtr;    
	for(byteCtr = 0; byteCtr < num_of_data; byteCtr++) 
	{
		crc ^= (check_data[byteCtr]);
		for(bit = 8; bit > 0; --bit) 
		{
			if(crc & 0x80) 
			{
				crc = (crc << 1) ^ 0x131; //// P(x) = x^8 + x^5 + x^4 + 1 = 100110001
			}    
			else 
			{
				crc = (crc << 1);
			}
		}
	}
 return crc;
}

static int sht30_crc8_check(uint8_t *p,uint8_t num_of_data,uint8_t CrcData)
{
	uint8_t crc;
	crc = crc8_compute(p, num_of_data);// calculates 8-Bit checksum
	if(crc == CrcData) 
	{   
		return 0;           
	}
	return -1;
}

void sht30_reset(cola_device_t *dev)
{
    uint8_t i;
	struct sht30_data *sht30 = (struct sht30_data *)dev->argv; 
    uint16_t value[]     = {CMD_SOFT_RESET,CMD_MEAS_PERI_1_H};
    sht30->i2c_bus->data = dev->data;

    for(i = 0; i < 1; i++)
    {
        cola_device_write(sht30->i2c_bus,value[i],0,0);
    }
}

static int sht30_get_sensor_value(cola_device_t *dev, int pos, void *buffer, int size)
{
    struct sht30_data *sht30 = (struct sht30_data *)dev->argv;
    uint8_t  rx[8];
    uint16_t temp_value = 0;
    int err = 0;
    int temp = 0,hum = 0;
    int  succeed_cnt = 0;
    int sum_temp = 0,sum_hum = 0;
    int i = 0;
    sht30->i2c_bus->data = dev->data;
    for(i = 0; i < 6; i++)
    {
        memset(buffer,0,size);
        err = cola_device_read(sht30->i2c_bus,CMD_MEAS_CLOCKSTR_H,rx,6);
        if (err == 0)
        {			
            sht30_reset(dev);
            cola_delay_ms(50);
            continue;
        }	
        //os_hex(rx,6);
        err = sht30_crc8_check(rx,2,rx[2]);
        if(err < 0) 
        {
            continue;
        }
        err = sht30_crc8_check(&rx[3],2,rx[5]);
        if(err < 0) 
        {
            continue;
        }
        succeed_cnt++;
        temp_value = (rx[0] << 8) | rx[1];
        temp = (int)((175.0 * ((float)temp_value) / 65535.0 - 45.0) *10.0);
        temp_value = (rx[3] << 8) | rx[4];
        hum = (100 * (float)temp_value / 65535)*10 ;  // RH = rawValue / (2^16-1) * 10
        sum_temp+=temp;
        sum_hum+=hum;
    }

    if(succeed_cnt)
    {
        temp    = sum_temp/succeed_cnt;
        hum     = sum_hum/succeed_cnt;
    }
    else
    {
        temp = 0;
        hum  = 0;
        return 0;
    }
    put_be_val(temp, (uint8_t *)rx, 2);
    put_be_val(hum,  (uint8_t *)&rx[2], 2);
    memcpy(buffer,rx,4);
    return 4;
}

static const struct cola_device_ops sht30_dev_ops =
{
    .read = sht30_get_sensor_value,
};

int sht30_init(const char *i2c_bus_name)
{
    union i2c_msg msg;
    sht30_data.i2c_bus  = cola_device_find(i2c_bus_name);
    if (sht30_data.i2c_bus == NULL)
    {
        return -1;
    }
    sht30_dev.dops      = &sht30_dev_ops;
    sht30_dev.argv      = &sht30_data;
    sht30_dev.name      = "sht30";
    msg.addr.addr       = SHT30_ADDR;
    msg.addr.reg_width  = I2C_REG_16BIT;
    sht30_dev.data      = msg.addr_msg;

    //sht30_reset(&sht30_dev);

    cola_device_register(&sht30_dev);

    return 0;
}

void setup_sht30(void)
{
    sht30_init("i2c0");
}

late_initcall(setup_sht30);
