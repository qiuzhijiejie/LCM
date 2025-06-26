#ifndef __lcm12864cog_H__
#define __lcm12864cog_H__

#define uchar unsigned char
#define uint unsigned int
#include "pic.h"
void push();
void Delay_Xms(unsigned int Xms);
void init_cpu(void);
void InitST7567(void);
void DispPic(unsigned char *dispdata);
void DispPic1(unsigned char *dispdata);
void DispFram(unsigned char x,unsigned char y);
void DispDot(unsigned char x,unsigned char y);
void DispDots(unsigned char x,unsigned char y);
void hz_display16(unsigned char pag, unsigned char col, unsigned char cok,unsigned char code *hzk);
void ST7567_WriteComd(unsigned char comd);
void ST7567_WriteData(unsigned char dat);
void ST7567_WriteData1(unsigned char dat);
void disp_char16(uint x,uint y,uchar dat[]);
void disp_char8(uint x,uint y,uchar dat[]);
void SetContrast(void);
unsigned char Change(unsigned int Xms);
void Pause();

#endif