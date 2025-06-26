#ifndef __oled242_H__
#define __oled242_H__

#include "stm32f10x.h"
/**********SPI引脚分配，连接oled屏，更具实际情况修改*********/

#define IIC_SCK_PIN 5
#define IIC_SDA_PIN 6

#define OLED_COLUMN_NUMBER 128
#define OLED_LINE_NUMBER 64
#define OLED_PAGE_NUMBER (OLED_LINE_NUMBER/8)
#define OLED_COLUMN_OFFSET 0
#define OLED_LINE_OFFSET 0
#define OLED_CMD_NUMBER 25
/**********SPI引脚分配，连接oled屏，更具实际情况修改*********/

#define IIC_SCK_0  GPIOB->BRR=0X0020       // 设置sck接口到PB5   清零
#define IIC_SCK_1  GPIOB->BSRR=0X0020       //置位
#define IIC_SDA_0  GPIOB->BRR=0X0040       // 设置SDA接口到PB6

#define IIC_SDA_1  GPIOB->BSRR=0X0040
#define READ_SDA	 (GPIOB->IDR & 0X0040)




void SYS_init(unsigned char PLL);
void IO_init(void);
void delay_us(unsigned int _us_time);
void delay_ms(unsigned int _ms_time);
void IIC_write(unsigned char date);
void IIC_start(void);
void IIC_stop(void);
void OLED_send_cmd(unsigned char o_command);
void OLED_send_data(unsigned char o_data);
void Column_set(unsigned char column);
void Page_set(unsigned char page);
void OLED_clear(void);
void OLED_full(void);
void OLED_HX(void) ;
void OLED_HX1(void);
void OLED_init(void);
void Picture_display(const unsigned char *ptr_pic);
void Picture_ReverseDisplay(const unsigned char *ptr_pic);
extern const unsigned char  OLED_init_cmd[OLED_CMD_NUMBER];
		

#endif

