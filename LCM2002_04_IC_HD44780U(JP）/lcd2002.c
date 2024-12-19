#include "LCD2002.h"
#include <REGX52.H>



sbit LCD_RS = P3^0; 
sbit LCD_RW = P3^1;
sbit LCD_EN = P3^2;
sbit   KEY1  =P2^0;	

unsigned int ReadKey1() // 按键检测函数
{
    if (KEY1 == 0) 
	{  // 按键被按下
        Delay(1);  // 简单的去抖动处理
        if (KEY1 == 0) 
		{  // 检查按键是否仍然按下
            while (KEY1 == 0);  // 等待按键释放
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
  * @brief  LCD2002写命令
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
  * @brief  LCD2002写数据
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
  
}

/**
  * @brief  LCD2002读数据
  * @param  Data 要写入的数据,搭配LCD_SetCursor()光标使用。
  * @retval 无
  */
void LCD_ReadData(unsigned char Data) 
{
	Busy();
    LCD_RS = 1; // 选择为数据，1为数据，0为指令
    LCD_RW = 1; // 选择为写， 1为读，0为写
	LCD_EN = 1;   // 使能脚E先上升沿写入
	Delay(0.1);
    LCD_DataPort = Data; // 写入指数据的内容
    LCD_EN = 0;   // 使能脚E后负跳变完成写入
}

/**
  * @brief  LCD2002初始化函数
  * @param  无
  * @retval 无
  */
void LCD_Init() 
{
    LCD_WriteCommand(0x38); // 设置20×4显示,5X7点阵,8位数据接口 
    LCD_WriteCommand(0x0c); // 显示开,光标关,闪烁关
//	LCD_WriteCommand(0x0f);//显示开,光标开.闪烁开
    LCD_WriteCommand(0x06); // 当读或写一个字符后地址指针加一,不移动
    LCD_WriteCommand(0x01); // 光标复位，清屏
	Delay(0.1); 
}

/**
  * @brief  LCD2002清屏
  * @param  无
  * @retval 无
  */
void LCD_Clear() 
{
    Busy();
    LCD_WriteCommand(0x01);
	Delay(0.1);           // 等待清屏完成
}

/**
  * @brief  LCD2002设置光标位置
  * @param  Line 行位置，范围：1~2
  * @param  Column 列位置，范围：1~20
  * @retval 无
  */
void LCD_SetCursor(unsigned int Line, unsigned int Column) 
{
    // 确保列值在0-19之间
    if (Column >= 20) Column = 0;
    // 确保行值在0-2之间
    Line &= 0x01;  // 限制 Y 位置在 0 到 3 之间		
    if (Line == 0) 
    {
        LCD_WriteCommand(0x80 | Column);  // 第一行
    } 
    else  
    {
        LCD_WriteCommand(0xC0 | Column);  // 第二行
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

// 
/**
 * @brief  显示字符函数
 * @param  Line0-3行 Column 0-19列 i字符
 * @retval
 */
void LCD_USER_ShowString(unsigned char Line, unsigned char Column, unsigned char  i) 
{			
	LCD_SetCursor(Line, Column); // 光标
	LCD_WriteData(i);
}


/**
 * @brief LCD2002显示字符串
 * @param Line 0-2行 Column 0-19列 *String 输入字符""
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


//每次写入的字符 a 是递增的
void write_CGROM(unsigned char a)
{
    unsigned char j;
    LCD_WriteCommand(0x02);  // 光标归位命令 (光标位置回到初始位置，通常是DDRAM的起始地址)
    Delay(1);                // 延时一段时间，确保命令有效
    LCD_WriteCommand(0x80);  // 设置光标到第1行的起始地址 (0x80对应DDRAM的第1行起始地址)
    
    // 向第1行写入40个字符数据
    for (j = 0; j < 40; j++) 
    {
        LCD_WriteData(a);    // 向LCD写入数据 'a'
        a += 1;              // 自增'a'，以便下一个字符是'a'的下一个值
    }

    LCD_WriteCommand(0xC0);  // 设置光标到第2行的起始地址 (0xC0对应DDRAM的第2行起始地址)
    
    // 向第2行写入40个字符数据
    for (j = 1; j < 40; j++) 
    {
        LCD_WriteData(a);    // 向LCD写入数据 'a'
        a += 1;              // 自增'a'，同样依次写入下一个字符
    }
    
    Delay(1);                // 再次延时，确保数据全部写入
}

/**
 * @brief  全屏显示HD44780 内置字符
 * @param  Line 0-3  Column 0-19  输入16进制0X...
 * @retval 无
 */
void Write_DDRAM(unsigned char a)
{
    unsigned char j;
    
    LCD_WriteCommand(0x02);  // 光标归位命令 (光标位置回到初始位置，通常是DDRAM的起始地址)
    Delay(1);               // 延时10个单位 (比之前的延时长)

    LCD_WriteCommand(0x80);  // 设置光标到第1行的起始地址 (0x80对应DDRAM的第1行起始地址)
    
    // 向第1行写入20个相同的字符   
    for (j = 0; j < 20; j++) 
    {
        LCD_WriteData(a);    // 将字符 'a' 写入 LCD 显示数据存储器
    }

    LCD_WriteCommand(0xC0);  // 设置光标到第2行的起始地址 (0xC0对应DDRAM的第2行起始地址)
    
    // 向第2行写入20个相同的字符
    for (j = 0; j < 20; j++) 
    {
        LCD_WriteData(a);    // 将字符 'a' 写入 LCD 显示数据存储器
    }  
    Delay(1);                // 延时，确保数据完全写入
}


// 向 LCD 的 CGRAM 写入自定义字符数据
void Write_CGRAM(unsigned char a[])
{
    unsigned char k;
    
    // 发送命令 0x40，选择 CGRAM 地址
    LCD_WriteCommand(0x40); 
    
    // 将自定义字符数据写入 CGRAM
    for(k = 0; k < 64; k++)
    {
        LCD_WriteData(a[k]);  // 写入字符数据
    }
}


void displayCustomCharacter1(const unsigned char pattern[8]) 
{
    unsigned char row, col;

    // 将自定义字符数组写入CGRAM
    Write_CGRAM(pattern);

    // 遍历LCD的每一行和每一列，显示自定义字符
    for (row = 0; row < 2; row++) 
    {
        for (col = 0; col < 20; col++) 
        {
            // 针对LCD2002的行起始地址做出修正
            unsigned char position;
            if (row == 0) 
				{
                position = 0x80 + col;   // 第1行地址 0x80 起始
				} else //if (row == 1) {
                position = 0xC0 + col;   // 第2行地址 0xC0 起始

            // 设置光标位置并显示自定义字符
            LCD_WriteCommand(position);
            LCD_WriteData(0x00);  // 显示存储在CGRAM中的第一个自定义字符
        }
    }
}

unsigned char data pattern1[8] = {0x15, 0x0A, 0x15, 0x0A, 0x15, 0x0A, 0x15}; 
unsigned char data pattern2[8] = {0x0A, 0x15, 0x0A, 0x15, 0x0A, 0x15, 0x0A};	
unsigned char data pattern3[8] = {0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15}; 
unsigned char data pattern4[8] = {0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A}; 
unsigned char data pattern5[8] = {0x1F, 0x00, 0x1F, 0x00, 0x1F, 0x00, 0x1F, 0x00}; 
unsigned char data pattern6[8] = {0x00, 0x1F, 0x00, 0x1F, 0x00, 0x1F, 0x00, 0x1F}; 	
unsigned char data pattern7[8] = {0x00, 0x15, 0x00, 0x15, 0x00, 0x15, 0x00, 0x15}; 
void HandleKey() 
{
	unsigned char mode;
	
		if (ReadKey1()) 
	  {  
		LCD_Clear();
		mode++;
		mode %= 10;			
		switch (mode) 
	  {
        case 0:            
			displayCustomCharacter1(pattern1); 
            break;
        case 1:
            displayCustomCharacter1(pattern2);  
            break;
        case 2:
            displayCustomCharacter1(pattern3); 
            break;
        case 3:
           displayCustomCharacter1(pattern4);  
            break;
        case 4:
            displayCustomCharacter1(pattern5);  
            break;
        case 5:
            displayCustomCharacter1(pattern6);  
            break;
		case 6:
			displayCustomCharacter1(pattern7);
			break;
		case 7:
			write_CGROM(0x21); 
		   break;
		case 8:
			write_CGROM(0xA1); 
		   break;
		case 9:
			Write_DDRAM(0xFF); 
		   break;  
		}
	}
}

void DisplayPatterns() 
{
    static int step = 0; // 当前显示图案步骤
    switch (step) 
    {
        case 0: displayCustomCharacter1(pattern1);  Delay(80); break;
        case 1: displayCustomCharacter1(pattern2);  Delay(80); break;
        case 2: displayCustomCharacter1(pattern3);  Delay(80); break;
        case 3: displayCustomCharacter1(pattern4);  Delay(80); break;
        case 4: displayCustomCharacter1(pattern5);  Delay(80); break;
        case 5: displayCustomCharacter1(pattern6);  Delay(80); break;
        case 6: displayCustomCharacter1(pattern7);  Delay(80); break;
        case 7: write_CGROM(0x21); Delay(80); break;
        case 8: Write_DDRAM(0xCE); Delay(80); break;
        case 9:	LCD_Clear();LCD_ShowString(1,6,"LCM2002_4");LCD_ShowString(0,2,"YeHuiDisplay.com");Delay(80); break;
    }
    step++;
    if (step > 9) step = 0; // 循环
}