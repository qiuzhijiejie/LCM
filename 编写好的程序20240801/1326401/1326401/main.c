// ST7567,ST7565R看，UC1701综合程序
// writer: 	laozhongmin
// version:01
// date:2013.01.11

//定义头文件
#include <reg52.h>
#include <intrins.h>
#include <ST24C02A.c>					 //读写存储器程序
#include "DATA.txt"						//图片存储数据
#include <stdio.h>
#define uchar unsigned char
#define uint unsigned int 


//模块串行接口定义(接口按实际测架更改)
sbit SDA = P3^4;  //SDA			  //定义串行数据脚为P3.4
sbit SCK = P3^3;				  //定义串行时钟脚为P3.3
sbit RS  = P3^2;  //A0			 //定义数据/指令控制脚为P3.2
sbit RST = P3^1;				 //定义复位脚为P3.1
sbit CS  = P3^0;			   //定义片选脚为P3.0
//---------------------------------------------------------	
 
//按键接口定义(与测试板一致)
//sbit SW	=P2^5;
//sbit DN	=P2^6;
//sbit UP	=P2^7;
sbit key=P2^0;
unsigned char ADJUST=25;	//对比度寄存器初始值	，vop调整设置  当前设置数值VDD=3.0V,VOP=23=9.5V

//0~9,ASCII码数组
unsigned char code ASC[10*16]=
{
0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00,0x00,0x07,0x08,0x10,0x10,0x08,0x07,0x00,/*"016"*/
0x00,0x10,0x10,0xF8,0x00,0x00,0x00,0x00,0x00,0x10,0x10,0x1F,0x10,0x10,0x00,0x00,/*"117"*/
0x00,0x30,0x08,0x08,0x08,0x88,0x70,0x00,0x00,0x18,0x14,0x12,0x11,0x10,0x18,0x00,/*"218"*/
0x00,0x30,0x08,0x88,0x88,0x48,0x30,0x00,0x00,0x0C,0x10,0x10,0x10,0x11,0x0E,0x00,/*"319"*/
0x00,0x80,0x40,0x20,0xF8,0x00,0x00,0x00,0x00,0x03,0x02,0x12,0x1F,0x12,0x12,0x00,/*"420"*/
0x00,0xF8,0x88,0x48,0x48,0x48,0x88,0x00,0x00,0x0C,0x10,0x10,0x10,0x10,0x0F,0x00,/*"521"*/
0x00,0xE0,0x10,0x88,0x88,0x98,0x00,0x00,0x00,0x0F,0x11,0x10,0x10,0x10,0x0F,0x00,/*"622"*/
0x00,0x38,0x08,0x08,0xC8,0x38,0x08,0x00,0x00,0x00,0x00,0x1F,0x00,0x00,0x00,0x00,/*"723"*/
0x00,0x70,0x88,0x88,0x88,0x88,0x70,0x00,0x00,0x0E,0x11,0x10,0x10,0x11,0x0E,0x00,/*"824"*/
0x00,0xF0,0x08,0x08,0x08,0x90,0xE0,0x00,0x00,0x00,0x19,0x11,0x11,0x08,0x07,0x00,/*"925"*/
};


//函数申明如下
void Delay_Xms(unsigned int Xms);
unsigned char Change(unsigned int Xms);
void InitST7567(void);
void ST7567_WriteComd(unsigned char comd);
void ST7567_WriteData(unsigned char dat);
void ST7567_WriteData1(unsigned char dat);
void DispPic(unsigned char *dispdata);
void DispPic1(unsigned char *dispdata);
void DispFram(unsigned char x,unsigned char y);
void DispDot(unsigned char x,unsigned char y);
void DispDots(unsigned char x,unsigned char y);


void push()
{
	 Delay_Xms(100);	
   
  	while(key==1)
   {
   	Delay_Xms(100);
   
   
   }
}

// ====================================//
// 延时函数
// 输入参数:Xms
// 输出参数:无
// 功能说明: Xms*1ms
// ====================================//
void Delay_Xms(unsigned int Xms)
{//需延时的ms数
	unsigned int i,j;
	for(i=0;i<Xms;i++)
		for(j=0;j<150;j++);//延时的基数约1ms
}

void init_cpu(void)
{
    IE = 0x00;
    Delay_Xms(50);
}

// ====================================//
// 初始化函数
// 输入参数:无
// 输出参数:无
// 注:与其它ST7567及NT75451/NT7534/NT7538是最后倍压数选择有差异其它相同
// ====================================//


 // ST7567初始化函数
void InitST7567(void)
{
 	Delay_Xms(100);	// 以下几行代码是产生硬件复位
	RST = 0;
 	Delay_Xms(50);	// 
	RST = 1;
 	Delay_Xms(100);	// 

	ST7567_WriteComd(0xe2);	//soft reset开始进行软复位
 	Delay_Xms(10);
	ST7567_WriteComd(0xa2);	//bias select确定为1/6BIAS,1/33duty
 	Delay_Xms(2);
	ST7567_WriteComd(0xa0);	// ADC select确定SEG反扫seg0--->seg131
 	Delay_Xms(2);
	ST7567_WriteComd(0xc8);	//SHL select确定COM为正扫com63--->com0
 	Delay_Xms(2);
	ST7567_WriteComd(0x25);	//regulator resistor select V0调整电阻设定(1 + Rb/Ra)=6.5		0X26+0X41		vdd=5.0v,vop=10.0v						 0x26原来的数值
 	Delay_Xms(2);
	ST7567_WriteComd(0x81);	//The Electronic Volume (Double Byte Command)数字电位器调节VO输出电压(也就是对比度)两字节				 0x81原来的数值
	ST7567_WriteComd(ADJUST);	 //vop 设置
 	Delay_Xms(2);
	ST7567_WriteComd(0x2c);	//power control1 on内部倍压电路开启,需分以下三步打开电路
 	Delay_Xms(2);
	ST7567_WriteComd(0x2e);	//power control2
 	Delay_Xms(2);
	ST7567_WriteComd(0x2f);	//power control3
 	Delay_Xms(2);
	ST7567_WriteComd(0xf1);	//The Booster Ratio (Double Byte Command)设定倍压数,两字节
 	Delay_Xms(2);
	ST7567_WriteComd(0x00);	//"00"最大4倍压,"01"最大5倍压,"03"最大6倍压
 	Delay_Xms(2);
	ST7567_WriteComd(0x40);	//设定起始行第1行
 	Delay_Xms(2);
	ST7567_WriteComd(0xa4);	//Display All Points ON/OFF to Normal display mode正常显示模式选择
 	Delay_Xms(2);
	ST7567_WriteComd(0xaf);	//Display ON/OFF开显示设定
 	Delay_Xms(2);
}
 //===========================


/*
 // ST7565R初始化函数

void InitST7567(void)
   	
{	Delay_Xms(100);	// 以下几行代码是产生硬件复位
	RST = 0;
 	Delay_Xms(50);	// 
	RST = 1;
 	Delay_Xms(100);	// 

	ST7567_WriteComd(0xe2);	//soft reset开始进行软复位
 	Delay_Xms(10);
	ST7567_WriteComd(0xa2);	//bias select确定为1/9BIAS,1/65duty
 	Delay_Xms(2);
	ST7567_WriteComd(0xa0);	// ADC select确定SEG反扫seg0--->seg131
 	Delay_Xms(2);
	ST7567_WriteComd(0xc8);	//SHL select确定COM为正扫com63--->com0
 	Delay_Xms(2);
	ST7567_WriteComd(0x26);	//regulator resistor select V0调整电阻设定(1 + Rb/Ra)=6.5		0X26+0X41		vdd=5.0v,vop=10.0v						 0x26原来的数值
 	Delay_Xms(2);
	ST7567_WriteComd(0x81);	//The Electronic Volume (Double Byte Command)数字电位器调节VO输出电压(也就是对比度)两字节				 0x81原来的数值
	ST7567_WriteComd(ADJUST);	 //vop 设置
 	Delay_Xms(2);												
	ST7567_WriteComd(0x2f);	//power control3
 	Delay_Xms(2);
	ST7567_WriteComd(0xf8);	//The Booster Ratio (Double Byte Command)设定倍压数,两字节
 	Delay_Xms(2);
	ST7567_WriteComd(0x01);	//"00"最大4倍压,"01"最大5倍压,"03"最大6倍压
 	Delay_Xms(2);
	ST7567_WriteComd(0x40);	//设定起始行第1行
 	Delay_Xms(2);
	ST7567_WriteComd(0xa4);	//Display All Points ON/OFF to Normal display mode正常显示模式选择
 	Delay_Xms(2);
	ST7567_WriteComd(0xaf);	//Display ON/OFF开显示设定
 	Delay_Xms(2);


 }

/*
void InitST7567(void)
   	
{	ST7567_WriteComd(0xa2);//   1/3bias
 		ST7567_WriteComd(0xa0);//ADC select:normal
  		ST7567_WriteComd(0xa4);//Display all point ON/OFF:normal display
  		ST7567_WriteComd(0xc0);//Common output mode select:normal direction
 		ST7567_WriteComd(0x22);//V0 boltage regulator ratio set:5.0(0X24)
        	Delay_Xms(10);
		ST7567_WriteComd(0x81);//Electronic volume register set
 		ST7567_WriteComd(17);//Electronic volume register set
 		Delay_Xms(10);
 		ST7567_WriteComd(0x2c);//Power control set:booster on
        	Delay_Xms(10);
  		ST7567_WriteComd(0x2e);//Power control set:booster on/regulator on
        	Delay_Xms(10);
 		ST7567_WriteComd(0x2f);//Power control set:booster on/regulator on/follower on
        	Delay_Xms(10);
 		ST7567_WriteComd(0xaf);//display on

   }

 */

//==============================================
unsigned char code    jin[128]= 
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xC0,0xF0,0xF8,0x7C,0x3C,0x7C,0xF8,0xE0,0xC0,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x20,0x60,0x70,0x78,0x38,0x3C,0x7E,0x6F,0x67,0x67,0x63,0x61,0xE0,0xE0,0xE0,0xE0,0x61,0x63,0x63,0x67,0x6F,0x7E,0x7E,0x1C,0x3C,0x38,0x78,0x78,0x10,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x18,0xD8,0xD8,0xD8,0x98,0x18,0x18,0xFF,0xFF,0xFF,0xFF,0x18,0x18,0x38,0xF8,0xD8,0xD8,0xD8,0x18,0x18,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x30,0x30,0x30,0x30,0x31,0x37,0x37,0x33,0x32,0x30,0x3F,0x3F,0x3F,0x3F,0x30,0x38,0x3F,0x3F,0x3F,0x33,0x30,0x30,0x30,0x30,0x30,0x00,0x00,0x00};/*"金",0*/
/* (32 X 32 , 黑体, 加粗 )*/
uchar code    se[128]= 
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xC0,0xF0,0xFC,0xFC,0xF8,0xC8,0xC8,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x10,0x30,0x38,0x3C,0xFE,0xFF,0xFF,0xF7,0x33,0x30,0x30,0x30,0x30,0xF0,0xF0,0xF8,0xFE,0x3F,0x3F,0x33,0x31,0x30,0xF0,0xF0,0xF0,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0x1C,0x1C,0x1C,0x1C,0x1C,0x1F,0x1F,0x1F,0x1F,0x1C,0x1C,0x1C,0x1C,0x1C,0x3F,0x3F,0x3F,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x1F,0x3F,0x3F,0x38,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x38,0x3E,0x1E,0x0C,0x00,0x00};/*"色",1*/
/* (32 X 32 , 黑体, 加粗 )*/
uchar code    tong[128]= 
{0x00,0x00,0x00,0x00,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0xE0,0xE0,0x60,0x68,0x7C,0x7C,0x7C,0x60,0x60,0xE0,0xE0,0xE0,0x60,0x60,0x60,0x60,0x60,0x60,0x00,0x00,0x00,
0x00,0x00,0x00,0x04,0x04,0x04,0xE4,0xE4,0xE4,0x64,0x67,0x67,0x67,0x67,0x64,0xE4,0xE4,0xE4,0x64,0x67,0x67,0x67,0x65,0x64,0xE4,0xE4,0xE4,0x04,0x04,0x04,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x3F,0x3F,0x3F,0x32,0x32,0x32,0x32,0x32,0x32,0xFF,0xFF,0xFF,0x32,0x32,0x32,0x32,0x32,0x32,0x3F,0x3F,0x3F,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x30,0x30,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x3F,0x3F,0x3F,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x30,0x30,0x00,0x00};/*"童",2*/
/* (32 X 32 , 黑体, 加粗 )*/
uchar code    nian[128]= 
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xE0,0xF8,0xFC,0xFC,0x98,0x88,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x00,
0x00,0x00,0x20,0x30,0x78,0x7C,0x3F,0xDF,0xCF,0xC3,0xC1,0xC1,0xC1,0xC1,0xC1,0xC1,0xFF,0xFF,0xFF,0xFF,0xC1,0xC1,0xC1,0xC1,0xC1,0xC1,0xC1,0xC1,0x01,0x00,0x00,0x00,
0x00,0x00,0xC0,0xC0,0xC0,0xC0,0xC0,0xFF,0xFF,0xFF,0xFF,0xC0,0xC0,0xC0,0xC0,0xC0,0xFF,0xFF,0xFF,0xFF,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0x7F,0x7F,0x7F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};/*"年",3*/
/* (32 X 32 , 黑体, 加粗 )*/



void hz_display16(unsigned char pag, unsigned char col, unsigned char cok,unsigned char code *hzk)	   //写入一个32*32的字符
{
		 uchar a=0;	
         uchar j=0;
         uchar i=0;
		
         for(j=0;j<4;j++)                          //这里写四页
         {
             ST7567_WriteComd(0xb0+pag+j);          //page在这里就是从page表示的这页开始写起。
				
			 ST7567_WriteComd(0x10+cok);		  //双指令控制列，需加变量来控制SEG起始端
             ST7567_WriteComd(0x00+col);          //从列为col表示的列开始
				 
          for(i=0;i<32;i++)                           //这里写32列
               ST7567_WriteData(hzk[32*j+i]);        //J为页，I，为第J行第I个数
         }
}







// ====================================//
// 显示一个图片函数
// 输入参数:显示图片数组
// 输出参数:无
// ====================================//
void DispPic(unsigned char *dispdata)
{
	unsigned char i,page;	//定义局部字符变量
	unsigned int j=0;		//定义局部整型变量

	// 下面这段循环为显示C0~C31
	page=0xb0;				//初始开始扫描页
	while(page<0xb4)		//共4页
	{
		ST7567_WriteComd(page);	//设定扫描页
	    ST7567_WriteComd(0x10);	//设定起始SEG数,两字节
	    ST7567_WriteComd(0x00);
	    for(i=0;i<132;i++)
    	{//写128SEG
      		ST7567_WriteData(dispdata[j]);	//调数组显示数据进行写显示
      		j++;
    	}
    	page++;	//页自动加1
	}
	// 下面这段循环为显示C32~C63
	page=0xb4;				//初始开始扫描页
	while(page<0xb8)		//共4页
	{
		ST7567_WriteComd(page);	//设定扫描页
	    ST7567_WriteComd(0x10);	//设定起始SEG数,两字节
	    ST7567_WriteComd(0x00);
	    for(i=0;i<132;i++)
    	{//写128SEG
      		ST7567_WriteData(dispdata[j]);	//调数组显示数据进行写显示
      		j++;
    	}
    	page++;	//页自动加1
	}
}

void DispPic1(unsigned char *dispdata)
{
	unsigned char i,page;	//定义局部字符变量
	unsigned int j=0;		//定义局部整型变量

	// 下面这段循环为显示C0~C31
	page=0xb0;				//初始开始扫描页
	while(page<0xb4)		//共4页
	{
		ST7567_WriteComd(page);	//设定扫描页
	    ST7567_WriteComd(0x10);	//设定起始SEG数,两字节
	    ST7567_WriteComd(0x00);
	    for(i=0;i<132;i++)
    	{//写128SEG
      		ST7567_WriteData1(dispdata[j]);	//调数组显示数据进行写显示
      		j++;
    	}
    	page++;	//页自动加1
	}
	// 下面这段循环为显示C32~C63
	page=0xb4;				//初始开始扫描页
	while(page<0xb8)		//共4页
	{
		ST7567_WriteComd(page);	//设定扫描页
	    ST7567_WriteComd(0x10);	//设定起始SEG数,两字节
	    ST7567_WriteComd(0x00);
	    for(i=0;i<132;i++)
    	{//写128SEG
      		ST7567_WriteData1(dispdata[j]);	//调数组显示数据进行写显示
      		j++;
    	}
    	page++;	//页自动加1
	}
}



// ====================================//
// 显示隔行隔列间隔点棋盘的函数
// 输入参数:双字节
// 输出参数:无
// ====================================//
void DispFram(unsigned char x,unsigned char y)
{
	unsigned char i,page;	//定义局部字符变量

	// 下面这段循环为显示C0~C31
	page=0xb0;				//初始开始扫描页
	while(page<0xb4)		//共4页
	{
		ST7567_WriteComd(page);	//设定扫描页
	    ST7567_WriteComd(0x10);	//设定起始SEG数,两字节
	    ST7567_WriteComd(0x00);
	    for(i=0;i<132;i++)
    	{//写128SEG
      		ST7567_WriteData(x);	//显示数据进行写显示
      		ST7567_WriteData(y);	//显示数据进行写显示
    	}
    	page++;	//页自动加1
	}
	// 下面这段循环为显示C32~C63
	page=0xb4;				//初始开始扫描页
	while(page<0xb8)		//共4页
	{
		ST7567_WriteComd(page);	//设定扫描页
	    ST7567_WriteComd(0x10);	//设定起始SEG数,两字节
	    ST7567_WriteComd(0x00);
	    for(i=0;i<132;i++)
    	{//写128SEG
      		ST7567_WriteData(x);	//显示数据进行写显示
      		ST7567_WriteData(y);	//显示数据进行写显示
    	}
    	page++;	//页自动加1
	}
}
// ====================================//
// 显示隔行隔列间隔点棋盘的函数
// 输入参数:双字节
// 输出参数:无
// ====================================//
void DispDot(unsigned char x,unsigned char y)
{
	unsigned char i,j,page;	//定义局部字符变量
	// 下面这段循环为显示C0~C31
	page=0xb0;				//初始开始扫描页
	while(page<0xb4)		//共4页
	{
	    ST7567_WriteComd(page);	//设定扫描页
	    ST7567_WriteComd(0x10);	//设定起始SEG数,两字节
	    ST7567_WriteComd(0x00);
	    for(i=0;i<17;i++)
    	    {
           for(j=0;j<4;j++)
            { 
      		ST7567_WriteData(x);	//显示数据进行写显示
              }
            for(j=0;j<4;j++)
            { 
      		ST7567_WriteData(y);	//显示数据进行写显示
            }
    	}
    	page++;	//页自动加1
	}
	// 下面这段循环为显示C32~C63
	page=0xb4;				//初始开始扫描页
	while(page<0xb8)		//共4页
	{
	    ST7567_WriteComd(page);	//设定扫描页
	    ST7567_WriteComd(0x10);	//设定起始SEG数,两字节
	    ST7567_WriteComd(0x00);
	    for(i=0;i<17;i++)
    	    {
           for(j=0;j<4;j++)
            { 
      		ST7567_WriteData(x);	//显示数据进行写显示
              }
            for(j=0;j<4;j++)
            { 
      		ST7567_WriteData(y);	//显示数据进行写显示
            }
    	}
    	page++;	//页自动加1
	}
}
// ====================================//
// 显示隔行隔列间隔点棋盘的函数
// 输入参数:双字节
// 输出参数:无
// ====================================//
void DispDots(unsigned char x,unsigned char y)
{
	unsigned char i,j,page;	//定义局部字符变量


	// 下面这段循环为显示C0~C31
	page=0xb0;				//初始开始扫描页
	while(page<0xb4)		//共4页
	{
	    ST7567_WriteComd(page);	//设定扫描页
	    ST7567_WriteComd(0x10);	//设定起始SEG数,两字节
	    ST7567_WriteComd(0x00);
	    for(i=0;i<33;i++)
    	    {
           for(j=0;j<2;j++)
            { 
      		ST7567_WriteData(x);	//显示数据进行写显示
              }
            for(j=0;j<2;j++)
            { 
      		ST7567_WriteData(y);	//显示数据进行写显示
            }
    	}
    	page++;	//页自动加1
	}
	// 下面这段循环为显示C32~C63
	page=0xb4;				//初始开始扫描页
	while(page<0xb8)		//共4页
	{
	    ST7567_WriteComd(page);	//设定扫描页
	    ST7567_WriteComd(0x10);	//设定起始SEG数,两字节
	    ST7567_WriteComd(0x00);
	    for(i=0;i<33;i++)
    	    {
           for(j=0;j<2;j++)
            { 
      		ST7567_WriteData(x);	//显示数据进行写显示
              }
            for(j=0;j<2;j++)
            { 
      		ST7567_WriteData(y);	//显示数据进行写显示
            }
    	}
    	page++;	//页自动加1
	}
}

// ====================================//
// 主函数,只举例,详细的显示内容根据规范
// ====================================//
void main(void)
{
	init_cpu();
	
	Delay_Xms(100);
	while(1)  
	{
	InitST7567();	//一开始初始化控制器
  Start:	//跳转标号
      			   	    
//-------------------------------------------------------------------------		
	
//		 hz_display16(0,0,0,jin); 
//	  		Delay_Xms(1000);
//			 
//			 
//	    hz_display16(0,0,18,se); 
//	  	Delay_Xms(1000);
	
	
		DispFram( 0xff,0xff );				 //全显
		 Delay_Xms(500);
		if( Change(70)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上
//		 push();


		DispFram( 0x00,0x00 );			   //无显
		 Delay_Xms(500);
		if( Change(70)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上
	 	 //push();
//------------------------------------------------------------------	
		DispDot( 0xf0,0x0f );			 //大棋盘
		 Delay_Xms(500);
	  	if( Change(70)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上
		 //push();
//------------------------------------------------------------------	
		DispDots( 0x33,0xcc );			  //小棋盘
		 Delay_Xms(500);
		if( Change(70)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上
		// push();
//------------------------------------------------------------------	
		DispFram( 0x55,0xaa );				  //花点		
		 Delay_Xms(500);
		if( Change(70)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上
	// push();
	
		DispFram( 0x55,0x55 );					 //	 横线
		 Delay_Xms(500);
		if( Change(70)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上
	 //push();

	//	DispFram( 0xaa,0xaa );				     //	横线
	//	  Delay_Xms(1000);
	 	//	if( Change(300)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上
		 //push();
//-------------------------------------------------------------------------		
		DispFram( 0xff,0x00 );					 //竖线 
		  Delay_Xms(500);
		if( Change(70)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上
	// push();

	//	DispFram( 0x00,0xff );
	//	 Delay_Xms(1000);
	//	if( Change(300)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上
	 //push();
//----------------------------------------------------------------------------	 
	 

		
	   //DispPic( PIC_3 );
	  // Delay_Xms(600);
	//if( Change(70)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上
	

	

	 

	}
}
//-------------------------------------
/*
// ====================================//
// 4线方式的写指令子程序
// 输入参数:comd
// 输出参数:无
// ====================================//
void ST7567_WriteComd(unsigned char comd)
{
	unsigned char i;
	CS = 0;								//置低CS表示选中芯片可以有效的对芯片进行控制
	RS = 0;								//置低RS表示执行指令的传送
	SCK = 0;							//置低SCK为产生上升沿作准备
	for(i=0;i<8;i++)
	{//循环传送共8位从高位开始
		if( (comd&0x80)==0x80 ) SDA = 1;//判断comd的最高位,是"1"则置SDA=1
		else SDA = 0;					//comd最高位为"0",则置SDA=0
		_nop_();_nop_();_nop_();_nop_();	//数据稳定一定时间
		_nop_();_nop_();_nop_();_nop_();	//数据稳定一定时间
		SCK = 1;						//置低SCK为产生上升沿作准备
		_nop_();_nop_();_nop_();_nop_();	//数据稳定一定时间
		_nop_();_nop_();_nop_();_nop_();	//数据稳定一定时间
		SCK=0;							//置高SCK产生上升沿
		_nop_();_nop_();_nop_();_nop_();	//数据稳定一定时间
		_nop_();_nop_();_nop_();_nop_();	//数据稳定一定时间
        comd = comd<<1;					//往高位移一位继续

	}
	CS = 1;								//置高CS表示任何动作对芯片无效
}

// ====================================//
// 4线方式的写数据子程序
// 输入参数:dat
// 输出参数:无
// ====================================//
void ST7567_WriteData(unsigned char dat)
{
	unsigned char i;
	CS = 0;								//置低CS表示选中芯片可以有效的对芯片进行控制
	RS = 1;								//置高RS表示执行数据的传送
	SCK = 0;							//置低SCK为产生上升沿作准备
	for(i=0; i<8; i++)
	{//循环传送共8位从高位开始
		if( (dat&0x80)==0x80 ) SDA = 1;//判断comd的最高位,是"1"则置SDA=1
		else SDA = 0;					//comd最高位为"0",则置SDA=0
		_nop_();_nop_();_nop_();_nop_();	//数据稳定一定时间
		_nop_();_nop_();_nop_();_nop_();	//数据稳定一定时间
		SCK = 1;						//置低SCK为产生上升沿作准备
		_nop_();_nop_();_nop_();_nop_();	//数据稳定一定时间
		_nop_();_nop_();_nop_();_nop_();	//数据稳定一定时间
		SCK=0;							//置高SCK产生上升沿
		_nop_();_nop_();_nop_();_nop_();	//数据稳定一定时间
		_nop_();_nop_();_nop_();_nop_();	//数据稳定一定时间
        dat = dat<<1;					//往高位移一位继续
	}
	CS = 1;								//置高CS表示任何动作对芯片无效
}

void ST7567_WriteData1(unsigned char dat)
{
	unsigned char i;
	CS = 1;								//置低CS表示选中芯片可以有效的对芯片进行控制
	RS = 1;								//置高RS表示执行数据的传送
	SCK = 0;							//置低SCK为产生上升沿作准备
	for(i=0; i<8; i++)
	{//循环传送共8位从高位开始
		if( (dat&0x80)==0x80 ) SDA = 1;//判断comd的最高位,是"1"则置SDA=1
		else SDA = 0;					//comd最高位为"0",则置SDA=0
		_nop_();_nop_();_nop_();_nop_();	//数据稳定一定时间
		_nop_();_nop_();_nop_();_nop_();	//数据稳定一定时间
		SCK = 1;						//置低SCK为产生上升沿作准备
		_nop_();_nop_();_nop_();_nop_();	//数据稳定一定时间
		_nop_();_nop_();_nop_();_nop_();	//数据稳定一定时间
		SCK=0;							//置高SCK产生上升沿
		_nop_();_nop_();_nop_();_nop_();	//数据稳定一定时间
		_nop_();_nop_();_nop_();_nop_();	//数据稳定一定时间
        dat = dat<<1;					//往高位移一位继续
	}
	CS = 1;								//置高CS表示任何动作对芯片无效
}

*/

// ====================================//
// 8位并行数据总线的写指令子程序
// 输入参数:comd
// 输出参数:无
// ====================================//


void ST7567_WriteComd(unsigned char comd)
{
	//8080时序
  //CS = 0;								//置低CS表示选中芯片可以有效的对芯片进行控制
	//RS = 0;								//置低RS表示执行指令传输
	//E_RD = 1;							//置高E_RD写动作准备
	//dataport = comd;					//把comd传递过来的参数写入到LCD数据线上
	//RW_WR = 0;							//置低RW_WR进行上升沿的产生作准备
	//_nop_();_nop_();_nop_();_nop_();	//数据建立一定时间
	//RW_WR = 1;							//置高RW_WR产生上升沿数据此时锁存进芯片内部电路
	//_nop_();_nop_();_nop_();_nop_();	//数据保持一定时间
	//CS = 1;								//置高CS表示任何动作对芯片无效


// //	6800时序
//	CS = 0;								//置低CS表示选中芯片可以有效的对芯片进行控制,顺序1
//	RS = 0;								//置低RS表示执行指令传输,顺序2
//	RW_WR = 0;							//置低RW_WR表示写动作准备,顺序3
//	E_RD = 1;							//置高E_RD进行下降沿的产生作准备,顺序4
//	dataport = comd;					//把comd传递过来的参数写入到LCD数据线上,顺序5
//	_nop_();_nop_();_nop_();_nop_();	//数据保持一定时间
//	E_RD = 0;							//置低E_RD产生下降沿数据此时锁存进芯片内部电路,顺序6
//	_nop_();_nop_();_nop_();_nop_();	//数据建立一定时间
//	CS = 1;								//置高CS表示任何动作对芯片无效,顺序7
}

// ====================================//
// 8位并行数据总线的写数据子程序
// 输入参数:dat
// 输出参数:无
// ====================================//
void ST7567_WriteData(unsigned char dat)
{
	//8080时序
	//CS = 0;								//置低CS表示选中芯片可以有效的对芯片进行控制
	//RS = 1;								//置高RS表示执行数据传输
	//E_RD = 1;							//置高E_RD写动作准备
	//dataport = dat;						//把dat传递过来的参数写入到LCD数据线上
	//RW_WR = 0;							//置低RW_WR进行上升沿的产生作准备
	//_nop_();_nop_();_nop_();_nop_();	//数据建立一定时间
	//RW_WR = 1;							//置高RW_WR产生上升沿数据此时锁存进芯片内部电路
	//_nop_();_nop_();_nop_();_nop_();	//数据保持一定时间
	//CS = 1;								//置高CS表示任何动作对芯片无效

//	//6800时序
//	CS = 0;								//置低CS表示选中芯片可以有效的对芯片进行控制,顺序1
//	RS = 1;								//置高RS表示执行数据传输
//	RW_WR = 0;							//置低RW_WR表示写动作准备,顺序3
//	E_RD = 1;							//置高E_RD进行下降沿的产生作准备,顺序4
//	dataport = dat;						//把dat传递过来的参数写入到LCD数据线上,顺序5
//	_nop_();_nop_();_nop_();_nop_();	//数据保持一定时间
//	E_RD = 0;							//置低E_RD产生下降沿数据此时锁存进芯片内部电路,顺序6
//	_nop_();_nop_();_nop_();_nop_();	//数据建立一定时间
//	CS = 1;								//置高CS表示任何动作对芯片无效,顺序7
}



 void ST7567_WriteData1(unsigned char dat)
{
	//8080时序
	//CS = 1;								//置低CS表示选中芯片可以有效的对芯片进行控制
	//RS = 1;								//置高RS表示执行数据传输
	//E_RD = 1;							//置高E_RD写动作准备
	//dataport = dat;						//把dat传递过来的参数写入到LCD数据线上
	//RW_WR = 0;							//置低RW_WR进行上升沿的产生作准备
	//_nop_();_nop_();_nop_();_nop_();	//数据建立一定时间
	//RW_WR = 1;							//置高RW_WR产生上升沿数据此时锁存进芯片内部电路
	//_nop_();_nop_();_nop_();_nop_();	//数据保持一定时间
	//CS = 1;								//置高CS表示任何动作对芯片无效

//	//6800时序
//	CS = 1;								//置低CS表示选中芯片可以有效的对芯片进行控制,顺序1
//	RS = 1;								//置高RS表示执行数据传输
//	RW_WR = 0;							//置低RW_WR表示写动作准备,顺序3
//	E_RD = 1;							//置高E_RD进行下降沿的产生作准备,顺序4
//	dataport = dat;						//把dat传递过来的参数写入到LCD数据线上,顺序5
//	_nop_();_nop_();_nop_();_nop_();	//数据保持一定时间
//	E_RD = 0;							//置低E_RD产生下降沿数据此时锁存进芯片内部电路,顺序6
//	_nop_();_nop_();_nop_();_nop_();	//数据建立一定时间
//	CS = 1;								//置高CS表示任何动作对芯片无效,顺序7
}




// ====================================//
// 对比度设定及显示对比度参数函数
// 输入参数:ADJUST
// 输出参数:无
// 实现对比度的更新,及显示当前调整的数值
// ====================================//
void SetContrast(void)
{
	unsigned char i,j,page;	//定义局部变量

	ST7567_WriteComd(0x81);	//对比度调节寄存器
	ST7567_WriteComd(ADJUST);	//写入对比度数值

	i=ADJUST;
	i=i/10;		//取出对比度数值的十位
	page=0xb2;	//在0xb2~0xb3页上显示16*8DOTS
	ST7567_WriteComd(page);	//设定页地址
	ST7567_WriteComd(0x10);	//双字节设定起始SEG
	ST7567_WriteComd(4);		//从SEG4开始显示
	for(j=0;j<8;j++)
	{
		ST7567_WriteData( ASC[(i)*16+j] );	//显示字符的上半节8*8
	}
	page++;	//页地址加1
	ST7567_WriteComd(page);	//设定页地址
	ST7567_WriteComd(0x10);	//双字节设定起始SEG
	ST7567_WriteComd(4);		//从SEG4开始显示
	for(j=8;j<16;j++)
	{
		ST7567_WriteData( ASC[(i)*16+j] );	//显示字符的下半节8*8,这样完成一个整字的显示
	}

	i=ADJUST;
	i=i%10;		//取出对比度数值的个位
	page=0xb2;	//在0xb2~0xb3页上显示16*8DOTS
	ST7567_WriteComd(page);	//设定页地址
	ST7567_WriteComd(0x10);	//双字节设定起始SEG
	ST7567_WriteComd(12);		//从SEG12开始显示
	for(j=0;j<8;j++)
	{
		ST7567_WriteData( ASC[(i)*16+j] );	//显示字符的上半节8*8
	}
	page++;	//页地址加1
	ST7567_WriteComd(page);	//设定页地址
	ST7567_WriteComd(0x10);	//双字节设定起始SEG
	ST7567_WriteComd(12);		//从SEG12开始显示
	for(j=8;j<16;j++)
	{
		ST7567_WriteData( ASC[(i)*16+j] );	//显示字符的下半节8*8,这样完成一个整字的显示
	}
}




// ====================================//
// 按键处理函数(带定时功能总体定时约1ms*Xms)
// 输入参数:Xms
// 输出参数:暂停键按下标志
// 功能说明: 
// 1, 在定时过程中首先判断是否有暂停键按下,如有则定时失效,此时同时判断对比度上下调节键及暂停键,
//    如果对比度调节键有按下则进行对比度调整,如果暂停键有按下则记录下暂停键有按下的状态并返回一个"1"参数并退出程序.
// 2, 在定时过程中暂停键一直没有动作,则直接返回"0"参数并退出程序.
// ====================================//
unsigned char Change(unsigned int Xms)
{
	unsigned int i;
	unsigned char j,h;
	j=h=0;
	for(i=0;i<Xms;i++)
	{//需定时的ms数
		for(j=0;j<150;j++)
		{//定时的基数约1ms
		//	if( SW==0 )
			{//如果暂停键按下有效
				Delay_Xms(10);	//消抖动
		//		if ( SW==0 )
				{//暂停键按下有效
		//			while( SW==0 );	//等待暂停键弹起
					Delay_Xms(10);	//消抖动
					while(1)
					{
		//				if( SW==0 )
						{//暂停键再次按下
							Delay_Xms(10);	//消抖动
		//					if ( SW==0 )
							{//暂停键按下有效
		//						while( SW==0 );	//等待暂停键弹起
								Delay_Xms(10);	//消抖动
								h = 1;	//把暂停键标志置1
								goto Eixt;	//跳到退出程序标号
							}
						}
		//				else if( UP==0 )
						{//对比度+键按下
							Delay_Xms(10);	//消抖动
		//					if ( UP==0 )
							{//对比度+键按下有效
		//						while( UP==0 )	//等待此键弹起
								{
		//							if( DN==0 )
									{
										Delay_Xms(5);	//消抖动
	//									if ( DN==0 )
										{//对比度-键按下有效
	//										while( (DN==1)&&(UP==1) );	//等待此键弹起
											Delay_Xms(5);	//消抖动
											h = 1;	//把暂停键标志置1
											goto Eixt;	//跳到退出程序标号
										}
									}
								}
								Delay_Xms(5);	//消抖动
								ADJUST++;	//对比度寄存器加一档
								//======adjust contrast=====对比度设定
								SetContrast();	
								//==========================
							}				
						}
//						else if( (DN==0)&&(UP==1) )
						{//对比度-键按下
							Delay_Xms(10);	//消抖动
//							if ( DN==0 )
							{//对比度-键按下有效
//								while( DN==0 );	//等待此键弹起
								Delay_Xms(5);	//消抖动
								ADJUST--;	//对比度寄存器减一档
								//======adjust contrast=====对比度设定
								SetContrast();	
								//==========================
							}				
						}
					}
				}
			}
		}
	}
	Eixt: Delay_Xms(1);	//退出程序标号
	return (h);	//返回参数
}

//------------------------------------------------------------正方形												   
//---------------------------------------------------------------