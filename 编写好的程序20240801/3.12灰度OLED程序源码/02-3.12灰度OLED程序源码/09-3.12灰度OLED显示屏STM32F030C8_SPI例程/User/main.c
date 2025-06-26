//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//中景园电子
//店铺地址：http://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//  文 件 名   : main.c
//  版 本 号   : v2.0
//  作    者   : HuangKai
//  生成日期   : 2018-03-29
//  最近修改   : 
//  功能描述   : OLED I2C接口演示例程(STM32F0系列)
//              说明: 
//              ----------------------------------------------------------------
//              GND  电源地
//              VCC  3.3v电源
//              D0   PA0（SCL）
//              D1   PA1（SDA）
//              RES  PA2
//              DC   PA3
//              CS   PA4 
// 修改历史   :
// 日    期   : 
// 作    者   : HuangKai
// 修改内容   : 创建文件
//版权所有，盗版必究。
//Copyright(C) 中景园电子2018/3/16
//All rights reserved
//******************************************************************************/
#include "stm32f0xx.h"
#include "delay.h"
#include "oled.h"
#include "bmp.h"


int main(void)
{
	u8 i;
  delay_init();
  OLED_Init();
	LED_ON;
  while (1)
  {
		OLED_DrawBMP(0,0,256,64,gImage_1,0);//灰度图片显示
		delay_ms(1000);
		OLED_Fill(0,0,256,64,0x00);
		OLED_DrawSingleBMP(0,0,256,64,gImage_2,0);//单色图片显示
		delay_ms(1000);
		OLED_Fill(0,0,256,64,0x00);
		for(i=0;i<3;i++)
		{
		  OLED_ShowChinese(88-i*20,0,"中景园电子",16+i*8,1);
			delay_ms(1000);
		  OLED_ShowChinese(88-i*20,0,"中景园电子",16+i*8,0);
			delay_ms(1000);
		}
		OLED_ShowString(76,32,"ZJY OLED TEST",16,0);
		delay_ms(1000);
		OLED_ShowString(0,48,"https://zjy-display.taobao.com/",16,0);
		delay_ms(1000);
		OLED_Fill(0,0,256,64,0x00);
	}
}



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
