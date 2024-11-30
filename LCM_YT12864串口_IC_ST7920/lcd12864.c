#include <REGX52.H>
#include "intrins.h"
#include "lcd12864.h"
#include "picture.h"
sbit CS   =P3^0;  //片选
sbit STD  =P3^1;  //串行数据
sbit SCLK =P3^2;  //串口时钟
sbit KEY1 = P2^0;

unsigned char i,X,Y,XPosition,YPosition,FCHARBUF;

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


void Delay(unsigned char xms)	//@12.000MHz
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

/**
 * @brief  一位数据(bit d)通过引脚发送到液晶显示控制器
 * @param  
 * @retval None
 */

void Trans_Bit(bit d)  // 将一位数据传输到液晶显示控制器
{
   STD = d;       // 先将传入的 bit 数据 d 送到数据引脚 STDPIN 上
   //_nop_();          
   SCLK = 1;      // 将时钟引脚 SCLKPIN 置高（开始一个时钟周期）
   //_nop_();        
   SCLK = 0;      // 将时钟引脚 SCLKPIN 置低（结束负脉冲的下降沿）
   //_nop_();       
   SCLK = 1;      // 将时钟引脚 SCLKPIN 置高（准备下一次数据传输）
}



/**
  * @brief  向LCD12864发一字节显示数据/指令  
  * @param  d
  * @retval None
  */

void Trans_Byte(unsigned char d)
{
    unsigned char i;
    for(i = 0; i < 8; i++)//一个字节
    {
        if((d & 0x80) == 0x80)//检查数据字节的最高位
            Trans_Bit(1);
        else
            Trans_Bit(0);
        d <<= 1;  // 左移后赋值
    }            
}



/**
  * @brief  向LCD12864写指令  
  * @param  None
  * @retval None
  */
void Write_Command(unsigned char e)
{
    CS = 1; //11111 00 0    // 激活片选信号
    Trans_Byte(0xf8);      // 发送控制字节,进行同步 P26 P10
    Trans_Byte(e & 0xf0);  // 发送命令的高四位
    Trans_Byte((e & 0x0f) << 4); // 发送命令的低四位
    CS = 0;                // 取消片选信号
}



/**
  * @brief  向LCD12864写数据  
  * @param  
  * @retval None
  */
void Write_Data(unsigned char e)
{
    CS = 1;  //11111 01 0   // 激活片选信号
    Trans_Byte(0xfa);      // 发送控制字节，进行同步 	P26 P10
    Trans_Byte(e & 0xf0);  // 发送数据的高四位
    Trans_Byte((e & 0x0f) << 4); // 发送数据的低四位
    CS = 0;                // 取消片选信号
}



/**
  * @brief  向LCD12864读状态  
  * @param  
  * @retval None
  */
void Write_Status(unsigned char e)
{
    CS = 1;  //11111 10 0   // 激活片选信号
    Trans_Byte(0xfc);      // 发送控制字节，进行同步 	P26 P10
    Trans_Byte(e & 0xf0);  // 发送数据的高四位
    Trans_Byte((e & 0x0f) << 4); // 发送数据的低四位
    CS = 0;   
}


/**
  * @brief  向LCD12864读数据  
  * @param  
  * @retval None
  */
void Read_Data(unsigned char e)
{
	CS=1;   //0x11111 11 0
    Trans_Byte(0xfc);      // 发送控制字节，进行同步 	P26 P10
    Trans_Byte(e & 0xf0);  // 发送数据的高四位
    Trans_Byte((e & 0x0f) << 4); // 发送数据的低四位
    CS = 0;   
}


/**
  * @brief  向LCD12864图形显示开
  * @param  None
  * @retval None
  */
void  Graphic_Display_ON(void)
{           //0x0011 0110
	Write_Command(0x36);  //P17
	Write_Command(0x30);  //恢复为基本指令集 P16
}
	

/**
  * @brief  向LCD12864图形显示关
  * @param  None
  * @retval None
  */
void  Graphic_Display_OFF(void)
{ 			//0x0011 0100
	Write_Command(0x34);  //P17
	Write_Command(0x30);  //恢复为基本指令集
}	

/**
  * @brief  显示开
  * @param  None
  * @retval None
  */
void  Display_ON(void)
{
	Write_Command(0x30);
	Write_Command(0x0c);//P20
}		  //0x0000 1100

/**
  * @brief  显示关
  * @param  None
  * @retval None
  */
void  Display_OFF(void)
{
	Write_Command(0x30);
	Write_Command(0x08);//P20
}		  //0x0000 1000


/**
  * @brief  反白关
  * @param  None
  * @retval None
  */
void  Versawhite_off(void)
{
	Write_Command(0x30);
	Write_Command(0x0e);//P20
}          //0x0000 1110


/**
  * @brief  反白开
  * @param  None
  * @retval None
  */
void  Versawhite_on(void)
{
	Write_Command(0x30);
	Write_Command(0x0f);//P20
}         //0x0000 1111


/**
  * @brief  显示清除
  * @param  None
  * @retval None
  */

void Display_Clear(void)
{
	Write_Command(0x30);
	Write_Command(0x01);//P19
}	     //0x0000 0001
 

/**
  * @brief  光标回到起始位
  * @param  None
  * @retval None
  */
void Return_Home(void)
{
	Write_Command(0x30);
	Write_Command(0x02);//P19
}         //0x0000 0010



/**
  * @brief  设置绘图区域内部RAM指针
  * @param  None
  * @retval None
  */

void Set_DrawingPosition(void)
{    
	Write_Command(0x36);//绘图指令
	Write_Command((YPosition&0x1f)| 0x80);//0x1f->>0-31,发送y轴地址 	P24
	if(YPosition>=32)//下半屏
		Write_Command((XPosition/16+8) | 0x80);//发送x轴地址
	else
		Write_Command((XPosition/16) | 0x80);
	
	Write_Command(0x30);//恢复基本指令
}




/**
  * @brief  屏幕填充为某个字节数据 DATA
  * @param  DATA
  * @retval None
  */
void FillScreen(unsigned char DATA)  // 整屏显示指定的字节数据
{
	unsigned char i;
	YPosition=0;XPosition=0;
    while (YPosition < 64)  // 遍历所有的Y轴行数
    {         
        Set_DrawingPosition();  // 设置当前的绘图位置
        for (i = 1; i <16; i++)  // 填充一行中的18个字节数据
        {
            Write_Data(DATA);  // 将指定的字节数据写入到当前绘图位置
        }
        YPosition++;  // 移动到下一行
    }   
    YPosition = 0;  // 完成后将Y轴位置重置
}

	


/**
  * @brief  屏幕初始化
  * @param  None
  * @retval None
  */
 void Lcd12864spi_init(void)
{

	Write_Command(0x30);
	Delay(1);
	Write_Command(0x30);
	Delay(1);
	Write_Command(0x08);
	Delay(1);
	Write_Command(0x10);
	Delay(1);
	Write_Command(0x0c);
	Delay(1);
	Write_Command(0x01);
	Delay(1);
	Write_Command(0x06);

}

/**
  * @brief  屏幕逐个显示字符。
  * @param  None
  * @retval None
  */

void moveToNextCharPosition(void)  // 移动光标到下一个字符位置
{
	
    X++;                       // 字符位置加1
    X &= 0x0F;                 // 限制CXPOS在0到15的范围内

    if (X == 0)                // 如果CXPOS为0，表示需要换行
    {   
        Y++;                   // 行位置加1
        Y &= 0x3;             // 限制CYPOS在0到3的范围内
    }
}





/**
  * @brief  设置字符光标在LCD内部RAM的地址
  * @param  None
  * @retval None
  */
void setLcdCursorPosition(void)  
{
	unsigned char addDDRAM;
	X &= 0xF;// 限制在0-15列 
	addDDRAM = X / 2;//每两个字符共享一个 DDRAM 单元
	if (Y == 0) Write_Command(addDDRAM | 0x80);// DDRAM地址:80H-87H
	else if (Y == 1) Write_Command(addDDRAM | 0x90);// DDRAM地址:90H-97H
	else if (Y == 2) Write_Command(addDDRAM | 0x88);// DDRAM地址:88H-8FH
	else if (Y == 3) Write_Command(addDDRAM | 0x98);// DDRAM地址:98H-9FH
}	



/**
  * @brief  处理图形数据
  * @param  imageData[] 图像数组
  * @retval None
  */
void drawimge(unsigned char code *imgeData)
{
    unsigned char width, height;
    
    // 获取图像的宽度
    width = *imgeData;
    imgeData++;  // 指针向前移动，指向图像高度
    
    // 获取图像的高度
    height = *imgeData;
    imgeData++;  // 指针向前移动，指向实际图像数据
    
    // 显示图像
    drawImageWithSize(width, height, imgeData);
}


//                                    128                  64
void drawImageWithSize(unsigned char width, unsigned char height, unsigned char code *imgeData)
{
    unsigned char pixelX, pixelY, x;
     x = XPosition;  // 保存当前X位置

    // 遍历图像的高度（逐行显示）
    for (pixelY = 0; pixelY < height; pixelY++, YPosition++)  // 每次遍历一行，GYPOS增加一行
    {
        // 遍历图像的宽度
        for (XPosition = x, pixelX = 0; pixelX < width;)
        {	if ((XPosition & 0x08) == 0) Set_DrawingPosition();  // 根据位置设置光标
            Write_Data(*imgeData);                   // 显示当前字节的图像数据
            imgeData++;                         // 移动到下一个字节
            pixelX += 8;                         // X位置每次移动8个像素（每个字节8像素）
            XPosition += 8;                          // 光标在X方向移动8个像素
        }
    }

    XPosition = x;  // 恢复X方向的起始位置
}


/**
  * @brief  自定义字符(点阵图形)写入LCD12864的字符生成 RAM
  * @param  charindex:0-7,pattern[] 图像数组
  * @retval None
  */
void setchartoCGRAM(unsigned char charindex ,unsigned char code *pattern)
{
	unsigned char i;
	 // 切换到扩展指令集并激活 CGRAM
    Write_Command(0x34);  // 进入扩展指令集，图形模式关闭
    Write_Command(0x02);  // 设置 SR = 0，激活 CGRAM
    Write_Command(0x30);  // 返回基本指令集
	
	// 设置 CGRAM 地址 (低 2 位取自 charIndex，并与 0x40 合并)
	Write_Command(((charindex &0x3)<<4) |0x40);//設定CGRAM位址,P21
	for(i=0;i<32;i++)
		Write_Data(*pattern); // 写入并移动到下一个字节
	
}



/**
 * @brief 在指定的 (cx, cy) 位置显示字符串
 * @param x 0-4 字符的X坐标位置
 * @param y 0-4 字符的Y坐标位置
 * @param s 要显示的字符串
 */
void DisplaystringAtposition(unsigned char x ,unsigned char y ,unsigned char code*s)
{
	X=x;
	Y=y;
	DisplayString(s);
}

       


/**
 * @brief 显示字符串，直到遇到字符码为0
 * @param str 要显示的字符串
 */
void DisplayString(unsigned char code *str) 
{
    unsigned int charCode;

    while (1) 
    {
        charCode = *str;  // 读取当前字符
        str++;  // 移动到下一个字符

        if (charCode == 0) break;  // 如果字符码为0，结束字符串显示

        if (charCode < 128)
        {
            putchar(charCode);  // 显示ASCII字符
        }
        else
        {
            // 处理扩展字符（高位和低位字节组合）
            putchar(charCode * 256 + *str);
            str++;  // 跳过下一个字符（低位字节）
        }
    }
}


/**
 * @brief 显示字符到LCD屏幕上，并处理字符类型和自动对齐
 *
 * 该函数用于在LCD上显示字符，支持ASCII字符和汉字的显示，并处理字符位置的对齐。
 * 对于汉字（字符码大于128），会自动处理两个字节并根据当前光标位置进行对齐；
 * 对于ASCII字符（字符码小于128），如果在奇数位置会进行适当的对齐补充。
 * 
 * @param c 要显示的字符，可能是ASCII字符或汉字字符。
 * 
 * @note 
 * - 汉字字符码大于128，且占两个字节（高字节和低字节）。该函数会自动分段显示。
 * - 自定义字符的高字节为0xFF，转换为ST7920控制器的字符码格式。
 */
void putchar(unsigned int c) 
{
    if (c > 128) // 字符码大于128表示为汉字
    {
        if ((X & 0x1) == 1) // 如果CXPOS字符位置在奇数位置
        {
            Write_Data(' '); // 自动补添一个空格对齐
            moveToNextCharPosition(); // 置字符位置为下一个有效位置
        }
        setLcdCursorPosition(); // 设置字符位置
        if ((c & 0xff00) == 0xff00) // 如果高位字节为0xFF表示自定义字符
        {
            c = 0xffff - c; // 转换为ST7920的字符码
        }
        Write_Data(c / 256); // 256写高位字符
        moveToNextCharPosition(); // 置字符位置为下一个有效位置
        Write_Data(c & 0xff); // 写低位字符
        moveToNextCharPosition(); // 置字符位置为下一个有效位置
    }
    else // 字符码小于128表示为ASCII字符
    {
        setLcdCursorPosition(); // 设置字符位置
        if ((X& 0x1) == 1) // 如果CXPOS字符位置在奇数位置
        {
            Write_Data(FCHARBUF); // 重新写高位字符缓冲区内容
            Write_Data(c); // 写当前字符
        }
        else // 如果CXPOS字符位置在偶数位置
        {
            Write_Data(c); // 直接写当前字符
            FCHARBUF = c; // 保存当前字符到高位字符缓冲变量
            Write_Data(' '); // 自动补显一个空格
        }
        moveToNextCharPosition(); // 置字符位置为下一个有效位置
    }
}



/**
 * @brief 在整个LCD屏幕上填充指定的ASCII字符
 * 该函数会在LCD的每个字符位置显示由参数 `c` 代表的ASCII字符。
 * 它通过循环逐个字符地在屏幕上显示，直到返回到初始位置（0,0），从而完成整个屏幕的填充。
 * @param c 要显示的ASCII字符
 */
void charlcdfill(unsigned int c) // 整屏显示指定ASCII字符的子程序
{   
    for (X = Y = 0; 1;) // 初始化X和Y坐标为0,0，并开始无限循环
    {   
        putchar(c); // 调用putchar函数，在当前位置显示字符
        if ((X == 0) && (Y == 0)) break; // 如果已经返回到屏幕起始位置(0,0)，则退出循环
    }
}





 
void FillScreenWithPattern(unsigned char pattern)
{    
    unsigned char x, y;

    for (y = 0; y < 64; y++) // 遍历Y轴的64个位置（0-63），屏幕高度为64像素
    {
        YPosition = y;         // 设置Y轴位置
        Set_DrawingPosition(); // 设置当前的绘图位置

        for (x = 0; x < 16; x++) // 遍历X轴的16个位置（每行16字节）
        {
            Write_Data(pattern); // 在当前X位置显示传入的图案数据
        }
    }
    YPosition = 0; // 完成后将Y轴位置重置
}



//麻点
void FillCheckerboardPattern(void)
{
    unsigned char x, y;

    for (y = 0; y < 64; y++)  // 遍历Y轴的64个位置
    {
        YPosition = y;
        Set_DrawingPosition();

        for (x = 0; x < 16; x++)  // 遍历X轴的16个字节
        {
            if (y % 2 == 0)   // 偶数行显示0xAA
            {
                Write_Data(0x55);
            }
            else  // 奇数行显示0x55
            {
                Write_Data(0xaa);
            }
        }
    }
}
void ReverseFillCheckerboardPattern(void)
{
    unsigned char x, y;

    for (y = 0; y < 64; y++)  // 遍历Y轴的64个位置
    {
        YPosition = y;
        Set_DrawingPosition();

        for (x = 0; x < 16; x++)  // 遍历X轴的16个字节
        {
            if (y % 2 == 0)   // 偶数行显示0xAA
            {
                Write_Data(0xaa);
            }
            else  // 奇数行显示0x55
            {
                Write_Data(0x55);
            }
        }
    }
}

//垂直条纹图案
void FillVerticalStripePattern(void)
{
    unsigned char x, y;

    for (y = 0; y < 64; y++)  // 遍历Y轴
    {
        YPosition = y;
        Set_DrawingPosition();

        for (x = 0; x < 16; x++)  // 遍历X轴
        {
            Write_Data(0xaa);  // 每个字节上半部分全亮，下半部分全暗
        }           //1010 1010
    }               //0x55->01010 01010
}
void ReverseFillVerticalStripePattern(void)
{
    unsigned char x, y;

    for (y = 0; y < 64; y++)  // 遍历Y轴
    {
        YPosition = y;
        Set_DrawingPosition();

        for (x = 0; x < 16; x++)  // 遍历X轴
        {
            Write_Data(0x55);  // 每个字节上半部分全亮，下半部分全暗
        }           //1010 1010
    }               //0x55->01010 01010
}


//水平条纹图案
void FillHorizontalStripePattern(void)
{
    unsigned char x, y;

    for (y = 0; y < 64; y++)  // 遍历Y轴
    {
        YPosition = y;
        Set_DrawingPosition();

        for (x = 0; x < 16; x++)  // 遍历X轴
        {
            if (y % 2 == 0)  // 偶数行显示全亮
            {
                Write_Data(0xFF);
            }
            else  // 奇数行显示全暗
            {
                Write_Data(0x00);
            }
        }
    }
}
void ReverseFillHorizontalStripePattern(void)
{
    unsigned char x, y;

    for (y = 0; y < 64; y++)  // 遍历Y轴
    {
        YPosition = y;
        Set_DrawingPosition();

        for (x = 0; x < 16; x++)  // 遍历X轴
        {
            if (y % 2 == 0)  // 偶数行显示全亮
            {
                Write_Data(0x00);
            }
            else  // 奇数行显示全暗
            {
                Write_Data(0xff);
            }
        }
    }
}

//斜线图案
void FillDiagonalLinePattern(void)
{
    unsigned char x, y;

    for (y = 0; y < 64; y++)  // 遍历Y轴
    {
        YPosition = y;
        Set_DrawingPosition();

        for (x = 0; x < 16; x++)  // 遍历X轴
        {
            Write_Data(1 << (y % 8));  
        }
    }

}




