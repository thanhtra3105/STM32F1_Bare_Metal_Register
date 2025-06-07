#include "stm32f10x.h"
#include "delay_ms.h"

void CAN_init()
{
	RCC->APB1ENR |= 0x01<<25; 	// CAN1 enable	// fmax = 36mhz
	// CANTX: PA12 AF pushpull
	// CANRX: PA11 input floating  | input pull-up
	RCC->APB2ENR |= 0x02; //ADC1 gpioA
	GPIOA->CRH &= ~(0xFF<<12);
	GPIOA->CRH |= 0x0B<<16;
	GPIOA->CRH |= 0x08<<12;
	// initial CAN
	CAN1->MCR |= 0x01;	// set INRQ to init CAN
	while(!(CAN1->MSR & 0x01));// wait bit INAK set mean can is inited
	// baudrate:  Baudrate= APBclock/[Prescaler×(TSEG1+TSEG2+1)]
	// baudrate = 500k baud
	CAN1->BTR = (0x09)|(0x04<<16)|(0x04<<20);
	CAN1->MCR &= ~0x01;	// clear bit INRQ to switch normal mode
	while(CAN1->MSR&0x01);	//left the init mode
	
}
void CAN_transmit(uint32_t ID, uint8_t data)	// 1 byte data
{
	CAN1->sTxMailBox[0].TIR &= ~(0x1<<2);	// 11 bit ID standard
	CAN1->sTxMailBox[0].TIR |= ID<<21;	// ID: 0x10
	CAN1->sTxMailBox[0].TDTR |= 0x01;	// data frame is 1 byte
	CAN1->sTxMailBox[0].TDLR = data;	// byte 1 = 48(dec) = 'a'(char)
	CAN1->sTxMailBox[0].TIR |= 0x01;	//set TXRQ to start transmit
	while(!(CAN1->TSR&(0x01<<1)));	// if TXOK0 = 1 -> success transmit
}
uint32_t CAN_receive()
{
	uint32_t ID;
	uint8_t data;
	
	if(CAN1->RF0R&(0x01<<3))
	{
		ID = CAN1->sFIFOMailBox[0].RIR&0x7FF;	// ID receiver
		data = CAN1->sFIFOMailBox[0].RDTR &0xFF;
		CAN1->RF0R |= 0x01<<5;	// release FIFO 0
	}
	
	return data;	// return data 1 byte
	
}
int main()
{
	delay_ms_init();
	CAN_init();
	while(1)
	{
		CAN_transmit(0x100,'a');
		delay_ms(100);
	}
}
