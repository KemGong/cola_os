#include <string.h>
#include "config.h"
#include "cola_os.h"
#include "cola_device.h"
#include "bsp.h"
#include "utils.h"
#include "aht20.h"


#define AHT20_ADDR       0x38

struct aht20_data
{
    cola_device_t *i2c_bus;
};

static struct aht20_data  aht20_data;
static struct cola_device aht20_dev;

static int crc8_compute(uint8_t *check_data, uint8_t num_of_data)
{
	uint8_t bit;        // bit mask
	uint8_t crc = 0xFF; // calculated checksum
	uint8_t byteCtr;    // byte counter
	// calculates 8-Bit checksum with given polynomial
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

static int aht20_crc8_check(uint8_t *p,uint8_t num_of_data,uint8_t CrcData)
{
	uint8_t crc;
	crc = crc8_compute(p, num_of_data);// calculates 8-Bit checksum
	if(crc == CrcData) 
	{   
		return 0;           
	}
	return -1;
}

static void aht20_reset(cola_device_t *i2c_bus)
{
    uint8_t i;
    uint8_t temp[8] = {0};
    uint8_t value[]     = {0x1b,0x1c,0x1e};
    for(i = 0; i < 3; i++)
    {
        memset(temp,0x00,sizeof(temp));
        temp[0] = value[i];
        cola_device_write(i2c_bus,0,temp,3);
        cola_delay_ms(5);
        cola_device_read(i2c_bus,0,temp,3);
        cola_delay_ms(10);
        temp[0] = 0xB0|value[i];
        cola_device_write(i2c_bus,0,temp,3);
    }
}

static void aht20_read_status_and_init(cola_device_t *i2c_bus)
{
    int err;
	uint8_t  rx[8];
	err = cola_device_read(i2c_bus,0,rx,1);
    if (err < 0 || (rx[0] & ACK_READ_STATE)!=ACK_READ_STATE)
    {	
        os_log("aht20 is error,init now!\n");    
        aht20_reset(i2c_bus);
        cola_delay_ms(10);
    }	
}

static int aht20_start_meas(cola_device_t *i2c_bus)
{
    int ret = 0;
    uint8_t tx[8];
    tx[0] = 0xac;
    tx[1] = 0x33;
    tx[2] = 0x00;
    ret = cola_device_write(i2c_bus,0,tx,3);
    return ret;
}

static int aht20_check_busy(cola_device_t *i2c_bus)
{
    uint8_t rx[2];
    cola_device_read(i2c_bus,0,rx,1);
    return (rx[0]>>7) & 0x01;
}

static int aht20_get_sensor_value(cola_device_t *dev, int pos, void *buffer, int size)
{
    struct aht20_data *aht20 = (struct aht20_data *)dev->argv;
    uint8_t  rx[8] = {0};
    int err = 0;
    int temp = 0,hum = 0;
    int i = 0;
    
    aht20->i2c_bus->data = dev->data;
    aht20_start_meas(aht20->i2c_bus);
    for(i = 0; i < 100; i++)
    {
        if(!aht20_check_busy(aht20->i2c_bus))
        {
            break;
        }
        cola_delay_ms(1);
        if(i == 99)
        {
            aht20_read_status_and_init(aht20->i2c_bus);
            return 0;
        }
    }
    cola_delay_ms(5);
    cola_device_read(aht20->i2c_bus,0,rx,7);
    //eslog_hex(NULL,rx,7);
    err = aht20_crc8_check(rx,6,rx[6]);
    if(err < 0) 
    {
        return 0;
    }
    
    int RetuData = 0;
    RetuData = (rx[1] << 8) | rx[2];
    RetuData = (RetuData << 4) | ((rx[3] & 0xF0) >> 4);
    hum      = RetuData;

    RetuData = rx[3] & 0x0F;
    RetuData = (RetuData << 8) | rx[4];
    RetuData = (RetuData << 8) | rx[5];
    temp     = RetuData;
    
    hum = ((double)hum*100/ 1048576.0)*100;//2^20=1048576
	temp = ((double)temp *200.0 / 1048576.0 -50)*100;
    put_be_val(temp,   (uint8_t *)rx, 2);
    put_be_val(hum,  (uint8_t *)&rx[2], 2);
    memcpy(buffer,rx,6);
    return 6;
}

static const struct cola_device_ops aht20_dev_ops =
{
    .read = aht20_get_sensor_value,
};

static int aht20_init(const char *i2c_bus_name)
{

    aht20_data.i2c_bus  = cola_device_find(i2c_bus_name);
    if (aht20_data.i2c_bus == NULL)
    {
        return -1;
    }
    aht20_dev.dops      = &aht20_dev_ops;
    aht20_dev.argv      = &aht20_data;
    aht20_dev.name      = "aht20";
    aht20_dev.data      = AHT20_ADDR;
    cola_delay_ms(500);
    aht20_read_status_and_init(aht20_data.i2c_bus);
    cola_device_register(&aht20_dev);

    return 0;
}

void aht20_register(void)
{
    aht20_init("i2c0");
}

late_initcall(aht20_register);
