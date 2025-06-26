#include "ht1622.h"

void main()
{

	lcd_init();  //initialization
	delay(50);
	while(1)
	{	
		display_sequence();
	}	
}
