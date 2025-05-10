/*
	引脚接线：
	CS-------PA0-------(片选)
	CD-------PA1-------(指令/数据)
	RST------PA2-------(复位)
	SCK------PA3-------(时钟)
	SDA------PA4-------(数据)
*/
#include"stm32f10x.h"
#include"gpio.h"															//包含显示屏引脚定义信息

unsigned char jiong1[]={//-- 文字: 囧 --
//-- 此字体下对应的点阵为：宽x 高=16x16 --
0x00,0xFE,0x82,0x42,0xA2,0x9E,0x8A,0x82,0x86,0x8A,0xB2,0x62,0x02,0xFE,0x00,0x00,
0x00,0x7F,0x40,0x40,0x7F,0x40,0x40,0x40,0x40,0x40,0x7F,0x40,0x40,0x7F,0x00,0x00};

unsigned char lei1[]={//-- 文字: 畾 --
//-- 此字体下对应的点阵为：宽x 高=16x16 --
0x80,0x80,0x80,0xBF,0xA5,0xA5,0xA5,0x3F,0xA5,0xA5,0xA5,0xBF,0x80,0x80,0x80,0x00,
0x7F,0x24,0x24,0x3F,0x24,0x24,0x7F,0x00,0x7F,0x24,0x24,0x3F,0x24,0x24,0x7F,0x00};


/* 用于短暂的延时，在72MHz的频率下延时时长5us */
void Delay(unsigned int z)
{
	unsigned int x,y;
	for(x=z;x>0;x--)
	{
		for(y=7;y>0;y--);
	}
}
//显示屏指令集
unsigned char IC[]={
	0xe2,						//显示屏复位指令
	0xa3,						//设置帧速率[A0: 76fps, A1b: 95fps, A2b: 132fps, A3b: 168fps(fps: frame-per-second)] 
	0xeb,						//设置LCD偏置比(亮度设置)
	0x2f,						//显示屏功耗设置
	0xc2,						//设置LCD映射控制
	0x81,						//设置SEG偏置电压(对比度) 双字节指令
	180, 						//设置SEG偏置电压(对比度) 双字节指令
	0xaf,						//开启显示指令
};
//向显示屏发送数据函数
//C=1表示发送的是显示数据 C=0表示发送的是指令代码
void WriteData(unsigned char data, unsigned char C)
{
	unsigned char i,dat;								//i用于循环写入8位数据，dat用于缓存数据
  CS_0();															//使能片选
	if(C==1)														//判断要写入的是指令还是数据
	{
		CD_1();														//等于1 就是数据
	}
	else
	{
		CD_0();														//等于0 就是指令
 	}
	dat=data;														//缓存数据
	for(i=0;i<8;i++)										//循环发送8位数据
	{
		if((dat&0x80)==0x80)							//判断最高位是否为1
		{
			SDA_1();												//为1 拉高总线写入1
		}
		else
		{
			SDA_0();												//为0 拉低总线写入0
		}
		SCK_0();													//产生一个时钟
		Delay(5);
		
		SCK_1();													//时钟结束
		Delay(5);
		
		dat<<=1;													//左移一位 准备写入下一位
	}
	CS_1();															//写入完毕，取消片选
}
//显示屏初始化函数
void Init_lcd(void)
{
	unsigned int i=0;										//用于循环写入指令
	Delay(50000);												//上电延时 等待电源稳定
	
	RST_0();														//复位显示屏
  Delay(3000);
	RST_1();														//复位完成
	Delay(6000);												//注意：此延时必须要有且保持5毫秒左右
	
  for(i=0;i<8;i++)										//循环写入初始化指令
  {
		WriteData(IC[i],0);								//写入指令
  }
}

//图片显示函数
void Dispgraphic(unsigned char D)
{
  unsigned char i,j;                 	//用于循环写入显示数据
  for(i=0;i<8;i++)                 	 	//总共写入8页的数据
  {
    WriteData(0x40,0);              	//设置滚动线
    WriteData(0xb0|i,0);            	//设置页地址
    
    WriteData(0x10,0);              	//设置列地址 （双字节指令）
    WriteData(0x00,0);             	 	//设置列地址为0从第一列开始写数据
    
    for(j=0;j<192;j++)             		//准备写入192列数据
    {
      WriteData(D,1);               	//写入显示数据
    }
  }
}

/*16*16汉字显示函数*/
//y:页地址  l:列地址  *p:要显示汉字的指针
void Display_Chinese(unsigned char y,unsigned char l,unsigned char *p)
{
	unsigned char i,j;									//用于循环写入显示数据
  for(i=0;i<2;i++)										//总共写入8页的数据
  {
		WriteData((0xb0|i)+y,0);					    //设置页地址
		
		WriteData(0x10+(l>>4&0x0F),0);		            //设置列地址 （双字节指令）
		WriteData(l&0x0F,0);							//设置列地址为0从第一列开始写数据
		
		for(j=0;j<16;j++)							    //准备写入16列数据
		{
			WriteData(*p++,1);							//写入显示数据
		}
	}
}
//主函数
int main(void)
{
	Gpio_Init();												//引脚初始化
	Init_lcd();													//显示屏初始化
	
	Dispgraphic(0x00);									//显示屏清屏
	
	Display_Chinese(0,0,jiong1);				//第0页的第0列显示汉字
	Display_Chinese(0,20,lei1);					//第0页的第20列显示汉字
	
  while(1);														//显示结束
}

