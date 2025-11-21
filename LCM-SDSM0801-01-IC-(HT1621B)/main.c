#include "LCM823160.h"

unsigned char addr;
void main()
{
	  lcd_init();  //initialization
	  delay(5);
	  lcd_clear();  // Clear Screen

	while(1)
	{	
		lcd_clear();  // Clear Screen
		lcd_all();     //Show  all
		delay(2000);	   // Time delay
//	    display_digits_simple() ;
		lcd_clear();  // Clear Screen

		
		for (addr = 0x40; addr <= 0x51; addr += 2)//0
		{
			write_data(addr,     0x0D);  // ×ó×Ö½Ú
			write_data(addr + 1, 0x0F);  // ÓÒ×Ö½Ú
		}		
		for (addr = 0x52; addr <= 0x5f; addr += 2)//0
		{
			write_data(addr,     0x0F);  // ×ó×Ö½Ú
			write_data(addr + 1, 0x0D);  // ÓÒ×Ö½Ú
		}
		delay(8000);
		
		for (addr = 0x40; addr <= 0x51; addr += 2)//1
		{
			write_data(addr,     0x00);  // ×ó×Ö½Ú
			write_data(addr + 1, 0x0E);  // ÓÒ×Ö½Ú
		}
		for (addr = 0x52; addr <= 0x5f; addr += 2)//1
		{
			write_data(addr,     0x0E);  // ×ó×Ö½Ú
			write_data(addr + 1, 0x00);  // ÓÒ×Ö½Ú
		}
		delay(8000);			
		
		for (addr = 0x40; addr <= 0x51; addr += 2)//2
		{
			write_data(addr,     0x0E);  // ×ó×Ö½Ú
			write_data(addr + 1, 0x0B);  // ÓÒ×Ö½Ú
		}
		for (addr = 0x52; addr <= 0x5f; addr += 2)//2
		{
			write_data(addr,     0x0B);  // ×ó×Ö½Ú
			write_data(addr + 1, 0x0E);  // ÓÒ×Ö½Ú
		}
		delay(8000);		
		
		for (addr = 0x40; addr <= 0x51; addr += 2)//3
		{
			write_data(addr,     0x0A);  // ×ó×Ö½Ú
			write_data(addr + 1, 0x0F);  // ÓÒ×Ö½Ú
		}		
		for (addr = 0x52; addr <= 0x5f; addr += 2)//3
		{
			write_data(addr,     0x0F);  // ×ó×Ö½Ú
			write_data(addr + 1, 0x0A);  // ÓÒ×Ö½Ú
		}
		delay(8000);		


		for (addr = 0x40; addr <= 0x51; addr += 2)//4
		{
			write_data(addr,     0x03);  // ×ó×Ö½Ú
			write_data(addr + 1, 0x0E);  // ÓÒ×Ö½Ú
		}
		for (addr = 0x52; addr <= 0x5F; addr += 2)//4
		{
			write_data(addr,     0x0E);  // ×ó×Ö½Ú
			write_data(addr + 1, 0x03);  // ÓÒ×Ö½Ú
		}
		delay(8000);


		for (addr = 0x40; addr <= 0x51; addr += 2)//5
		{
			write_data(addr,     0x0B);  // ×ó×Ö½Ú
			write_data(addr + 1, 0x0D);  // ÓÒ×Ö½Ú
		}
		for (addr = 0x52; addr <= 0x5F; addr += 2)//5
		{
			write_data(addr,     0x0D);  // ×ó×Ö½Ú
			write_data(addr + 1, 0x0B);  // ÓÒ×Ö½Ú
		}
		delay(8000);

		for (addr = 0x40; addr <= 0x51; addr += 2)//6
		{
			write_data(addr,     0x0F);  // ×ó×Ö½Ú
			write_data(addr + 1, 0x0D);  // ÓÒ×Ö½Ú
		}
		for (addr = 0x52; addr <= 0x5F; addr += 2)//6
		{
			write_data(addr,     0x0D);  // ×ó×Ö½Ú
			write_data(addr + 1, 0x0F);  // ÓÒ×Ö½Ú
		}
		delay(8000);
				
	
		
		
		for (addr = 0x40; addr <= 0x51; addr += 2)//7
		{
			write_data(addr,     0x00);  // ×ó×Ö½Ú
			write_data(addr + 1, 0x0F);  // ÓÒ×Ö½Ú
		}
		for (addr = 0x52; addr <= 0x5f; addr += 2)//7
		{
			write_data(addr,     0x0F);  // ×ó×Ö½Ú
			write_data(addr + 1, 0x00);  // ÓÒ×Ö½Ú
		}
		delay(8000);


		for (addr = 0x40; addr <= 0x51; addr += 2)//8
		{
			write_data(addr,     0x0F);  // ×ó×Ö½Ú
			write_data(addr + 1, 0x0F);  // ÓÒ×Ö½Ú
		}
		for (addr = 0x52; addr <= 0x5F; addr += 2)//8
		{
			write_data(addr,     0x0F);  // ×ó×Ö½Ú
			write_data(addr + 1, 0x0F);  // ÓÒ×Ö½Ú
		}
		delay(8000);

		for (addr = 0x40; addr <= 0x51; addr += 2)//9
		{
			write_data(addr,     0x03);  // ×ó×Ö½Ú
			write_data(addr + 1, 0x0F);  // ÓÒ×Ö½Ú
		}		
		for (addr = 0x52; addr <= 0x5f; addr += 2)//9
		{
			write_data(addr,     0x0F);  // ×ó×Ö½Ú
			write_data(addr + 1, 0x03);  // ÓÒ×Ö½Ú
		}
		delay(8000);		
	
	}	
}