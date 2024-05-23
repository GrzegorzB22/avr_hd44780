
#include "pin_defines.h"
#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>
#include "lcd.h"

static hd44780 lcd;

static void lcd_data_in(void)
{
	LCD_CLEAR_DDR(LCD_DB4_PIN);
	LCD_CLEAR_DDR(LCD_DB5_PIN);
	LCD_CLEAR_DDR(LCD_DB6_PIN);
	LCD_CLEAR_DDR(LCD_DB7_PIN);

	LCD_SET_PORT(LCD_DB4_PIN);
	LCD_SET_PORT(LCD_DB5_PIN);
	LCD_SET_PORT(LCD_DB6_PIN);
	LCD_SET_PORT(LCD_DB7_PIN);
}

static void lcd_data_out(void)
{
	LCD_SET_DDR(LCD_DB4_PIN);
	LCD_SET_DDR(LCD_DB5_PIN);
	LCD_SET_DDR(LCD_DB6_PIN);
	LCD_SET_DDR(LCD_DB7_PIN);
}

static void lcd_write4bits(uint8_t bits)
{
	if (bits & 0x01)
		LCD_SET_PORT(LCD_DB4_PIN);
	else			
		LCD_CLEAR_PORT(LCD_DB4_PIN);

	if (bits & 0x02)	
		LCD_SET_PORT(LCD_DB5_PIN);
	else			
		LCD_CLEAR_PORT(LCD_DB5_PIN);

	if (bits & 0x04)	
		LCD_SET_PORT(LCD_DB6_PIN);
	else			
		LCD_CLEAR_PORT(LCD_DB6_PIN);

	if (bits & 0x08)	
		LCD_SET_PORT(LCD_DB7_PIN);
	else		
		LCD_CLEAR_PORT(LCD_DB7_PIN);
}

static uint8_t lcd_read4bits(void)
{
	uint8_t bits = 0;

	if (LCD_READ_PIN(LCD_DB4_PIN))	bits |= 0x01;
	if (LCD_READ_PIN(LCD_DB5_PIN))	bits |= 0x02;
	if (LCD_READ_PIN(LCD_DB6_PIN))	bits |= 0x04;
	if (LCD_READ_PIN(LCD_DB7_PIN))	bits |= 0x08;

	return bits;
}

static uint8_t lcd_readByte(void)
{
	uint8_t byte = 0;

	LCD_SET_PORT(LCD_RW_PIN);
	lcd_data_in();

	LCD_SET_PORT(LCD_E_PIN);
	byte = (lcd_read4bits() << 4);
	LCD_CLEAR_PORT(LCD_E_PIN);

	LCD_SET_PORT(LCD_E_PIN);
	byte |= lcd_read4bits();
	LCD_CLEAR_PORT(LCD_E_PIN);

	return byte;
}

static uint8_t lcd_readBF(void)
{
	LCD_CLEAR_PORT(LCD_RS_PIN);
	return lcd_readByte();
}

static bool lcd_busy(void)
{
	return (lcd_readBF() & LCD_BUSY_FLAG);
}

static void lcd_writeByte(uint8_t byte)
{
	LCD_CLEAR_PORT(LCD_RW_PIN);
	lcd_data_out();

	LCD_SET_PORT(LCD_E_PIN);
	lcd_write4bits(byte >> 4);
	LCD_CLEAR_PORT(LCD_E_PIN);

	LCD_SET_PORT(LCD_E_PIN);
	lcd_write4bits(byte);
	LCD_CLEAR_PORT(LCD_E_PIN);

	while (lcd_busy());
}

static void lcd_writeData(uint8_t data)
{
	LCD_SET_PORT(LCD_RS_PIN);
	lcd_writeByte(data);
}

static void lcd_writeCommand(uint8_t command)
{
	LCD_CLEAR_PORT(LCD_RS_PIN);
	lcd_writeByte(command);
}

void LCD_ClearBuffer(void)
{
	lcd.x = lcd.y = 0;

	for (uint8_t y = 0; y < LCD_HEIGHT; y++)
		for (uint8_t x = 0; x < LCD_WIDTH; x++)
			lcd.screen_buffer[y][x] = ' ';
}

void LCD_WriteChar(char c)
{
	lcd.screen_buffer[lcd.y][lcd.x++] = c;

	if (lcd.x == LCD_WIDTH) {
		lcd.x = 0;
		lcd.y++;
		if (lcd.y == LCD_HEIGHT)
			lcd.y = 0;
	}
}

void LCD_WriteString(char *s)
{
	while (*s)
		LCD_WriteChar(*s++);
}

void LCD_SetCursorXY(uint8_t x, uint8_t y)
{
	lcd.x = x % LCD_WIDTH;
	lcd.y = y % LCD_HEIGHT;
}

void LCD_Update(void)
{
	lcd_writeCommand(0x80);
	for (uint8_t i = 0; i < LCD_WIDTH; i++)
		lcd_writeData(lcd.screen_buffer[0][i]);

	lcd_writeCommand(0x80 | 0x40);
	for (uint8_t i = 0; i < LCD_WIDTH; i++)
		lcd_writeData(lcd.screen_buffer[1][i]);
}

void LCD_Init(void)
{
	LCD_SET_DDR(LCD_RS_PIN);
	LCD_SET_DDR(LCD_RW_PIN);
	LCD_SET_DDR(LCD_E_PIN);

	LCD_CLEAR_PORT(LCD_RS_PIN);
	LCD_CLEAR_PORT(LCD_RW_PIN);
	LCD_CLEAR_PORT(LCD_E_PIN);

	lcd_data_out();

	_delay_ms(100);

	for (uint8_t i = 0; i < 3; i++) {
		LCD_SET_PORT(LCD_E_PIN);
		lcd_write4bits(0x03);
		LCD_CLEAR_PORT(LCD_E_PIN);
		_delay_ms(5);
	}

	LCD_SET_PORT(LCD_E_PIN);
	lcd_write4bits(0x02);
	LCD_CLEAR_PORT(LCD_E_PIN);
	_delay_us(150);

	lcd_writeCommand(LCD_FUNCTION);
	lcd_writeCommand(0x08);
	lcd_writeCommand(0x01);
	lcd_writeCommand(LCD_ENTRY);
	lcd_writeCommand(LCD_DISPLAY);
	lcd_writeCommand(LCD_CLEAR);
	lcd_writeCommand(LCD_HOME);
	LCD_ClearBuffer();
}
