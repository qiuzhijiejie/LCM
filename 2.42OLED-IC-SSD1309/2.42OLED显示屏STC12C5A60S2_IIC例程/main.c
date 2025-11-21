#include "REG51.h"
#include "oled.h"
#include "bmp.h"

int main(void)
{	
//	u8 t=' ';
//	OLED_Init();//初始化OLED
//	OLED_ColorTurn(0);//0正常显示，1 反色显示
//    OLED_DisplayTurn(0);//0正常显示 1 屏幕翻转显示
	while(1) 
	{		
	u8 t=' ';
	OLED_Init();//初始化OLED
	OLED_ColorTurn(0);//0正常显示，1 反色显示
    OLED_DisplayTurn(0);//0正常显示 1 屏幕翻转显示

OLED_Fill_Column(0x00, 0x00); 
		OLED_ShowString(13,2,"MAIN IN CHINA",16);
		OLED_ShowString(20,4,"2025/09/10",16);
		OLED_ShowString(0,6,"ASCII:",16);  
		OLED_ShowString(63,6,"CODE:",16);
		OLED_ShowChar(48,6,t,16);
		t++;
		if(t>'~')t=' ';
		OLED_ShowNum(103,6,t,3,16);
		delay_ms(500);
		OLED_Clear();
		
	OLED_DrawBMP(0,0,128,64,BMP1);
	delay_ms(100);

OLED_Fill_Column(0x00, 0x00); 
	OLED_DrawBMP(0,0,128,64,BMP2);
//	OLED_ShowChinese(18,1,0,16); 
//	OLED_ShowChinese(36,1,1,16);
//	OLED_ShowChinese(54,1,2,16);
//	OLED_ShowChinese(72,1,3,16);
//	OLED_ShowChinese(90,1,4,16);
//	
//	OLED_ShowChinese(18,3,5,16);
//	OLED_ShowChinese(36,3,6,16);
//	OLED_ShowChinese(54,3,7,16);
//	OLED_ShowChinese(72,3,8,16);
//	OLED_ShowChinese(90,3,9,16);
	OLED_ShowString(14,3,"OLED2.24Inch",16);
	delay_ms(100);
	
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
	
	OLED_Clear();	
	OLED_ColorTurn(1);//0正常显示，1 反色显示
	
	OLED_DrawBMP(0,0,128,64,BMP1);
	delay_ms(100);
	OLED_DrawBMP(0,0,128,64,BMP2);
//	OLED_ShowChinese(18,1,0,16); 
//	OLED_ShowChinese(36,1,1,16);
//	OLED_ShowChinese(54,1,2,16);
//	OLED_ShowChinese(72,1,3,16);
//	OLED_ShowChinese(90,1,4,16);
//	
//	OLED_ShowChinese(18,3,5,16);
//	OLED_ShowChinese(36,3,6,16);
//	OLED_ShowChinese(54,3,7,16);
//	OLED_ShowChinese(72,3,8,16);
//	OLED_ShowChinese(90,3,9,16);
	OLED_ShowString(14,3,"OLED2.24Inch",16);
	delay_ms(100);
	}	  
	
}

	