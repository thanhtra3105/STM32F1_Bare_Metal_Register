#include "stm32f10x.h"
#include "string.h"
#include "stdio.h"

#define PORTA 2
#define PORTB 3
#define PORTC 4
#define PORTD 5

//uint8_t flag;
// RS RW EN D0 D1 D2 D3 D4 D5 D6 D7 output a01234567
void lcd_config_gpio()
{
	RCC->APB2ENR |= 0x01 << PORTA;
	GPIOA->CRL &= ~0x00; // clear bit 0-31;
	GPIOA->CRL |= 0x33333333;	//50mhz  a0
	GPIOA->ODR &= ~0xFF;	// reset
	// config RS EN
	//RCC->APB2ENR |= 0x01 << PORTB;
	GPIOA->CRH &= ~0x33; // clear bit 0-31;
	GPIOA->CRH |= 0x33;	//50mhz  b0 b1
	GPIOA->ODR &= ~0x3<<8;	// reset b0b1
	
	
}
void config_gpio_input()
{
	RCC->APB2ENR |= 0x01<<3; // portb
	GPIOB->CRL &= ~0x0F;
	GPIOB->CRL |= 0x01<<3;	// input b0
	GPIOB->ODR |= 0x01;	// pull up
}
void delay(uint32_t time)
{
	while(time--);
}
void lcd_cmd(uint8_t cmd)
{
	// RS EN: b0 b1
	GPIOA->ODR &= ~0x01<<8;		// cmd mode
	GPIOA->ODR |= 0x01<<9;	//  en
	GPIOA->ODR &= ~0xFF;  // clear
	GPIOA->ODR |= cmd;
	delay(1000);
	GPIOA->ODR &= ~(0x01<<9);// unable

}
void lcd_data(uint8_t data)
{
	GPIOA->ODR |= 0x01<<8;		// data mode
	GPIOA->ODR |= 0x01<<9;	//  en
	GPIOA->ODR &= ~0xFF;  // clear
	GPIOA->ODR |= data;
	delay(1000);
	GPIOA->ODR &= ~(0x01<<9);// unable
}
void lcd_init()
{
	lcd_config_gpio();
	lcd_cmd(0x0C);
	lcd_cmd(0x38);
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
void lcd_number(uint16_t num)
{
	unsigned char* num_str;
	sprintf(num_str,"%d",num);
	lcd_string(num_str);
}