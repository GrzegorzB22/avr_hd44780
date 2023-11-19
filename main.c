
#include <avr/io.h>
#include "lcd.h"


int main(void)
{

	LCD_Init();
	LCD_SetCursorXY(0, 0);
	LCD_WriteString("HD44780");
	LCD_Update();

	while (1);

	return 0;
}
