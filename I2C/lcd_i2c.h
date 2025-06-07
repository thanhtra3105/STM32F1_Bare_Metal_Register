#ifndef LCD_I2C_H
#define LCD_I2C_H

#include "i2c.h"
#include "stdio.h"

#define PCF8574_ADDR (0x27 << 1) // address pcf8574 0x4e
// pin lcd in pcf8574
#define LCD_RS  0x01  // P0
#define LCD_RW  0x02  // P1
#define LCD_EN  0x04  // P2
#define LCD_BACKLIGHT 0x08 // P3 (Backlight)

uint8_t lcd_backlight;
void lcd_send(uint8_t data, uint8_t mode)
{
    uint8_t high_nibble = (data & 0xF0) | mode | lcd_backlight;
    uint8_t low_nibble = ((data << 4) & 0xF0) | mode | lcd_backlight;

    i2c_start();
    i2c_address(PCF8574_ADDR);
    i2c_write(high_nibble | LCD_EN);
    i2c_write(high_nibble);
    i2c_write(low_nibble | LCD_EN);
    i2c_write(low_nibble);
    i2c_stop();
}

// ======== Các hàm LCD ========
void lcd_cmd(uint8_t cmd)
{
    lcd_send(cmd, 0);
}

void lcd_data(uint8_t data)
{
    lcd_send(data, 1);
}

void lcd_init(uint8_t back_light)	// turn on back light 1, turn off 0
{
		if(back_light)
		{
			lcd_backlight = LCD_BACKLIGHT;
		}
		else
			lcd_backlight = 0x00;
		i2c_config();
    lcd_cmd(0x32); 	// set 4 bit mode
    lcd_cmd(0x28); 	// set 4 bit mode
    lcd_cmd(0x0C); 	// lcd on cursor off
    lcd_cmd(0x01); 	// clear
}

void lcd_setCursor(uint8_t row, uint8_t col)
{
    uint8_t address = (row == 0) ? 0x80 : 0xC0;
    lcd_cmd(address + col);
}

void lcd_string(char *str)
{
    while (*str)
    {
        lcd_data(*str++);
    }
}

void lcd_number(int num)
{
    char buffer[10];
    sprintf(buffer, "%d", num);
    lcd_string(buffer);
}


#endif