#include "stm32f10x.h"
void gpio_config()
{
	GPIOA->CRL &= ~0x0F;
	GPIOA->CRL |= 0x03;
	GPIOA->ODR &= ~0x01;
	GPIOA->ODR |= 0x01;
}
void TIM2_config()	
{
	// init time base
	
	RCC->APB1ENR |=0x01;		//TIM2 clock enable
	TIM2->PSC = 7;	// chia 8mhz/(7+1) = 1mhz	=> 1 tick = 1us
	TIM2->ARR = 2000;
	
	TIM2->CR1 |= 0x01;	// counter enable
	
	TIM2->EGR |=0x01;	//SET bit UG to reload PSC and ARR
	
	/* interrupt*/
	/*RCC->APB1ENR |=0x01;		//TIM2 clock enable
	TIM2->PSC = 7;	// chia 8mhz/(7+1) = 1mhz	=> 1 tick = 1us
	TIM2->ARR = 1000;
	TIM2->DIER |= 0x01;	// set UIE 
	TIM2->CR1 |= 0x01;	// counter enable
	NVIC_EnableIRQ(TIM2_IRQn);
*/
}



// ham xu li ngat
void TIM2_IRQHandler()
{
	if((TIM2->SR &0x01) == 1)
	{
		GPIOA->ODR |= 0x01;
	}
	
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
int main()
{
	TIM2_config();
	
	gpio_config();
	
	
	while(1)
	{
		GPIOA->ODR &= ~0x01;
		/*
		GPIOA->ODR |= 0x01;
		delay_ms(100);
		GPIOA->ODR &= ~0x01;
		delay_ms(100);
		*/
	}
}

