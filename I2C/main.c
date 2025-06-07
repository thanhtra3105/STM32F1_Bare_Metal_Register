#include "stm32f10x.h"
#include "stdio.h"
#include "ds3231.h"
#include "lcd_i2c.h"
#include "delay_ms.h"
#include "uart_lib/uart.h"

extern DS3231_t time;
int main()
{
	/*
	ds3231_init();
	delay_ms_init();
	uart_config(usart1,115200);
	lcd_init(1);
	int i=0;
	uint8_t data[30];
	*/
	//ds3231_setTime(22,27,0,1,11,5,25);
	i2c_slave_config(0x08);
	uint8_t data=0;
	while(1)
	{
		data = i2c_slave_receive();
		//delay_ms(500);
		/*
		ds3231_read();
		sprintf(data,"%d,%d:%d:%d,%d/%d/%d\r\n",time.weekDay,time.hour,time.minute,time.second,time.day,time.month,time.year+2000);
		uart_sendString(data);
		lcd_setCursor(0,0);
		lcd_string("Time");
		lcd_setCursor(0,4);
		lcd_number(time.hour);
		lcd_setCursor(0,6);
		lcd_string(":");
		lcd_setCursor(0,7);
		lcd_number(time.minute);
		lcd_setCursor(0,9);
		lcd_string(":");
		lcd_setCursor(0,10);
		lcd_number(time.second);
		
		delay_ms(1000);
		lcd_cmd(0x01);// clear
		*/
	}
}
