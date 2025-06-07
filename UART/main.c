#include "stm32f10x.h"
#include "delay_ms.h"
#include "uart.h"

void gpio_config()
{
	RCC->CFGR |= 0x06 <<24; // HSE
	RCC->APB2ENR |= 0x01<<2;
	GPIOA->CRL &= ~0x0F;
	GPIOA->CRL |= 0x03;
	GPIOA->ODR |= 0x01;
	
	RCC->APB2ENR |=0x01<<4;
	GPIOC->CRH |= 0x03<<20; // pb1 output
	GPIOC->ODR |= 0x01<<13;	// reset
	
	
}


int main()
{
	delay_ms_init();
	uart_config(usart1,115200);
	gpio_config();
	
	while(1)
	{
		uart_sendString("lethanhtra\r\n");
		delay_ms(100);
		/*char t = uart_receiveChar();
		if(t==49)
			GPIOC->ODR |= 0x01<<13;
		//delay_ms(500);
		else 
			GPIOC->ODR &= ~0x01<<13;*/
		//delay_ms(500);*/
	}
}

