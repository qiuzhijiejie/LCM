#ifndef LCD12864_H
#define LCD12864_H

#include "stm32c0xx_hal.h"

// LCD 控制引脚定义
#define LCD_CS1_PIN GPIO_PIN_6
#define LCD_CS2_PIN GPIO_PIN_2
#define LCD_RS_PIN GPIO_PIN_15
#define LCD_RW_PIN GPIO_PIN_0
#define LCD_E_PIN  GPIO_PIN_14
#define LCD_DATA_PORT GPIOA // 数据引脚假设都连接在GPIOA上

// 函数声明
void Lcd12864_Init(void);        // 初始化 LCD
void WriteCommand_L(uint8_t Command); // 写左半屏指令
void WriteCommand_R(uint8_t Command); // 写右半屏指令
void WriteDate(uint8_t Date);    // 写数据
uint8_t ReadDate(void);          // 读数据
void SetPage_Column(void);       // 设置光标位置，页，列
void Clear(void);                // 清屏
void Showpicture(uint8_t DotByte, uint8_t DotByte1); // 显示图像

#endif // LCD12864_H
