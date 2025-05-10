#include "lcd.h"

u8 LCD_POLARITY=0;
u8 CGRAM[LCD_Max];

//第一行的偶数
code u8 leddata1[]={ 
                0xEE,  //"0"
                0x28,  //"1"
                0xCD,  //"2"
                0x6D,  //"3"
                0x2B,  //"4"
                0x67,  //"5"
                0xE7,  //"6"
                0x2C,  //"7"
                0xEF,  //"8"
                0x6F,  //"9"
                0xAF,  //"A"
                0xE3,  //"B"
                0xC6,  //"C"
                0xE9,  //"D"
                0xC7,  //"E"
                0x87,  //"F"
                0xAB,  //"H"
                0xC2,  //"L"
                0xAE,  //"n"
                0xEA,  //"u"
                0x8F,  //"P"
                0xE1,  //"o"
                0x01,  //"-"
                0x00,  //熄灭
                0x00  //自定义
 
                         };

//第一行的奇数 
code u8 leddata2[]={ 
 
                0xEE,  //"0"
                0x82,  //"1"
                0xDC,  //"2"
                0xD6,  //"3"
                0xB2,  //"4"
                0x76,  //"5"
                0x7E,  //"6"
                0xC2,  //"7"
                0xFE,  //"8"
                0xF6,  //"9"
                0xFA,  //"A"
                0x3E,  //"B"
                0x6C,  //"C"
                0x9E,  //"D"
                0x7C,  //"E"
                0x78,  //"F"
                0xBA,  //"H"
                0x2C,  //"L"
                0xEA,  //"n"
                0xAE,  //"u"
                0xF8,  //"P"
                0x1E,  //"o"
                0x10,  //"-"
                0x00,  //熄灭
                0x00  //自定义
 
                         };
//第二行后面
code u8 leddata3[]={ 
 
                0xBD,  //"0"
                0x30,  //"1"
                0x9B,  //"2"
                0xBA,  //"3"
                0x36,  //"4"
                0xAE,  //"5"
                0xAF,  //"6"
                0x38,  //"7"
                0xBF,  //"8"
                0xBE,  //"9"
                0x3F,  //"A"
                0xA7,  //"B"
                0x8D,  //"C"
                0xB3,  //"D"
                0x8F,  //"E"
                0x0F,  //"F"
                0x37,  //"H"
                0x85,  //"L"
                0x3D,  //"n"
                0xB5,  //"u"
                0x1F,  //"P"
                0xA3,  //"o"
                0x02,  //"-"
                0x00,  //熄灭
                0x00  //自定义
 
                         };
                         
 //第三行
code u8 leddata4[]={ 
 
                0xBE,  //"0"
                0x0C,  //"1"
                0x76,  //"2"
                0x5E,  //"3"
                0xCC,  //"4"
                0xDA,  //"5"
                0xFA,  //"6"
                0x0E,  //"7"
                0xFE,  //"8"
                0xDE,  //"9"
                0xEE,  //"A"
                0xF8,  //"B"
                0xB2,  //"C"
                0x7C,  //"D"
                0xF2,  //"E"
                0xE2,  //"F"
                0xEC,  //"H"
                0xB0,  //"L"
                0xAE,  //"n"
                0xBC,  //"u"
                0xE6,  //"P"
                0x78,  //"o"
                0x40,  //"-"
                0x00,  //熄灭
                0x00  //自定义
 
                         };


code u8 leddata10[]={ 
 
                0x3F,  //"0"
                0x06,  //"1"
                0x5B,  //"2"
                0x4F,  //"3"
                0x66,  //"4"
                0x6D,  //"5"
                0x7D,  //"6"
                0x07,  //"7"
                0x7F,  //"8"
                0x6F,  //"9"
                0x77,  //"A"
                0x7C,  //"B"
                0x39,  //"C"
                0x5E,  //"D"
                0x79,  //"E"
                0x71,  //"F"
                0x76,  //"H"
                0x38,  //"L"
                0x37,  //"n"
                0x3E,  //"u"
                0x73,  //"P"
                0x5C,  //"o"
                0x40,  //"-"
                0x00,  //熄灭
                0x00  //自定义
 
                         };
code u8 leddata20[]={ 
 
                0xC0,  //"0"
                0xF9,  //"1"
                0xA4,  //"2"
                0xB0,  //"3"
                0x99,  //"4"
                0x92,  //"5"
                0x82,  //"6"
                0xF8,  //"7"
                0x80,  //"8"
                0x90,  //"9"
                0x88,  //"A"
                0x83,  //"B"
                0xC6,  //"C"
                0xA1,  //"D"
                0x86,  //"E"
                0x8E,  //"F"
                0x89,  //"H"
                0xC7,  //"L"
                0xC8,  //"n"
                0xC1,  //"u"
                0x8C,  //"P"
                0xA3,  //"o"
                0xBF,  //"-"
                0xFF,  //熄灭
                0xFF  //自定义
 
                         };
                         

u8 LCD_Get_LEDDATA(u8 pos,u8 dat)
{
	u8 ret_dat=0;
	switch(pos)
	{
		case 14:
		case 15:
		case 16:
		{
			ret_dat=leddata1[dat];
		}break;
		case 17:
		case 18:
		case 19:
		case 13:
		{
			ret_dat=leddata2[dat];
		}break;
		case 10:
		case 11:
		case 12:
		{
			ret_dat=leddata3[dat];
		}break;
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		{
			ret_dat=leddata4[dat];
		}break;
		default:break;
	}
	return ret_dat;
}



void LCD_Delay(u16 nops)
{
	while(nops--);
}


void LCD_Port_Init(void)
{
	SH_CP =1;
	DS=1;
	ST_CP=1;
}




void LCD_Write_Byte(u8 dat)
{
	u8 i;
	for(i=0;i<8;i++)
	{
		SH_CP =0;
		//LCD_Delay(1);
		if(dat&0x80)
			DS=1;
		else
			DS=0;
		//LCD_Delay(1);	
		SH_CP =1;
		//LCD_Delay(1);
		dat<<=1;
	}
}


//LCD_Set_All(1,LCD_Max,leddata10[index]);
void LCD_Set_All(u8 start,u8 stop,u8 dat)
{
	u8 i;
	for(i=start;i<stop;i++)
	{
		CGRAM[i]=dat;
	}
}
void LCD_Display_All(void)
{
	u8 i;
	for(i=0;i<LCD_Max;i++)
	{
		CGRAM[i]=0xff;
	}
}
void LCD_Clear_All(void)
{
	u8 i;
	for(i=0;i<LCD_Max;i++)
	{
		CGRAM[i]=0x00;
	}
}
void LCD_Update_CGRAM(u8 bit_state)
{
	u8 i;
	ST_CP=0;
	if(bit_state==0)
	{
		for(i=0;i<LCD_Max;i++)
		{
			LCD_Write_Byte(~CGRAM[i]);
		}
	}
	else
	{
		for(i=0;i<LCD_Max;i++)
		{
			LCD_Write_Byte(CGRAM[i]);
		}
	}
	ST_CP=1;	//将数据更新到端口
}





