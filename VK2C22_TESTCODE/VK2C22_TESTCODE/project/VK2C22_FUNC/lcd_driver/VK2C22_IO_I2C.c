/**
  ******************************************************************************
  * @file    VK2C22.c
  * @author  kevin_guo
  * @version V1.2
  * @date    10-09-2024
  * @brief   This file contains all the VK2C22 functions. 
	*          此文件适用于 VK2C22
  ******************************************************************************
  * @attention
  ******************************************************************************
  */	
/* Includes ------------------------------------------------------------------*/
#include "VK2C22_IO_I2C.h"
  
/* extern variables ----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define VK2C22_CLK 100 //SCL信号线频率,由delay_nus实现 50->10kHz 10->50kHz 5->100kHz
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
//segtab[]数组对应实际的芯片到LCD连线，连线见-VK2C22参考电路
//4com 
//VK2C22A
//unsigned char VK2C22_segtab[VK2C22_SEGNUM]={
//	0,1,2,3,4,5,6,7,8,9,						//SEG0-SEG9
//	10,11,12,13,14,15,16,17,18,19,	//SEG10-SEG19
//	20,21,22,23,24,25,26,27,28,29,	//SEG20-SEG29
//	30,31,32,33,34,35,36,37,38,39,  //SEG30-SEG39
//	40,41,42,43,									  //SEG40-SEG43
//};
//VK2C22B 
unsigned char VK2C22_segtab[VK2C22_SEGNUM]={
	0,1,2,3,4,5,6,7,8,9,						//SEG0-SEG9
	10,11,12,13,14,15,16,17,18,19,	//SEG10-SEG19
	20,21,22,23,24,25,26,27,28,29,	//SEG20-SEG29
	30,31,32,33,34,35,36,37,38,39,  //SEG30-SEG39
};

//例：LCD实际只接了SEG10到SEG21使用下面数组
//.h文件配置VK2C22_SEGNUM=12
//const unsigned char VK2C22_segtab[VK2C22_SEGNUM]={	
//	10,11,12,13,14,15,16,17,18,19,	//SEG10-SEG19
//	20,21,													//SEG20-SEG21
//};
//VK2C22_dispram对应芯片的显示RAM
//4com
unsigned char VK2C22_dispram[VK2C22_SEGNUM/2];//4COM时每个字节数据对应2个SEG
//对应数组VK2C22_segtab[VK2C22_SEGNUM]
//显示RAM buffer为8位对应2个SEG，bit7/bit3->com3,bit6/bit2->com2,bit5/bit1->com1,bit4/bit0->com0
unsigned char VK2C22_segi,VK2C22_comi;
unsigned char VK2C22_maxcom;//VK2C22是4com
unsigned char VK2C22_readbuf[20];
/* Private function prototypes -----------------------------------------------*/
void VK2C22_I2C_WRCmd(unsigned char Cmd);
void VK2C22_I2C_WRDat(unsigned char Addr,unsigned char *Databuf,unsigned char cnt);
void VK2C22_I2C_RDDat(unsigned char Addr,unsigned char *Databuf,unsigned char cnt);
void VK2C22_InitSequence(void);
/* Private function ----------------------------------------------------------*/
/*******************************************************************************
* Function Name  : Delay_nuS
* Description    : 延时1uS程序,取决于单片机主频
* Input          : n->延时时间nuS
* Output         : None
* Return         : None
*******************************************************************************/
void Delay_nuS(unsigned int n)	   
{
	unsigned char i;
	while(n--)
	{
		i=10;
		while(i--)
		{//nop指令根据单片机做相应的修改
			__nop();
		}
	}
}
/*******************************************************************************
* Function Name  : Delay_nmS
* Description    : 延时1mS程序
* Input          : n->延时时间nmS
* Output         : None
* Return         : None
*******************************************************************************/
void Delay_nmS(unsigned long int n)
{
	while(n--)
	{
		Delay_nuS(1000);
	}
}
/*******************************************************************************
* Function Name  : WriteCmdVK2C22
* Description    : 写命令到VK2C22
* Input          : cmd-要写入的命令
* Output         : None
* Return         : None
*******************************************************************************/
void  WriteCmdVK2C22(unsigned char Cmd)
{
	VK2C22_I2C_WRCmd(Cmd);
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
unsigned char WritenDataVK2C22(unsigned char Addr,unsigned char *Databuf,unsigned char Cnt)
{
  VK2C22_I2C_WRDat(Addr,Databuf,Cnt);
	return 0;
}
/*******************************************************************************
* Function Name  : ReadnData
* Description    : 从显示RAM读多个数据到
* Input          : Addr-读ram的起始地址
*                : VKL144C_readbuf->读ram的数据buffer
*                : Cnt->读ram的数据个数
* Output         : None
* Return         : 0-ok 1-fail
*******************************************************************************/
unsigned char  ReadnDataVK2C22(unsigned char Addr,unsigned char *Databuf,unsigned char Cnt)
{
  VK2C22_I2C_RDDat(Addr,Databuf,Cnt);
	return 0;
}
/*******************************************************************************
* Function Name  : VK2C22_DisAll
* Description    : 所有SEG显示同一个数据，bit7/bit3-COM3 bit6/bit2-COM2 bit5/bit1-COM1 bit4/bit0-COM0
* 					     : 例如：0xff全亮 0x00全灭 0x55灭亮灭亮 0xaa亮灭亮灭 0x33灭灭亮亮 
* Input          ：dat->写入ram的数据(1个字节数据对应2个SEG)  
* Output         : None
* Return         : None
*******************************************************************************/
void VK2C22_DisAll(unsigned char dat)
{
	unsigned char segi;
	
	for(segi=0;segi<22;segi++)
	{
		VK2C22_dispram[segi]=dat;
	}
	WritenDataVK2C22(0,VK2C22_dispram,22);//这里送8bit数据对应2个SEG，每8bit数据地址加1，每8位数据1个ACK	
	
}
/*******************************************************************************
* Function Name  : DisSegComOn
* Description    : 点亮1个点(1个seg和1个com交叉对应的显示点)
* Input          ：seg->点对应的seg脚  
* 		           ：com->点对应com脚  
* Output         : None
* Return         : None
*******************************************************************************/
void VK2C22_DisDotOn(unsigned char seg,unsigned char com)
{
	unsigned char addrbit,tempdat;
	
  if(seg%2==0)
			addrbit=(1<<com);//这里送8位数据低4bit有效，每8bit数据地址加1，每8位数据1个ACK
		else
			addrbit=(1<<(4+com));//这里送8位数据高4bit有效，每8bit数据地址加1，每8位数据1个ACK
		tempdat=VK2C22_dispram[seg/2]|addrbit;
	  VK2C22_dispram[seg/2]=tempdat;
		WritenDataVK2C22(seg/2,&tempdat,1);	
}
/*******************************************************************************
* Function Name  : DisSegComOff
* Description    : 关闭1个点(1个seg和1个com交叉对应的显示点)
* Input          ：seg->点对应的seg脚  
* 		           ：com->点对应com脚  
* Output         : None
* Return         : None
*******************************************************************************/
void VK2C22_DisDotOff(unsigned char seg,unsigned char com)
{
	unsigned char addrbit,tempdat;
  
	if(seg%2==0)
			addrbit=(1<<com);//这里送8位数据低4bit有效，每8bit数据地址加1，每8位数据1个ACK
		else
			addrbit=(1<<(4+com));//这里送8位数据高4bit有效，每8bit数据地址加1，每8位数据1个ACK
		tempdat=VK2C22_dispram[seg/2]&~addrbit;
	  VK2C22_dispram[seg/2]=tempdat;
		WritenDataVK2C22(seg/2,&tempdat,1);
	
}
/*******************************************************************************
* Function Name  : Enter_PowerOff
* Description    : 进入掉电低功耗模式,掉电无显示
* Input          ：None 
* Output         : None
* Return         : 0-ok 1-fail
*******************************************************************************/
void VK2C22_Enter_PowerOff(void)
{		
	//	WriteCmdVK2C22(VK2C22_MODESET|BIAS_1_2|F_80HZ|RCOFF_LCDOFF);  //模式设置  1/2 Bais 1/4 Duty帧频率80Hz,内部系统振荡器开，lcd关显示
	WriteCmdVK2C22(VK2C22_MODESET|BIAS_1_3|F_80HZ|RCOFF_LCDOFF);  //模式设置  1/3 Bais 1/4 Duty帧频率80Hz,内部系统振荡器开，lcd关显示
//	WriteCmdVK2C22(VK2C22_MODESET|BIAS_1_2|F_160HZ|RCOFF_LCDOFF); //模式设置  1/2 Bais 1/4 Duty帧频率160Hz,内部系统振荡器开，lcd关显示
//	WriteCmdVK2C22(VK2C22_MODESET|BIAS_1_3|F_160HZ|RCOFF_LCDOFF); //模式设置  1/3 Bais 1/4 Duty帧频?60Hz,内部系统振荡器开，lcd关显示
}
/*******************************************************************************
* Function Name  : Exit_PowerOff
* Description    : 退出掉电低功耗模式
* Input          ：None 
* Output         : None
* Return         : None
*******************************************************************************/
void VK2C22_Exit_PowerOff(void)
{	
	//	WriteCmdVK2C22(VK2C22_MODESET|BIAS_1_2|F_80HZ|RCON_LCDON);  //模式设置  1/2 Bais 1/4 Duty帧频率80Hz,内部系统振荡器开，lcd开显示
	WriteCmdVK2C22(VK2C22_MODESET|BIAS_1_3|F_80HZ|RCON_LCDON);  //模式设置  1/3 Bais 1/4 Duty帧频率80Hz,内部系统振荡器开，lcd开显示
//	WriteCmdVK2C22(VK2C22_MODESET|BIAS_1_2|F_160HZ|RCON_LCDON); //模式设置  1/2 Bais 1/4 Duty帧频率160Hz,内部系统振荡器开，lcd开显示
//	WriteCmdVK2C22(VK2C22_MODESET|BIAS_1_3|F_160HZ|RCON_LCDON); //模式设置  1/3 Bais 1/4 Duty帧频?60Hz,内部系统振荡器开，lcd开显示
}
/*******************************************************************************
* Function Name  : VK2C22_InitSequence
* Description    : 初始化配置
* Input          ：None 
* Output         : None
* Return         : None
*******************************************************************************/
void VK2C22_InitSequence(void)
{	
	//1/4 Duty
//	WriteCmdVK2C22(VK2C22_MODESET|BIAS_1_2|F_80HZ|RCON_LCDON);  //模式设置  1/2 Bais 1/4 Duty帧频率80Hz,内部系统振荡器开，lcd开显示
	WriteCmdVK2C22(VK2C22_MODESET|BIAS_1_3|F_80HZ|RCON_LCDON);  //模式设置  1/3 Bais 1/4 Duty帧频率80Hz,内部系统振荡器开，lcd开显示
//	WriteCmdVK2C22(VK2C22_MODESET|BIAS_1_2|F_160HZ|RCON_LCDON); //模式设置  1/2 Bais 1/4 Duty帧频率160Hz,内部系统振荡器开，lcd开显示
//	WriteCmdVK2C22(VK2C22_MODESET|BIAS_1_3|F_160HZ|RCON_LCDON); //模式设置  1/3 Bais 1/4 Duty帧频?60Hz,内部系统振荡器开，lcd开显示
	VK2C22_maxcom=4;
	WriteCmdVK2C22(VK2C22_BLINKSET|BLINK_OFF); 		//闪烁关闭	
//	WriteCmdVK2C22(VK2C22_BLINKSET|BLINK_2HZ); 		//闪烁2HZ
//	WriteCmdVK2C22(VK2C22_BLINKSET|BLINK_1HZ); 		//闪烁1HZ
//	WriteCmdVK2C22(VK2C22_BLINKSET|BLINK_0D5HZ); 	//闪烁0.5HZ
	//SEG/VLCD共用脚设为VLCD，内部电压调整功能关闭,VLCD和VDD短接VR=0偏置电压=VDD
//	WriteCmdVK2C22(VK2C22_IVASET|VLCDSEL_IVAOFF_R0); 
	//SEG/VLCD共用脚设为VLCD，内部电压调整功能关闭,VLCD和VDD串接电阻VR>0偏置电压=VLCD
//	WriteCmdVK2C22(VK2C22_IVASET|VLCDSEL_IVAOFF_R1); 
	//SEG/VLCD共用脚设为SEG，内部偏置电压调整：1/3bias=0.652VDD 1/4bias=0.714VDD
	WriteCmdVK2C22(VK2C22_IVASET|SEGSEL_IVA02H);	 
}
/*******************************************************************************
* Function Name  : Init
* Description    : 初始化配置
* Input          ：None 
* Output         : None
* Return         : None
*******************************************************************************/
void VK2C22_Init(void)
{	
	//初始化时序
	VK2C22_InitSequence();
}

/*******************************************************************************
* Function Name  : test_Main
* Description    : 测试主程序
* Input          ：None 
* Output         : None
* Return         : None
*******************************************************************************/
void VK2C22_Main(void)
{	
	VK2C22_Init();
	//上电后显示数据清0
	VK2C22_DisAll(0x00);	
	while(1)
	{			
		VK2C22_DisAll(0xff);			//LCD全显
		Delay_nmS(3000);					//延时1S
	
		VK2C22_DisAll(0x00);			//LCD全关
		Delay_nmS(3000);					//延时1S
		
		//LCD半显
		VK2C22_DisAll(0xAA);			
		ReadnDataVK2C22(0,VK2C22_readbuf,22);	//读回RAM数据
		Delay_nmS(1500);					//延时
		VK2C22_DisAll(0x55);			
		ReadnDataVK2C22(0,VK2C22_readbuf,22);	//读回RAM数据
		Delay_nmS(1500);					//延时

		//掉电
		VK2C22_DisAll(0x00);			//LCD全关		
		VK2C22_Enter_PowerOff();	//进入掉电模式
		Delay_nmS(5000);					//延时5S		
		VK2C22_Exit_PowerOff();	//退出掉电模式
		
		VK2C22_DisAll(0x00);			//LCD全关
		Delay_nmS(500);					  //延时
		for(VK2C22_segi=0;VK2C22_segi<VK2C22_SEGNUM;VK2C22_segi++)//seg
		{
			for(VK2C22_comi=0;VK2C22_comi<VK2C22_maxcom;VK2C22_comi++)//com
			{
				VK2C22_DisDotOn(VK2C22_segtab[VK2C22_segi],VK2C22_comi);	//LCD单点点亮
				Delay_nmS(300);				//延时300mS
				VK2C22_DisDotOff(VK2C22_segtab[VK2C22_segi],VK2C22_comi);	  //此行配合单点点亮函数实现单点点亮，屏蔽此行顺序点亮
			}
		}
		
		VK2C22_DisAll(0xff);			//LCD全显
		Delay_nmS(500);					  //延时
		for(VK2C22_segi=0;VK2C22_segi<VK2C22_SEGNUM;VK2C22_segi++)//seg
		{
			for(VK2C22_comi=0;VK2C22_comi<VK2C22_maxcom;VK2C22_comi++)//com
			{
				if(VK2C22_maxcom==8)
					VK2C22_DisDotOff(VK2C22_segtab[VK2C22_segi],VK2C22_comi);	//LCD单点关闭
				else
					VK2C22_DisDotOff(VK2C22_segtab[VK2C22_segi],VK2C22_comi);	//LCD单点关闭
				Delay_nmS(300);				//延时300mS
				VK2C22_DisDotOn(VK2C22_segtab[VK2C22_segi],VK2C22_comi);  //此行配合单点关闭函数实现单点关闭，屏蔽此行顺序关闭
			}
		}
		Delay_nmS(1000);					  //延时
	}
}
/************************END OF FILE****/
