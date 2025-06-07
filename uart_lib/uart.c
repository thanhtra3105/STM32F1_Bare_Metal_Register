#include "uart.h"

USART_TypeDef *USARTx;
void uart_config(uint8_t usart_gate ,uint32_t baud)
{
	
	/*-------------------- USART1----------------------*/
	
	//config clock
	if(usart_gate == usart1)
	{
		RCC->APB2ENR |=0x01<<14;	// UART1
		RCC->APB2ENR |= 0x01;	// bat chuc nang thay the Alternate function
		RCC->APB2ENR |= 0x01<<2;	// GPIOA
		
		GPIOA->CRH &= ~(0x0F<<4);
		GPIOA->CRH |= 0x02<<6;	// TX PA9 AF push-pull 
		GPIOA->CRH |= 0x03<<4;	// max speed
		
		GPIOA->CRH &= ~(0x0F<<8);
		GPIOA->CRH |= 0x08<<8;	// RX PA10 input pullup
		// config uart 1
		USART1->CR1 = 0x00;	// clear all
		USART1->CR1 |= 0x01<<13;	// enable uart
		USART1->CR1 &= ~0x01<<12;	// 0: 8 bit data
		// stop bit = 0; 
		// 72MHz / (16*115200) = 39.0625	=> 39 16*0.0625=1	
		if(baud==9600)
			USART1->BRR = 0x1D4C;
		else if(baud==115200)
			USART1->BRR = 0x271;	// Correct BRR value for 9600 baud	
		USART1->CR1 |= 0x01<<3;		//TX enable
		USART1->CR1 |= 0x01 <<2;  // RX Enable
		USARTx = USART1;
	}
	/*-----------------USART2-------------------*/
	else if(usart_gate == usart2)
	{
		//config clock
		RCC->APB1ENR |=0x01<<17;	// USART2
		RCC->APB2ENR |= 0x01;	// bat chuc nang thay the Alternate function
		RCC->APB2ENR |= 0x01<<2;	// GPIOA
		
		GPIOA->CRL &= ~(0x0F<<8);
		GPIOA->CRL |= 0x02<<10;	// TX PA2 AF push-pull 
		GPIOA->CRL |= 0x03<<8;	// max speed
		 
		GPIOA->CRL &= ~(0x0F<<12);
		GPIOA->CRL |= 0x08<<12;	// RX PA3 input pullup
		GPIOA->ODR |=0x01<<3;	// pull-up
		// config uart 2
		USART2->CR1 = 0x00;	// clear all
		USART2->CR1 |= 0x01<<13;	// enable uart
		USART2->CR1 &= ~0x01<<12;	// 0: 8 bit data
		// stop bit = 0; 
		// 36MHz / (16*115200) 
		if(baud==9600)
			USART2->BRR = 0xEA6;
		else if(baud==115200)
			USART2->BRR = 0x138;	// Correct BRR value for 9600 baud	
		USART2->CR1 |= 0x01<<3;		//TX enable
		USART2->CR1 |= 0x01 <<2;  // RX Enable
		
		USARTx = USART2;
}
	else if(usart_gate == usart3)
	{
	/*-----------------USART3-------------------*/
		//config clock
		RCC->APB1ENR |=0x01<<18;	// USART3
		RCC->APB2ENR |= 0x01;	// bat chuc nang thay the Alternate function
		RCC->APB2ENR |= 0x01<<3;	// GPIOB
		
		GPIOB->CRH &= ~(0x0F<<8);
		GPIOB->CRH |= 0x02<<10;	// TX PB10 AF push-pull 
		GPIOB->CRH |= 0x03<<8;	// max speed
		GPIOB->CRH &= ~(0x0F<<12);
		GPIOB->CRH |= 0x08<<12;	// RX PB11 input pullup
		
		// config uart 2
		USART3->CR1 = 0x00;	// clear all
		USART3->CR1 |= 0x01<<13;	// enable uart
		USART3->CR1 &= ~0x01<<12;	// 0: 8 bit data
		// stop bit = 0; 
		// 36MHz / (16*115200) 
		if(baud==9600)
			USART3->BRR = 0xEA6;
		else if(baud==115200)
			USART3->BRR = 0x271;	// Correct BRR value for 9600 baud	
		USART3->CR1 |= 0x01<<3;		//TX enable
		USART3->CR1 |= 0x01 <<2;  // RX Enable
		
		USARTx = USART3;
	}	
}

void uart_sendChar(unsigned char data)
{
	USARTx->DR = data;
	while(!(USARTx->SR & (0x01<<6)));	// wait bit TC set(transmision complete)
	
}
void uart_sendString(unsigned char* data)
{
	for(int i=0; data[i]!='\0'; i++)
	{
		uart_sendChar(data[i]);
	}
	/*while(*data)
	{
		uart_sendChar(*data++);
	}*/
}

void uart_sendNumber(int num)
{
	unsigned char *buffer[12];
	sprintf(buffer, "%d", num);
	uart_sendString(buffer);
}

unsigned char uart_receiveChar()
{
	while(!(USARTx->SR & (0x01<<5)));	// wait until bit RXNE set
	return USARTx->DR;
}

