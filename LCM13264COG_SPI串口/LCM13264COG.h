#ifndef __LCM13264COG_H__
#define __LCM13264COG_H__

#define uchar unsigned char
#define uint unsigned int
#define RATIO       0X27  //0X20-0X27 SET Regulation Ratio(RR):3.0-6.5	
#define VOP_COMPENSATE  0.05	 //VOP补偿
#define SEG_START	32
#define LINE1		3	//adjust vop screen	(EV)
#define LINE2		1	//adjust vop screen	(VOP=**.**V)



//函数申明如下
void Delay_Xms(unsigned int Xms);
unsigned char Change(unsigned int Xms);
void InitST7567(void);
void ST7567_WriteComd(unsigned char comd);
void ST7567_WriteData(unsigned char dat);
void ST7567_WriteData1(unsigned char dat);
void DispPic(unsigned char *dispdata);
void DispPic1(unsigned char *dispdata);
void DispFram(unsigned char x,unsigned char y);
void DispDot(unsigned char x,unsigned char y);
void DispDots(unsigned char x,unsigned char y);
void Pause();
void hz_display16(unsigned char pag, unsigned char col, unsigned char cok,unsigned char code *hzk);
void init_cpu(void);
void push();

#endif