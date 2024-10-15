#include <INTRINS.H>        
#include <MATH.H>    
#include <ABSACC.H> 
#include <REGx52.H> 
#include <STDIO.H> 
#include "LCD12864.H"
#include "HANZI.H"



sbit LCD12864_RS = P3^0; 
sbit LCD12864_RW = P3^1;
sbit LCD12864_E  = P3^2;
sbit CS1=P3^3;       	// 左半屏
sbit CS2=P3^4;		 	// 右半屏
//sbit LCD12864_RST=P3^5;	//复位
//sbit int0=P2^0;		//中断
sbit KEY=P2^0;			//按键


//bit loge=0; 


#define uint unsigned int
#define uchar unsigned char

unsigned char column;  //列
unsigned char Page;    //页
unsigned char code BMP3[]; 
unsigned char code BMP4[]; 
unsigned char code font[];
unsigned char code Character[];



uchar ReadKey() //按键函数
{
    if (KEY == 1) 
		{  // 按键被按下
			Delay(100);  // 简单的去抖动处理
			if (KEY == 1) 
				{
					while (KEY == 1);  // 等待按键释放
					return 1;  // 返回按键被按下
				}
    }
    return 0;  // 返回按键没有被按下
}



void Lcd12864_BusyL(void) // 测左半屏
{    
    uchar temp = 0xFF; // 初始化变量 temp 为 0xFF
    CS1 = 1;          // 使能选择信号 CS1
    CS2 = 0;          // 禁用选择信号 CS2
    LCD12864_RS = 0;  // 设置 RS 端口为 0，选择命令寄存器
    LCD12864_RW = 1;  // 设置 WR 端口为 1，切换为读取模式
    P1 = 0xFF;        // 将端口1设置为 0xFF
    
    while ((temp & 0x80) == 0x80) // 循环直到忙碌标志位被清除
    {
        LCD12864_E = 1;  // 使能 LCD12864（将 E 端口设为高电平）
        //_nop_();         // 空操作，可能用于延时
        temp = P1;       // 读取端口1的状态到 temp
        LCD12864_E = 0;  // 禁用 LCD12864（将 E 端口设为低电平）
    }
}



void Lcd12864_BusyR(void) // 测右半屏
{
    uchar temp = 0xFF;// 初始化变量 temp 为 0xFF
    CS1 = 0;          // 禁用选择信号 CS1
    CS2 = 1;          // 使能选择信号 CS2
    LCD12864_RS = 0;  // 设置 RS 端口为 0，选择命令寄存器
    LCD12864_RW = 1;  // 设置 WR 端口为 1，切换为读取模式
    P1 = 0xFF;        // 将端口1设置为 0xFF
    
    while ((temp & 0x80) == 0x80) // 循环直到忙碌标志位被清除
    {
        LCD12864_E = 1;  // 使能 LCD12864（将 E 端口设为高电平）
        //_nop_();         // 空操作，可能用于延时
        temp = P1;       // 读取端口1的状态到 temp
        LCD12864_E = 0;  // 禁用 LCD12864（将 E 端口设为低电平）
    }
}



void SetPage_Column(void)   // 设置光标位置，页，列
{   
    unsigned char x, y;

    // 根据列地址的高 2 位选择处理函数
    switch (column & 0xC0)   /*0X1100 0000,保持低6位就是0-63*/
			{
        case 0:    Lcd12864_BusyL(); break;   //0X0000处理左半部分0-63
        case 0x40: Lcd12864_BusyR(); break;   //0X0100处理右半部分64-127
			}
   
    // 计算列地址 (y)
    y = (column & 0x3f) | 0x40;  //0X0011 1111保留低 6 位并设置列地址基准值 0x40

    // 计算页地址 (x)
    x = (Page & 0x07)   | 0xb8;  //0X0000 0111保留低 3 位并设置页地址基准值 0xb8

    // 发送页地址到 LCD
    LCD12864_RS = 0;      
    LCD12864_RW = 0;      
    P1 = x;  
    LCD12864_E = 1;   
    //_nop_();   
    LCD12864_E = 0;   
    //_nop_();
    
    // 发送列地址到 LCD
    LCD12864_RS = 0;      
    LCD12864_RW = 0;      
    P1 = y;  
    LCD12864_E = 1;   
    //_nop_();   
    LCD12864_E = 0;   
    //_nop_();
}


void WriteCommand_L(uint Command)// 写左指令1-64
{
		  Lcd12864_BusyL();
		  P1 = Command ;   
		  LCD12864_RS = 0;   
		  LCD12864_RW = 0;   
		  LCD12864_E  = 1;   
		  //_nop_();   
		  LCD12864_E  = 0;   
		  //_nop_();
			
}





void WriteCommand_R(uint Command)// 写右指令65-128
{
		  Lcd12864_BusyR();
		  P1 = Command; 
		  LCD12864_RS = 0;   
		  LCD12864_RW = 0;   
		  LCD12864_E  = 1;   
		  //_nop_();   
		  LCD12864_E  = 0;   
		  //_nop_();
			
}


uchar ReadDate(void)// 读数据
{	
			uchar Date;
			SetPage_Column();
			LCD12864_RS=1;
			LCD12864_RW=1;
			P1=0xff;
			_nop_(); 
			LCD12864_E=1;
			_nop_();
			Date=P1;
			LCD12864_E=0;
			_nop_();
			CS1=0;
			CS2=0;
			return Date;
}

void WriteDate(uint Date)//写数据
{	
			SetPage_Column();
			LCD12864_RS=1;
			LCD12864_RW=0;
			P1 = Date; 
			LCD12864_E=1;
			_nop_(); 
			LCD12864_E=0;
			_nop_();
			CS1=0;
			CS2=0;
}


void Clear(void) // 清屏
{
		 Page = 0;   
		 column  = 0;   
		 for(Page=0;Page<8;Page++) //页  
		  for(column=0;column<128;column++) //列  
		   WriteDate(0); 
						
}


void Lcd12864_Init(void)//初始化
{
		 WriteCommand_L(0x3f); //显示开
		 WriteCommand_R(0x3f);   
			 
		 WriteCommand_L(0xc0); //显示开始行	P13
		 WriteCommand_R(0xc0);   
			 
		 WriteCommand_L(0x3f); //显示开  
		 WriteCommand_R(0x3f);   
	
		 P1 = 0xFF; // 假设按键接在P1口，设置为输入
}




void Showpicture(uchar DotByte,uchar DotByte1 ) //并在这些位置上写入指定的图案数据  
{   
		Page 	= 0;   
		column  = 0;   
			 for(Page=0;Page<8;Page++)//行像素（由 8 页组成）
			 {   
					for(column=0;column<128;column++) //128 列  
					{   
					 WriteDate( DotByte );
					 column++;
					 WriteDate( DotByte1 );  
					} 
			
				}   
}





void Showpicture_VerticalWave(uchar DotByte, uchar DotByte1)//中间往两边波浪般
{
    unsigned char i;
    unsigned char mid_col = 64;    // 屏幕中间列
    unsigned char delay_time = 50; // 延迟时间，用于控制波动速度

    // 从中间向上下绘制波动图案
    for (i = 0; i < 64; i++) 
    {
        // 向上绘制波动图案
        for (Page = 0; Page < 8; Page++) 
        {
            SetPage_Column();
            for ( column = 0; column < 128; column++) 
            {
                // 如果当前列在波动范围内，则显示点图案
                if ((column >= mid_col - i) && (column <= mid_col + i)) 
                {
                    WriteDate(DotByte);
                }
                else 
                {
                    WriteDate(0x00); // 填充背景色
                }
            }
        }
        
        // 向下绘制波动图案
        for ( Page = 0; Page < 8; Page++) 
        {
            SetPage_Column();
            for ( column = 0; column < 128; column++) 
            {
                // 如果当前列在波动范围内，则显示点图案
                if ((column >= mid_col - i) && (column <= mid_col + i)) 
                {
                    WriteDate(DotByte1);
                }
                else 
                {
                    WriteDate(0x00); // 填充背景色
                }
            }
        }

        // 添加延迟，以便视觉效果明显
        Delay(delay_time);
    }

    // 清除屏幕内容
    for ( Page = 0; Page < 8; Page++) 
    {
        SetPage_Column();
        for (column = 0; column < 128; column++) 
        {
            WriteDate(0); // 清除内容
        }
    }
}







/*中间向两边同时展开的效果*/
void Showpicture_Advanced(uchar DotByte, uchar DotByte1)
{
    unsigned char col;
    unsigned char mid_col = 64;  // 中间列，宽度的一半
    unsigned char delay_time = 1000; // 减少延迟时间，加快速度

    // 遍历所有页
    for (Page = 0; Page < 8; Page++)
    {
        for (col = 0; col <= mid_col; col++)
        {
            // 同时从中间向左和向右绘制图案
            column = mid_col - col;
            SetPage_Column(); // 设置页和列位置
            WriteDate(DotByte);
            WriteDate(DotByte1);

            column = mid_col + col;
            SetPage_Column(); // 设置页和列位置
            WriteDate(DotByte);
            WriteDate(DotByte1);
        }

        // 为了加快速度，减少延迟
        Delay(delay_time);
    }

    // 清除屏幕内容
    for (Page = 0; Page < 8; Page++)
    {
        for (col = 0; col < 128; col++)
        {
            column = col;
            SetPage_Column(); 
            WriteDate(0); // 清除内容
        }
    }
}




/* 从两边向中间同时收缩的效果（奇数行），逐渐加速和减速 */
void Showpicture_Advanced1(uchar DotByte, uchar DotByte1)
{
    unsigned char col;
    unsigned char mid_col 		= 64;   // 中间列，宽度的一半
    unsigned char delay_time 	= 1000; // 初始延迟时间
    unsigned char speed_factor 	= 10;   // 加速/减速因子

    // 从中间向外扩展绘制图案
    for (Page = 0; Page < 8; Page++)
    {
        for (col = 0; col <= mid_col; col++)
        {
            // 从中间向两侧扩展
            column = mid_col + col; // 从中间向右扩展
            SetPage_Column(); // 设置页和列位置
            WriteDate(DotByte);
            WriteDate(DotByte1);

            column = mid_col - col; // 从中间向左扩展
            SetPage_Column(); // 设置页和列位置
            WriteDate(DotByte);
            WriteDate(DotByte1);

            // 动态调整延迟时间，逐渐减速
            delay_time = delay_time > speed_factor ? delay_time - speed_factor : 1;
            Delay(delay_time);
        }
        // 重置延迟时间以准备下一页的绘制
        delay_time = 1000;
    }

    // 清除屏幕内容（从两侧向中间收缩清除）
    delay_time = 100; // 减小初始延迟时间，加快清除速度
    for (Page = 0; Page < 8; Page++)
    {
        for (col = 0; col <= mid_col; col++)
        {
            // 从中间向两侧收缩清除
            column = mid_col + col; // 从中间向右扩展清除
            SetPage_Column(); 
            WriteDate(0); // 清除内容

            column = mid_col - col; // 从中间向左扩展清除
            SetPage_Column(); 
            WriteDate(0); // 清除内容

            // 动态调整延迟时间，逐渐加速
            delay_time += speed_factor;
            Delay(delay_time);
        }
    }
}





/* 从两边向中间同时收缩的效果（偶数行） */
void Showpicture_Advanced2(uchar DotByte, uchar DotByte1)
{
    unsigned char col;
    unsigned char mid_col = 64;  // 中间列，宽度的一半
    unsigned char delay_time = 1000; // 减少延迟时间，加快速度

    // 遍历所有页
    for (Page = 0; Page < 8; Page+=2)
    {
        for (col = 0; col <= mid_col; col++)
        {
            // 同时从左边和右边向中间绘制图案
            column = col; // 从左边向中间
            SetPage_Column(); // 设置页和列位置
            WriteDate(DotByte);
            WriteDate(DotByte1);

            column = 127 - col; // 从右边向中间
            SetPage_Column(); // 设置页和列位置
            WriteDate(DotByte);
            WriteDate(DotByte1);
        }

        // 为了加快速度，减少延迟
        Delay(delay_time);
    }

    // 清除屏幕内容
    for (Page = 0; Page < 8; Page++)
    {
        for (col = 0; col < 128; col++)
        {
            column = col;
            SetPage_Column(); 
            WriteDate(0); // 清除内容
        }
    }
}





/*中间向四个方向（上下左右）同时展开的十字光标效果*/

void Showpicture_CrossCursorEffect(uchar DotByte, uchar DotByte1)
{
    unsigned char i;
    unsigned char mid_col = 64;  // 屏幕中间列
    unsigned char mid_Page = 4;  // 屏幕中间页
    unsigned char delay_time = 50; // 延迟时间，用于控制光标移动速度

    // 从中间向四个方向同时绘制图案
    for (i = 0; i <= mid_Page; i++)
    {
        // 向上绘制图案
        Page = mid_Page - i;
        for (column = mid_col - i; column <= mid_col + i; column++)
        {
            SetPage_Column(); // 设置页和列位置
            WriteDate(DotByte);
            WriteDate(DotByte1);
        }

        // 向下绘制图案
        Page = mid_Page + i;
        for (column = mid_col - i; column <= mid_col + i; column++)
        {
            SetPage_Column(); // 设置页和列位置
            WriteDate(DotByte);
            WriteDate(DotByte1);
        }

        // 向左绘制图案
        column = mid_col - i;
        for (Page = mid_Page - i; Page <= mid_Page + i; Page++)
        {
            SetPage_Column(); // 设置页和列位置
            WriteDate(DotByte);
            WriteDate(DotByte1);
        }

        // 向右绘制图案
        column = mid_col + i;
        for (Page = mid_Page - i; Page <= mid_Page + i; Page++)
        {
            SetPage_Column(); // 设置页和列位置
            WriteDate(DotByte);
            WriteDate(DotByte1);
        }

        // 添加延迟，以便视觉效果明显
        Delay(delay_time);
    }

    // 清除屏幕内容
    for (Page = 0; Page < 8; Page++)
    {
        for (column = 0; column < 128; column++)
        {
            SetPage_Column(); 
            WriteDate(0); // 清除内容
        }
    }
}






void LcmPutBMP( uchar *puts )   //显示一幅完整的图像
{   
	 uint X=0;   
	 Page = 0;   
	 column  = 0;   
		 for(Page=0;Page<8;Page++)  //遍历8页,一页8行 
		 {   
			for(column=0;column<128;column++)//遍历128列 
			{   
			 WriteDate( puts[X] );   
			 X++;   
			}   
	 }   
}




unsigned char offset; 
void LcmPutBMPsjw(uchar *puts) //rubbot
{
    uint X = 0;
    int i, j;
    int maxOffset = 8; // 最大偏移量
    unsigned char currentPage;

    for ( offset = 0; offset < maxOffset; offset++) 
    {
        // 每次绘制前重新从图像起始位置读取数据
        X = 0;

        // 遍历所有的页面
        for (i = 0; i < 8; i++) 
        {
            currentPage = i;

            // 设置当前页和列
            SetPage_Column();
            
            // 绘制一整页 (128列)
            for (j = 0; j < 128; j++) 
            {
                if (currentPage >= offset && currentPage < (8 - offset)) 
                {
                    WriteDate(puts[X]); // 写入图像数据
                } 
                else 
                {
                    WriteDate(0x00); // 填充背景色
                }
                X++;
            }
        }
        Delay(50); // 控制渐变效果的速度
    }
}

		
void LcmReverseBMPP( unsigned char *puts ) //取反  
{   
 uint X=0;   
 Page = 0;   
 column  = 0;   
 for(Page=0;Page<8;Page++)   
 {   
  for(column=0;column<128;column++)   
  {   
   WriteDate(~puts[X] ); //反面  
   X++;   
  }   
 }   
}


//8x8的图形
void Display8_8(uchar C[],uchar i)//i图形要显示的行位置
{
	uchar k,num=0;
	num=i*0x08;
	 for(k=0;k<0x08;k++)
	 {
			WriteDate(C[num+k]);
			column+=0x01;
	 } 

}

//16x16的图形
void Display16_16(uchar F[], uchar i)
{
uchar k=0;
uint	num=0;
 num=i*0x20;//32

	for(k=0;k<0x10;k++)//前16行,上半部分
	 {
			WriteDate(F[num+k]);
			column+=0x01;
	 } 
		column-=0x10;//向左移动16个位置,回到最左边的列
		Page+=0x01;   //下一页，下半部分
	 
  for(k=16;k<0x20;k++)//后16行，下半部分
	 {
			WriteDate(F[num+k]);
			column+=0x01;
	 } 
  Page-=0x01; 
  Delay(80); 
}


//3个8×8的
void Display_character()
{ 
		uchar k,c, a=0;//C-页,K-列
		column  = 0;//从最左边开始
	 for(c=0;c<8;c++)//第1个图案
	 { 
			Page = c; 
			for(k=0;k<16;k++)
			{ 
				Display8_8(character,0);
			}
			column  = 0;
	 }
  Delay(1500);

 for(c=0;c<8;c++)//第2个图案
	{ 
		Page = c; 
		for(k=0;k<16;k++)
		{ 
			Display8_8(character,1);
		}
			column  = 0;
 }
  Delay(1500);

 for(c=0;c<8;c++)//第3个图案
	{ 
		Page = c; 
		for(k=0;k<16;k++)
		{ 
			Display8_8(character,2);
		}
			column  = 0;
 }
  Delay(1000);

}


//4个16×16的汉字
void Display_Chinese()
{
	 uchar a=0;
	 Page 	 = 0;   //第一页
	 column  = 0;//最左列
	 for(a=0;a<8;a++)
		 {
				Display16_16(font,a);/* [font]是存储多个16x16汉字点阵数据的数组。
														  a决定从[font]数组的哪个位置开始读取数据
															以便显示正确的汉字*/
		 }
		 
	 Page = 2; //第三页和第四页  
	 column  = 0;
	 for(a=8;a<16;a++)
		 {
				Display16_16(font,a);
		 }
		 
   Page = 4;   
	 column  = 0;
	 for(a=16;a<24;a++)
		 {
				Display16_16(font,a);
		 }

   Page = 6;   
	 column  = 0;
	 for(a=24;a<32;a++)
		 {
				Display16_16(font,a);
		 }

}



void Delay(unsigned int xms) //@12.000MHz
{
    unsigned char i, j;
    while(xms--) // 每次循环减少 1 毫秒
    {
        i = 12;    // 内部循环次数
        j = 169;   // 内部循环次数
        
        do
        {
            while (--j); // 内部延时
        } while (--i); // 外部延时
    }
}


void LcmPutDots( unsigned char DotByte,unsigned char DotByte1 )   
{   
 Page = 0;   
 column  = 0;   
 for(Page=0;Page<8;Page++)   
 {   
  for(column=0;column<128;column++)   
  {   
   WriteDate( DotByte );
   column++;
   WriteDate( DotByte1 );  
  } 
    
 }   
} 

void LcmPutDots1(unsigned char DotByte)
{
    Page = 0;
    column  = 0;
    for(Page = 0; Page < 8; Page++)
    {
        for(column = 0; column< 128; column++)
        {
            WriteDate(DotByte);
        }
    }
}



unsigned char i;
void LcmPutDotsOddColumns(void)//奇数列
{
    unsigned char DotByte9[] = {0x3C, 0x66, 0xC3, 0xC3, 0x3F, 0x03, 0x03, 0xFE}; // 这是示例数据，可以换成任意内容

    for (Page = 0; Page < 8; Page++) // 遍历所有8个页面
    {
        for (column = 1; column < 128; column += 2) // 遍历奇数列，起始列为1，步长为2
        {
            SetPage_Column(); // 设置当前页和奇数列的位置

            for (i = 0; i < 8; i++) // 写入8个字节的数据到当前列
            {
                WriteDate(DotByte9[i]); // 写入每列的点阵数据，或者根据需要写入其他数据
            }
        }
    }
}




void LcmPutDotsStars(unsigned char pattern)
{
  
    for (Page = 0; Page < 8; Page++) // 遍历所有8个页面
    {
        for (column = 0; column < 128; column++) // 遍历每一列
        {
            SetPage_Column(); // 设置光标位置

            if (column % 2 == 0) // 每隔一列显示一个随机模式
            {
                WriteDate(pattern); // 显示给定的模式
            }
            else
            {
                WriteDate(~pattern); // 显示反模式，产生“星罗棋布”效果
            }
        }
    }
}





/*
void Step(void) interrupt 0 using 0//中断
{ _nop_(); 
  _nop_(); 
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_(); 
  _nop_(); 
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  
	if(int0==0)
	loge=!loge;
    while(!int0);
  return;
}
*/