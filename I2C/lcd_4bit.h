#ifndef LCD_4BIT_H
#define LCD_4BIT_H

#include "stm32f10x.h"
#include "string.h"
#include "stdio.h"

#define PORTA 2
#define PORTB 3
#define PORTC 4
#define PORTD 5

//uint8_t flag;
// RS EN a8 a9 ;RW low to write;  D4 D5 D6 D7 output a0123
void lcd_config_gpio()
{
	RCC->APB2ENR |= 0x01 << PORTA;
	GPIOA->CRL &= ~(0xFFFF); // clear bit 0-31;
	GPIOA->CRL |= 0x3333;	//50mhz  a0
	GPIOA->ODR &= ~0x0F;	// reset
	// config RS EN
	GPIOA->CRH &= ~0x33; // clear bit 0-31;
	GPIOA->CRH |= 0x33;	//50mhz  a8 a9
	GPIOA->ODR &= ~(0x3<<8);	// reset a8a9
	
	  
}

void delay(uint32_t time)
{
	while(time--);
}
void lcd_cmd(uint8_t cmd)
{
	uint8_t high_bit = cmd&0xF0;
	uint16_t low_bit = cmd&0x0F;
	// send 4 bit higher
	// RS EN: b0 b1
	GPIOA->ODR &= ~(0x01<<8);		// cmd mode
	GPIOA->ODR &= ~0x0F;  // clear
	GPIOA->ODR |= high_bit >>4;
	GPIOA->ODR |= 0x01<<9;	//  en
	delay(1000);
	GPIOA->ODR &= ~(0x01<<9);// unable
	delay(1000);
	// send 4 bit lower
	GPIOA->ODR &= ~0x0F;  // clear
	GPIOA->ODR |= low_bit;
	GPIOA->ODR |= 0x01<<9;	//  en
	delay(1000);
	GPIOA->ODR &= ~(0x01<<9);// unable
	delay(1000);
}
void lcd_data(uint8_t data)
{
	uint8_t high_bit = data&0xF0;
	uint16_t low_bit = data&0x0F;
	// send 4 bit higher
	GPIOA->ODR |= 0x01<<8;		// data mode
	GPIOA->ODR &= ~0x0F;  // clear
	GPIOA->ODR |= high_bit >>4;
	GPIOA->ODR |= 0x01<<9;	//  en
	delay(1000);
	GPIOA->ODR &= ~(0x01<<9);// unable
	delay(1000);
	// send 4 bit lower
	GPIOA->ODR &= ~0x0F;  // clear
	GPIOA->ODR |= low_bit;
	GPIOA->ODR |= 0x01<<9;	//  enable
	delay(1000);
	GPIOA->ODR &= ~(0x01<<9);// unable
	delay(1000);
}
void lcd_init()
{
	lcd_config_gpio();
	lcd_cmd(0x02);	// 4 bit
	lcd_cmd(0x28);	// 4 bit
	lcd_cmd(0x0C);
	lcd_cmd(0x80);
}
void lcd_setcursor(uint8_t row,uint8_t col)
{
	if(row == 0)
	{
		lcd_cmd(0x80+col);
	}
	else{
		lcd_cmd(0xC0+col);
	}
}
void lcd_string(unsigned char *str)
{
	for(int i=0; i<strlen(str); i++)
	{
		lcd_data(str[i]);
	}
}
void lcd_number(int num)
{
	unsigned char* num_str;
	sprintf(num_str,"%d",num);
	lcd_string(num_str);
}

#endif