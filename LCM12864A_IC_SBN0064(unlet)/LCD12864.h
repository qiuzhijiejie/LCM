#ifndef __lcd12864_H__
#define __lcd12864_H__

#define uint  unsigned int
#define uchar unsigned char


void BusyL(void);
void BusyR(void);
void Locatexy(void);   
void WriteCommandL( unsigned char CommandByte );
void WriteCommandR( unsigned char CommandByte );
unsigned char ReadData( void );
void WriteData( unsigned char DataByte );
void LcmClear( void );
void LcmInit( void );
void LcmPutDots( unsigned char DotByte,unsigned char DotByte1 );
void LcmPutBMP( unsigned char *puts );
void LcmReverseBMP( void );
void LcmReverseBMPP( unsigned char *puts )   ;
void Delay(unsigned int MS);
void Display_Chinese();
void Display_16_16(unsigned char F[],unsigned char i);
void Display_8_8(unsigned char C[],unsigned char i);
void Display_character();
unsigned char key(void);
void HandleKey();
void DisplayPatterns(); 
  



#endif