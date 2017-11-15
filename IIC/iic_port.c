#include "iic_port.h"
#include "stm32f10x.h"

void iic_tim_init()
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 
	
	TIM_TimeBaseStructure.TIM_Period = 4-1; 
	TIM_TimeBaseStructure.TIM_Prescaler = 18-1; 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	TIM_Cmd(TIM3, DISABLE);
}

void tim3_delay_us(u32 us)
{
	TIM3->CR1 |= TIM_CR1_CEN;
	while(us--)
	{
		while((TIM3->SR & TIM_FLAG_Update) == RESET); 
		TIM3->SR &= ~TIM_FLAG_Update;
	}
	TIM3->CR1 &= ~TIM_CR1_CEN;
}

/* PD2 -- SDA  PC12 -- SCL */
void iic_port_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE );
	  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOC,GPIO_Pin_12); 	
	GPIO_SetBits(GPIOD,GPIO_Pin_2); 

	iic_tim_init();
}

void iic_sda_in(void)
{
	SDA_IN();
}

void iic_sda_out(void)
{
	SDA_OUT();
}

void iic_scl_high(void)
{
	IIC_SCL = 1;
}

void iic_scl_low(void)
{
	IIC_SCL = 0;
}

void iic_sda_high(void)
{
	IIC_SDA = 1;
}

void iic_sda_low(void)
{
	IIC_SDA = 0;
}

char iic_read_sda()
{
	return READ_SDA;
}

void iic_delay_us(u32 us)
{
	tim3_delay_us(us);
}

