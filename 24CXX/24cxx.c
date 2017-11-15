#include "24cxx.h"

static IIC_phy *iic_port;

void AT24CXX_init(IIC_phy *port)
{
	iic_port = port;
}

u8 AT24CXX_ReadOneByte(u16 ReadAddr)
{
    u8 temp=0;
    iic_start(iic_port);
    if(EE_TYPE>AT24C16)
    {
        iic_send_byte(iic_port, 0xA0);      
        iic_wait_ack(iic_port);
        iic_send_byte(iic_port, ReadAddr>>8);
        iic_wait_ack(iic_port);
    }
    else iic_send_byte(iic_port, 0xA0+((ReadAddr/256)<<1));    

    iic_wait_ack(iic_port);
    iic_send_byte(iic_port, ReadAddr%256);   
    iic_wait_ack(iic_port);
    iic_start(iic_port);
    iic_send_byte(iic_port, 0XA1);           
    iic_wait_ack(iic_port);
    temp=iic_read_byte(iic_port, 0);
    iic_stop(iic_port);
    return temp;
}


void AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite)
{
    iic_start(iic_port);
    if(EE_TYPE>AT24C16)
    {
        iic_send_byte(iic_port, 0xA0);     
        iic_wait_ack(iic_port);
        iic_send_byte(iic_port, WriteAddr>>8);
    }
    else
    {
        iic_send_byte(iic_port, 0xA0+((WriteAddr/256)<<1));  
    }
    iic_wait_ack(iic_port);
    iic_send_byte(iic_port, WriteAddr%256);  
    iic_wait_ack(iic_port);
    iic_send_byte(iic_port, DataToWrite);   
    iic_wait_ack(iic_port);
    iic_stop(iic_port);
    iic_port->delay_us(10000);
}

void AT24CXX_WriteLenByte(u16 WriteAddr,u32 DataToWrite,u8 Len)
{
    u8 t;
    for(t=0; t<Len; t++)
    {
        AT24CXX_WriteOneByte(WriteAddr+t,(DataToWrite>>(8*t))&0xff);
    }
}


u32 AT24CXX_ReadLenByte(u16 ReadAddr,u8 Len)
{
    u8 t;
    u32 temp=0;
    for(t=0; t<Len; t++)
    {
        temp<<=8;
        temp+=AT24CXX_ReadOneByte(ReadAddr+Len-t-1);
    }
    return temp;
}

u8 AT24CXX_Check(void)
{
    u8 temp;
    temp=AT24CXX_ReadOneByte(255);
    if(temp==0X55)return 0;
    else
    {
        AT24CXX_WriteOneByte(255,0X55);
        temp=AT24CXX_ReadOneByte(255);
        if(temp==0X55)return 0;
    }
    return 1;
}


void AT24CXX_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead)
{
    while(NumToRead)
    {
        *pBuffer++=AT24CXX_ReadOneByte(ReadAddr++);
        NumToRead--;
    }
}

void AT24CXX_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite)
{
    while(NumToWrite--)
    {
        AT24CXX_WriteOneByte(WriteAddr,*pBuffer);
        WriteAddr++;
        pBuffer++;
    }
}












