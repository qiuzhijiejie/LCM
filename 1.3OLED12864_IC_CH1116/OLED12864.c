#include <REGX52.H>
#include <intrins.h>
#include "picture.h"

sbit OLED_ROM_CS	= P1^2;	  //L 有效,字库片选
sbit OLED_READ_FS0	= P1^1;	  //数据输出
sbit OLED_CS	= P1^0;	  //L 有效,屏片选
sbit OLED_DC		= P3^2;	  //L:command; H:data,命令/数据 
sbit OLED_SCL	= P3^0;   //serial clock input时钟
sbit OLED_SDA		= P3^1;   //serial data input数据输入
sbit KEY    = P2^0;


void Delay(unsigned int time)
{
	  unsigned int i,j;
	  for(i=0;i<time;i++)
	     for(j=0;j<20000;j++)
		 {;}
}


void Ins_trans(unsigned char command)
{
    unsigned char counter;
	  OLED_CS=0;
	  OLED_DC=0;
    for(counter=0;counter<8;counter++)
       {
	     OLED_SCL=0;
         OLED_SDA=(command&0x80)>>7;
         command=command<<1;
	       _nop_();
	     OLED_SCL=1;
	     //  _nop_();
	     _nop_();
	     OLED_SCL=0;
        }
	OLED_DC=1;
   OLED_CS=1;
 }

 void Data_trans(unsigned char command)
{
    unsigned char counter;
	  OLED_CS=0;
	  OLED_DC=1;
    for(counter=0;counter<8;counter++)
       {
         OLED_SCL=0;
	     OLED_SDA=(command&0x80)>>7;
			command=command<<1;
	       _nop_();
	     OLED_SCL=1;
	      // _nop_();
	     _nop_();
	     OLED_SCL=0;
       }  
	OLED_DC=1;
   OLED_CS=1;
 }


 
void Column_set(unsigned char column)
{
	     Ins_trans(0x10|(column>>4));
	     Ins_trans(0x00|(column&0x0f));
}


void Page_set(unsigned char page)
{
	    Ins_trans(0xb0+page);
}


void Screen_FillClear(unsigned char FC)
{
   unsigned char page,column;
  for(page=0;page<8;page++)             //page loop
  {
	   Page_set(page);
	   Column_set(2);
	   for(column=0;column<128;column++)	//column loop
		{
		  Data_trans(FC);
		}
  }
}



void OLED_fullCOL(unsigned char oddData, unsigned char evenData)
{
    unsigned char page, column;
    for (page = 0; page < 8; page++) 
    { 
        Page_set(page);
        Column_set(0);  
        for (column = 0; column < 130; column++) // column loop
        {
         
            if (column % 2 == 0)
            {
                Data_trans(oddData); 
            }
            else
            {
                Data_trans(evenData); 
            }
        }
    }
}


void Picture_display(unsigned char *ptr_pic)
        {
	  unsigned char page,column;
 	  for(page=0;page<(64/8);page++)        //page loop
	     {
		   Page_set(page);
		   Column_set(2);
		   for(column=0;column<128;column++)	//column loop
		      {
		     Data_trans(*ptr_pic++);
		  }
		 }
	  Delay(15);
	}

	
void Initial()
{
		Ins_trans(0xAE);//--turn off oled panel

		Ins_trans(0x02);//--set low column address
		Ins_trans(0x10);//--set high column address

		Ins_trans(0x40);//--set start line address

		Ins_trans(0xB0);//--set page address

		Ins_trans(0x81);//--set contrast control register
		Ins_trans(0xFF);
	
		Ins_trans(0xA1);//--set segment re-map 127 to 0   a0:0 to seg127
		Ins_trans(0xA4);//--set normal display
		Ins_trans(0xA6);//--set indication display
		Ins_trans(0xC8);//--set com(N-1)to com0  c0:com0 to com(N-1)

		Ins_trans(0xA8);//--set multiples ratio(1to64)
		Ins_trans(0x3F);//--1/64 duty

		Ins_trans(0xD5);//--set display clock divide ratio/oscillator frequency
		Ins_trans(0x80);//--set divide ratio

		Ins_trans(0xD3);//--set display offset
		Ins_trans(0x00);//--not offset

		Ins_trans(0xAD);//--OLED_DC-OLED_DC ON/OFF Mode Set
		Ins_trans(0x8B);//--8A:OLED_DC-OLED_DC is disable,8B:OLED_DC-OLED_DC will be turned on when display on.(POR)

		Ins_trans(0xDA);//--set com pins hardware configuration
		Ins_trans(0x12);

		Ins_trans(0xDB);//--set vcomh
		Ins_trans(0x40);

		Ins_trans(0xD9);//--set charge period
		Ins_trans(0xF1);

		Ins_trans(0xAF);//--turn on oled panel
}

//==================display a piture of 16*16 dots================
void display_zifu_16x16(char page,char column,char *dp)
{
	char i,j;
	for(j=0;j<2;j++)
	{
		
 	 	Page_set(page+j);  	 	
		Column_set(column); 
		for (i=0;i<16;i++)
		{	
			Data_trans(*dp);		
			dp++;
		}
	}
}

void display_zifu_16x8(char page,char column,char *dp)
{
	char i,j;
	for(j=0;j<2;j++)
	{
		
 	 	Page_set(page+j);  	 	
		Column_set(column); 
		for (i=0;i<8;i++)
		{	
			Data_trans(*dp);		
			dp++;
		}
	}
}

void Display_Sequence()
{

	Picture_display(picture_tab);
    display_zifu_16x16(1, (8 + 16), guang);
    display_zifu_16x16(1, (8 + 16 * 2), dong);
    display_zifu_16x16(1, (8 + 16 * 3), ye);
    display_zifu_16x16(1, (8 + 16 * 4), hui);
    display_zifu_16x16(1, (8 + 16 * 5), ke);
	
    display_zifu_16x16(4, (8 + 16 ),ji);
    display_zifu_16x16(4, (8 + 16 * 2), you);
    display_zifu_16x16(4, (8 + 16 * 3), xian);
    display_zifu_16x16(4, (8 + 16 * 4), gong);
    display_zifu_16x16(4, (8 + 16 * 5), si);
    display_zifu_16x16(4, (8 + 16 * 6), gangtan);
}


void Display_Sequence2()
{

	Picture_display(picture_tab);
    display_zifu_16x8(1,  8 , one);
    display_zifu_16x8(1, (8 + 12 * 1), two);
    display_zifu_16x8(1, (8 + 12 * 2), eight);
    display_zifu_16x8(1, (8 + 12 * 3), cheng);
    display_zifu_16x8(1, (8 + 12 * 4), six);
    display_zifu_16x8(1, (8 + 12 * 5),four);
	display_zifu_16x8(1, (8 + 12 * 6), d);
    display_zifu_16x8(1, (8 + 12 * 7), o);
    display_zifu_16x8(1, (8 + 12 * 8), t);
	display_zifu_16x8(1, (8 + 12 * 9),s);

	
	
    display_zifu_16x8(4,  (8 + 10), yi  );
    display_zifu_16x8(4,  (8 + 10  * 2), dian);
    display_zifu_16x8(4,  (8 + 10  * 3), shang);
	
    display_zifu_16x16(4, (8 + 13 * 4), dai);
	display_zifu_16x16(4, (8 + 13*  5), zi);
    display_zifu_16x16(4, (8 + 13 * 6), ku);
	
}


unsigned int ReadKey() 
{
    if (KEY == 0) 
	{ 
        Delay(1);  
        if (KEY == 0) 
		{  
            while (KEY == 0);  
            return 1; 
        }
    }
    return 0;  
}



void PerformScreenFillSequence() 
{
    Screen_FillClear(0xAA);
    Delay(5);
    Screen_FillClear(0x55);
    Delay(5);
    OLED_fullCOL(0xFF, 0x00);
    Delay(5);
    OLED_fullCOL(0x00, 0xFF);
    Delay(5);
    OLED_fullCOL(0xAA, 0x55);
    Delay(5);
    OLED_fullCOL(0x55, 0xAA);
}
