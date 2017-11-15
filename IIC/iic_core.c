#include "iic_core.h"
#include "iic_port.h"


void iic_start(IIC_phy *port)
{
    port->sda_out();
    port->sda_high();
    port->scl_high();
    port->delay_us(4);
    port->sda_low();
    port->delay_us(4);
    port->scl_low();
}

void iic_stop(IIC_phy *port)
{
    port->sda_out();
    port->scl_low();
    port->sda_low();
    port->delay_us(4);
    port->scl_high();
    port->sda_high();
    port->delay_us(4);
}

unsigned char iic_wait_ack(IIC_phy *port)
{
    unsigned char ucErrTime=0;

    port->sda_in();
    port->sda_high();
    port->delay_us(1);
    port->scl_high();
    port->delay_us(1);
    while(port->read_sda())
    {
        ucErrTime++;
        if(ucErrTime>250)
        {
            iic_stop(port);
            return 1;
        }
    }
    port->scl_low();
    return 0;
}

void iic_ack(IIC_phy *port)
{
    port->scl_low();
    port->sda_out();
    port->sda_low();
    port->delay_us(2);
    port->scl_high();
    port->delay_us(2);
    port->scl_low();
}

void iic_nack(IIC_phy *port)
{
    port->scl_low();
    port->sda_out();
    port->sda_high();
    port->delay_us(2);
    port->scl_high();
    port->delay_us(2);
    port->scl_low();
}

void iic_send_byte(IIC_phy *port, unsigned char txd)
{
    unsigned char t;
    port->sda_out();
    port->scl_low();
    for(t=0; t<8; t++)
    {
        if((txd&0x80)>>7)
            port->sda_high();
        else
            port->sda_low();
        txd<<=1;
        port->delay_us(2);
        port->scl_high();
        port->delay_us(2);
        port->scl_low();
        port->delay_us(2);
    }
}

unsigned char iic_read_byte(IIC_phy *port, unsigned char ack)
{
    unsigned char i,receive=0;
    port->sda_in();
    for(i=0; i<8; i++ )
    {
        port->scl_low();
        port->delay_us(2);
        port->scl_high();
        receive<<=1;
        if(port->read_sda())
            receive++;
        port->delay_us(1);
    }
    if (!ack)
        iic_nack(port);
    else
        iic_ack(port);
    return receive;
}



























