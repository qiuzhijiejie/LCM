//  功能描述   : OLED I2C接口演示例程(STM32F103系列)
//              GND  电源地
//              VCC  3.3v电源
//              D0   PA0（SCL）
//              D1   PA1（SDA）
//              RES  PA2（SPI模块改成IIC模块需要接此引脚，IIC模块用户请忽略）

#include "delay.h"
#include "sys.h"
#include "oled.h"
#include "bmp.h"

int main(void)
{
	u8 t=' ';
	delay_init();
	OLED_Init();
	OLED_ColorTurn(0);  //0正常显示，1 反色显示
    OLED_DisplayTurn(0);//0正常显示 1 屏幕翻转显示
	while(1)
	{
//		OLED_ShowPicture(0,0,128,64,BMP1,1);
//		OLED_Refresh();
//		delay_ms(500);
//		OLED_Clear();

//		delay_ms(500);
//		OLED_Clear();
//		
//		OLED_Fill_Row(0X55) ;
//		delay_ms(500);
//		
//		OLED_Fill_Row(0XAA) ;
//		delay_ms(500);
//		
//		OLED_Fill_Column(0xFF, 0x00); 
//		delay_ms(500);

//		OLED_Fill_Column(0x00, 0xFF); 
//		delay_ms(500);

//		OLED_Fill_Column(0x55, 0xAA); 
//		delay_ms(500);

//		OLED_Fill_Column(0xAA, 0x55); 
//		delay_ms(500);	
//		OLED_ShowChinese(18 ,0,0,16,1);//中
//		OLED_ShowChinese(36, 0,1,16,1);//景
//		OLED_ShowChinese(54, 0,2,16,1);//园
//		OLED_ShowChinese(72, 0,3,16,1);//电
//		OLED_ShowChinese(90,0,4,16,1);//子
//		
//	    OLED_ShowChinese(18 ,16,5,16,1);//中
//		OLED_ShowChinese(36, 16,6,16,1);//景
//		OLED_ShowChinese(54, 16,7,16,1);//园
//		OLED_ShowChinese(72, 16,8,16,1);//电
//		OLED_ShowChinese(90,16,9,16,1);//子		
////		OLED_ShowString(8,16,"ZHONGJINGYUAN",16,1);
//		OLED_ShowString(20,32,"205/06/28",16,1);
//		OLED_ShowString(0,48,"ASCII:",16,1);  
//		OLED_ShowString(63,48,"CODE:",16,1);
//		OLED_ShowChar(48,48,t,16,1);//显示ASCII字符	   
//		t++;
//		if(t>'~')t=' ';
//		OLED_ShowNum(103,48,t,3,16,1);
//		OLED_Refresh();
//		delay_ms(500);
//		OLED_Clear();
		OLED_ShowChinese(0, 0, 0,16,1);   //16*16 中
	    OLED_ShowChinese(16,0, 0,24,1);   //24*24 中
//		OLED_ShowChinese(24,20,0,32,1);   //32*32 中
	    OLED_ShowChinese(64,0, 0,64,1);   //64*64 中
		OLED_Refresh();
	    delay_ms(500);
  	    OLED_Clear();
		OLED_ShowString(0,0,"SJW",8,1);//6*8 “ABC”
		OLED_ShowString(0,8,"SJW",12,1);//6*12 “ABC”
	    OLED_ShowString(0,20,"SJW",16,1);//8*16 “ABC”
		OLED_ShowString(0,36,"SJW",24,1);//12*24 “ABC”
	    OLED_Refresh();
//		delay_ms(500);
//		OLED_ScrollDisplay(10,4,1);
//		delay_ms(500);
//		OLED_Clear();

	}
}
