#ifndef __LCM19264_H__
#define __LCM19264_H__

#include <INTRINS.H>    
#include <STDIO.H>     
#include <MATH.H>    
#include <ABSACC.H>    
#include <REG52.H>     

sbit RS  =P3^0;    
sbit RW  =P3^1;     
sbit E   =P3^2;    
sbit CS1 =P3^3;     //active "L"
sbit CS2 =P3^4;     //active "L"  
sbit CS3 =P3^5;     //active "L"
sbit int0=P2^2;
sbit LCD_KEY=P2^0;
  

void BusyL(void);
void BusyM(void);
void BusyR(void);
void Locatexy(void);   
void WriteCommandL( unsigned char CommandByte );
void WriteCommandM( unsigned char CommandByte );
void WriteCommandR( unsigned char CommandByte );
unsigned char ReadData( void );
void WriteData( unsigned char DataByte );
void LcmClear( void );
void LcmInit( void );
void LcmPutDots( unsigned char DotByte,unsigned char DotByte1 );
void LcmPutBMP( unsigned char *puts );
void LcmReverseBMP( void );
void Delay(unsigned int MS);
void Display_Chinese();
void Display_16_16(unsigned char F[],unsigned char i);
void Display_8_8(unsigned char C[],unsigned char i);
void Display_character();
void Busy(unsigned char cs1, unsigned char cs2, unsigned char cs3);
unsigned int  ReadKey();

#endif