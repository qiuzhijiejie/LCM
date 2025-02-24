//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//测试硬件：单片机MSP430F149,晶振8M  单片机工作电压3.3V或5V
//QDtech-OLED液晶驱动 for MSP430
//xiao冯@ShenZhen QDtech co.,LTD
//公司网站:www.qdtft.com
//淘宝网站：http://qdtech.taobao.com
//wiki技术网站：http://www.lcdwiki.com
//我司提供技术支持，任何技术问题欢迎随时交流学习
//固话(传真) :+86 0755-23594567 
//手机:15989313508（冯工） 
//邮箱:lcdwiki01@gmail.com    support@lcdwiki.com    goodtft@163.com
//技术支持QQ:3002773612  3002778157
//技术交流QQ群:324828016
//创建日期:2018/9/14
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 深圳市全动电子技术有限公司 2018-2028
//All rights reserved
/****************************************************************************************************
//=========================================电源接线================================================//
// OLED模块                C51单片机
//   VCC         接       DC 5V/3.3V      //OLED屏电源正
//   GND         接          GND          //OLED屏电源地
//=======================================液晶屏数据线接线==========================================//
//本模块默认数据总线类型为IIC
// OLED模块                C51单片机
//   SDA        接           P53          //OLED屏SPI写信号
//=======================================液晶屏控制线接线==========================================//
// OLED模块                C51单片机
//   SCL        接           P54          //OLED屏SPI时钟信号
//=========================================触摸屏接线=========================================//
//本模块不带触摸功能，所以不需要触摸屏接线
****************************************************************************************************/	
/***************************************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, QD electronic SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
****************************************************************************************************/	
#include "iic.h"

/*****************************************************************************
 * @name       :void IIC_Start(void)
 * @date       :2018-09-13 
 * @function   :start iic bus
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void IIC_Start(void)
{
	OLED_SCL_SET();
	OLED_SDA_SET();
	OLED_SDA_CLR();
	OLED_SCL_CLR();
}

/*****************************************************************************
 * @name       :void IIC_Stop(void)
 * @date       :2018-09-13 
 * @function   :stop iic bus
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void IIC_Stop(void)
{
	OLED_SCL_SET();
	OLED_SDA_CLR();
	OLED_SDA_SET();
}

/*****************************************************************************
 * @name       :void IIC_Wait_Ack(void)
 * @date       :2018-09-13 
 * @function   :wait iic ack
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void IIC_Wait_Ack(void)
{
	OLED_SCL_SET();
	OLED_SCL_CLR();
}

/*****************************************************************************
 * @name       :void Write_IIC_Byte(u8 IIC_Byte)
 * @date       :2018-09-13 
 * @function   :Write a byte of content with iic bus
 * @parameters :IIC_Byte
 * @retvalue   :None
******************************************************************************/
void Write_IIC_Byte(u8 IIC_Byte)
{
	u8 i;
	u8 m,da;
	da=IIC_Byte;
	OLED_SCL_CLR();
	for(i=0;i<8;i++)		
	{
		m=da;
		m=m&0x80;
		if(m==0x80)
		{
			OLED_SDA_SET();
		}
		else
		{
			OLED_SDA_CLR();
		}
		da=da<<1;
		OLED_SCL_SET();
		OLED_SCL_CLR();
		}
}

/*****************************************************************************
 * @name       :void Write_IIC_Command(u8 IIC_Command)
 * @date       :2018-09-13 
 * @function   :Write a byte of command to oled screen
 * @parameters :IIC_Command:command to be written
 * @retvalue   :None
******************************************************************************/
void Write_IIC_Command(u8 IIC_Command)
{
	IIC_Start();
	Write_IIC_Byte(IIC_SLAVE_ADDR);            //Slave address,SA0=0
	IIC_Wait_Ack();	
	Write_IIC_Byte(0x00);			//write command
	IIC_Wait_Ack();	
	Write_IIC_Byte(IIC_Command); 
	IIC_Wait_Ack();	
	IIC_Stop();
}

/*****************************************************************************
 * @name       :void Write_IIC_Data(u8 IIC_Data)
 * @date       :2018-09-13 
 * @function   :Write a byte of data to oled screen
 * @parameters :IIC_Data:data to be written
 * @retvalue   :None
******************************************************************************/
void Write_IIC_Data(u8 IIC_Data)
{
	IIC_Start();
	Write_IIC_Byte(IIC_SLAVE_ADDR);			//D/C#=0; R/W#=0
	IIC_Wait_Ack();	
	Write_IIC_Byte(0x40);			//write data
	IIC_Wait_Ack();	
	Write_IIC_Byte(IIC_Data);
	IIC_Wait_Ack();	
	IIC_Stop();
}
