//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//中景园电子
//店铺地址：http://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//  文 件 名   : main.c
//  版 本 号   : v2.0
//  作    者   : HuangKai
//  生成日期   : 2018-10-31
//  最近修改   : 
//  功能描述   : OLED SPI接口演示例程(51系列)
//              说明: 
//              ----------------------------------------------------------------
//              GND    电源地
//              VCC  3.3v电源
//              D0   P1^0（SCL）
//              D1   P1^1（SDA）
//              RES  P1^2
//              DC   P1^3
//              CS   P1^4
//              ----------------------------------------------------------------
// 修改历史   :
// 日    期   : 
// 作    者   : HuangKai
// 修改内容   : 创建文件
//版权所有，盗版必究。
//Copyright(C) 中景园电子2018-10-31
//All rights reserved
//******************************************************************************/
#include "REG51.h"
#include "oled.h"
#include "bmp.h"

int main(void)
{
	u8 i;
	OLED_Init();
	while(1)
	{
//		OLED_DrawBMP(0,0,256,64,gImage_1,0);//灰度图片显示
//		delay_ms(500);
		OLED_Fill(0,0,256,64,0x55);
		delay_ms(200);
		OLED_Fill(0,0,256,64,0xF0);
		OLED_DrawSingleBMP(0,0,256,64,gImage_2,0);//单色图片显示
		delay_ms(200);
		OLED_Fill(0,0,256,64,0x00);
		for(i=0;i<1;i++)
		{
		  OLED_ShowChinese(60-i*20,0,"智力光电股份有限公司",16+i*8,1);
			delay_ms(200);
		  OLED_ShowChinese(60-i*20,0,"智力光电股份有限公司",16+i*8,0);
			delay_ms(200);
		}
		OLED_ShowString(76,32,"WISETIP OLED TEST",16,0);
		delay_ms(200);
		OLED_ShowString(40,48,"https://wisetip.com.tw/",16,0);
		delay_ms(200);
		OLED_Fill(0,0,256,64,0x00);
	}
}