#include "LCD1602.h"
#include <REGX52.H>



sbit LCD_RS = P3^0; 
sbit LCD_RW = P3^1;
sbit LCD_EN = P3^2;
sbit   KEY1  = P2^0;	

extern unsigned int i,j,maxSteps;


unsigned int ReadKey1() // 按键检测函数
{
    if (KEY1 == 1) 
	{  // 按键被按下
        Delay(1);  // 简单的去抖动处理
        if (KEY1 == 1) 
		{  // 检查按键是否仍然按下
            while (KEY1 == 1);  // 等待按键释放
            return 1;  // 返回按键被按下
        }
    }
    return 0;  // 按键没有被按下
}







void  Delay(xms)	//@12.000MHz
{
while(xms--){
	unsigned char data i, j;

	i = 12;
	j = 169;
	do
	{
		while (--j);
	} while (--i);
}
	}


//判断忙不忙
void Busy()
{
	unsigned char temp;
	LCD_DataPort=0xff;
	LCD_RS=0;//RW=1,RS=0读状态
	LCD_RW=1;
	
	do{
		LCD_EN=1;
		temp=LCD_DataPort;
		LCD_EN=0;
	}while(temp&0x80);}/*对控制器每次进行读写操作之前， 
					  都必须进行读写检测，确保STA7为0*/



/**
  * @brief  LCD1602写命令
  * @param  Command 要写入的命令
  * @retval 无
  */
void LCD_WriteCommand(unsigned char Command) 
{
	Busy();
    LCD_RS = 0; // 选择为指令，1为数据，0为指令
    LCD_RW = 0; // 选择为写， 1为读，0为写
    LCD_DataPort = Command; // 写入指令的内容
    LCD_EN = 1;   // 使能脚E先上升沿写入
    Delay(0.1);
    LCD_EN = 0;   // 使能脚E后负跳变完成写入
}



/**
  * @brief  LCD1602写数据
  * @param  Data 要写入的数据,搭配LCD_SetCursor()光标使用。
  * @retval 无
  */
void LCD_WriteData(unsigned char Data) 
{
	Busy();
    LCD_RS = 1; // 选择为数据，1为数据，0为指令
    LCD_RW = 0; // 选择为写， 1为读，0为写
    LCD_DataPort = Data; // 写入指数据的内容
    LCD_EN = 1;   // 使能脚E先上升沿写入
    Delay(0.1);
    LCD_EN = 0;   // 使能脚E后负跳变完成写入
	Delay(0.1);
  
}



/**
  * @brief  LCD1602初始化函数
  * @param  无
  * @retval 无
  */
void LCD_Init() 
{
    LCD_WriteCommand(0x38); // 设置16X2显示,5X7点阵,8位数据接口 
    LCD_WriteCommand(0x0c); // 显示开,光标开,闪烁开
    LCD_WriteCommand(0x06); // 当读或写一个字符后地址指针加一,且光标加一,画面不动
    LCD_WriteCommand(0x01); // 光标复位，清屏
	Delay(1.5); 
}

/**
  * @brief  LCD1602清屏
  * @param  无
  * @retval 无
  */
void LCD_Clear() 
{
    Busy();
    LCD_WriteCommand(0x01);
	Delay(1.5);           // 等待清屏完成
}


/**
  * @brief  LCD1602设置光标位置
  * @param  Line 行位置，范围：1~2
  * @param  Column 列位置，范围：1~16
  * @retval 无
  */
void LCD_SetCursor(unsigned char Line, unsigned char Column) 
{
	
			if (Line == 1) 
				{
					LCD_WriteCommand(0x80 | (Column - 1)); // 第一行
				} 
			else  
				{
					LCD_WriteCommand(0x80 | (Column - 1 + 0x40)); // 第二行
				}
	
}
	
 
/**
 * @brief LCD1602显示字符串
 * @param Line1-2行 Column 1-16列 *String 输入字符""
 * @retval 无
 */
void LCD_ShowString(unsigned char Line, unsigned char Column, char *String) 
{	unsigned char i;		
	LCD_SetCursor(Line, Column);
    for (i = 0; String[i] != '\0'; i++) 
	{
      LCD_WriteData(String[i]);
    }
}


unsigned char LCD_Readdata(void)               //读数据子程序
{   unsigned char d;
    Busy();                       //HD44780液晶显示控制器忙检测
    LCD_DataPort=0xff;
    LCD_RS=1;                             //RS=1 RW=1 E=高电平
    LCD_RW=1;
    LCD_EN=1;
    Delay(0.005);
    d=LCD_DataPort;
    LCD_EN=0;
    return d;
}




/**
 * @brief  显示字符函数
 * @param  Line1-2行 Column 1-16列 i字符
 * @retval
 */
void LCD_USER_ShowString(unsigned char Line, unsigned char Column, unsigned char  i) 
{
			
			LCD_SetCursor(Line, Column); // 光标
			LCD_WriteData(i );
}


/**
 * @brief  全屏显示HD44780 内置字符
 * @param  0x...输入16进制
 * @retval 无
 */
unsigned int Line,Column;
void HD44780_DisplayChar(unsigned char pattern) {
    for (Line = 1; Line <= 2; Line++) {
        for (Column = 1; Column <= 16; Column++) {
            LCD_USER_ShowString(Line, Column, pattern);
        }
    }
}




// 存入自定义字符函数：向LCD1602内部的CGRAM中存入自定义字符
void LCD_USER_SetCustomChar(const char data  *table, unsigned char num) 
{
	
		switch (num) {
        case 1: LCD_WriteCommand(0x40); break;
        case 2: LCD_WriteCommand(0x48); break;
        case 3: LCD_WriteCommand(0x50); break;
        case 4: LCD_WriteCommand(0x58); break;
        case 5: LCD_WriteCommand(0x60); break;
        case 6: LCD_WriteCommand(0x68); break;
        case 7: LCD_WriteCommand(0x70); break;
        case 8: LCD_WriteCommand(0x78); break;
    }
    for (i = 0; i < 8; i++) {
        LCD_WriteData(table[i]);
    }
}



//写入CGRAM
void Write_CGRAM(unsigned char  a[])
{
	unsigned char i=64,k;
	LCD_WriteCommand(0x40);
	for(k=0;k<64;k++)
	 {
	  LCD_WriteData(a[k]);
	 }
}





