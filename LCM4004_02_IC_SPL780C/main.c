//#include <REGX52.H>
//#include "LCD4004.h"
//	unsigned char mode;
//	unsigned char data pattern1[8] = {0x15, 0x0A, 0x15, 0x0A, 0x15, 0x0A, 0x15}; // 棋盘
//	unsigned char data pattern2[8] = {0x0A, 0x15, 0x0A, 0x15, 0x0A, 0x15, 0x0A};	
//	unsigned char data pattern3[8] = {0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15}; //竖线
//    unsigned char data pattern4[8] = {0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A}; 
//    unsigned char data pattern5[8] = {0x1F, 0x00, 0x1F, 0x00, 0x1F, 0x00, 0x1F, 0x00};//横线 
//	unsigned char data pattern6[8] = {0x00, 0x1F, 0x00, 0x1F, 0x00, 0x1F, 0x00, 0x1F}; 	
//	unsigned char data pattern7[8] = {0x00, 0x15, 0x00, 0x15, 0x00, 0x15, 0x00, 0x15}; //麻点
//int main() 
//{
//	LCD_Init(); 
//	LCD_Clear();
//	displayCustomCharacter1(pattern1); // 显示棋盘
//	Delay(80); 
//	displayCustomCharacter1(pattern2); 
//	Delay(80); 
//	displayCustomCharacter1(pattern3); // 显示竖线
//	Delay(80); 
//	displayCustomCharacter1(pattern4); 
//	Delay(80); 
//	displayCustomCharacter1(pattern5); // 显示横线
//	Delay(80); 
//	displayCustomCharacter1(pattern6); 
//	Delay(80); 
//	displayCustomCharacter1(pattern7); // 显示麻点
//	Delay(80); 
//	write_CGROM(0x10);//内置
//	Delay(80);
//	write_CGROM(0x60);//内置
//	Delay(80);
//	write_CGROM(0xB0);//内置
//	Delay(80);
//	Write_DDRAM(0xCE);//内置字符
//	Delay(80);
//	LCD_Clear();
//	LCD_ShowString(2,5,"LCM4004_01");
//	LCD_ShowString(1,2,"YeHuiDisplay.com");
//			
//while (1) 
//{

//		if (ReadKey1()) 
//	  {  
//		LCD_Clear();
//		mode++;
//		mode %= 10;			
//		switch (mode) 
//	  {
//        case 1:            
//			displayCustomCharacter1(pattern1); 
//            break;
//        case 2:
//            displayCustomCharacter1(pattern2);  
//            break;
//        case 3:
//            displayCustomCharacter1(pattern3); 
//            break;
//        case 4:
//           displayCustomCharacter1(pattern4);  
//            break;
//        case 5:
//            displayCustomCharacter1(pattern5);  
//            break;
//        case 6:
//            displayCustomCharacter1(pattern6);  
//            break;
//		case 7:
//			displayCustomCharacter1(pattern7);
//			break;
//		case 8:
//			write_CGROM(0x10); 
//		   break;
//		case 9:
//			write_CGROM(0x60); 
//		   break;
//        default://mode不等于1到9中的任意一个值,则执行write_CGROM(0xB0) 
//			write_CGROM(0xB0);
//            break;
//    
//		}
//	}			
//}

//	}

//----------------------------------------------------------------------
/*EASTRISING TECHNOLOGY CO,.LTD.*/
// Module    : ERM4004-1
// Create    : JAVEN
// Date      : 2009-06-18
// Drive IC  : KS0066/SPLC780D/AIP31066*2
// INTERFACE : 6800_8bit
// MCU 		 : AT89C52
// VDD		 : 5V
//----------------------------------------------------------------------
#include <reg51.h>
#include <intrins.h>

#define	uchar	unsigned char
#define	uint	unsigned int


sbit 	RS=P3^0;
sbit 	R_W=P3^1;
sbit 	E=P3^2;
sbit 	E1=P3^3;
bit 	log=0;
sbit	int0=P2^2;
#define  DATA_BUS	P1
#define  TIMING     8                 //8     8 bit data transmission            4       4 bit data transmission

void LCD_initialize(void);
void Write_CGRAM(uchar  a[]);
void Write_Command(uchar cmd);
void Write_Data(uchar dat);
void Busy(void);
void Delay(uint n);
void Delay1(uint n);
void Write_DDRAM(uchar a);
void write_CGROM(uchar a);
void shaw_character_string(uchar a[]);


uchar code character_string[]=
{
	   "buydisplay.com*buydisplay.ocm*buydisplay"
	   "buydisplay.com*buydisplay.ocm*buydisplay"
};

uchar code font[]=
{
	0x1f,0x00,0x1f,0x00,0x1f,0x00,0x1f,0x00,
	0x00,0x1f,0x00,0x1f,0x00,0x1f,0x00,0x1f,
	0x15,0x15,0x15,0x15,0x15,0x15,0x15,0x15,
	0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,
	0x08,0x0f,0x12,0x0f,0x0a,0x1f,0x02,0x02,	//  
	0x0f,0x09,0x0f,0x09,0x0f,0x09,0x09,0x13,	//  
	0x1f,0x11,0x11,0x1f,0x11,0x11,0x11,0x1F,	//  
	0x0C,0x0a,0x11,0x1f,0x09,0x09,0x09,0x13,	//  
};



void Step(void) interrupt 0 using 0
{
	Delay1(500);
	if(int0==0)	log=!log;
   while(!int0);
  return;

}


void Delay1(uint n)
{while(n--);
   
}

void Delay(uint n)
{while(n)
 { 
	  n--;
 }
 	for(;log==1;)
	{

	}

}

#if  (TIMING==8)

void Busy(void)
{
    uchar flag=0xff;
    RS=0;E=0;R_W=1;
	while ((flag&0x80)==0x80)
	{	P1=0xff;
		E=1;
		_nop_();
	    flag=DATA_BUS;
    	E=0;
	}

   flag=0xff;
    RS=0;E1=1;R_W=1;
	while ((flag&0x80)==0x80)
	{	P1=0xff;
		E1=1;
		_nop_();
	    flag=DATA_BUS;
    	E1=0;
	}


}

#else
void Busy(void)
{
    uchar flag=0xf0;
    RS=0;E=0;R_W=1;
	while ((flag&0x80)==0x80)
	{	P1=0xf0;
		E=1;
		_nop_();
	    flag=DATA_BUS;
    	E=0;
	}

     flag=0xf0;
    RS=0;E1=0;R_W=1;
	while ((flag&0x80)==0x80)
	{	P1=0xf0;
		E1=1;
		_nop_();
	    flag=DATA_BUS;
    	E1=0;
	}



}


#endif







#if  (TIMING==8)
void Write_Data(uchar dat) 
{
    Busy();
    RS=1;
    R_W=0;
	DATA_BUS=dat;
	E=1;
    _nop_();
    E=0;
}

#else

void Write_Data(uchar dat) 
{
    Busy();
    RS=1;
    R_W=0;
	DATA_BUS=dat&0xf0;
	E=1;
    _nop_();
    E=0;
	dat<<=4;
	DATA_BUS=dat&0xf0;
	E=1;
    _nop_();
    E=0;

}

#endif


#if  (TIMING==8)
void Write_Data1(uchar dat) 
{
    Busy();
    RS=1;
    R_W=0;
	DATA_BUS=dat;
	E1=1;
    _nop_();
    E1=0;
}

#else

void Write_Data1(uchar dat) 
{
    Busy();
    RS=1;
    R_W=0;
	DATA_BUS=dat&0xf0;
	E1=1;
    _nop_();
    E1=0;
	dat<<=4;
	DATA_BUS=dat&0xf0;
	E1=1;
    _nop_();
    E1=0;

}

#endif




#if  (TIMING==8)
void Write_Command(uchar cmd)
{
   Busy();
    RS=0;
    R_W=0;
	DATA_BUS=cmd;
	E=1;
	_nop_();
    E=0;
}

#else					
void Write_Command(uchar cmd)
{
    Busy();
    RS=0;
    R_W=0;
	DATA_BUS=cmd&0xf0;			
	E=1;
	_nop_();
    E=0;
	cmd<<=4;
	DATA_BUS=cmd&0xf0;
	E=1;
    _nop_();
    E=0;

}
#endif

#if  (TIMING==8)
void Write_Command1(uchar cmd)
{
   Busy();
    RS=0;
    R_W=0;
	DATA_BUS=cmd;
	E1=1;
	_nop_();
    E1=0;
}

#else					
void Write_Command1(uchar cmd)
{
    Busy();
    RS=0;
    R_W=0;
	DATA_BUS=cmd&0xf0;			
	E1=1;
	_nop_();
    E1=0;
	cmd<<=4;
	DATA_BUS=cmd&0xf0;
	E1=1;
    _nop_();
    E1=0;

}
#endif




void Write_CGRAM(uchar  a[])
{uchar i=64,k;
	Write_Command(0x40);
	for(k=0;k<64;k++)
	{Write_Data(a[k]);
	}
	Write_Command1(0x40);
	for(k=0;k<64;k++)
	{Write_Data1(a[k]);
	}
}


void LCD_initialize(void)
{
if(TIMING==8)   Write_Command(0x38);
  else   Write_Command(0x28);

 Write_Command(0x0c);
 Write_Command(0x06);
 Write_Command(0x01);

if(TIMING==8)   Write_Command1(0x38);
  else   Write_Command1(0x28);

 Write_Command1(0x0c);
 Write_Command1(0x06);
 Write_Command1(0x01);

 Delay(1000);
 Write_CGRAM(font);

}

void Write_DDRAM(uchar a)
{uchar j;
 Write_Command(0x02);
 Write_Command1(0x02);
  Delay(1000);
 Write_Command(0x80);
	for(j=0;j<40;j++)
	{Write_Data(a);
	}
 Write_Command(0xc0);
	for(j=0;j<40;j++)
	{Write_Data(a);
	}


 Write_Command1(0x80);
	for(j=0;j<40;j++)
	{Write_Data1(a);
	}
 Write_Command1(0xc0);
	for(j=0;j<40;j++)
	{Write_Data1(a);
	}


		Delay(35000);
		Delay(65000);
		LCD_initialize();
}



void write_CGROM(uchar a)
{uchar j,k=a;
 Write_Command(0x02);
 Write_Command1(0x02);
  Delay(1000);
 Write_Command(0x80);
 	for(j=0;j<40;j++)
	{Write_Data(a);a+=1;
	}
 Write_Command(0xc0);
	for(j=0;j<40;j++)
	{Write_Data(a);a+=1;
	}

	a=k;
 Write_Command1(0x80);
 	for(j=0;j<40;j++)
	{Write_Data1(a);a+=1;
	}
 Write_Command1(0xc0);
	for(j=0;j<40;j++)
	{Write_Data1(a);a+=1;
	}

		Delay(35000);
		Delay(65000);
		LCD_initialize();
}


void shaw_character_string(uchar a[])
{uchar j;
 Write_Command(0x02);
 Write_Command1(0x02);
  Delay(1000);
 Write_Command(0x80);
 	for(j=0;j<40;j++)
	{Write_Data(a[j]);
	}
 Write_Command(0xc0);
 	for(j=40;j<80;j++)
	{Write_Data(a[j]);
	}

 Write_Command1(0x80);
 	for(j=0;j<40;j++)
	{Write_Data1(a[j]);
	}
 Write_Command1(0xc0);
 	for(j=40;j<80;j++)
	{Write_Data1(a[j]);
	}

		Delay(35000);
		Delay(65000);
	LCD_initialize();
}

void main(void)
{	IE=0x81;
	IP=0x01;
	TCON=0x01;
	int0=1;
	DATA_BUS=0xff;
	P2=0xff;
	Delay(1000);
	LCD_initialize();
while(1)
	{Write_DDRAM(0xff);	
	Write_DDRAM(0);	
	Write_DDRAM(1);
	Write_DDRAM(2);
	Write_DDRAM(3);
	write_CGROM(0xa0);
	Write_DDRAM(4);	
	Write_DDRAM(5);
	Write_DDRAM(6);
	Write_DDRAM(7);
	shaw_character_string(character_string);

	}
}






