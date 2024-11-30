#ifndef __lcd24064_H__
#define __lcd24064_H__

#define uchar  unsigned char
#define uint  unsigned int
#define DataBus		P1
#define Graphic		1
#define	Character 	0


sbit CD =P3^0;    	// 	COMMAND/DATA
sbit RESET =P2^4; 	//	RESET
sbit FS =P3^3; 		//  0??8*8   1 6*8     
sbit _WR =P3^1; 	//	WRITE    
sbit _RD =P3^2;		//	READ
sbit  Key=P2^0;

void Locatexy(uchar xa,uchar ya,uchar mode);
void CheckBusy_S0S1();
void CheckBusy_S2();
void CheckBusy_S3();
void WriteCommand( uchar dat1,uchar dat2,uchar command,uchar parameters_num );
void WriteData( uchar DataByte );
void LcmClear( void ); 
void Display_Str(uchar x,uchar y,uchar *ascll);
void Display_16_16(uchar x,uchar y,uchar  code *Chinese_character,uchar character_code);
void DisplayDots( uchar DotByte,uchar DotByte1 );
void LcmInit( void );
void DisplayBMP(uchar x,uchar y,uchar W,uchar H,  uchar  code *puts );
void ReverseDisplayBMP(uchar x,uchar y,uchar W,uchar H,  uchar  code *puts );
void Delay(uint MS); 
void TestAscll(void); 
void wordageEng(void);
void wordageCha(void);
void Display_FullCGROM();
void Display_8_16(uchar x, uchar y, uchar code *English_character, uchar character_code);
void Display_FullCGROM_SpecifiedPositions();
unsigned char ReadKey() ;

#endif