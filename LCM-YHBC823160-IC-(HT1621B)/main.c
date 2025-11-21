#include "LCM823160.h"

//unsigned char addr;
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
	    display_digits_simple() ;
/*
		for (addr = 0x40; addr <= 0x5a; addr += 2)//0
		{
			write_data(addr,     0x0A);  // ×ó×Ö½Ú
			write_data(addr + 1, 0x0F);  // ÓÒ×Ö½Ú
		}
		delay(2000);

		for (addr = 0x40; addr <= 0x5a; addr += 2)//1
		{
			write_data(addr,     0x00);  // ×ó×Ö½Ú
			write_data(addr + 1, 0x06);  // ÓÒ×Ö½Ú
		}
		delay(2000);

		for (addr = 0x40; addr <= 0x4a; addr += 2)//2
		{
			write_data(addr,     0x0c);  // ×ó×Ö½Ú
			write_data(addr + 1, 0x0b);  // ÓÒ×Ö½Ú
		}
		for (addr = 0x4f; addr <= 0x5d; addr += 2)//2
		{
			write_data(addr,     0x0d);  // ×ó×Ö½Ú
			write_data(addr + 1, 0x06);  // ÓÒ×Ö½Ú
		}
		delay(2000);

		for (addr = 0x40; addr <= 0x4a; addr += 2)//3
		{
			write_data(addr,     0x04);  // ×ó×Ö½Ú
			write_data(addr + 1, 0x0f);  // ÓÒ×Ö½Ú
		}
		for (addr = 0x4f; addr <= 0x5d; addr += 2)//3
		{
			write_data(addr,     0x0f);  // ×ó×Ö½Ú
			write_data(addr + 1, 0x04);  // ÓÒ×Ö½Ú
		}
		delay(2000);

		for (addr = 0x40; addr <= 0x4a; addr += 2)//4
		{
			write_data(addr,     0x06);  // ×ó×Ö½Ú
			write_data(addr + 1, 0x06);  // ÓÒ×Ö½Ú
		}
		for (addr = 0x4f; addr <= 0x5d; addr += 2)//4
		{
			write_data(addr,     0x06);  // ×ó×Ö½Ú
			write_data(addr + 1, 0x0c);  // ÓÒ×Ö½Ú
		}
		delay(2000);


		for (addr = 0x40; addr <= 0x4a; addr += 2)//5
		{
			write_data(addr,     0x06);  // ×ó×Ö½Ú
			write_data(addr + 1, 0x0d);  // ÓÒ×Ö½Ú
		}
		for (addr = 0x4f; addr <= 0x5d; addr += 2)//5
		{
			write_data(addr,     0x0b);  // ×ó×Ö½Ú
			write_data(addr + 1, 0x0c);  // ÓÒ×Ö½Ú
		}
		delay(2000);

		for (addr = 0x40; addr <= 0x4a; addr += 2)//6
		{
			write_data(addr,     0x0e);  // ×ó×Ö½Ú
			write_data(addr + 1, 0x0d);  // ÓÒ×Ö½Ú
		}
		for (addr = 0x4f; addr <= 0x5d; addr += 2)//6
		{
			write_data(addr,     0x0b);  // ×ó×Ö½Ú
			write_data(addr + 1, 0x0e);  // ÓÒ×Ö½Ú
		}
		delay(2000);


		for (addr = 0x40; addr <= 0x4a; addr += 2)//7
		{
			write_data(addr,     0x00);  // ×ó×Ö½Ú
			write_data(addr + 1, 0x07);  // ÓÒ×Ö½Ú
		}
		for (addr = 0x4f; addr <= 0x5d; addr += 2)//7
		{
			write_data(addr,     0x0e);  // ×ó×Ö½Ú
			write_data(addr + 1, 0x00);  // ÓÒ×Ö½Ú
		}
		delay(2000);


		for (addr = 0x40; addr <= 0x4a; addr += 2)//8
		{
			write_data(addr,     0x0e);  // ×ó×Ö½Ú
			write_data(addr + 1, 0x0f);  // ÓÒ×Ö½Ú
		}
		for (addr = 0x4f; addr <= 0x5d; addr += 2)//8
		{
			write_data(addr,     0x0f);  // ×ó×Ö½Ú
			write_data(addr + 1, 0x0e);  // ÓÒ×Ö½Ú
		}
		delay(2000);

		for (addr = 0x40; addr <= 0x4a; addr += 2)//9
		{
			write_data(addr,     0x06);  // ×ó×Ö½Ú
			write_data(addr + 1, 0x07);  // ÓÒ×Ö½Ú
		}
		for (addr = 0x4f; addr <= 0x5d; addr += 2)//9
		{
			write_data(addr,     0x0e);  // ×ó×Ö½Ú
			write_data(addr + 1, 0x0c);  // ÓÒ×Ö½Ú
		}
		delay(2000);		
*/	
	}	
}