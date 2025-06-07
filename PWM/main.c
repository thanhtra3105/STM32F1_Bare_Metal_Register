#include "stm32f10x.h" 



void PWM_TIM2_CH1_Init(void) {
	RCC->APB1ENR |= 0x01;
	RCC->APB2ENR |= 0x01<<2;
	GPIOA->CRL &= ~(0x0F);	// clear
	GPIOA->CRL |= 0x0B;	// Alternate function push-pull
	TIM2->ARR =1000;
	TIM2->PSC = 8;
	TIM2->CR1 |= 0x01<<7;	// AUTO reload 
	TIM2->CCMR1 = 0x6060;	// PWM mode 1
	TIM2->CCMR1 |= 0x01<<3;	
	TIM2->CCMR2 = 0x6060;
	TIM2->CCR1 = (10*1000)/100;
	TIM2->CCR2 = (50*1000)/100;
	TIM2->CCR3 = (50*1000)/100;
	TIM2->CCR4 = (50*1000)/100;
	TIM2->CCER = 0x1111;	// enable 
	TIM2->CR1 = 0x01;
	TIM2->EGR = 0x01;
	
}


void PWM_StandardLibrary(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef timerInit;
	TIM_OCInitTypeDef pwmInit;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2| GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	timerInit.TIM_CounterMode = TIM_CounterMode_Up;
	timerInit.TIM_Period = 1000 - 1;
	timerInit.TIM_Prescaler = 7200 - 1;
	
	TIM_TimeBaseInit(TIM2, &timerInit);
	
	TIM_Cmd(TIM2, ENABLE);
	
	
	pwmInit.TIM_OCMode = TIM_OCMode_PWM1;
	pwmInit.TIM_OCPolarity = TIM_OCPolarity_High;
	pwmInit.TIM_Pulse = (99*10000)/100;
	pwmInit.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OC1Init(TIM2, &pwmInit);
	
	
	pwmInit.TIM_OCMode = TIM_OCMode_PWM1;
	pwmInit.TIM_OCPolarity = TIM_OCPolarity_High;
	pwmInit.TIM_Pulse = (25*10000)/100;
	pwmInit.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OC2Init(TIM2, &pwmInit);
	
	
	pwmInit.TIM_OCMode = TIM_OCMode_PWM1;
	pwmInit.TIM_OCPolarity = TIM_OCPolarity_High;
	pwmInit.TIM_Pulse = (40*10000)/100;
	pwmInit.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OC3Init(TIM2, &pwmInit);

	
	pwmInit.TIM_OCMode = TIM_OCMode_PWM1;
	pwmInit.TIM_OCPolarity = TIM_OCPolarity_High;
	pwmInit.TIM_Pulse = (80*10000)/100;
	pwmInit.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OC4Init(TIM2, &pwmInit);
	
}

int main(void) {
    PWM_TIM2_CH1_Init(); 
	int i;
	int j=0;
		//PWM_StandardLibrary();
    while (1) {
			for( i=0; i<100; i++)
			{
				TIM2->CCR1 = (i*1000)/100;
				if(i==99)
				{
					i=0;
				}
				for(j=0; j<100000;j++);
			}
       
    }
}



