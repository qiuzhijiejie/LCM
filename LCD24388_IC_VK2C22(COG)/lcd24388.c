 #include "lcd24388.h"
#include <intrins.h> 
sbit   KEY1  = P2^0;	

//segtab[]数组对应实际的芯片到LCD连线，连线见-VK2c22参考电路
//4com 
//Vk2c22a 
//unsigned char vk2c22_segtab[Vk2c22_SEGNUM]={
//	0,1,2,3,4,5,6,7,8,9,			//SEG0-SEG9
//	10,11,12,13,14,15,16,17,18,19,	//SEG10-SEG19
//	20,21,22,23,24,25,26,27,28,29,	//SEG20-SEG29
//	30,31,32,33,34,35,36,37,38,39,  //SEG30-SEG39
//	40,41,42,43,					//SEG40-SEG43
//};
//Vk2c22b 
//unsigned char vk2c22_segtab[Vk2c22_SEGNUM]={
//	0,1,2,3,4,5,6,7,8,9,			//SEG0-SEG9
//	10,11,12,13,14,15,16,17,18,19,	//SEG10-SEG19
//	20,21,22,23,24,25,26,27,28,29,	//SEG20-SEG29
//	30,31,32,33,34,35,36,37,38,39,  //SEG30-SEG39
//	40,41,
//};
//例：LCD实际只接了SEG10到SEG21使用下面数组
//.h文件配置VK2c22_SEGNUM=12
//const unsigned char vk2c22_segtab[VK2c22_SEGNUM]={	
//	10,11,12,13,14,15,16,17,18,19,	//SEG10-SEG19
//	20,21,													//SEG20-SEG21
//};
//vk2c22_dispram对应芯片的显示RAM
//4com
unsigned char vk2c22_dispram[Vk2c22_SEGNUM/2];//4COM时每个字节数据对应2个SEG
//对应数组vk2c22_segtab[VK2c22_SEGNUM]
//显示 RAM buffer为8位对应2个SEG，bit7/bit3->com3,bit6/bit2->com2,bit5/bit1->com1,bit4/bit0->com0
//LCD 实例  3个8字
//1是百位，2是十位，3是个位
/*显示buffer和lcd图案映射关系如下
/4COM
vk2c22_dispram[VK2c22_SEGNUM/2]=
{//com3   com2   com1     com0 //对应芯片SEG脚  对应显示RAM数据
	1D,     1E,    1F,      1A,     //seg4   			vk2c22_dispram[2]-bit3-bit0
	  ,     1C,    1G,      1B,     //seg5				vk2c22_dispram[2]-bit7-bit4
	2D,     2E,    2F,      2A,     //seg6				vk2c22_dispram[3]-bit3-bit0
	  ,     2C,    2G,      2B,     //seg7				vk2c22_dispram[3]-bit7-bit4
	3D,     3E,    3F,      3A,		//seg8				vk2c22_dispram[4]-bit3-bit0
	  ,     3C,    3G,      3B,		//seg9				vk2c22_dispram[4]-bit7-bit4
}
*/



unsigned char Vk2c22_InitSequence(void);
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
		i=10;
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
void Vk2c22_I2CStart( void )
{
  Vk2c22_SCL_H();
  Vk2c22_SDA_H();
  delay_nus(VK2c22_CLK);
  Vk2c22_SDA_L();
  delay_nus(VK2c22_CLK);
}
/*******************************************************************************
* Function Name  : I2CStop
* Description    : 时钟线高时，数据线由低到高的跳变，表示I2C停止信号
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Vk2c22_I2CStop( void )
{
  Vk2c22_SCL_H();
  Vk2c22_SDA_L();
  delay_nus(VK2c22_CLK);
  Vk2c22_SDA_H();
  delay_nus(VK2c22_CLK);
}
/*******************************************************************************
* Function Name  : I2CSlaveAck
* Description    : I2C从机设备应答查询
* Input          : None
* Output         : None
* Return         : 0-ok 1-fail
*******************************************************************************/
unsigned char Vk2c22_I2CSlaveAck( void )
{
  unsigned int TimeOut;
  unsigned char RetValue;
	
  Vk2c22_SCL_L();
	//单片机SDA脚为单向IO要设为输入脚
//	Vk2c22_SET_SDA_IN();
  delay_nus(VK2c22_CLK);
  Vk2c22_SCL_H();//第9个sclk上升沿

  TimeOut = 10000;
  while( TimeOut-- > 0 )
  {
    if( Vk2c22_GET_SDA()!=0 )//读取ack
    {
      RetValue = 1;
    }
    else
    {
      RetValue = 0;
      break;
    }
  } 
	Vk2c22_SCL_L();
	//单片机SDA脚为单向IO要设为输出脚
//	Vk2c22_SET_SDA_OUT();  
  
  return RetValue;
}
/*******************************************************************************
* Function Name  : I2CWriteByte
* Description    : I2C写一字节命令,命令先送高位
* Input          : byte-要写入的数据
* Output         : None
* Return         : None
*******************************************************************************/
void Vk2c22_I2CWriteByte( unsigned char byte )
{
	unsigned char i=8;
	while (i--)
	{ 
		Vk2c22_SCL_L();
		if(byte&0x80)
			Vk2c22_SDA_H();
		else
			Vk2c22_SDA_L();
		byte<<=1; 
		delay_nus(VK2c22_CLK);
		Vk2c22_SCL_H();     
		delay_nus(VK2c22_CLK);
	}
}

/*******************************************************************************
* Function Name  : WriteCmd
* Description    : 写1字节命令
* Input          : cmd-要写入的命令
* Output         : None
* Return         : 0-ok 1-fail
*******************************************************************************/
unsigned char WriteCmdVk2c22(unsigned char cmd)
{
	Vk2c22_I2CStart();

	Vk2c22_I2CWriteByte( Vk2c22_ADDR|0x00 );
	if( 1 == Vk2c22_I2CSlaveAck() )
	{
		Vk2c22_I2CStop(); 
		return 0;   
	}
	Vk2c22_I2CWriteByte( cmd );
	if( 1 == Vk2c22_I2CSlaveAck() )
	{
		Vk2c22_I2CStop(); 
		return 0;   
	}
  Vk2c22_I2CStop();
 
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
unsigned char Write1DataVk2c22(unsigned char Addr,unsigned char Dat)
{
	//START 信号
	Vk2c22_I2CStart(); 
	//SLAVE地址
	Vk2c22_I2CWriteByte(Vk2c22_ADDR); 
	if( 1 == Vk2c22_I2CSlaveAck() )
	{		
		Vk2c22_I2CStop();
		return 1; 
	}
	//显示RAM地址
	Vk2c22_I2CWriteByte(Addr); 
	if( 1 == Vk2c22_I2CSlaveAck() )
	{
		Vk2c22_I2CStop();
		return 1;
	}
	//显示数据，1字节数据包含2个SEG
	Vk2c22_I2CWriteByte(Dat);
	if( Vk2c22_I2CSlaveAck()==1 )
	{
		Vk2c22_I2CStop();
		return 1;
	}
	//STOP信号
 Vk2c22_I2CStop();
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
unsigned char  WritenDataVk2c22(unsigned char Addr,unsigned char *Databuf,unsigned char Cnt)
{
	unsigned char n;
	
	//START信号	
	Vk2c22_I2CStart(); 									
	//SLAVE地址
	Vk2c22_I2CWriteByte(Vk2c22_ADDR); 	
	if( 1 == Vk2c22_I2CSlaveAck() )
	{
		Vk2c22_I2CStop();													
		return 0;										
	}
	//显示RAM起始地址
	Vk2c22_I2CWriteByte(Addr); 						
	if( 1 == Vk2c22_I2CSlaveAck() )
	{
		Vk2c22_I2CStop();													
		return 0;
	}
	//发送Cnt个数据到显示RAM
	for(n=0;n<Cnt;n++)
	{ 
		Vk2c22_I2CWriteByte(*Databuf++);
		if( Vk2c22_I2CSlaveAck()==1 )
		{
			Vk2c22_I2CStop();													
			return 0;
		}
	}
	//STOP信号
	 Vk2c22_I2CStop();											
	 return 0;    
}
/*******************************************************************************
* Function Name  : Vk2c22_DisAll
* Description    : 所有SEG显示同一个数据，bit7/bit3-COM3 bit6/bit2-COM2 bit5/bit1-COM1 bit4/bit0-COM0
* 					     : 例如：0xff全亮 0x00全灭 0x55灭亮灭亮 0xaa亮灭亮灭 0x33灭灭亮亮 
* Input          ：dat->写入ram的数据(1个字节数据对应2个SEG)  
* Output         : None
* Return         : None
*******************************************************************************/
void Vk2c22_DisAll(unsigned char dat)
{
	unsigned char segi;
	unsigned char dispram[22];
	
	for(segi=0;segi<28;segi++)
	{
		dispram[segi]=dat;
	}
	WritenDataVk2c22(0,dispram,22);//这里送8bit数据对应2个SEG，每8bit数据地址加1，每8位数据1个ACK	
}
/*******************************************************************************
* Function Name  : DisSegComOn
* Description    : 点亮1个点(1个seg和1个com交叉对应的显示点)
* Input          ：seg->点对应的seg脚  
* 		           ：com->点对应com脚  
* Output         : None
* Return         : None
*******************************************************************************/

void Vk2c22_DisSegComOn(unsigned char seg,unsigned char com)
{
	unsigned char addrbit, tempdat;
//	if(seg%2==0)
//		Write1DataVk2c22(seg/2,(1<<(com)));//这里送8位数据低4bit有效，每8bit数据地址加1，每8位数据1个ACK)
//	else
//		Write1DataVk2c22(seg/2,(1<<(4+com)));//这里送8位数据高4bit有效，每8bit数据地址加1，每8位数据1个ACK	
	
    if (seg % 2 == 0)//判断是不是偶数
        addrbit = (1 << com); // 对应低 4 位
    else
        addrbit = (1 << (4 + com)); // 对应高 4 位
    // 将显示状态累加，而不是覆盖
    tempdat = vk2c22_dispram[seg / 2] | addrbit;
    vk2c22_dispram[seg / 2] = tempdat;
    WritenDataVk2c22(seg / 2, &tempdat, 1);	
}
/*******************************************************************************
* Function Name  : DisSegComOff
* Description    : 关闭1个点(1个seg和1个com交叉对应的显示点)
* Input          ：seg->点对应的seg脚  
* 		           ：com->点对应com脚  
* Output         : None
* Return         : None
*******************************************************************************/
void Vk2c22_DisSegComOff(unsigned char seg,unsigned char com)
{
	if(seg%2==0)
		Write1DataVk2c22(seg/2,~(1<<com));//这里送8位数据低4bit有效，每8bit数据地址加1，每8位数据1个ACK
	else
		Write1DataVk2c22(seg/2,~(1<<(4+com)));//这里送8位数据高4bit有效，每8bit数据地址加1，每8位数据1个ACK	
}
/*******************************************************************************
* Function Name  : Enter_Standby
* Description    : 进入掉电低功耗模式,掉电无显示
* Input          ：None 
* Output         : None
* Return         : 0-ok 1-fail
*******************************************************************************/
void Vk2c22_Enter_Standby(void)
{		
//	WriteCmdVk2c22(Vk2c22_MODESET|BIAS_1_2|F_80HZ|RCOFF_LCDOFF);  //模式设置  1/2 Bais 1/4 Duty帧频率80Hz,内部系统振荡器开，lcd关显示
	WriteCmdVk2c22(Vk2c22_MODESET|BIAS_1_3|F_80HZ|RCOFF_LCDOFF);  //模式设置  1/3 Bais 1/4 Duty帧频率80Hz,内部系统振荡器开，lcd关显示
//	WriteCmdVk2c22(Vk2c22_MODESET|BIAS_1_2|F_160HZ|RCOFF_LCDOFF); //模式设置  1/2 Bais 1/4 Duty帧频率160Hz,内部系统振荡器开，lcd关显示
//	WriteCmdVk2c22(Vk2c22_MODESET|BIAS_1_3|F_160HZ|RCOFF_LCDOFF); //模式设置  1/3 Bais 1/4 Duty帧频?60Hz,内部系统振荡器开，lcd关显示
}
/*******************************************************************************
* Function Name  : Exit_Standby
* Description    : 退出掉电低功耗模式
* Input          ：None 
* Output         : None
* Return         : None
*******************************************************************************/
void Vk2c22_Exit_Standby(void)
{	
//	WriteCmdVk2c22(Vk2c22_MODESET|BIAS_1_2|F_80HZ|RCON_LCDON);  //模式设置  1/2 Bais 1/4 Duty帧频率80Hz,内部系统振荡器开，lcd开显示
	WriteCmdVk2c22(Vk2c22_MODESET|BIAS_1_3|F_80HZ|RCON_LCDON);  //模式设置  1/3 Bais 1/4 Duty帧频率80Hz,内部系统振荡器开，lcd开显示
//	WriteCmdVk2c22(Vk2c22_MODESET|BIAS_1_2|F_160HZ|RCON_LCDON); //模式设置  1/2 Bais 1/4 Duty帧频率160Hz,内部系统振荡器开，lcd开显示
//	WriteCmdVk2c22(Vk2c22_MODESET|BIAS_1_3|F_160HZ|RCON_LCDON); //模式设置  1/3 Bais 1/4 Duty帧频?60Hz,内部系统振荡器开，lcd开显示
}

/*******************************************************************************
* Function Name  : Lowlevel_Init
* Description    : 配置通信线GPIO
* Input          ：None 
* Output         : None
* Return         : None
*******************************************************************************/
void Vk2c22_Lowlevel_Init(void)
{
	//通信线电平不同，建议加电平转换电路
	//此函数根据客户单片机做相应的修改	
//	Vk2c22_SET_SDA_DIR();
//	GPIO_SetMode(Vk2c22_SCL_PORT, Vk2c22_SCL_PIN, GPIO_MODE_OUTPUT);
//	GPIO_SetMode(Vk2c22_SDA_PORT, Vk2c22_SDA_PIN, GPIO_MODE_QUASI);
		     
	Vk2c22_SCL_H();  
	Vk2c22_SDA_H(); 	
}
/*******************************************************************************
* Function Name  : Init
* Description    : 初始化配置
* Input          ：None 
* Output         : None
* Return         : None
*******************************************************************************/
void Vk2c22_Init(void)
{	
	//管脚配置根据客户单片机做相应的修改
	Vk2c22_Lowlevel_Init();
	//1/4 Duty
//	WriteCmdVk2c22(Vk2c22_MODESET|BIAS_1_2|F_80HZ|RCON_LCDON);  //模式设置  1/2 Bais 1/4 Duty帧频率80Hz,内部系统振荡器开，lcd开显示
	WriteCmdVk2c22(Vk2c22_MODESET|BIAS_1_3|F_80HZ|RCON_LCDON);  //模式设置  1/3 Bais 1/4 Duty帧频率80Hz,内部系统振荡器开，lcd开显示
//	WriteCmdVk2c22(Vk2c22_MODESET|BIAS_1_2|F_160HZ|RCON_LCDON); //模式设置  1/2 Bais 1/4 Duty帧频率160Hz,内部系统振荡器开，lcd开显示
//	WriteCmdVk2c22(Vk2c22_MODESET|BIAS_1_3|F_160HZ|RCON_LCDON); //模式设置  1/3 Bais 1/4 Duty帧频?60Hz,内部系统振荡器开，lcd开显示
	vk2c22_maxcom=4;
	WriteCmdVk2c22(Vk2c22_BLINKSET|BLINK_OFF); 		//闪烁关闭	
//	WriteCmdVk2c22(Vk2c22_BLINKSET|BLINK_2HZ); 		//闪烁2HZ
//	WriteCmdVk2c22(Vk2c22_BLINKSET|BLINK_1HZ); 		//闪烁1HZ
//	WriteCmdVk2c22(Vk2c22_BLINKSET|BLINK_0D5HZ); 	//闪烁0.5HZ
	//SEG/VLCD共用脚设为VLCD，内部电压调整功能关闭,VLCD和VDD短接VR=0偏置电压=VDD
//	WriteCmdVk2c22(Vk2c22_IVASET|VLCDSEL_IVAOFF_R0); 
	//SEG/VLCD共用脚设为VLCD，内部电压调整功能关闭,VLCD和VDD串接电阻VR>0偏置电压=VLCD
//	WriteCmdVk2c22(Vk2c22_IVASET|VLCDSEL_IVAOFF_R1); 
	//SEG/VLCD共用脚设为SEG，内部偏置电压调整：1/3bias=0.652VDD 1/4bias=0.714VDD
	WriteCmdVk2c22(Vk2c22_IVASET|SEGSEL_IVA02H);	 
}
/*******************************************************************************
* Function Name  : disp_3num
* Description    : 显示3位数字
* Input          ：dat->3位数字 十进制 
* Output         : None
* Return         : None
*******************************************************************************/
//void disp_3num(unsigned int dat)
//{		
//	unsigned dat8;
//		
//	dat8=dat/100%10;	//显示百位
//	vk2c22_dispram[2]&=0xf0;
//	vk2c22_dispram[2]|=shuzi_zimo[dat8]&0x0f;
//	vk2c22_dispram[2]&=0x8f;
//	vk2c22_dispram[2]|=shuzi_zimo[dat8]&0xf0;
//	
//	dat8=dat/10%10; 	//显示十位
//	vk2c22_dispram[3]&=0xf0;
//	vk2c22_dispram[3]|=shuzi_zimo[dat8]&0x0f;
//	vk2c22_dispram[3]&=0x8f;
//	vk2c22_dispram[3]|=shuzi_zimo[dat8]&0xf0;
//	
//	dat8=dat%10;			//显示个位
//	vk2c22_dispram[4]&=0xf0;
//	vk2c22_dispram[4]|=shuzi_zimo[dat8]&0x0f;
//	vk2c22_dispram[4]&=0x8f;
//	vk2c22_dispram[4]|=shuzi_zimo[dat8]&0xf0;
//		
//	if(dat<100)				//数字小于100，百位不显示
//	{
//	vk2c22_dispram[2]&=0xf0;
//	vk2c22_dispram[2]&=0x8f;
//	}
//	if(dat<10) 	//数字小于10，十位不显示
//	{
//	vk2c22_dispram[3]&=0xf0;
//	vk2c22_dispram[3]&=0x8f;
//	}
//	//SEG不连续1个1个数据送
//		Write1DataVk2c22(2,vk2c22_dispram[2]);
//		Write1DataVk2c22(3,vk2c22_dispram[3]);
//		Write1DataVk2c22(4,vk2c22_dispram[4]);
//	//SEG连续送多个数据
////	WritenDataVk2c22(2,&vk2c22_dispram[2],3);	
//}	




unsigned char Key() 
{
    if (KEY1 == 0) 
	{  
        delay_nms(1);  
        if (KEY1 == 0) 
		{  
            while (KEY1 == 0);  
            return 1;  
        }
    }
    return 0;  
}


unsigned char data shuzi_zimo[15]= //数字和字符字模
{             
// 0    1    2    3    4    5    6    7    8    9    -    L    0    H    
  0XAF,0xA0,0xCB,0xE9,0xE4,0X6D,0X6F,0XA8,0XEF,0XEC,0X40,0X07,0XAF,0XE6,0xFF,
};

unsigned char data shuzi_zimo2[15]= //数字和字符字模
{             
//   0    1    2    3    4    5    6    7    8    9    -    L    0    H 	
	0XFA,0X0A,0xBC,0X9E,0X4E,0XD6,0XF6,0X8A,0XFE,0XCE,0X40,0X70,0XFA,0X6E,0xFF,
};

void DisplayDigitOrChar(unsigned char pos, unsigned char index) 
{
	unsigned char  Data;
    // 检查索引是否在数组范围内
    if (index >= sizeof(shuzi_zimo)) 
		return;

    // 获取字模数据
    Data = shuzi_zimo[index];

    // 使用 Write1DataVk2c22 将数据写入显示 RAM 的指定位置
    Write1DataVk2c22(pos, Data);
}


void DisplayDigitOrChar2(unsigned char pos, unsigned char index) 
{
	unsigned char  Data;
    // 检查索引是否在数组范围内
    if (index >= sizeof(shuzi_zimo2)) 
		return;

    // 获取字模数据
    Data = shuzi_zimo2[index];

    // 使用 Write1DataVk2c22 将数据写入显示 RAM 的指定位置
    Write1DataVk2c22(pos, Data);
}



