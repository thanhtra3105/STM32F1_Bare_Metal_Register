#include "stm32f10x.h"
void delay_ms_init()	// dung timer 2
{
	// init time base
	
	RCC->APB1ENR |=0x01;		//TIM2 clock enable
	TIM2->PSC = 7;	// chia 8mhz/(7+1) = 1mhz	=> 1 tick = 1us
	TIM2->ARR = 2000;
	
	TIM2->CR1 |= 0x01;	// counter enable
	
	TIM2->EGR |=0x01;	//SET bit UG to reload PSC and ARR
}

void delay_ms(uint16_t time)
{
	
	while(time)
	{
		TIM2->CNT = 0U;
		while(TIM2->CNT <1000);
		time--;
		
	}
	
}
