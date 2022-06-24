#include "config.h"
#include "SWM181.h"
#include "cola_device.h"
#include "cola_init.h"
#include "utils.h"
#include "cola_fifo.h"
#include "cola_os.h"
#include "SWM181_i2c.h"

#ifdef  USING_I2C0


static cola_device_t i2c0_dev;

static void i2c0_init(void)
{
    I2C_InitStructure I2C_initStruct;
	
	PORT_Init(PORTA, PIN4, FUNMUX_I2C0_SCL, 1);		//GPIOA.4����ΪI2C0 SCL����
	PORTA->OPEND |= (1 << PIN4);					//��©
	PORTA->PULLU |= (1 << PIN4);					//����
	PORT_Init(PORTA, PIN5, FUNMUX_I2C0_SDA, 1);		//GPIOA.5����ΪI2C0 SDA����
	PORTA->OPEND |= (1 << PIN5);					//��©
	PORTA->PULLU |= (1 << PIN5);					//����
	
	I2C_initStruct.Master = 1;
	I2C_initStruct.Addr7b = 1;
	I2C_initStruct.MstClk = 10000;
	I2C_initStruct.MstIEn = 0;
	I2C_Init(I2C0, &I2C_initStruct);
	
	I2C_Open(I2C0);
}

static int i2c0_write(cola_device_t *dev, int pos, const void *buffer, int size)
{
    int ret,i;
    uint8_t addr = dev->data;
    uint8_t *tx = (uint8_t *)buffer;
    ret = I2C_Start(I2C0, (addr << 1) | 0);
    if(ret == 0)
    {
        os_log("Master send NACK for address:%d\r\n",addr );
        I2C_Stop(I2C0);
        return ret;
    }
    for(i = 0; i < size; i++)
    {
        ret = I2C_Write(I2C0, tx[i]);
        if(ret == 0)
        {
            os_log("Master send NACK for data\r\n");
            I2C_Stop(I2C0);
            return ret;
        }
    }
    I2C_Stop(I2C0);
	return ret;
}
static int i2c0_read(cola_device_t *dev, int pos, void *buffer, int size)
{
    int ret,i;
    uint8_t addr = dev->data;
    uint8_t *rx = (uint8_t *)buffer;
    ret = I2C_Start(I2C0, (addr << 1) | 1);
    if(ret == 0)
    {
        os_log("Slave send NACK for address\r\n");
        I2C_Stop(I2C0);
        return ret;
    }
   
    for(i = 0; i < size-1; i++)
    {
        rx[i] = I2C_Read(I2C0, 1);
    }
    rx[i] = I2C_Read(I2C0, 0);
    I2C_Stop(I2C0);
	return ret;
}

static struct cola_device_ops ops =
{
    .write = i2c0_write,
    .read  = i2c0_read,
};

#endif

void board_setup_i2c(void)
{
#ifdef USING_I2C0
    i2c0_init();
    i2c0_dev.name = "i2c0";
    i2c0_dev.dops = &ops;
    cola_device_register(&i2c0_dev);
#endif
}

device_initcall(board_setup_i2c);
