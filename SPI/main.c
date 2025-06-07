
#include "stm32f10x.h"
#include "delay_ms.h"
#include "string.h"
#include "uart.h"
#include "stdio.h"
uint8_t tx_master[] = "lethanhtra\r\n";
uint8_t rx_slave[1] = {0};
uint8_t rx_buffer[20] = {0};
int idx= 0;
uint8_t *rx_data;
uint8_t flag = 0;

void spi1_config(void)
{
    // Enable clocks
	/*********** CONFIG SPI 1 MASTER ***************
		1. Enable SPI clock and Enable PORT A Alternate Function
		2. Configure the Control Register 1
		3. Configure the CR2
	************************************************/
		RCC->APB2ENR |= 0x01<<12; 	// SPI1 enable
		RCC->APB2ENR |= 0x01<<2;	// port A enable
		RCC->APB2ENR |= 0x01;		// AF enable

		// Master MODE: SCK: af push pull, mosi;af push pull, miso: input floating, NSS: GPIO Output
		RCC->APB2ENR |= 0x01;				// enable alternate function
		GPIOA->CRL &= ~(0x0f<<20);	// SCK pa5
		GPIOA->CRL |= 0x0B<<20;		// output max speed and AF push-pull
		GPIOA->CRL &= ~(0x0f<<28);	// MOSI pa7
		GPIOA->CRL |= 0x0B<<28;		// output max speed and AF push-pull
		GPIOA->CRL &= ~(0x0f<<24);	// MISO pa6
		GPIOA->CRL |= 0x04<<24;		// input floating
		GPIOA->CRL |= 0x07<<16;		// Output max speed

		SPI1->CR1 |= 0x03<<3;		// Fclk/16
		SPI1->CR1 |= 0x01;			// CPHA = 1
		SPI1->CR1 |= 0x01<<1;		// CPOL = 1
		SPI1->CR1 &= ~(0x01<<10);	// RxOnly full duplex
		SPI1->CR1 |= 0x01<<9;		// SSM
		SPI1->CR1 |= 0x01<<8;		// SSI
		SPI1->CR1 &= ~(0x01<<11);	// 8 bit frame
		SPI1->CR1 |= 0x01<<2;		// enable master
		SPI1->CR1 |= 0x01<<6;		// spi enable
}

void spi2_config()
{
	/* config SPI 2 for Slave */
		RCC->APB1ENR |= 0x01<<14; 	// SPI 2 enable
		RCC->APB2ENR |= 0x01<<3;		// port B enable
		RCC->APB2ENR |= 0x01;	// enable alternate function
		// Slave MODE: SCK: input floating, mosi: input floating/pullup, miso: AF push-pull(point to point), NSS: GPIO Output
		GPIOB->CRH &= ~(0x0f<<20);	// SCK PB13
		GPIOB->CRH |= 0x04<<20;		// input floating
		GPIOB->CRH &= ~(0x0f<<28);	// mosi PB15
		GPIOB->CRH |= 0x04<<28;			// input floating
		GPIOB->CRH &= ~(0x0f<<24);	// miso PB14
		GPIOB->CRH |= 0x0B<<24;			// output max speed
		GPIOB->CRH &= ~(0x0f<<16);	// NSS PB12
		GPIOB->CRH |= 0x0B<<16;			// output max speed

		//SPI2->CR1 |= 0x02<<3;		// Fclk/8 // in salve mode don't care clock so it dependent sck of master
		SPI2->CR1 &= ~(0x01<<11);	// data frame 8bit
		SPI2->CR1 |= 0x01;			// cpha = 1
		SPI2->CR1 |= 0x01<<1;			// CPOL = 1
		SPI2->CR1 &= ~(0x01<<7); 	// MSB first
		SPI2->CR1 &= ~(0x01<<10);	// RxOnly full duplex
		SPI2->CR1 |= 0x01<<9;			// set SSM
		SPI2->CR1 &= ~(0x01<<8); 	// clear SSI
		SPI2->CR1 &= ~(0x01<<2);	// slave mode
		SPI2->CR2 |= 0x01<<6;		// enable interrupt
		SPI2->CR1 |= 0x01<<6;	// spi enable

		NVIC_EnableIRQ(SPI2_IRQn);    // Enable IRQ in NVIC for Interrupt
}
void spi1_master_transmit(uint8_t data)
{
	while(!(SPI1->SR & (0x01<<1)));	// wait tx set -> empty
	SPI1->DR = data;
	while(SPI1->SR &(0x01<<7));	// BSY retset -> not busy: not transmiting
}
void spi_transmit(uint8_t *data)
{
	uint8_t i=0;
	for(i=0;i<strlen(data);i++)
	{
		spi1_master_transmit(data[i]);
	}
}
uint16_t spi1_master_receive()
{
	while (((SPI1->SR)&(1<<7))) {};  // wait for BSY bit to Reset -> This will indicate that SPI is not busy in communication
	SPI1->DR = 0;  // send dummy data
	while (!((SPI1->SR) &(1<<0))){};  // Wait for RXNE to set -> This will indicate that the Rx buffer is not empty
	return SPI1->DR;
}
uint8_t spi2_slave_receive()		// read by Polling
{
	while(!(SPI2->SR & 0x01)); // wait RX set -> has data
	return SPI2->DR;		// read and clear bit RX flag
}


void SPI2_IRQHandler(void)		// Slave Receive with Interrupt
{
	if((SPI2->SR & 0x01)) // if RX set -> has data
	{
		rx_slave[0] = SPI2->DR;		// read and clear bit RX flag
		rx_buffer[idx++] = rx_slave[0];
		
		if(rx_slave[0] == '\n')
		{
			rx_buffer[idx] = '\0';
			flag = 1;
			idx = 0;
		}
	}
	
}
int main()
{
	delay_ms_init();
	uart_config(usart1,9600);
	spi1_config();
	spi2_config();
	
	while(1)
	{
		spi_transmit(tx_master);
		if(flag)
		{
			uart_sendString(rx_buffer);
			flag = 0;
		}
		//uart_sendString("tra");
		delay_ms(1000);
		//spi_transmit("len\r\n");
		//delay_ms(500);
		
	}
}

