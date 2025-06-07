#ifndef I2C_H
#define I2C_H

#include "stm32f10x.h"

void i2c_config()
{
    RCC->APB1ENR |= 0x01 << 21; // B?t I2C1
    RCC->APB2ENR |= 0x01 << 3;  // B?t GPIOB

    // C?u hình PB6 (SCL) và PB7 (SDA) ch? d? Alternate Function Open-Drain
    GPIOB->CRL &= ~(0xFF << 24);  
    GPIOB->CRL |= (0xFF << 24) | (0xFF << 28); 

    I2C1->CR1 |= 0x01 << 15; // Reset I2C
    I2C1->CR1 &= ~(0x01 << 15);

    I2C1->CR2 = 36;   // PCLK1 = 36MHz
    I2C1->CCR = 180;  // 100kHz
    I2C1->TRISE = 37; // Max rise time

    I2C1->CR1 |= 0x01; // Enable I2C
}

void i2c_start()
{
    I2C1->CR1 |= 0x01 << 10; // Enable ACK
    I2C1->CR1 |= 0x01 << 8;  // Start condition
    while (!(I2C1->SR1 & 0x01)); // Ch? bit SB
}

void i2c_write(uint8_t data)
{
    while (!(I2C1->SR1 & (0x01 << 7))); 
    I2C1->DR = data;
    while (!(I2C1->SR1 & (0x01 << 2))); 
}
uint16_t i2c_read(uint8_t ack)
{
	if(ack)
    I2C1->CR1 |= I2C_CR1_ACK;
else
    I2C1->CR1 &= ~I2C_CR1_ACK;
	while(!(I2C1->SR1 & (0x01<<6)));
	return I2C1->DR;
}
void i2c_address(uint8_t address)
{
    I2C1->DR = address;
    while (!(I2C1->SR1 & (0x01 << 1))); 
    uint8_t temp = I2C1->SR1 | I2C1->SR2; 
}

void i2c_stop()
{
    I2C1->CR1 |= 0x01 << 9; // Stop condition
}

void i2c_slave_config(uint8_t i2c_slave_address)
{
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

    // PB6 - SCL, PB7 - SDA (AF Open Drain)
    GPIOB->CRL &= ~((0xF << 24) | (0xF << 28));
    GPIOB->CRL |= ((0xB << 24) | (0xB << 28)); // Mode 50MHz, CNF = AF_OD

    // Reset I2C1
    I2C1->CR1 |= I2C_CR1_SWRST;
    I2C1->CR1 &= ~I2C_CR1_SWRST;

    I2C1->CR2 = 36;               // PCLK1 = 36MHz
    I2C1->OAR1 = (i2c_slave_address << 1);  
    I2C1->OAR1 |= I2C_OAR1_ADDMODE; // 7-bit

    I2C1->CR1 |= I2C_CR1_ACK;     // Enable ACK
    I2C1->CR1 |= I2C_CR1_PE;      // Enable I2C
}


uint8_t i2c_slave_receive()
{
    while (!(I2C1->SR1 & (0x01<<1))); // waiting set
    volatile uint32_t temp = I2C1->SR1 | I2C1->SR2; // Clear ADDR flag

    while (!(I2C1->SR1 & I2C_SR1_RXNE)); // Ch? có d? li?u
    return I2C1->DR;
}
#endif