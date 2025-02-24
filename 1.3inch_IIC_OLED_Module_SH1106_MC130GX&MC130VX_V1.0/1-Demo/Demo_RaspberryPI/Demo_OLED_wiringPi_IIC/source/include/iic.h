//////////////////////////////////////////////////////////////////////////////////////////////////////	 
//This program is for learning only,Not to be used for any other purpose 
//without the permission of the author
//Testing Hardware:Raspberry PI 3 Model B and B+,  basic frequency:1GHz
//QDtech-OLED liquid crystal driver for Raspberry PI
//xiaofeng@ShenZhen QDtech co.,LTD
//Company Website:www.qdtft.com
//Taobao Website:http://qdtech.taobao.com
//wiki Technology Website:http://www.lcdwiki.com
//We provide technical support,Any technical questions are welcome to 
//exchange and study at any time
//Fixed telephone (fax):+86 0755-23594567 
//cell-phone:15989313508(Mr Feng)
//E-mail:lcdwiki01@gmail.com    support@lcdwiki.com    goodtft@163.com
//Technical Support QQ:3002773612  3002778157
//Technical Exchange QQ group:324828016
//Date:2018/9/14
//Version:V1.0
//Copyright reserved, piracy must be investigated
//Copyright(C) ShenZhen QDtech co.,LTD 2018-2028
//All rights reserved
/****************************************************************************************************
//This program uses the wiringPi gpio library,
//so the pin definition using wiringPi coding
//=====================================power supply wiring===========================================//
// OLED Module                  Raspberry PI    
//    VCC        connect         DC 5V/3.3V        //OLED power positive, Physical pin 1,2,4
//    GND        connect            GND            //OLED power ground,Physical pin 6,9,14,20,25,30,34,39
//======================================data line wiring=============================================//
//The default data bus type for this module is IIC
// OLED Module                  Raspberry PI 
//    SDA        connect        3(wiringPi:8)     //OLED iic write signal
//======================================control line wiring==========================================//
// OLED Module                  Raspberry PI 
//    SCL        connect        5(wiringPi:9)     //OLED iic colck signal
//========================================touch screen wiring========================================//
//This module has no touch function,so it don't need touch screen wiring
*****************************************************************************************************/	
/****************************************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, QD electronic SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*****************************************************************************************************/

#ifndef _IIC_H_
#define _IIC_H_
#include <wiringPiI2C.h>
#include "type.h"

#define IIC_SLAVE_ADDR 0x3C
#define IIC_COMMAND 0x00
#define IIC_DATA 0x40

//This program uses the hardware IIC bus,
//the pin definition of colok signal,write signal and read signal can't be modified
//The SPI clock pin is fixed to 5(Physical pin,wiringPi coding is 9)
//The SPI write pin is fixed to 3(Physical pin,wiringPi coding is 8)
//The SPI read pin is not be used

 
uint32_t IIC_init(void);
void IIC_WriteCmd(uint8_t I2C_Command);
void IIC_WriteDat(uint8_t I2C_Data);

#endif
