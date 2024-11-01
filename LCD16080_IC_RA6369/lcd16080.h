#ifndef __lcd16080_H__
#define __lcd16080_H__

#define uchar    unsigned char 	
#define uint     unsigned int 
	
void Delay(uint MS) ; 
unsigned int ReadKey1(); // °´¼ü¼ì²âº¯Êý
void CheckBusy();
void CheckBusy();
void AUTOREAD_CheckBusy();
void AUTOWRITE_CheckBus();
void WriteData( uchar Data_Byte );
void WriteCommand( uchar dat1,uchar dat2,uchar command,uchar parameters_num )  ;
void LcmClear( void )   ;
void Locatexy(uchar x,uchar y,uchar mode) ;
void Display_Str(uchar x,uchar y,uchar *ascll);
void Display_16_16(uchar x,uchar y,uchar  code *Chinese_character,uchar character_code);
void DisplayDots( uchar DotByte,uchar DotByte1 );
void DisplayBMP(uchar x,uchar y,uchar W,uchar H,  uchar code *puts );
void ReverseDisplayBMP(uchar x,uchar y,uchar W,uchar H,  uchar code *puts );  
void LcmInit( void ); 
void picture(void);
void Display_FullCGROM();
void Display_8_16(uchar x, uchar y, uchar code *English_character, uchar character_code);




#endif