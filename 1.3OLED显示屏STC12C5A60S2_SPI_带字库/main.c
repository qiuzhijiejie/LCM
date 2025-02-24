#include <reg52.H>
#include "oled.h"
#include "bmp.h"

int main(void)
{
	float t=0;
	OLED_Init();
	OLED_ColorTurn(1);		//0正常显示,1 反色显示
	OLED_DisplayTurn(0);	//0正常显示,1 屏幕翻转显示
	OLED_Clear();


	
	while(1)
	{	
//	    t+=0.01;
//	    OLED_ShowNum(0,0,t,5);
//	    OLED_Display_8x16(1,1,C);		
		Picture_display(picture_tab);
		OLED_Display_GB2312_string(0,1," 广 东 烨 辉 科");
		OLED_Display_GB2312_string(0,5," 技 有 限 公 司");
		delay_ms(500);
		PerformScreenFillSequence();
		delay_ms(500);	
		OLED_Display_128x64(bmp1);
		delay_ms(500);
		OLED_Display_GB2312_string(0,0,"12864,带中文字库");	/*在第1页，第1列，显示一串16x16点阵汉字或8x16的ASCII字*/
		OLED_Display_GB2312_string(0,2,"16X16简体汉字库,");  /*显示一串16x16点阵汉字或8x16的ASCII字.以下雷同*/
		OLED_Display_GB2312_string(0,4,"或8X16点阵ASCII,");
		OLED_Display_GB2312_string(0,6,"或5X7点阵ASCII码");
		delay_ms(500);				
		OLED_Clear();
		OLED_Display_GB2312_string(0,0,"烨辉科技有限公司");	
		OLED_Display_GB2312_string(0,2,"主要生产OLED模块");
		OLED_Display_GB2312_string(0,4,"顾客至上真诚服务");
		OLED_Display_GB2312_string(0,6,"诚信与质量第一！");
		delay_ms(500);	
		OLED_Clear();		
		OLED_Display_GB2312_string(0,0,"GB2312简体字库及");	
		OLED_Display_GB2312_string(0,2,"有图型功能,可自");	
		OLED_Display_GB2312_string(0,4,"编大字或图像或生");
		OLED_Display_GB2312_string(0,6,"僻字,例如：");
		OLED_Display_16x16(97,6,jiong1);					/*在第7页，第81列显示单个自编生僻汉字“囧”*/
		OLED_Display_16x16(113,6,lei1);
		delay_ms(500);
		OLED_Clear();
		OLED_Display_GB2312_string(0,0,"<!@#$%^&*()_-+]/");	/*在第1页，第1列，显示一串16x16点阵汉字或8*16的ASCII字*/
		OLED_Display_string_5x7(0,2,"<!@#$%^&*()_-+]/;.,?[");/*在第3页，第1列，显示一串5x7点阵的ASCII字*/
		OLED_Display_string_5x7(0,3,"XY electronics Co., ");/*显示一串5x7点阵的ASCII字*/
		OLED_Display_string_5x7(0,4,"Ltd. established at  ");/*显示一串5x7点阵的ASCII字*/	
		OLED_Display_string_5x7(0,5,"year 2024.Focus OLED ");/*显示一串5x7点阵的ASCII字*/
		OLED_Display_string_5x7(0,6,"Mobile:13622496694");/*显示一串5x7点阵的ASCII字*/
		OLED_Display_string_5x7(0,7,"Tel:0762-32910715");/*显示一串5x7点阵的ASCII字*/
		delay_ms(500);
		OLED_Clear();	
		OLED_Display_GB2312_string(0,0,"啊阿埃挨哎唉哀皑");	/*在第1页，第1列，显示一串16x16点阵汉字或8x16的ASCII字*/
		OLED_Display_GB2312_string(0,2,"癌蔼矮艾碍爱隘鞍");  /*显示一串16x16点阵汉字或8x16的ASCII字.以下雷同*/
		OLED_Display_GB2312_string(0,4,"氨安俺按暗岸胺案");
		OLED_Display_GB2312_string(0,6,"肮昂盎凹敖熬翱袄");		        
		delay_ms(500);
		OLED_Clear();	
		OLED_Display_GB2312_string(0,0,"鬟鬣麽麾縻麂麇麈");
		OLED_Display_GB2312_string(0,2,"麋麒鏖麝麟黛黜黝");
		OLED_Display_GB2312_string(0,4,"黠黟黢黩黧黥黪黯");
		OLED_Display_GB2312_string(0,6,"齄鼬鼹鼷鼽鼢癌蔼");
		delay_ms(500);
		OLED_Clear();
  }
}