#ifndef UART_H
#define UART_H

#include "stm32f10x.h"
#include <stdio.h>
#define usart1 1
#define usart2 2
#define usart3 3
extern USART_TypeDef *USARTx;

void uart_config(uint8_t usart_gate ,uint32_t baud);
void uart_sendString(unsigned char* data);
void uart_sendChar(unsigned char data);
void uart_sendNumber(int num);
unsigned char uart_receiveChar();

#endif