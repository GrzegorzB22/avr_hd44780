
#ifndef LCD_H
#define LCD_H

#include <avr/io.h>

/*------------------ BITY KONFIGURACYJNE WYSWIETLACZA --------------*/

// 1 - kursor przesywa sie w prawo
// 0 - kursor przesuwa sie w lewo
#define LCD_ID_CONFIG	1

// 1 - przesuwanie calej zawartosci w prawo (bit ID = 0) lub w lewo (bit ID = 1)
// 0 - nieprzesuwanie zawartosci wyswietlacza
#define LCD_S_CONFIG	0

// 1 - kursor na wyswietlaczu miga
// 0 - kursor na wyswietlaczu nie miga
#define LCD_B_CONFIG	0

// 1 - kursor widoczny
// 0 - kursor wylaczony
#define LCD_C_CONFIG	1

// 1 - rozmiar znaku: 5x10
// 0 - rozmiar znaku: 5x8
#define LCD_F_CONFIG	0

// 1 - liczba wierszy: 2
// 0 - liczba wierszy: 1
#define LCD_N_CONFIG	1

// 1 - ramka danych, interfejs 8-bitowy
// 0 - interfejs 4-bitowy
#define LCD_DL_CONFIG	0

// 1 - wyswietlacz wlaczony
// 0 - wyswietlacz wylaczony
#define LCD_D_CONFIG	1

/*-----------------------------------------------------------------*/

#define LCD_CLEAR	0x01
#define LCD_HOME	0x02
#define LCD_ENTRY	0x04 | (LCD_ID_CONFIG << 1) | (LCD_S_CONFIG << 0)
#define LCD_DISPLAY	0x08 | (LCD_D_CONFIG << 2)  | (LCD_C_CONFIG << 1) | (LCD_B_CONFIG << 0)
#define LCD_FUNCTION	0x20 | (LCD_DL_CONFIG << 4) | (LCD_N_CONFIG << 3) | (LCD_F_CONFIG << 2)

#define LCD_BUSY_FLAG	0x80

#define LCD_WIDTH	16
#define LCD_HEIGHT	2

#define LCD_DATA	0
#define LCD_COMMAND	1

typedef struct {
	uint8_t x;
	uint8_t y;
	uint8_t screen_buffer[LCD_HEIGHT][LCD_WIDTH];
} hd44780;


void LCD_Init(void);
void LCD_ClearBuffer(void);
void LCD_WriteChar(char c);
void LCD_WriteString(char *s);
void LCD_SetCursorXY(uint8_t x, uint8_t y);
void LCD_Update(void);



#endif
