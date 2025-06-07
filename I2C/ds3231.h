#ifndef DS3231_H
#define DS3231_H

#include "i2c.h"
#define DS3231_ADDRESS 0x68
typedef struct{
	uint8_t second;
	uint8_t minute;
	uint8_t hour;
	uint8_t day;
	uint8_t month;
	uint8_t year;
	uint8_t weekDay;
} DS3231_t;

DS3231_t time;

uint8_t bcd2dec(uint8_t bcd)
{
	return ((bcd>>4)*10)+(bcd&0x0F);
}
uint8_t dec2bcd(uint8_t dec)
{
	return ((dec/10)<<4)+(dec%10);
}
void ds3231_init()
{
	i2c_config();
}
void ds3231_read()
{
	i2c_start();
	i2c_address(DS3231_ADDRESS<<1);	// write mode
	i2c_write(0x00);	// 
	i2c_stop();
	i2c_start();
	i2c_address(0xD1);	//read mode
	time.second = bcd2dec(i2c_read(1));
	time.minute = bcd2dec(i2c_read(1));
	time.hour = bcd2dec(i2c_read(1));
	time.weekDay = bcd2dec(i2c_read(1));
	time.day = bcd2dec(i2c_read(1));
	time.month = bcd2dec(i2c_read(1));
	time.year = bcd2dec(i2c_read(0));
	i2c_stop();
}
void ds3231_setTime(uint8_t hour, uint8_t minute, uint8_t second, uint8_t weekDay, uint8_t day, uint8_t month, uint8_t year)
{
	uint8_t buffer[8] = 
	{
		0x00,
		dec2bcd(second),
		dec2bcd(minute),
		dec2bcd(hour),
		dec2bcd(weekDay),
		dec2bcd(day),
		dec2bcd(month),
		dec2bcd(year)
	};
	uint8_t i=0;
	i2c_start();
	i2c_address(DS3231_ADDRESS<<1);
	for(i=0;i<8;i++)
	{
		i2c_write(buffer[i]);
	}
	i2c_stop();
}



#endif