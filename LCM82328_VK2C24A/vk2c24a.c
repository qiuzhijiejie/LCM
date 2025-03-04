#include "vk2c24a.h"
#include <intrins.h>
#define Vk2c24a_CLK 2 //SCL信号线频率,由delay_nus实现 50->10kHz 10->50kHz 5->100kHz

//8com
unsigned char Vk2c24a_segtab[Vk2c24a_SEGNUM]={//vk2c24A
	1,2,3,4, 5, 6, 7, 8, 9,10,11,12,13,14,15,
	16,17,18,19
};


//8com
unsigned char vk2c24a_dispram[Vk2c24a_SEGNUM];//8COM时每个字节数据对应1个SEG

unsigned char data shuzi_zimo[15]= //数字和字符字模
{             
// 0    1    2    3    4    5    6    7    8    9    -    L    H    
  0X6F,0x24,0x6B,0x6D,0x3C,0X6C,0X5F,0X64,0X7F,0X7C,0X08,0X13,0X3E
};

unsigned char Vk2c24a_InitSequence(void);
/*******************************************************************************
* Function Name  : delay_nus
* Description    : 延时1uS程序
* Input          : n->延时时间nuS
* Output         : None
* Return         : None
*******************************************************************************/
void delay_nus(unsigned int n)	   
{
	unsigned char i;
	while(n--)
	{
		i=20;
		while(i--)
		{//nop指令根据单片机做相应的修改
			_nop_();
		}
	}
}
/*******************************************************************************
* Function Name  : delay_nms
* Description    : 延时1mS程序
* Input          : n->延时时间nmS
* Output         : None
* Return         : None
*******************************************************************************/
void delay_nms(unsigned long int n)
{
	while(n--)
	{
		delay_nus(1000);
	}
}
/*******************************************************************************
* Function Name  : I2CStart
* Description    : 时钟线高时，数据线由高到低的跳变，表示I2C开始信号
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Vk2c24a_I2CStart( void )
{
  Vk2c24a_SCL_H();
  Vk2c24a_SDA_H();
  delay_nus(Vk2c24a_CLK);
  Vk2c24a_SDA_L();
  delay_nus(Vk2c24a_CLK);
}
/*******************************************************************************
* Function Name  : I2CStop
* Description    : 时钟线高时，数据线由低到高的跳变，表示I2C停止信号
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Vk2c24a_I2CStop( void )
{
  Vk2c24a_SCL_H();
  Vk2c24a_SDA_L();
  delay_nus(Vk2c24a_CLK);
  Vk2c24a_SDA_H();
  delay_nus(Vk2c24a_CLK);
}
/*******************************************************************************
* Function Name  : I2CSlaveAck
* Description    : I2C从机设备应答查询
* Input          : None
* Output         : None
* Return         : 0-ok 1-fail
*******************************************************************************/
unsigned char Vk2c24a_I2CSlaveAck( void )
{
  unsigned int TimeOut;
  unsigned char RetValue;
	
  Vk2c24a_SCL_L();
	//单片机SDA脚为单向IO要设为输入脚
//	Vk2c24a_SET_SDA_IN();
  delay_nus(Vk2c24a_CLK);
  Vk2c24a_SCL_H();//第9个sclk上升沿

  TimeOut = 10000;
  while( TimeOut-- > 0 )
  {
    if( Vk2c24a_GET_SDA()!=0 )//读取ack
    {
      RetValue = 1;
    }
    else
    {
      RetValue = 0;
      break;
    }
  } 
	Vk2c24a_SCL_L();
	//单片机SDA脚为单向IO要设为输出脚
//	Vk2c24a_SET_SDA_OUT(); 
  
  return RetValue;
}
/*******************************************************************************
* Function Name  : I2CWriteByte
* Description    : I2C写一字节,先送高位
* Input          : byte-要写入的数据
* Output         : None
* Return         : None
*******************************************************************************/
void Vk2c24a_I2CWriteByte( unsigned char byte )
{
	unsigned char i=8;
	while (i--)
	{ 
		Vk2c24a_SCL_L();
		if(byte&0x80)
			Vk2c24a_SDA_H();
		else
			Vk2c24a_SDA_L();
		byte<<=1; 
		delay_nus(Vk2c24a_CLK);
		Vk2c24a_SCL_H();     
		delay_nus(Vk2c24a_CLK);
	}
}

/*******************************************************************************
* Function Name  : WriteCmd
* Description    : 写1字节命令带参数
* Input          : cmd-要写入的命令
*                : data-要写入的参数
* Output         : None
* Return         : 0-ok 1-fail
*******************************************************************************/
unsigned char WriteCmdVk2c24a(unsigned char cmd, unsigned char Data )
{
	Vk2c24a_I2CStart();

	Vk2c24a_I2CWriteByte( Vk2c24a_ADDR|0x00 );
	if( 1 == Vk2c24a_I2CSlaveAck() )
	{
		Vk2c24a_I2CStop(); 
		return 0;   
	}
	Vk2c24a_I2CWriteByte( cmd );
	if( 1 == Vk2c24a_I2CSlaveAck() )
	{
		Vk2c24a_I2CStop(); 
		return 0;   
	}
	Vk2c24a_I2CWriteByte( Data );
	if( 1 == Vk2c24a_I2CSlaveAck() )
	{
		Vk2c24a_I2CStop(); 
		return 0;   
	}
  Vk2c24a_I2CStop();
 
  return 0;    //返回操作成败标志
}
/*******************************************************************************
* Function Name  : Write1Data
* Description    : 写1字节数据到显示RAM
* Input          : Addr-写入ram的地址
*                : Dat->写入ram的数据
* Output         : None
* Return         : 0-ok 1-fail
*******************************************************************************/
unsigned char Write1DataVk2c24a(unsigned char Addr,unsigned char Dat)
{
	//START 信号
	Vk2c24a_I2CStart(); 
	//SLAVE地址
	Vk2c24a_I2CWriteByte(Vk2c24a_ADDR); 
	if( 1 == Vk2c24a_I2CSlaveAck() )
	{		
		Vk2c24a_I2CStop();
		return 1; 
	}
	//写显示RAM命令
	Vk2c24a_I2CWriteByte(Vk2c24a_RWRAM); 						
	if( 1 == Vk2c24a_I2CSlaveAck() )
	{
		Vk2c24a_I2CStop();													
		return 0;
	}
	//显示RAM地址
	Vk2c24a_I2CWriteByte(Addr); 
	if( 1 == Vk2c24a_I2CSlaveAck() )
	{
		Vk2c24a_I2CStop();
		return 1;
	}
	//显示数据，1字节数据包含2个SEG
	Vk2c24a_I2CWriteByte(Dat);
	if( Vk2c24a_I2CSlaveAck()==1 )
	{
		Vk2c24a_I2CStop();
		return 1;
	}
	//STOP信号
 Vk2c24a_I2CStop();
 return 0;   
}
/*******************************************************************************
* Function Name  : WritenData
* Description    : 写多个数据到显示RAM
* Input          : Addr-写入ram的起始地址
*                : Databuf->写入ram的数据buffer指针
*                : Cnt->写入ram的数据个数
* Output         : None
* Return         : 0-ok 1-fail
*******************************************************************************/
unsigned char  WritenDataVk2c24a(unsigned char Addr,unsigned char *Databuf,unsigned char Cnt)
{
	unsigned char n;
	
	//START信号	
	Vk2c24a_I2CStart(); 									
	//SLAVE地址
	Vk2c24a_I2CWriteByte(Vk2c24a_ADDR); 	
	if( 1 == Vk2c24a_I2CSlaveAck() )
	{
		Vk2c24a_I2CStop();													
		return 0;										
	}
	//写显示RAM命令
	Vk2c24a_I2CWriteByte(Vk2c24a_RWRAM); 						
	if( 1 == Vk2c24a_I2CSlaveAck() )
	{
		Vk2c24a_I2CStop();													
		return 0;
	}
	//显示RAM起始地址
	Vk2c24a_I2CWriteByte(Addr); 						
	if( 1 == Vk2c24a_I2CSlaveAck() )
	{
		Vk2c24a_I2CStop();													
		return 0;
	}
	//发送Cnt个数据到显示RAM
	for(n=0;n<Cnt;n++)
	{ 
		Vk2c24a_I2CWriteByte(*Databuf++);
		if( Vk2c24a_I2CSlaveAck()==1 )
		{
			Vk2c24a_I2CStop();													
			return 0;
		}
	}
	//STOP信号
	 Vk2c24a_I2CStop();											
	 return 0;    
}
/*******************************************************************************
* Function Name  : Vk2c24a_DisAll
* Description    : 所有SEG显示同一个数据，bit7/bit3-COM3 bit6/bit2-COM2 bit5/bit1-COM1 bit4/bit0-COM0
* 					     : 例如：0xff全亮 0x00全灭 0x55灭亮灭亮 0xaa亮灭亮灭 0x33灭灭亮亮 
* Input          ：dat->写入ram的数据(1个字节数据对应2个SEG)  
* Output         : None
* Return         : None
*******************************************************************************/
void Vk2c24a_DisAll(unsigned char dat)
{
	unsigned char segi;
	unsigned char dispram[120];
	
	if(vk2c24a_maxcom==4)
	{
		for(segi=0;segi<36;segi++)
		{
			dispram[segi]=dat;
		}
		WritenDataVk2c24a(0,dispram,36);//这里送8bit数据对应2个SEG，每8bit数据地址加1，每8位数据1个ACK
	}
	else if(vk2c24a_maxcom==8)
	{
		for(segi=0;segi<68;segi++)
		{
			dispram[segi]=dat;
		}
		WritenDataVk2c24a(0,dispram,68);//这里送8bit数据对应1个SEG，每8bit数据地址加1，每8位数据1个ACK
	}
	else if(vk2c24a_maxcom==16)
	{
		for(segi=0;segi<120;segi++)
		{
			dispram[segi]=dat;
		}
		WritenDataVk2c24a(0,dispram,120);//这里送8bit数据对应1个SEG，每8bit数据地址加1，每8位数据1个ACK
	}
}
/*******************************************************************************
* Function Name  : DisSegComOn
* Description    : 点亮1个点(1个seg和1个com交叉对应的显示点)
* Input          ：seg->点对应的seg脚  
* 		         ：com->点对应com脚  
* Output         : None
* Return         : None
*******************************************************************************/
void Vk2c24a_DisSegComOn(unsigned char seg,unsigned char com)
{
	if(vk2c24a_maxcom==4)
	{
		if(seg%2==0)
			Write1DataVk2c24a(seg/2,(1<<(com)));//这里送8位数据低4bit有效，每8bit数据地址加1，每8位数据1个ACK)
		else
			Write1DataVk2c24a(seg/2,(1<<(4+com)));//这里送8位数据高4bit有效，每8bit数据地址加1，每8位数据1个ACK
	}
	else
	{
		Write1DataVk2c24a(seg,(1<<(com)));//这里送8位数据低4bit有效，每8bit数据地址加1，每8位数据1个ACK
	}
	
	
//	  unsigned char addrbit, tempdat;
//	  if (seg % 2 == 0)//判断是不是偶数
//        addrbit = (1 << com); // 对应低 4 位
//    else
//        addrbit = (1 << (4 + com)); // 对应高 4 位
//    // 将显示状态累加，而不是覆盖
//    tempdat = vk2c24a_dispram[seg / 2] | addrbit;
//    vk2c24a_dispram[seg / 2] = tempdat;
//    WritenDataVk2c24a(seg / 2, &tempdat, 1);	
}
/*******************************************************************************
* Function Name  : DisSegComOff
* Description    : 关闭1个点(1个seg和1个com交叉对应的显示点)
* Input          ：seg->点对应的seg脚  
* 		           ：com->点对应com脚  
* Output         : None
* Return         : None
*******************************************************************************/
void Vk2c24a_DisSegComOff(unsigned char seg,unsigned char com)
{
	if(vk2c24a_maxcom==4)
	{
		if(seg%2==0)
			Write1DataVk2c24a(seg/2,~(1<<com));//这里送8位数据低4bit有效，每8bit数据地址加1，每8位数据1个ACK
		else
			Write1DataVk2c24a(seg/2,~(1<<(4+com)));//这里送8位数据高4bit有效，每8bit数据地址加1，每8位数据1个ACK
	}
	else
	{
		Write1DataVk2c24a(seg,~(1<<com));//这里送8位数据低4bit有效，每8bit数据地址加1，每8位数据1个ACK
	}
}
/*******************************************************************************
* Function Name  : Enter_Standby
* Description    : 进入掉电低功耗模式,掉电无显示
* Input          ：None 
* Output         : None
* Return         : 0-ok 1-fail
*******************************************************************************/
void Vk2c24a_Enter_Standby(void)
{		
	WriteCmdVk2c24a(Vk2c24a_SYSSET,SYSOFF_LCDOFF);
}
/*******************************************************************************
* Function Name  : Exit_Standby
* Description    : 退出掉电低功耗模式
* Input          ：None 
* Output         : None
* Return         : None
*******************************************************************************/
void Vk2c24a_Exit_Standby(void)
{	
	WriteCmdVk2c24a(Vk2c24a_SYSSET,SYSON_LCDON);
}

/*******************************************************************************
* Function Name  : Lowlevel_Init
* Description    : 配置通信线GPIO
* Input          ：None 
* Output         : None
* Return         : None
*******************************************************************************/
void Vk2c24a_Lowlevel_Init(void)
{
	//通信线电平不同，建议加电平转换电路
	//此函数根据客户单片机做相应的修改	
//	Vk2c24a_SET_SDA_DIR();
//	GPIO_SetMode(Vk2c24a_SCL_PORT, Vk2c24a_SCL_PIN, GPIO_MODE_OUTPUT);
//	GPIO_SetMode(Vk2c24a_SDA_PORT, Vk2c24a_SDA_PIN, GPIO_MODE_QUASI);
		      
	Vk2c24a_SCL_H();  
	Vk2c24a_SDA_H(); 	
}
/*******************************************************************************
* Function Name  : Init
* Description    : 初始化配置
* Input          ：None 
* Output         : None
* Return         : None
*******************************************************************************/
void Vk2c24a_Init(void)
{	
	//管脚配置根据客户单片机做相应的修改
	Vk2c24a_Lowlevel_Init();
//	WriteCmdVk2c24a(Vk2c24a_MODESET,BIAS_1_2|COM_4); 	//模式设置  1/2 Bais 1/8 Duty
//	WriteCmdVk2c24a(Vk2c24a_MODESET,BIAS_1_3|COM_4); 	//模式设置  1/3 Bais 1/4 Duty
//	WriteCmdVk2c24a(Vk2c24a_MODESET,BIAS_1_4|COM_4); 	//模式设置  1/4 Bais 1/4 Duty
//	WriteCmdVk2c24a(Vk2c24a_MODESET,BIAS_1_5|COM_4); 	//模式设置  1/5 Bais 1/4 Duty
//	vk2c24a_maxcom=4;
	
//	WriteCmdVk2c24a(Vk2c24a_MODESET,BIAS_1_2|COM_8); 	//模式设置  1/2 Bais 1/8 Duty
	WriteCmdVk2c24a(Vk2c24a_MODESET,BIAS_1_3|COM_8); 	//模式设置  1/3 Bais 1/8 Duty
//	WriteCmdVk2c24a(Vk2c24a_MODESET,BIAS_1_4|COM_8); 	//模式设置  1/4 Bais 1/8 Duty
//	WriteCmdVk2c24a(Vk2c24a_MODESET,BIAS_1_5|COM_8); 	//模式设置  1/5 Bais 1/8 Duty
	vk2c24a_maxcom=8;
//	WriteCmdVk2c24a(Vk2c24a_MODESET,BIAS_1_3|COM_16); //模式设置  1/3 Bais 1/16 Duty
//	WriteCmdVk2c24a(Vk2c24a_MODESET,BIAS_1_4|COM_16); //模式设置  1/3 Bais 1/16 Duty
//	WriteCmdVk2c24a(Vk2c24a_MODESET,BIAS_1_5|COM_16); //模式设置  1/3 Bais 1/16 Duty
//	vk2c24a_maxcom=16;
	WriteCmdVk2c24a(Vk2c24a_SYSSET,SYSON_LCDON); 	//内部系统振荡器开，lcd开显示
//	WriteCmdVk2c24a(Vk2c24a_FRAMESET,FRAME_80HZ); 	//帧频率80Hz
//	WriteCmdVk2c24a(Vk2c24a_FRAMESET,FRAME_160HZ);  //帧频率160Hz
	
//	WriteCmdVk2c24a(Vk2c24a_BLINKSET,BLINK_OFF); 		//闪烁关闭	
//	WriteCmdVk2c24a(Vk2c24a_BLINKSET,BLINK_2HZ); 		//闪烁2HZ
//	WriteCmdVk2c24a(Vk2c24a_BLINKSET,BLINK_1HZ); 		//闪烁1HZ
//	WriteCmdVk2c24a(Vk2c24a_BLINKSET,BLINK_0D5HZ); 	  //闪烁0.5HZ


	//SEG/VLCD共用脚设为VLCD，内部电压调整功能关闭,VLCD和VDD短接VR=0偏置电压=VDD
//	WriteCmdVk2c24a(Vk2c24a_IVASET,VLCDSEL_IVAOFF_R0); 
	//SEG/VLCD共用脚设为VLCD，内部电压调整功能关闭,VLCD和VDD串接电阻VR>0偏置电压=VLCD
//	WriteCmdVk2c24a(Vk2c24a_IVASET,VLCDSEL_IVAOFF_R1); 
	//SEG/VLCD共用脚设为SEG，内部偏置电压调整：1/3bias=0.652VDD 1/4bias=0.714VDD
	WriteCmdVk2c24a(Vk2c24a_IVASET,SEGSEL_IVA00H);	
	




}
/*******************************************************************************
* Function Name  : disp_3num
* Description    : 显示3位数字
* Input          ：dat->3位数字 十进制 
* Output         : None
* Return         : None
*******************************************************************************/
void disp_3num(unsigned int dat)
{		
	unsigned dat8;
		
	if(vk2c24a_maxcom==4)
	{
		dat8=dat/100%10;	//显示百位
		vk2c24a_dispram[2]&=0xf0;
		vk2c24a_dispram[2]|=shuzi_zimo[dat8]&0x0f;
		vk2c24a_dispram[2]&=0x8f;
		vk2c24a_dispram[2]|=shuzi_zimo[dat8]&0xf0;
		
		dat8=dat/10%10; 	//显示十位
		vk2c24a_dispram[3]&=0xf0;
		vk2c24a_dispram[3]|=shuzi_zimo[dat8]&0x0f;
		vk2c24a_dispram[3]&=0x8f;
		vk2c24a_dispram[3]|=shuzi_zimo[dat8]&0xf0;
		
		dat8=dat%10;			//显示个位
		vk2c24a_dispram[4]&=0xf0;
		vk2c24a_dispram[4]|=shuzi_zimo[dat8]&0x0f;
		vk2c24a_dispram[4]&=0x8f;
		vk2c24a_dispram[4]|=shuzi_zimo[dat8]&0xf0;
			
		if(dat<100)				//数字小于100，百位不显示
		{
		vk2c24a_dispram[2]&=0xf0;
		vk2c24a_dispram[2]&=0x8f;
		}
		if(dat<10) 	//数字小于10，十位不显示
		{
		vk2c24a_dispram[3]&=0xf0;
		vk2c24a_dispram[3]&=0x8f;
		}
		//SEG不连续1个1个数据送
//		Write1DataVk2c24a(2,vk2c24a_dispram[2]);
//		Write1DataVk2c24a(3,vk2c24a_dispram[3]);
//		Write1DataVk2c24a(4,vk2c24a_dispram[4]);
		//SEG连续送多个数据
		WritenDataVk2c24a(2,&vk2c24a_dispram[2],3);
	}
	else
	{
		dat8=dat/100%10;	//显示百位
		vk2c24a_dispram[2]&=0x80;
		vk2c24a_dispram[2]|=shuzi_zimo[dat8];
		
		dat8=dat/10%10; 	//显示十位
		vk2c24a_dispram[3]&=0x80;
		vk2c24a_dispram[3]|=shuzi_zimo[dat8];
		
		dat8=dat%10;			//显示个位
		vk2c24a_dispram[4]&=0x80;
		vk2c24a_dispram[4]|=shuzi_zimo[dat8];
			
		if(dat<100)				//数字小于100，百位不显示
		{
			vk2c24a_dispram[2]&=0x80;
		}
		if(dat<10) 	//数字小于10，十位不显示
		{
			vk2c24a_dispram[3]&=0x80;
		}
		//SEG不连续1个1个数据送
//		Write1DataVk2c24a(2,vk2c24a_dispram[2]);
//		Write1DataVk2c24a(3,vk2c24a_dispram[3]);
//		Write1DataVk2c24a(4,vk2c24a_dispram[4]);
		//SEG连续送多个数据
		WritenDataVk2c24a(2,&vk2c24a_dispram[2],3);
	}
}	


void DisplayDigitOrChar(unsigned char pos, unsigned char index) 
{
	unsigned char  Data;
    // 检查索引是否在数组范围内
    if (index >= sizeof(shuzi_zimo)) 
		return;

    // 获取字模数据
    Data = shuzi_zimo[index];

    // 使用 Write1DataVk2c22 将数据写入显示 RAM 的指定位置
    Write1DataVk2c24a(pos, Data);
}