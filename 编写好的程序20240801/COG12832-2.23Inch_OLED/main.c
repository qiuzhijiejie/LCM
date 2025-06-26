#include "REG51.h"
#include "oled.h"
#include "bmp.h"

int main(void)
{	
	u8 t=' ';
	OLED_Init();//初始化OLED
	OLED_ColorTurn(0);//0正常显示，1 反色显示
    OLED_DisplayTurn(0);//0正常显示 1 屏幕翻转显示

	while(1) 
	{		
    OLED_ColorTurn(0);//0正常显示，1 反色显示
	OLED_ShowChinese(18,4,0,16); 
	OLED_ShowChinese(36,4,1,16);
	OLED_ShowChinese(54,4,2,16);
	OLED_ShowChinese(72,4,3,16);
	OLED_ShowChinese(90,4,4,16);
	
	OLED_ShowChinese(18,6,5,16);
	OLED_ShowChinese(36,6,6,16);
	OLED_ShowChinese(54,6,7,16);
	OLED_ShowChinese(72,6,8,16);
	OLED_ShowChinese(90,6,9,16);
    delay_ms(150);
	OLED_Clear();
	OLED_ShowString(15,5,"2.23Inch_OLED",16);
	delay_ms(150);
	OLED_Clear();
	OLED_ShowString(30,5,"COG12832",16);	
	delay_ms(150);
	OLED_Clear();
	
	OLED_Fill_Row(0X55) ;
	delay_ms(100);
	
	OLED_Fill_Row(0XAA) ;
	delay_ms(100);
	
	OLED_Fill_Column(0xFF, 0x00); 
	delay_ms(100);

	OLED_Fill_Column(0x00, 0xFF); 
	delay_ms(100);

	OLED_Fill_Column(0x55, 0xAA); 
	delay_ms(100);

	OLED_Fill_Column(0xAA, 0x55); 
	delay_ms(100);
	OLED_Fill_Column(0x00, 0x00); 
	OLED_Clear();	
	OLED_ColorTurn(1);//0正常显示，1 反色显示
	


	OLED_Clear();
	OLED_ShowChinese(18,4,0,16); 
	OLED_ShowChinese(36,4,1,16);
	OLED_ShowChinese(54,4,2,16);
	OLED_ShowChinese(72,4,3,16);
	OLED_ShowChinese(90,4,4,16);
	
	OLED_ShowChinese(18,6,5,16);
	OLED_ShowChinese(36,6,6,16);
	OLED_ShowChinese(54,6,7,16);
	OLED_ShowChinese(72,6,8,16);
	OLED_ShowChinese(90,6,9,16);
	delay_ms(150);
	OLED_Clear();
	OLED_ShowString(15,5,"2.23Inch_OLED",16);	
	delay_ms(150);
	OLED_Clear();
	OLED_ShowString(30,5,"COG12832",16);	
	delay_ms(150);
	OLED_Clear();
	}	  
	
}

	