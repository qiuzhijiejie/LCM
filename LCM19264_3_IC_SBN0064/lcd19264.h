#ifndef __lcd19264_H__
#define __lcd19264_H__


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
unsigned char key();
void DisplayPatterns() ;
void HandleKey() ;




#endif