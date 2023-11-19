
#ifndef PIN_DEFINES_H
#define PIN_DEFINES_H

#define CONCATENATE(x, y)	x ## y

#define SETx(dp, dp_name, dp_pin_number)	CONCATENATE(dp, dp_name) |= _BV(dp_pin_number)
#define CLEARx(dp, dp_name, dp_pin_number)	CONCATENATE(dp, dp_name) &= ~_BV(dp_pin_number)
#define READx(dp, dp_name, dp_pin_number)	CONCATENATE(dp, dp_name) & _BV(dp_pin_number)


#define LCD_DB4_PIN	D,3
#define LCD_DB5_PIN	D,4
#define LCD_DB6_PIN	D,5
#define LCD_DB7_PIN	D,7

#define LCD_RS_PIN	D,0
#define LCD_RW_PIN	D,1
#define LCD_E_PIN	D,2

#define LCD_SET_DDR(x)		SETx(DDR, x)
#define LCD_CLEAR_DDR(x)	CLEARx(DDR, x)

#define LCD_SET_PORT(x)		SETx(PORT, x)
#define LCD_CLEAR_PORT(x)	CLEARx(PORT, x)

#define LCD_READ_PIN(x)		READx(PIN, x)


#endif

