#ifndef __IIC_CORE_H__
#define __IIC_CORE_H__

typedef void (*handle_t)(void);
typedef char (*rd_sda_handle_t)(void);
typedef void (*iic_delay_us_handle_t)(unsigned int);

typedef struct
{
    handle_t port_init;
    handle_t sda_in;
    handle_t sda_out;
    handle_t scl_high;
    handle_t scl_low;
    handle_t sda_high;
    handle_t sda_low;
    rd_sda_handle_t read_sda;
    iic_delay_us_handle_t delay_us;
} IIC_phy;


void iic_start(IIC_phy *port);
void iic_stop(IIC_phy *port);
unsigned char iic_wait_ack(IIC_phy *port);
void iic_ack(IIC_phy *port);
void iic_nack(IIC_phy *port);
void iic_send_byte(IIC_phy *port, unsigned char txd);
unsigned char iic_read_byte(IIC_phy *port, unsigned char ack);



#endif

