#ifndef __IIC_PORT_H__
#define __IIC_PORT_H__

#include "sys.h"
#include "delay.h"

#define SDA_IN()  {GPIOD->CRL&=(u32)~(0xf<<8);GPIOD->CRL|=(u32)8<<8;}
#define SDA_OUT() {GPIOD->CRL&=(u32)~(0xf<<8);GPIOD->CRL|=(u32)3<<8;}

//IO²Ù×÷º¯Êý	 
#define IIC_SCL    PCout(12) //SCL
#define IIC_SDA    PDout(2) //SDA	 
#define READ_SDA   PDin(2)  //ÊäÈëSDA 

void iic_port_init(void);
void iic_sda_in(void);
void iic_sda_out(void);
void iic_scl_high(void);
void iic_scl_low(void);
void iic_sda_high(void);
void iic_sda_low(void);
char iic_read_sda(void);
void iic_delay_us(u32 us);

#endif

