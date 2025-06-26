#ifndef __OLED_H
#define __OLED_H 

#include "sys.h"

#define USE_HORIZONTAL 0  //设置显示方向 0：正向显示；1：旋转180度显示

//SCL=SCLK 
//SDA=MOSI
//RES=RES
//DC=DC
//CS=CS
//-----------------OLED端口定义---------------- 

#define OLED_SCL_Clr() GPIO_ResetBits(GPIOG,GPIO_Pin_12)//SCL
#define OLED_SCL_Set() GPIO_SetBits(GPIOG,GPIO_Pin_12)

#define OLED_SDA_Clr() GPIO_ResetBits(GPIOD,GPIO_Pin_5)//SDA
#define OLED_SDA_Set() GPIO_SetBits(GPIOD,GPIO_Pin_5)

#define OLED_RES_Clr()  GPIO_ResetBits(GPIOD,GPIO_Pin_4)//RES
#define OLED_RES_Set()  GPIO_SetBits(GPIOD,GPIO_Pin_4)

#define OLED_DC_Clr()  GPIO_ResetBits(GPIOD,GPIO_Pin_15)//DC
#define OLED_DC_Set()  GPIO_SetBits(GPIOD,GPIO_Pin_15)
 		     
#define OLED_CS_Clr()  GPIO_ResetBits(GPIOD,GPIO_Pin_1)//CS
#define OLED_CS_Set()  GPIO_SetBits(GPIOD,GPIO_Pin_1)


//显示屏功能函数
void OLED_WR_REG(u8 reg);//写入一个指令
void OLED_WR_Byte(u8 dat);//写入一个数据
void Column_Address(u8 a,u8 b);//设置列地址
void Row_Address(u8 a,u8 b);//设置行地址
void OLED_Fill(u16 xstr,u8 ystr,u16 xend,u8 yend,u8 color);//填充函数
void OLED_ShowChinese(u8 x,u8 y,u8 *s,u8 sizey,u8 mode);//显示汉字串
void OLED_ShowChinese16x16(u8 x,u8 y,u8 *s,u8 sizey,u8 mode);//显示16x16汉字
void OLED_ShowChinese24x24(u8 x,u8 y,u8 *s,u8 sizey,u8 mode);//显示24x24汉字
void OLED_ShowChinese32x32(u8 x,u8 y,u8 *s,u8 sizey,u8 mode);//显示32x32汉字
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 sizey,u8 mode);//显示字符
void OLED_ShowString(u8 x,u8 y,u8 *dp,u8 sizey,u8 mode);//显示字符串
u32 oled_pow(u8 m,u8 n);//幂函数
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 sizey,u8 mode);//显示整数变量
void OLED_DrawBMP(u8 x,u8 y,u16 length,u8 width,const u8 BMP[],u8 mode);//显示灰度图片
void OLED_DrawSingleBMP(u8 x,u8 y,u16 length,u8 width,const u8 BMP[],u8 mode);//显示单色图片
void OLED_Init(void);

#endif







