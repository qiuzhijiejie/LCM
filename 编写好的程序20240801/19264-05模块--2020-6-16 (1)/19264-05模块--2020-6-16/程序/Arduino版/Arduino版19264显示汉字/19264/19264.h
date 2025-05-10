#ifndef _LCD19264_H_
#define _LCD19264_H_
#include "Arduino.h"

#define CS    2                                                       //显示屏的片选引脚
#define CD    3                                                       //显示屏的指令与数据选择引脚
#define RST   4                                                       //显示屏复位引脚
#define SDA   5                                                       //显示屏数据引脚
#define SCK   6                                                       //显示屏时钟引脚

class LCD19264
{
  public:
    void WriteData(unsigned char data, unsigned char C);               //数据发送函数
    void Init_lcd(void);                                               //显示屏初始化函数
    void Dispgraphic(unsigned char D);                                 //显示屏清屏函数
    void Display_Chinese(unsigned char y,unsigned char l,unsigned char *p);//数据显示函数
};

#endif
