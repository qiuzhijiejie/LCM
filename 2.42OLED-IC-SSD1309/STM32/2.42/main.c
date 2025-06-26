/********************金逸晨**************************本公司模块SDA,SCK已内置上拉电阻，
*****************2.42  4PIN IIC OLED FOR STM32F103*************
*****STM32F103驱动**************************
***** PB5----3  SCK   ,  PB6----4  SDA   ,    
***** BY:GU 
* IC：SSD1309  本模块为竖屏模式
********************************************************/

#include "oled242.h"
#include "PIC.h"
 
const unsigned char  *point; 
int main()
{ 
  point= &picture_tab[0];
  IO_init();

  OLED_init();
  OLED_full();
  delay_ms(10000);
  OLED_clear();
  while(1)
  {
      OLED_full();
	  delay_ms(10000);
	  OLED_clear();
	  OLED_HX();
	  delay_ms(10000);
      OLED_clear();
	  OLED_HX1();
	  delay_ms(10000);
      OLED_clear();
	  point= &picture_tab[0];
      Picture_display(point);
      delay_ms(10000);
    
      Picture_ReverseDisplay(point);	 
      delay_ms(10000);
    
  }
}
