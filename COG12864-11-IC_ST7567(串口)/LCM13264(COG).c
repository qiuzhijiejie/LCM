#include <REGX52.H>
#include "lcm13264(cog).h"
#include "pic.h"
#include "intrins.h" 

uchar ADJUST=25;	//对比度寄存器初始值	，vop调整设置  当前设置数值VDD=3.0V,VOP=13=8.0V
uchar code e_char[8] = {0x7f,0x49,0x49,0x49,0x49,0x49,0x00,0x00 };
float VOP,RR;


//按键函数
unsigned int HOLD()
{
    if (Key1 == 0) 
	{  
        Delay_Xms(5);  
        if (Key1 == 0) 
		{  
            while (Key1 == 0); 
            return 1; 
        }
    }
    return 0; 
}

//按键函数
unsigned int UP() 
{
    if (Key2 == 0) 
	{  
        Delay_Xms(5);  
        if (Key2 == 0) 
		{ 
            while (Key2 == 0);  
            return 1;  
        }
    }
    return 0; 
}

//按键函数
unsigned int DN() 
{
    if (Key3 == 0) 
	{  
        Delay_Xms(5);  
        if (Key3 == 0) 
		{ 
            while (Key3 == 0);  
            return 1;  
        }
    }
    return 0; 
}



void ResetChip() 
{
    RST = 0;  // 拉低复位引脚
    Delay_Xms(10);  // 等待 10ms，确保复位有效
    RST = 1;  // 释放复位

}

void push()
{
	 Delay_Xms(100);	
   
  	while(Key1==1)
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
{	
	unsigned int i,j;
	for(i=0;i<Xms;i++)
		for(j=0;j<100;j++);//延时的基数约1ms
}

void init_cpu(void)
{
    IE = 0x00;//关闭所有中断
    Delay_Xms(50);
}

// ====================================//
// 初始化函数
// 输入参数:无
// 输出参数:无
// 注:与其它ST7567及NT75451/NT7534/NT7538是最后倍压数选择有差异其它相同

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
	ST7567_WriteComd(0xa2);	//bias select确定为1/9BIAS,1/65duty
 	Delay_Xms(10);
	ST7567_WriteComd(0xa0);	// ADC select确定SEG反扫seg0--->seg131
 	Delay_Xms(10);
	ST7567_WriteComd(0xc8);	//SHL select确定COM为正扫com63--->com0
 	Delay_Xms(10);
	ST7567_WriteComd(0x25);	//regulator resistor select V0调整电阻设定(1 + Rb/Ra)=5.0		0X26+0X41		vdd=5.0v,vop=10.0v						 0x26原来的数值
 	Delay_Xms(10);
	ST7567_WriteComd(0x81);	//The Electronic Volume (Double Byte Command)数字电位器调节VO输出电压(也就是对比度)两字节				 0x81原来的数值
	ST7567_WriteComd(ADJUST);//vop 设置
 	Delay_Xms(10);
	ST7567_WriteComd(0x2c);	//power control1 on内部倍压电路开启,需分以下三步打开电路
 	Delay_Xms(10);
	ST7567_WriteComd(0x2e);	//power control2
	Delay_Xms(10);
	ST7567_WriteComd(0x2f);	//power control3
 	Delay_Xms(10);
	ST7567_WriteComd(0xf1);	//The Booster Ratio (Double Byte Command)设定倍压数,两字节
 	Delay_Xms(10);
	ST7567_WriteComd(0x00);	//"00"最大4倍压,"01"最大5倍压,"03"最大6倍压
 	Delay_Xms(10);
	ST7567_WriteComd(0x40);	//设定起始行第1行
 	Delay_Xms(10);
	ST7567_WriteComd(0xa4);	//Display All Points ON/OFF to Normal display mode正常显示模式选择
 	Delay_Xms(10);
	ST7567_WriteComd(0xaf);	//Display ON/OFF开显示设定
 	Delay_Xms(10);
}


/* 
// ST7565R初始化函数

void InitST7567(void)
   	
{	Delay_Xms(1);	// 以下几行代码是产生硬件复位
	RST = 0;
 	Delay_Xms(5);	// 
	RST = 1;
 	Delay_Xms(1);	// 

	ST7567_WriteComd(0xe2);	//soft reset开始进行软复位
 	Delay_Xms(10);
	ST7567_WriteComd(0xa2);	//bias select确定为1/9BIAS,1/65duty
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
	ST7567_WriteComd(0x2f);	//power control3
 	Delay_Xms(2);
	ST7567_WriteComd(0xf8);	//The Booster Ratio (Double Byte Command)设定倍压数,两字节
 	Delay_Xms(2);
	ST7567_WriteComd(0x00);	//"00"最大4倍压,"01"最大5倍压,"03"最大6倍压
 	Delay_Xms(2);
	ST7567_WriteComd(0x40);	//设定起始行第1行
 	Delay_Xms(2);
	ST7567_WriteComd(0xa4);	//Display All Points ON/OFF to Normal display mode正常显示模式选择
	Delay_Xms(2);

	ST7567_WriteComd(0x35);	// ADC select确定SEG反扫seg0--->seg131
 	Delay_Xms(2);

	ST7567_WriteComd(0xaf);	//Display ON/OFF开显示设定
 	Delay_Xms(2);

 }
 */
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
	    for(i=0;i<128;i++)
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
	    for(i=0;i<128;i++)
    	{//写132SEG
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
	page=0xb4;				//初始开始扫描页
	while(page<0xb8)		//共4页
	{
		ST7567_WriteComd(page);	//设定扫描页
	    ST7567_WriteComd(0x10);	//设定起始SEG数,两字节
//	    ST7567_WriteComd(0x00);
	    for(i=0;i<128;i++)
    	{//写132SEG
      		ST7567_WriteData1(dispdata[j]);	//调数组显示数据进行写显示
      		j++;
    	}
    	page++;	//页自动加1
	}
	// 下面这段循环为显示C32~C63
	page=0xb0;				//初始开始扫描页
	while(page<0xb4)		//共4页
	{
		ST7567_WriteComd(page);	//设定扫描页
	    ST7567_WriteComd(0x10);	//设定起始SEG数,两字节
	    ST7567_WriteComd(0x00);
	    for(i=0;i<128;i++)
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
	    for(i=0;i<128;i++)
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
	    for(i=0;i<128;i++)
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



void hz_display16(unsigned char pag, unsigned char col, unsigned char cok,unsigned char code *hzk)	   //写入一个32*32的字符
{
		 uchar a=0;	
         uchar j=0;
         uchar i=0;
		
         for(j=0;j<4;j++)                          //这里写四页
         {
             ST7567_WriteComd(0xb0+pag+j);         //page在这里就是从page表示的这页开始写起。
				
			 ST7567_WriteComd(0x10+cok);		   //双指令控制列，需加变量来控制SEG起始端
             ST7567_WriteComd(0x00+col);           //从列为col表示的列开始
				 
          for(i=0;i<32;i++)                        //这里写32列
               ST7567_WriteData(hzk[32*j+i]);      //J为页，I，为第J行第I个数
         }
}

//hz_display8x16(2,6,0,guang); 
void hz_display8x16(unsigned char pag, unsigned char col, unsigned char cok, unsigned char code *hzk) 
{
    uchar a = 0;
    uchar j = 0;
    uchar i = 0;

    for (j = 0; j < 2; j++) {  // 8x16字符占用2页
        ST7567_WriteComd(0xb0 + pag + j);  // 设置页地址

        ST7567_WriteComd(0x10 + cok);  // 设置列地址高4位
        ST7567_WriteComd(0x00 + col);  // 设置列地址低4位

        for (i = 0; i < 16; i++) {  // 每页写入8列数据
            ST7567_WriteData(hzk[16 * j + i]);  // 读取并写入数据
        }
    }
}



//		hz_display8x8(5,0, 1, words3[0]);
//	    hz_display8x8(5,0, 2, words3[1]);
//	    hz_display8x8(5,0, 3, words3[2]);
void hz_display8x8(unsigned char pag, unsigned char col, unsigned char cok, unsigned char code *hzk) 
{

    uchar j = 0;
    uchar i = 0;
	
    for (j = 0; j < 2; j++) // 8x16字符占用2页
	{  
        ST7567_WriteComd(0xb0 + pag + j);  // 设置页地址

        ST7567_WriteComd(0x10 + cok);  // 设置列地址高4位
        ST7567_WriteComd(0x00 + col);  // 设置列地址低4位

        for (i = 0; i < 8; i++) // 每页写入8列数据
		{  
            ST7567_WriteData(hzk[8 * j + i]);  // 读取并写入数据
        }
    }
}





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

/*

// ====================================//
// 8位并行数据总线的写指令子程序
// 输入参数:comd
// 输出参数:无
// ====================================//


void ST7567_WriteComd(unsigned char comd)
{
	//8080时序
//  CS = 0;								//置低CS表示选中芯片可以有效的对芯片进行控制
//	RS = 0;								//置低RS表示执行指令传输
//	E_RD = 1;							//置高E_RD写动作准备
//	dataport = comd;					//把comd传递过来的参数写入到LCD数据线上
//	RW_WR = 0;							//置低RW_WR进行上升沿的产生作准备
//	_nop_();_nop_();_nop_();_nop_();	//数据建立一定时间
//	RW_WR = 1;							//置高RW_WR产生上升沿数据此时锁存进芯片内部电路
//	_nop_();_nop_();_nop_();_nop_();	//数据保持一定时间
//	CS = 1;								//置高CS表示任何动作对芯片无效


//=============================================================
 //	6800时序
	CS = 0;								//置低CS表示选中芯片可以有效的对芯片进行控制,顺序1
	RS = 0;								//置低RS表示执行指令传输,顺序2
	RW_WR = 0;							//置低RW_WR表示写动作准备,顺序3
	E_RD = 1;							//置高E_RD进行下降沿的产生作准备,顺序4
	dataport = comd;					//把comd传递过来的参数写入到LCD数据线上,顺序5
	_nop_();_nop_();_nop_();_nop_();	//数据保持一定时间
	E_RD = 0;							//置低E_RD产生下降沿数据此时锁存进芯片内部电路,顺序6
	_nop_();_nop_();_nop_();_nop_();	//数据建立一定时间
	CS = 1;								//置高CS表示任何动作对芯片无效,顺序7
}

// ====================================//
// 8位并行数据总线的写数据子程序
// 输入参数:dat
// 输出参数:无
// ====================================//
void ST7567_WriteData(unsigned char dat)
{
	//8080时序
//	CS = 0;								//置低CS表示选中芯片可以有效的对芯片进行控制
//	RS = 1;								//置高RS表示执行数据传输
//	E_RD = 1;							//置高E_RD写动作准备
//	dataport = dat;						//把dat传递过来的参数写入到LCD数据线上
//	RW_WR = 0;							//置低RW_WR进行上升沿的产生作准备
//	_nop_();_nop_();_nop_();_nop_();	//数据建立一定时间
//	RW_WR = 1;							//置高RW_WR产生上升沿数据此时锁存进芯片内部电路
//	_nop_();_nop_();_nop_();_nop_();	//数据保持一定时间
//	CS = 1;								//置高CS表示任何动作对芯片无效
//===============================================================
	//6800时序
	CS = 0;								//置低CS表示选中芯片可以有效的对芯片进行控制,顺序1
	RS = 1;								//置高RS表示执行数据传输
	RW_WR = 0;							//置低RW_WR表示写动作准备,顺序3
	E_RD = 1;							//置高E_RD进行下降沿的产生作准备,顺序4
	dataport = dat;						//把dat传递过来的参数写入到LCD数据线上,顺序5
	_nop_();_nop_();_nop_();_nop_();	//数据保持一定时间
	E_RD = 0;							//置低E_RD产生下降沿数据此时锁存进芯片内部电路,顺序6
	_nop_();_nop_();_nop_();_nop_();	//数据建立一定时间
	CS = 1;								//置高CS表示任何动作对芯片无效,顺序7
}



 void ST7567_WriteData1(unsigned char dat)
{
	//8080时序
//	CS = 1;								//置低CS表示选中芯片可以有效的对芯片进行控制
//	RS = 1;								//置高RS表示执行数据传输
//	E_RD = 1;							//置高E_RD写动作准备
//	dataport = dat;						//把dat传递过来的参数写入到LCD数据线上
//	RW_WR = 0;							//置低RW_WR进行上升沿的产生作准备
//	_nop_();_nop_();_nop_();_nop_();	//数据建立一定时间
//	RW_WR = 1;							//置高RW_WR产生上升沿数据此时锁存进芯片内部电路
//	_nop_();_nop_();_nop_();_nop_();	//数据保持一定时间
//	CS = 1;								//置高CS表示任何动作对芯片无效
//=================================================================
	//6800时序
	CS = 1;								//置低CS表示选中芯片可以有效的对芯片进行控制,顺序1
	RS = 1;								//置高RS表示执行数据传输
	RW_WR = 0;							//置低RW_WR表示写动作准备,顺序3
	E_RD = 1;							//置高E_RD进行下降沿的产生作准备,顺序4
	dataport = dat;						//把dat传递过来的参数写入到LCD数据线上,顺序5
	_nop_();_nop_();_nop_();_nop_();	//数据保持一定时间
	E_RD = 0;							//置低E_RD产生下降沿数据此时锁存进芯片内部电路,顺序6
	_nop_();_nop_();_nop_();_nop_();	//数据建立一定时间
	CS = 1;								//置高CS表示任何动作对芯片无效,顺序7
}
*/

/*
// ====================================//
// 对比度设定及显示对比度参数函数,
// 输入参数:ADJUST
// 输出参数:只能显示到十位数
// 实现对比度的更新,及显示当前调整的数值
// ====================================//
void SetContrast(void)
{
	unsigned char i,j,page;	//定义局部变量

	ST7567_WriteComd(0x81);	//对比度调节寄存器
	ST7567_WriteComd(ADJUST);	//写入对比度数值
 //=================================================
//     i=ADJUST;
//	i=ADJUST/100;		//取出对比度数值的百位
//	page=0xb2;	//在0xb2~0xb3页上显示16*8DOTS
//	ST7567_WriteComd(page);	//设定页地址
//	ST7567_WriteComd(0x10);	//双字节设定起始SEG
//	ST7567_WriteComd(4);		//从SEG4开始显示
//	for(j=0;j<8;j++)
//	{
//		ST7567_WriteData( ASC[(i)*16+j] );	//显示字符的上半节8*8
//	}
//	page++;	//页地址加1
//	ST7567_WriteComd(page);	//设定页地址
//	ST7567_WriteComd(0x10);	//双字节设定起始SEG
//	ST7567_WriteComd(4);		//从SEG4开始显示
//	for(j=8;j<16;j++)
//	{
//		ST7567_WriteData( ASC[(i)*16+j] );	//显示字符的下半节8*8,这样完成一个整字的显示
//	} 

  //=================================================
	i=ADJUST;
	i=i/10;		//取出对比度数值的十位
	page=0xb2;	//在0xb2~0xb3页上显示16*8DOTS
	ST7567_WriteComd(page);	//设定页地址
	ST7567_WriteComd(0x10);	//双字节设定起始SEG
	ST7567_WriteComd(12);		//从SEG4开始显示
	for(j=0;j<8;j++)
	{
		ST7567_WriteData( ASC[(i)*16+j] );	//显示字符的上半节8*8
	}
	page++;	//页地址加1
	ST7567_WriteComd(page);	//设定页地址
	ST7567_WriteComd(0x10);	//双字节设定起始SEG
	ST7567_WriteComd(12);		//从SEG4开始显示
	for(j=8;j<16;j++)
	{
		ST7567_WriteData( ASC[(i)*16+j] );	//显示字符的下半节8*8,这样完成一个整字的显示
	}
  //=================================================
	i=ADJUST;
	i=i%10;		//取出对比度数值的个位
	page=0xb2;	//在0xb2~0xb3页上显示16*8DOTS
	ST7567_WriteComd(page);	//设定页地址
	ST7567_WriteComd(0x11);	//双字节设定起始SEG
	ST7567_WriteComd(4);		//从SEG12开始显示
	for(j=0;j<8;j++)
	{
		ST7567_WriteData( ASC[(i)*16+j] );	//显示字符的上半节8*8
	}
	page++;	//页地址加1
	ST7567_WriteComd(page);	//设定页地址
	ST7567_WriteComd(0x11);	//双字节设定起始SEG
	ST7567_WriteComd(4);		//从SEG12开始显示
	for(j=8;j<16;j++)
	{
		ST7567_WriteData( ASC[(i)*16+j] );	//显示字符的下半节8*8,这样完成一个整字的显示
	}
}

*/




// ====================================//
// 字符显示函数
// 输入参数:x-行，y-列 dat[]数组：显示字符16bit*8bit
// 输出参数:无
// AUTHOR: laozhongmin
//=====================================//
void disp_char(uint x,uint y,uchar dat[])  //一个字符大小：16bit*8bit
{
	unsigned char j;
 	ST7567_WriteComd(0xb0|x);	  //PAGE
	ST7567_WriteComd(0x10|(((y*8+SEG_START)&0xf0)>>4));	//MSB SEG	     
	ST7567_WriteComd((y*8+SEG_START)&0x0f);	 //LSB	SEG
	for(j=0;j<8;j++)
	{
	   ST7567_WriteData( dat[j] );	//显示字符的上半节8*8
	}
	if(LINE1<LINE2)
	ST7567_WriteComd(0xb0|x+1);	  //PAGE
	else
	ST7567_WriteComd(0xb0|x-1);	  //PAGE
	ST7567_WriteComd(0x10|(((y*8+SEG_START)&0xf0)>>4));	//MSB SEG	     
	ST7567_WriteComd((y*8+SEG_START)&0x0f);	 //LSB	SEG
	for(j=8;j<16;j++)
	{
		ST7567_WriteData( dat[j]  );	//显示字符的下半节8*8
	}
}
 // ================================================================================================
// 对比度设定及显示对比度参数函数
// 输入参数:ADJUST
// 输出参数:可以显示到百位数
// 实现对比度的更新,及显示当前调整的数值
// AUTHOR: laozhongmin
// ====================================//
void SetContrast(void)
{
  	int i;
  	RR=(float)(RATIO&0x07)*0.5+3.0;	

	ST7567_WriteComd(0x81);	//对比度调节寄存器
	ST7567_WriteComd(ADJUST);	//写入对比度数值

	VOP=RR*(1-(63.0-ADJUST)/162.0)*2.1+VOP_COMPENSATE;
	i=ADJUST/100;		//取出对比度数值的百位
	disp_char(LINE1,0,num[i]);
  
	i=ADJUST%100/10;		//取出对比度数值的十位
	disp_char(LINE1,1,num[i]);

	i=ADJUST%10;		//取出对比度数值的个位
	disp_char(LINE1,2,num[i]);
//====================================================== 显示公式计算的VOP值
//	disp_char(LINE2,0,disp_vop[0]);
//	disp_char(LINE2,1,disp_vop[1]);
//	disp_char(LINE2,2,disp_vop[2]);
//	disp_char(LINE2,3,disp_vop[3]);
//
//	i=VOP/10;		  //显示VOP数值 十位
//	disp_char(LINE2,4,num[i]);
//
//	i=(int)(VOP)%10;	//个位
//	disp_char(LINE2,5,num[i]);
//
//	disp_char(LINE2,6,disp_vop[4]);//显示小数点
//
//	i=(int)(VOP*10.0)%10; //显示小数位1
//	disp_char(LINE2,7,num[i]);
//
//	i=(int)(VOP*100.0)%10; //显示小数位2
//	disp_char(LINE2,8,num[i]);
//  	disp_char(LINE2,9,disp_vop[5]);	//跃V
}  



//=======================================================================================

// ====================================//
// 按键处理函数(带定时功能总体定时约1ms*Xms)
// 输入参数:Xms
// 输出参数:暂停键按下标志
// 功能说明: 
// 1, 在定时过程中首先判断是否有暂停键按下,如有则定时失效,此时同时判断对比度上下调节键及暂停键,
//    如果对比度调节键有按下则进行对比度调整,如果暂停键有按下则记录下暂停键有按下的状态并返回一个"1"参数并退出程序.
// 2, 在定时过程中暂停键一直没有动作,则直接返回"0"参数并退出程序.
// ====================================//
/*
unsigned char Change(unsigned int Xms)
{
	unsigned int i;
	unsigned char j,h;
	j=h=0;
	for(i=0;i<Xms;i++)
	{//需定时的ms数
		for(j=0;j<150;j++)
		{//定时的基数约1ms
			if( SW==0 )
			{//如果暂停键按下有效
				Delay_Xms(10);	//消抖动
				if ( SW==0 )
				{//暂停键按下有效
					while( SW==0 );	//等待暂停键弹起
					Delay_Xms(10);	//消抖动
					while(1)
					{
						if( SW==0 )
						{//暂停键再次按下
							Delay_Xms(10);	//消抖动
							if ( SW==0 )
							{//暂停键按下有效
								while( SW==0 );	//等待暂停键弹起
								Delay_Xms(10);	//消抖动
								h = 1;	//把暂停键标志置1
								goto Eixt;	//跳到退出程序标号
							}
						}
						else if( UP==0 )
						{//对比度+键按下
							Delay_Xms(10);	//消抖动
							if ( UP==0 )
							{//对比度+键按下有效
								while( UP==0 )	//等待此键弹起
								{
									if( DN==0 )
									{
										Delay_Xms(5);	//消抖动
										if ( DN==0 )
										{//对比度-键按下有效
											while( (DN==1)&&(UP==1) );	//等待此键弹起
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
						else if( (DN==0)&&(UP==1) )
						{//对比度-键按下
							Delay_Xms(10);	//消抖动
							if ( DN==0 )
							{//对比度-键按下有效
								while( DN==0 );	//等待此键弹起
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
*/

//------------------------------------------------------------正方形												   
//---------------------------------------------------------------

/***********Aouto and Step********************/		//不错的自动调节VOP电压对比度的程序，值得借鉴
void Pause()			   //自动调节VOP对比度最新按键程序。
{
while(AOUTOB==1&STEP==0)
	{
        if(UPT==1)
        {
            Delay_Xms(100);
           {
			ST7567_WriteComd(0x81);	//The Electronic Volume (Double Byte Command)数字电位器调节VO输出电压(也就是对比度)两字节				 0x81原来的数值
	        ST7567_WriteComd(ADJUST);	 //vop 设置
            ADJUST++;	//对比度寄存器加一档;//VOP UP ONE STEP	
			SetContrast(); 
       		}
        }
       
	    if(DOWN==1)
        {
            Delay_Xms(100);
         	{
			   	ST7567_WriteComd(0x81);	//The Electronic Volume (Double Byte Command)数字电位器调节VO输出电压(也就是对比度)两字节				 0x81原来的数值
	            ST7567_WriteComd(ADJUST);	 //vop 设置
              	ADJUST--;	 	  //VOP DOWN ONE STEP
				SetContrast(); 
            }
        }
	
     }

	if(STEP==1)
	{
	    Delay_Xms(80);
		while(AOUTOB==0)
			{
		Delay_Xms(80);	
			}
    }
	else if(AOUTOB==1)
		{
		Delay_Xms(80);
		}
}

void yehui_UI()
{
	unsigned char i,k;
	char *words1[] = {guang, dong, ye, hui, ke};
	char *words2[] = {ji, you, xian, gong, si};
	char *words3[] = {L, C, M, one,two,eight,xing,six,four};
	
//	DispPic( PIC_99 );
	for (i = 0; i < 5; i++) 
	{
		hz_display8x16(1, 8 + i, i + 1, words1[i]);
	}

	for (i = 0; i < 5; i++) 
	{	
		hz_display8x16(3, 8 + i, i + 1, words2[i]);
	}
	
	for ( k = 0; k < 9; k++)  // 显示 3 个字符
	{
	    unsigned char col_start = 28;  // 第一个字符的起始列
	    unsigned char spacing = 8;     // 每个字符占 8 列
		hz_display8x8(5, (col_start + k * spacing) & 0x0F, (col_start + k * spacing) >> 4, words3[k]);
	}
	
}


void WhatsApp()
{
	unsigned char k;
	char *words4[] = {W,h,a,t,S,a,p,p};
	char *words5[] = {JIA,eight,six,KG,one,three,six,two,two,four,nine,six,six,nine,four};

	
	DispPic( PIC_99 );	
	for ( k = 0; k < 8; k++)  // 显示 3 个字符
	{
	    unsigned char col_start = 30;  // 第一个字符的起始列
	    unsigned char spacing = 8;     // 每个字符占 8 列
		hz_display8x8(1, (col_start + k * spacing) & 0x0F, (col_start + k * spacing) >> 4, words4[k]);
	}
	
	for ( k = 0; k < 15; k++)  // 显示 3 个字符
	{
	    unsigned char col_start = 6;  // 第一个字符的起始列
	    unsigned char spacing = 8;     // 每个字符占 8 列
		hz_display8x8(4, (col_start + k * spacing) & 0x0F, (col_start + k * spacing) >> 4, words5[k]);
	}
	
}


void  YHLTD()
{
	unsigned char k;
	char *words6[] = {G,U,aa,N,G,D,O,N,G,Y,E,hh,U,I,};
	char *words7[] = {tt,E,C,hh,N,O,L,O,G,Y,C,O,DIAN,DH,L,tt,D};
	char *words8[] = {C,O,DIAN,DH,L,tt,D};
	
//	DispPic( PIC_99 );	
	for ( k = 0; k < 14; k++)  // 显示  个字符
	{
	    unsigned char col_start = 8;  // 第一个字符的起始列
	    unsigned char spacing = 8;     // 每个字符占 8 列
		hz_display8x8(1, (col_start + k * spacing) & 0x0F, (col_start + k * spacing) >> 4, words6[k]);
	}
	
	for ( k = 0; k < 14; k++)  // 显示  个字符
	{
	    unsigned char col_start = 8;  // 第一个字符的起始列
	    unsigned char spacing = 8;     // 每个字符占 8 列
		hz_display8x8(3, (col_start + k * spacing) & 0x0F, (col_start + k * spacing) >> 4, words7[k]);
	}
	
	for ( k = 0; k < 7; k++)  // 显示  个字符
	{
	    unsigned char col_start = 20;  // 第一个字符的起始列
	    unsigned char spacing = 8;     // 每个字符占 8 列
		hz_display8x8(5, (col_start + k * spacing) & 0x0F, (col_start + k * spacing) >> 4, words8[k]);
	}
	
}