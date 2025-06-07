#include "stm32f10x.h"

uint16_t time = 0;
/*
void RCC_config(uint8_t MCO, uint8_t PLL, uint8_t HPRE, uint8_t APB1, uint8_t APB2, uint8_t SWS, uint8_t SW)
{
	
	RCC->CR |= 0x01<<16;	// HSE enable
	RCC->CFGR |= (MCO<<24);	// pick MCO
	RCC->CFGR |= (PLL)<<18;	// PLL multiplication factor
	RCC->CFGR |= (HPRE)<<4;	// AHP prescale
	RCC->CFGR |= (APB1)<<8;	// APB1 prescale
	RCC->CFGR |= (APB2)<<11;// APB2 prescale
	//RCC->CFGR |= (ACDPRE)<<14;	// default 00: PCLK2/2
	RCC->CFGR |= (SWS)<<2;
	RCC->CFGR |= (SW);
	
}*/

void RCC_config()
{
	
	RCC->CR |= 0x01<<16;		// HSE enable
	while(!(RCC->CR & (0x01<<17)));
	RCC->CR |= 0x01<<24;		// PLL enable
	while(!(RCC->CR & (0x01<<25)));    // Wait until PLL is ready
	RCC->CFGR |= 0x06<<24;	// pick MCO is HSE
	RCC->CFGR |= 0x07<<18;	// PLL multiplication factor x9
	RCC->CFGR |= 0x01<<16;	// PLL source HSE
	RCC->CFGR |= 0x00<<4;	// AHB prescale/2
	RCC->CFGR &= ~(0x07<<8);	// APB1 prescale/1	=> TIMER equal APB1
	RCC->CFGR &= ~(0x07<<11);// APB2 prescale/1	=> => TIMER equal APB2
	//RCC->CFGR |= (ACDPRE)<<14;	// default 00: PCLK2/2
	RCC->CFGR |= 0x01<<2;
	RCC->CFGR |= 0x01;
	
}

void TIM_config()
{
	// config timer 2
	
	RCC->APB1ENR |= 0x01;
	TIM2->CR1 |= 0x01<<7;	// AUTO reload 
	TIM2->PSC = 8;	// APB2 = 72MHz	=> 1 tick = 1us
	TIM2->ARR = 2000;	// count to 1000 auto reload TIMx_CNT => 1ms
	TIM2->EGR |= 0x01;	// set bit UG
	TIM2->CR1 |= 0x01;
	TIM2->DIER |= 0x01;	// update interrupt enbale
	NVIC_EnableIRQ(TIM2_IRQn);
	
	
	/* config TIM1_CH1 use PWM PA8 */
	
	
}
void PWM_config(uint16_t period, uint8_t channel)
{
	RCC->APB2ENR |= 0x01<<2;
	RCC->APB2ENR |= 0x01<<11;	
	GPIOA->CRH &= ~(0x0F);	// clear
	GPIOA->CRH |= 0x0B;	// Alternate function push-pull
	
	TIM1->PSC = 8;	// APB2 = 72MHz	=> 1 tick = 1us
	TIM1->ARR = period;	// count to 1000 auto reload TIMx_CNT => 1ms
	TIM1->CR1 |= 0x01<<7;	// AUTO reload 
	TIM1->EGR |= 0x01;	// set bit UG
	TIM1->CCMR1 &= ~(0xFF);
	TIM1->CCMR1 |= 0x06<<4;	// pwm mode 1
	TIM1->CCMR1 |= 0x01<<3;
	TIM1->CCR1 = 100;	
	TIM1->CCER |= 0x01;	// CCxE
	TIM1->CCER |= 0x01<<2;	// CCxNE
	TIM1->BDTR |= 0x01<<15;	// MOE (interest only in TIMER 1)
	TIM1->CR1 |= 0x01;	// enable timer
}

void TIM1_PWM(uint8_t duty_cycle)	// percent(%) 0->100
{
	TIM1->CCR1 = (duty_cycle*TIM1->ARR)/100;
}

void TIM2_IRQHandler(void)
{
	if(TIM2->SR & 0x01)
	{
		TIM2->SR &= ~(0x01);
		time++;
	}
}


void delay(uint16_t time)
{
	while(time)
	{
		TIM2->CNT = 0U;
		while(TIM2->CNT <=1000);
		time--;
	}
}

uint16_t millis()
{
	return time;
}
void GPIO_config()
{
	RCC->APB2ENR |= 0x01<<2;	// gpioa
	GPIOA->CRL &= ~(0x0F);
	GPIOA->CRL |= 0x03;
	GPIOA->ODR &= ~(0x01);	// reset 
}


int main()
{
	//SystemInit();
	//RCC_config();
	TIM_config();
	PWM_config(2000,1);
	//GPIO_config();
	//uint16_t pre_time = millis();
	//pwm(500);
	TIM1_PWM(10);
	int i,j;
	while(1)
	{
		/*for( i=0; i<100; i++)
			{
				TIM1->CCR1 = (i*1000)/100;
				if(i==99)
				{
					i=0;
				}
				delay(100);
				//for(j=0; j<100000;j++);
			}
		*/
		//TIM1->CCR1 = 990;
	}
}