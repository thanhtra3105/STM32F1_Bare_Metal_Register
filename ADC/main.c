#include "stm32f10x.h"
#include "lcd.h"
#include "uart.h"
#include "delay_ms.h"
void ADC1_config()
{
	RCC->APB2ENR |=0x01<<9;	// adc1
	RCC->APB2ENR |=0x01<<3;	// PB
	RCC->CFGR |= 0x03<<14;	// ADC prescale /8
	GPIOB->CRL &=~0x0F;	// pb0: 0000 input analog mode
	//GPIOB->CRL |= 0x08;	// pb0 input pull up
	ADC1->SMPR2 |= 0x07<<24;	// chanel 8 235.5 cycle
	ADC1->SQR3 |= 0x08; // set chanel 8 in sequence 1
	ADC1->CR2 |=0x01; // ON adc first time
	ADC1->CR2 |= 0x01<<1; // Continuous conversion mode
	ADC1->CR2 |= 0x01 <<22;	// Start conversion of regular channels
	delay(1);
	ADC1->CR2 |=0x01; // ON adc second time
	delay(1);
	ADC1->CR2 |= 0x01<<2;	// enable calibration
	delay_ms(2);
	
	
	
}
void gpio_config()
{
	RCC->APB2ENR |=0x01<<4;
	GPIOC->CRH |= 0x03<<20; // pb1 output
	GPIOC->ODR |= 0x01<<13;	// reset
}

uint16_t adc_read()
{
	//ADC1->CR2 |=0x01;
	while (!(ADC1->SR & ADC_SR_EOC));  // Ki?m tra dúng bit EOC

	return ADC1->DR;
}

uint16_t get_temp() {
    uint16_t adc_value = adc_read();  
    float V_sense = (adc_value * 3.3) / 4096.0; 
    uint16_t temperature = ((V_sense - 1.43) / 0.0043) +25;  
    return temperature;
}

int main()
{
	//lcd_init();
	delay_ms_init();
	ADC1_config();
	gpio_config();
	uart_config(usart1,9600);
	int i=0;
	while(1)
	{
		//uint16_t temp = 0;
		//temp = (int)get_temp();
		
		uint16_t adc = adc_read();
		uart_sendNumber(adc);
		uart_sendChar('\n');
		delay_ms(300);
		if(adc>500)
			GPIOC->ODR |= 0x01<<13;
		else
			GPIOC->ODR &= ~0x01<<13;
		/*GPIOC->ODR |= 0x01<<13;
		delay_ms(500);
		GPIOC->ODR &= ~0x01<<13;
		delay_ms(500);*/
	}
}



