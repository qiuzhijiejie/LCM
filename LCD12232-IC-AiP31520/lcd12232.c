#include <REGX52.H>
#include <AT89X52.H>
#include <intrins.h>
#include "picture.h"

#define uint  unsigned int
#define uchar unsigned char
#define A0                    P3_0  	//Selection of command or data.  1 :data  0 : command
#define E1                    P3_2		//Enable signal (E1) Right 
#define E2                    P3_3		//Enable signal (E2) Left
#define R_W                   P3_1		//Read/Write (R/W) signal for the 68-type microcontroller, or WRITE(WR) signal for the 80-type microcontroller.
#define RST                   P3_5		//RESET PIN;
#define DATA_BUS              P1		//Data bus	

sbit LCD_KEY=P2^0;
sbit int0 = P2^0;						//external interrup input
bit  log  = 0;							//display stop mark
unsigned char i;
void Delay(unsigned char ms)	//@12.000MHz
{
	while(ms--)
	{
		unsigned char data i, j;
		i = 12;
		j = 169;
		do
		{
			while (--j);
		} while (--i);
	}
}

uint ReadKey1() // 按键检测函数
{
    if (LCD_KEY == 1)           
	{  // 按键被按下
       Delay(1); // 简单的去抖动处理
        if (LCD_KEY == 1) 
		{  // 检查按键是否仍然按下
            while (LCD_KEY == 1);  // 等待按键释放
            return 1;  // 返回按键被按下
        }
    }
    return 0;  // 按键没有被按下
}


void Delay1(uint n)
{
	while(n--);
}


void Delay2(uint x)
{
	while(x)
	{   
		x--;	 
		for(;log==1;);
 	}
}
	




//write Data to left half screen 
void Write_Data_L(unsigned char Data) 
{  	Delay1(5);
    A0=1;
    R_W=0;
	DATA_BUS=Data;
    E1=1;
    _nop_();
    E1=0;
	return;
}

//write data to right half screen 
void Write_Data_R(unsigned char Data)
{
	Delay1(5);
	A0=1;//1:D7-D0上为显示数据
	R_W=0;//R_W=0 时表示写操作
	DATA_BUS=Data;
	E2=1;
	_nop_();
	E2=0;
	return;    
}

//write command to left half screen
void write_command_L(unsigned char cmd)
{	
	Delay1(20);
    A0=0;//0:D7-D0上为显示控制命令
    R_W=0;
	DATA_BUS=cmd;
    E1=1;
	_nop_();
    E1=0;
	return;
}


//write command to right half screen
void write_command_R(unsigned char cmd)
{	
	Delay1(20);
    A0=0;
    R_W=0;
	DATA_BUS=cmd;
    E2=1;
	_nop_();
    E2=0;
	return;
}


// Set page address 0~4
void Set_Page_Address(unsigned char add)
{
    add=0xb8|add;// (B8 Hex)-Table 15
    write_command_L(add);
    write_command_R(add);
	return;
}


//Set left half screen column address
void Set_Column_Address_L(unsigned char address)
{
    write_command_L(address&0x7F);//0x4F
}

//Set right half screen column address
void Set_Column_Address_R(unsigned char address)
{
    write_command_R(address&0x7F);
}


// Initial 
void Initial(void)
{	
	RST=1;
	Delay(10);
	RST=0;
	Delay(2);
	RST=1;
	Delay(10);

	write_command_L(0xE2);		//Software RESET left half screen
	write_command_R(0xE2);		//Table 27

	write_command_L(0xA4);		//Static Drive OFF 
	write_command_R(0xA4);//A5  //Table 21 A5                    
	
	write_command_L(0xA9);		//Select Duty Register 1/32/ (1/16-A8)
	write_command_R(0xA9);		//Table 23

	write_command_L(0xA0);		//Memory/Segment Mapping Register  Normal Mapping
	write_command_R(0xA0);//A1	//Table 19

	write_command_L(0xC0);		//Start line
	write_command_R(0xC0);		//Table 13


	write_command_L(0xAF);		//Turn on display
	write_command_R(0xAF);//AE	//Table 11

}


//在屏幕上显示图案或点阵
void Display_dot(unsigned char value,unsigned char value1)
{
    unsigned char i,j;
    for(i=0;i<0x04;i++)//i从0到3,4页
	{
		Set_Page_Address(i);
        Set_Column_Address_L(0x00);
        Set_Column_Address_R(0x00);

		for(j=0;j<32;j++)//每一页内写入32列的数据
		{
		        Write_Data_L(value);
		        Write_Data_L(value1);
		        Write_Data_R(value1);
		        Write_Data_R(value);
		}
	}
    return;
}


/*
display one 16*16 dot character
page：指定要在哪个页地址上开始显示汉字。页是屏幕的垂直方向上的一个逻辑区域。

col：指定在哪一列开始显示汉字，列是水平方向的位置。

Character_coding：汉字的编码，决定从字库中选择哪个字。

ft[]：字模数据数组，存储所有汉字的字模信息，每个汉字用32字节（16x16点阵）表示。

*/
//display one 16*16 dot character

void Display16_16Chinese(unsigned char page, unsigned char col, unsigned char Character_coding,  unsigned char ft[]) 
{
    unsigned char i,columnaddress=col;

	Set_Page_Address(page);
	if(col<61) 
	{		       
		for(i=0;i<16;i++)
	  	{
		 if(columnaddress<61)		 
			{Set_Column_Address_L(columnaddress);
			Write_Data_L(ft[Character_coding*32+i]);
			}
		else
			{Set_Column_Address_R(columnaddress-61);
			Write_Data_R(ft[Character_coding*32+i]);
			}
		 columnaddress+=1;
		}

		columnaddress=col;
		Set_Page_Address(page+1);	
		for(i=16;i<32;i++)
	  	{
		 if(columnaddress<61)		 
			{Set_Column_Address_L(columnaddress);
			Write_Data_L(ft[Character_coding*32+i]);
			}
		else
			{Set_Column_Address_R(columnaddress-61);
			Write_Data_R(ft[Character_coding*32+i]);
			}
		 columnaddress+=1;
		}
	}


	else  
	{		       
		for(i=0;i<16;i++)
	  	{
		 	if(columnaddress<122)		 
			{Set_Column_Address_R(columnaddress-61);
			Write_Data_R(ft[Character_coding*32+i]);
			}
			else
			{if(page<=1)
					{Set_Page_Address(page+2);
					Set_Column_Address_L(columnaddress-122);
					Write_Data_L(ft[Character_coding*32+i]);
					}
			}	
		 columnaddress+=1;
		}

		columnaddress=col;
		Set_Page_Address(page+1);	
		for(i=16;i<32;i++)
	  	{
			 if(columnaddress<122)		 
			{Set_Column_Address_R(columnaddress-61);
			Write_Data_R(ft[Character_coding*32+i]);
			}
			else
			{if(page<=1){Set_Page_Address(page+3);
						Set_Column_Address_L(columnaddress-122);
						Write_Data_L(ft[Character_coding*32+i]);
						}
			}
		 columnaddress+=1;
		}
	}

}

// display one 8×16 dot character
void Display8_16English(unsigned char page, unsigned char col, unsigned char Character_coding, unsigned char ft[]) 
{
    unsigned char i, columnaddress = col;

    // 设置第一个页面（显示前8列的数据）
    Set_Page_Address(page);
    if (col < 61) 
    {		       
        // 处理前8行数据（上半部分）
        for (i = 0; i < 8; i++)
        {
            if (columnaddress < 61) // 左屏幕
            {
                Set_Column_Address_L(columnaddress);      // 设置左屏列地址
                Write_Data_L(ft[Character_coding * 16 + i]);  // 写入数据
            }
            else  // 右屏幕
            {
                Set_Column_Address_R(columnaddress - 61); // 设置右屏列地址
                Write_Data_R(ft[Character_coding * 16 + i]);  // 写入数据
            }
            columnaddress += 1;  // 列地址递增
        }

        // 处理下半部分
        columnaddress = col;
        Set_Page_Address(page + 1);  // 切换到下一页，显示下半部分的8行
        for (i = 8; i < 16; i++)
        {
            if (columnaddress < 61)  // 左屏幕
            {
                Set_Column_Address_L(columnaddress);      // 设置左屏列地址
                Write_Data_L(ft[Character_coding * 16 + i]);  // 写入数据
            }
            else  // 右屏幕
            {
                Set_Column_Address_R(columnaddress - 61); // 设置右屏列地址
                Write_Data_R(ft[Character_coding * 16 + i]);  // 写入数据
            }
            columnaddress += 1;  // 列地址递增
        }
    }
    else  
    {		       
        // 如果在右屏的范围内
        for (i = 0; i < 8; i++)
        {
            if (columnaddress < 122)  // 右屏幕
            {
                Set_Column_Address_R(columnaddress - 61);  // 设置右屏列地址
                Write_Data_R(ft[Character_coding * 16 + i]);  // 写入数据
            }
            else if (page <= 1)  // 跨到下一个页面的左屏显示
            {
                Set_Page_Address(page + 2);                // 跳到下一个页面
                Set_Column_Address_L(columnaddress - 122); // 设置左屏列地址
                Write_Data_L(ft[Character_coding * 16 + i]);  // 写入数据
            }
            columnaddress += 1;  // 列地址递增
        }

        // 显示下半部分
        columnaddress = col;
        Set_Page_Address(page + 1);	
        for (i = 8; i < 16; i++)
        {
            if (columnaddress < 122)  // 右屏幕
            {
                Set_Column_Address_R(columnaddress - 61);  // 设置右屏列地址
                Write_Data_R(ft[Character_coding * 16 + i]);  // 写入数据
            }
            else if (page <= 1)  // 跨页显示
            {
                Set_Page_Address(page + 3);                // 跳到下一个页面
                Set_Column_Address_L(columnaddress - 122); // 设置左屏列地址
                Write_Data_L(ft[Character_coding * 16 + i]);  // 写入数据
            }
            columnaddress += 1;  // 列地址递增
        }
    }
	
}


//Show some Chinese characters
void show_chinese(void)
{	
	Display_dot(0x00,0x00);
    for (i = 0; i < 7; i++) 
    {
        Display16_16Chinese(2, i * 16, i, ft);
    }

    // 在 page 0 显示字符 7 到 9
    for (i = 0; i < 3; i++) 
    {
        Display16_16Chinese(0, i * 16, i + 7, ft);
    }
}

void show_english(void)
{	
	Display_dot(0x00,0x00);
    for (i = 0; i < 12; i++) 
    {
        Display8_16English(2, i * 8, i, English_character);
    }

    for (i = 0; i < 11; i++) 
    {
        Display8_16English(0, i * 8, i + 16,English_character);
    }
}



//display Picture
void Display_Picture(unsigned char pic[])
{
    unsigned char i,j,num=0;

	for(i=0;i<0x04;i++)
	{
	Set_Page_Address(i);
    Set_Column_Address_L(0x00);
    Set_Column_Address_R(0x00);

        for(j=0;j<61;j++)				//display left half screen
		{
		    Write_Data_L(pic[i*122+j]);
		}
        for(j=61;j<122;j++)				//display right half screen
		{
		    Write_Data_R(pic[i*122+j]);
		}
	}
    return;
}