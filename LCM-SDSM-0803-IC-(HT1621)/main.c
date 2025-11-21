#include "ht1621.h"
extern unsigned int i;

void main()
{
	while(1)
	{	
	lcd_init();  
	delay(500);
	lcd_clear();		
	lcd_all();
	delay(3000);

	for (i = 0; i <= 9; i++) 
	{
		display_digit(i, 1); // 显示数字 i 的 1dot 布局
		display_digit(i, 2); // 显示数字 i 的 2dot 布局
		delay(3000);
	}
	
	}	
}
