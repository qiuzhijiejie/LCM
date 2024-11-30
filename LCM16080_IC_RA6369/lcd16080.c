#include <REG52.H>   
#include <INTRINS.H>    
#include <STDIO.H>     
#include <MATH.H>    
#include <ABSACC.H>   
#include "lcd16080.h"
#include "picture.h"

uchar group,i,j;

void Delay(uint MS)   
{   
 unsigned char us,usn;   
 while(MS!=0)   
 {   
  usn = 2; //for 12M    
  while(usn!=0)   
  {   
   us=0xf6;   
   while (us!=0){us--;};   
   usn--;   
  }   
  MS--;   
 }  
}

uint ReadKey1() // 按键检测函数
{
    if (LCD_KEY == 1)           
	{  // 按键被按下
        Delay(1);  // 简单的去抖动处理
        if (LCD_KEY == 1) 
		{  // 检查按键是否仍然按下
            while (LCD_KEY == 1);  // 等待按键释放
            return 1;  // 返回按键被按下
        }
    }
    return 0;  // 按键没有被按下
}

//忙信号判断
void CheckBusy()
{uchar b;
	 do	
	{
		DataBus=0xff;
		 LCD_CD=1;
		 LCD_RD=0;
		 b=DataBus;
		 LCD_RD=1;	
	 }
	 while(b&0x03!=0x03);//Table 6-4 Note1/2
		
 }

//自动读忙碌判断 
void AUTOREAD_CheckBusy()
{
 uchar b;
 do	
 {
	 DataBus=0xff;
	 LCD_CD=1;
	 LCD_RD=0;
	 b=DataBus;
	 LCD_RD=1;	
 }
	 while(b&0x03==0);//Table 6-4 Note3
}
//自动写忙碌判断 
void AUTOWRITE_CheckBus()
{
	uchar b;
	 do	
	{
		 DataBus=0xff;
		 LCD_CD=1;
		 LCD_RD=0;
		 b=DataBus;
		 LCD_RD=1;	
	 }
	 while(b&0x08!=0x08);
} 

//写数据
void WriteData( uchar Data_Byte )   //Table 5-1 
{    
	 LCD_CD = 0;   
	 DataBus = Data_Byte;   
	 LCD_WR = 0;  
	 _nop_();    
	 LCD_WR  = 1;   
}   

/*
写命令
dat1: 第一个要写入的数据字节。
dat2: 如果有第二个要写入的数据字节。
command: 要发送的命令字节。
parameters_num: 参数数量，指定要发送的数据字节数（1或2）。
*/
void WriteCommand( uchar dat1,uchar dat2,uchar command,uchar parameters_num )   
{   
  if((parameters_num==1)|(parameters_num==2))
  	{
		CheckBusy();	
 	    WriteData(dat1);
	}
	
  if(parameters_num==2)
  	{
		CheckBusy();	
 	    WriteData(dat2);
	}

	AUTOWRITE_CheckBus();//写检测忙信号
	
 	LCD_CD = 1;   
 	DataBus = command; 
 	LCD_WR = 0; 
	_nop_();     
 	LCD_WR  = 1;      
}

//清屏
void LcmClear( void )   
{   uint a;
	Locatexy(0,0,Character);
	WriteCommand(0,0,0xb0,0 ); //0xb0:Set Data Auto Write
	for(a=0;a<8192;a++)
	{
		WriteData( 0 ); 
	}   
	WriteCommand(0,0,0xb2,0 );//0xb2:Auto Reset 
}   

/*
光标位置，支持图形模式和字符模式
x：光标的横坐标（列数）。
y：光标的纵坐标（行数）。
mode：操作模式，决定是图形模式还是字符模式：
mode = 1：图形模式。
mode = 0：字符模式。
*/
void Locatexy(uchar x,uchar y,uchar mode)   
{  uint a=0;
	if(mode)				//Graphic mode   x=0-29  y=0-127    START ADDRESS ：0800
	{
		a=y*30+x;	                       
		WriteCommand((uchar)a,(uchar)(a>>8)+0x08,0x24,2);//a 是一个 16 位的整数，
//		                                                   包含了高位和低位
	}

	else					//Character mode   x=0-29  y=0-15    START ADDRESS ：0000
	{
		a=y*30+x;	                      //0x24-Set Address Pointer-Table 6-6 
		WriteCommand((uchar)(a),(uchar)(a>>8),0x24,2);
	}
}


/*
屏幕上的指定位置显示一个ASCII字符串
x：表示字符串在LCD上的起始横坐标。
y：表示字符串在LCD上的起始纵坐标。
ascll：一个指向ASCII字符数组（字符串）的指针，即要显示的字符串内容。
*/
void Display_Str(uchar x,uchar y,uchar *ascll)
{
	Locatexy(x,y,Character);
	while(*ascll>0)//=!\0
	{
		WriteCommand(*ascll-0x20,0,0xc0,1 );//Table 6-19-Data Write and Increment ADP  
		ascll++;//*ascll - 0x20 将其转换为LCD可以识别的字符编码
	} 
	
}
//内部字库
void Display_FullCGROM()
{
    uchar x = 0, y = 0;       // 起始位置 (0, 0)
    uchar charCode = 0x01;    // 从 CGROM 的 0x00 开始

    // 遍历整个 LCD16080 屏幕 (30 列 x 16 行)
    while (y < 16)
    {
        Locatexy(x, y, Character);  // 设置光标位置

        // 显示当前的 CGROM 字符
        WriteCommand(charCode, 0, 0xc0, 1);  // 发送当前字符的代码到 LCD

        // 更新位置和字符
        x++;             // 移动到下一列
        charCode++;      // 显示下一个字符

        // 当到达每行最后一个字符，换到下一行
        if (x >= 30)
        {
            x = 0;       // 列回到 0
            y++;         // 行数增加，换行
        }

        // 如果 charCode 超过 0xFF，就回到 0x00，重新循环显示字符
        if (charCode > 0xFF)
        {
            charCode = 0x01;
        }
    }
	Display_FullCGROM_SpecifiedPositions();
}

void Display_FullCGROM_SpecifiedPositions()
{
    uchar charCode = 0x01;  

    uchar positions[][2] = 
	{
    {7, 4}, {0, 5},{0, 6},{0, 7},{0, 8},
	};
    uchar num_positions = sizeof(positions) / sizeof(positions[0]); 

    for ( i = 0; i < num_positions; i++)
    {
        uchar x = positions[i][0];
        uchar y = positions[i][1];

        Locatexy(x, y,Character); 
        for ( j = 0; j < 20; j++) // 遍历每个位置显示内容
        {       
            WriteCommand(charCode, 0, 0xc0, 1);
            charCode++;
            if (charCode > 0xFF)
            {
                charCode = 0x01; 
            }
        }
    }
}


/*
显示16x16点阵的汉字字符

x：字符在LCD上的起始横坐标

y：字符在LCD上的起始纵坐标

Chinese_character：指向字库中的汉字数据（点阵数据）的指针

character_code：表示汉字的索引或编码，用来确定要显示的汉字在字库中的位置
*/
void Display_16_16(uchar x,uchar y,uchar  code *Chinese_character,uchar character_code)
{	
	//uchar i,j;    
	//uint num=0;
	//num=character_code*0x20;
	//Locatexy(x,y,Graphic);
	//for(j = 0; j < 16; j++)  // 循环显示16行
	//{
	//	for(i = 0; i < 2; i++)  // 每行2个字节，共16列
	//	{
	//		WriteCommand(*(Chinese_character + num), 0, 0xc0, 1);
	//		num++;
	//	}
	//	y += 1;  // 移动到下一行
	//	Locatexy(x, y, Graphic);  // 更新光标位置
	//}
	
	
	
	uchar j;    // 行循环控制变量
    uint num = 0;  // 偏移量初始为0
    num = character_code * 0x10;  // 每个字符占用16字节（8×16）
    
    Locatexy(x, y, Graphic);  // 定位到显示的起始位置

    for(j = 0; j < 16; j++)  // 控制16行的循环
    {
        WriteCommand(*(Chinese_character + num), 0, 0xc0, 1);  // 写入1个字节的点阵数据
        num++;  // 移动到字库中的下一个字节
        y += 1;  // 移动到下一行
        Locatexy(x, y, Graphic);  // 更新光标位置
    }

}
/*
显示8x16点阵的英文字符

x：字符在LCD上的起始横坐标

y：字符在LCD上的起始纵坐标

Chinese_character：指向字库中的英文数据（点阵数据）的指针

character_code：表示英文的索引或编码，用来确定要显示的英文在字库中的位置
*/
void Display_8_16(uchar x, uchar y, uchar code *English_character, uchar character_code)
{
    uchar j;    // 行循环控制变量
    uint num = 0;  // 偏移量初始为0
    num = character_code * 0x10;  // 每个字符占用16字节（8×16）
    
    Locatexy(x, y, Graphic);  // 定位到显示的起始位置

    for(j = 0; j < 16; j++)  // 控制16行的循环
    {
        WriteCommand(*(English_character + num), 0, 0xc0, 1);  // 写入1个字节的点阵数据
        num++;  // 移动到字库中的下一个字节
        y += 1;  // 移动到下一行
        Locatexy(x, y, Graphic);  // 更新光标位置
    }
}


/* 
函数用于在LCD屏幕上以图形模式填充一组点阵模式

DotByte：要显示的第一个字节数据(点阵模式),用于填充每行的前30个字节

DotByte1：要显示的第二个字节数据(点阵模式),用于填充每行的后30个字节
*/
void DisplayDots( uchar DotByte,uchar DotByte1 )   
{ 	uchar i,j;  
	Locatexy(0,0,Graphic);  
 	WriteCommand(0,0,0xb0,0 );//Set Data Auto Write  
	for(j = 0; j < 64; j++)  // 循环64行
	{
		for(i = 0; i < 30; i++)  // 每行的前30个字节写入DotByte
		{
			WriteData(DotByte);
		}

		for(i = 0; i < 30; i++)  // 每行的后30个字节写入DotByte1
		{
			WriteData(DotByte1);
		}
	} 
  	WriteCommand(0,0,0xb2,0 ); //Auto Reset
}  



/* 
函数用于在LCD屏幕的指定位置绘制位图图像
x:图像在LCD屏幕上的起始横坐标
y:图像在LCD屏幕上的起始纵坐标
W:图像的宽度（以像素为单位）
H:图像的高度（以像素为单位）
puts：指向位图图像数据的指针,数据以位为单位
*/
void DisplayBMP(uchar x,uchar y,uchar W,uchar H,  uchar code *puts )   
{  	uchar k,j;
	Locatexy(x,y,Graphic); 
		for(j = 0; j < H; j++)  // 遍历每一行
	{
		WriteCommand(0, 0, 0xb0, 0);  // 进入数据自动写入模式
		for(k = 0; k < W/8; k++)  // 每行遍历W/8个字节（8个像素为1字节）
		{
			WriteData(*puts++);  // 逐字节写入图像数据
		}
		WriteCommand(0, 0, 0xb2, 0);  // 退出数据自动写入模式
		y += 1;  // 移动到下一行
		Locatexy(x, y, Graphic);  // 更新光标位置
	}
  
} 
//图片取反
void ReverseDisplayBMP(uchar x,uchar y,uchar W,uchar H,  uchar code *puts )   
{  	uchar i,j;
	Locatexy(x,y,Graphic); 
      
	for(j=0;j<H;j++)
		{	WriteCommand(0,0,0xb0,0 ); 
		 	for(i=0;i<W/8;i++)
			{
				WriteData(~(*puts));//取反
				puts++;
			} 
		WriteCommand(0,0,0xb2,0 ); 
			y+=1;
			Locatexy(x,y,Graphic);
		}  
} 


//初始化
void LcmInit( void )   
{  
 	LCD_Reset=0;
 	Delay(50);
	LCD_Reset=1;
 	Delay(50);
	//Set Control Word
	WriteCommand(0,0,0x40,2 );   //Set Text Home Address
	WriteCommand(30,0,0x41,2 );  //Set Text Area   
	WriteCommand(0,0x08,0x42,2 );//Set Graphic Home Address    
	WriteCommand(30,0,0x43,2 );  //Set Graphic Area 
	
	WriteCommand(0,0,0xa7,0);//8-line cursor - Table 6-17
	WriteCommand(0,0,0x80,0);//OR Mode - Table 6-14
	
//	WriteCommand(0,0,0x81,0);//EXOR Mode - Table 6-14
//	WriteCommand(0,0,0x83,0);//AND Mode - Table 6-14
//	WriteCommand(0,0,0x84,0);//Text Attribute Mode - Table 6-14
//	WriteCommand(0,0,0x80,0);//Internal Character Generator Mode - Table 6-14
//	WriteCommand(0,0,0x90,0);//External Character Generator Mode  - Table 6-14
	
	WriteCommand(0,0,0x9c,0);//Text on, Graphic on - Table 6-16 
	
//	WriteCommand(0,0,0x90,0);//Display off
//	WriteCommand(0,0,0x92,0);//Cursor on, Blink off
//	WriteCommand(0,0,0x93,0);//Cursor on, Blink on 
}



uchar start_x[] = {32, 32, 34};  // 每组显示的起始X坐标
uchar start_y[] = {7, 32, 57};  // 每组显示的起始Y坐标
uchar start_code[] = {0, 16, 32};  // 每组显示的起始字符编码
uchar count[] = {16, 16, 12};  // 每组显示的字符数量
void Show_Eng(void)
{   
	unsigned char i;
	DisplayBMP(0,0,160,80,BMP0); 
	for ( group = 0; group < 3; group++)  // 共三个不同的显示区域
	{
		for ( i = 0; i < count[group]; i++)
		{
			Display_8_16(start_x[group] + i, start_y[group], EnglishCode, start_code[group] + i);
		}
	}

}
