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
#include "delay.h"
#include "sys.h"
#include "oled.h"
#include "gui.h"
#include "test.h"
int main(void)
{
    HAL_Init();                     //初始化HAL库   
    Stm32_Clock_Init(360,25,2,8);   //设置时钟,180Mhz
    delay_init(180);                //初始化延时函数
    OLED_Init();	                //初始化OLED
	  OLED_Clear(0);                  //清屏（全黑）
    while(1)
    {
		TEST_MainPage();         //主界面显示测试
		OLED_Clear(0); 
		Test_Color();            //刷屏测试
		OLED_Clear(0); 
		Test_Rectangular();      //矩形绘制测试
		OLED_Clear(0); 
		Test_Circle();           //圆形绘制测试
		OLED_Clear(0); 
		Test_Triangle();         //三角形绘制测试
		OLED_Clear(0);  
		TEST_English();          //英文显示测试
		OLED_Clear(0); 
		TEST_Number_Character(); //数字和符号显示测试
		OLED_Clear(0); 
		TEST_Chinese();          //中文显示测试
		OLED_Clear(0); 
		TEST_BMP();              //BMP单色图片显示测试
		OLED_Clear(0); 
		TEST_Menu1();            //菜单1显示测试
		OLED_Clear(0); 
		TEST_Menu2();            //菜单2显示测试
		OLED_Clear(0); 
	}
}
