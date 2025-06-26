/*************************************************************************  
Driver IC    :  ST7565R / UC1701x
interface    :  4-wire serialbus (S8)intertace
************************************************************************/   
//#include "int/hanzi1616.c"
#include "int/shuhanzi1616.c"
#include "12864.h"
#include "int/photo.c"
 
void main()   
{ 
  while(1)
  {	
	  Init_IC(); 
	  lcd_test();  // lcd ×é×°²âÊÔ
	  delayMs(1200);
	  Yehui_UI();
	  delayMs(1200);
	  Clear();
	   
//	  WhatsApp();
//	  delayMs(1200);
//	  Clear();
 	  
	  display_photo(tupian,0,0); 
	  delayMs(1200);
	  Clear();
  }    
}   