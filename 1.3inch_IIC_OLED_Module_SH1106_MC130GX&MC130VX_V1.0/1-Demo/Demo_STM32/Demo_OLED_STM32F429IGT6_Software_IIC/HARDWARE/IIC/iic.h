//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//测试硬件：单片机STM32F429IGT6,正点原子Apollo STM32F4/F7开发板,主频180MHZ，晶振25MHZ
//QDtech-OLED液晶驱动 for STM32
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
//创建日期:2018/9/13
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 深圳市全动电子技术有限公司 2018-2028
//All rights reserved
/****************************************************************************************************
//=========================================电源接线================================================//
// OLED模块               STM32单片机
//   VCC         接       DC 5V/3.3V      //OLED屏电源正
//   GND         接          GND          //OLED屏电源地
//=======================================液晶屏数据线接线==========================================//
//本模块默认数据总线类型为IIC
// OLED模块               STM32单片机
//   SDA         接          PF9         //OLED屏IIC数据信号
//=======================================液晶屏控制线接线==========================================//
// OLED模块               STM32单片机
//   SCL         接          PF7         //OLED屏IIC时钟信号
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
#ifndef _IIC_H_
#define _IIC_H_
#include "sys.h"

//本测试程序使用的是软件模拟IIC接口驱动
//IIC的数据引脚定义和时钟引脚定义都可以任意修改
//修改引脚定义后，需要对应修改oled.c中OLED_Init_GPIO函数里面引脚初始化

//定义IIC从设备地址
#define IIC_SLAVE_ADDR 0x78


//--------------IIC总线引脚定义-----------------------
#define OLED_SDA       GPIO_PIN_9  //OLED屏IIC数据信号 GPIOF9
#define OLED_SCL       GPIO_PIN_7  //OLED屏IIC时钟信号 GPIOF7

//--------------IIC端口操作定义---------------------
#define	OLED_SDA_SET()  	HAL_GPIO_WritePin(GPIOF, OLED_SDA, GPIO_PIN_SET) 
#define	OLED_SDA_CLR()  	HAL_GPIO_WritePin(GPIOF, OLED_SDA, GPIO_PIN_RESET) 
  
#define	OLED_SCL_SET()  	HAL_GPIO_WritePin(GPIOF, OLED_SCL, GPIO_PIN_SET)     
#define	OLED_SCL_CLR()  	HAL_GPIO_WritePin(GPIOF, OLED_SCL, GPIO_PIN_RESET)    

//定义数据IO的方向
#define OLED_SDA_IN()  {GPIOF->MODER&=~(3<<(9*2));GPIOF->MODER|=0<<(9*2);}	//PF9输入模式
#define OLED_SDA_OUT() {GPIOF->MODER&=~(3<<(9*2));GPIOF->MODER|=1<<(9*2);} //PF9输出模式

//IIC操作函数
void IIC_Start(void);
void IIC_Stop(void);
void IIC_Wait_Ack(void);
void Write_IIC_Byte(u8 IIC_Byte);
void Write_IIC_Command(u8 IIC_Command);
void Write_IIC_Data(u8 IIC_Data);

#endif
