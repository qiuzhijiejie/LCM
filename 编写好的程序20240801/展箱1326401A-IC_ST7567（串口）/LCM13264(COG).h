#ifndef __lcm13264_H__
#define __lcm13264_H__
#include <REGX52.H>

#define uchar unsigned char
#define uint unsigned int
	

//模块串行接口定义(接口按实际测架更改)
sbit SDA = P3^4;  //SDA			  //定义串行数据脚为P1.4
sbit SCK = P3^3;				  //定义串行时钟脚为P1.3
sbit RS  = P3^2;  //A0			 //定义数据/指令控制脚为P1.2
sbit RST = P3^1;				 //定义复位脚为P1.1
sbit CS  = P3^0;			   //定义片选脚为P10
//---------------------------------------------------------	
///按键接口定义(与测试板一致)
sbit Key1 =P2^5;//HOLD
sbit Key2 =P2^6;//UP
sbit Key3 =P2^7;//DN



sbit AOUTOB = P0^4;      //S_S            EQU     P2.4  pause the display when it is '0'
sbit STEP   = P0^5;      //STP            EQU     P2.5
sbit UPT    = P0^7;      //Increase by voltage when it is '1'
sbit DOWN   = P0^6;      //Decrease by voltage when it is '1'


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
unsigned int HOLD();
unsigned int UP() ;
unsigned int DN() ;
void init_cpu(void);
void yehui_UI();
void WhatsApp();
void  YHLTD();

#endif