#ifndef __lcd12864_H__
#define __lcd12864_H__

#define uint  unsigned int
#define uchar unsigned char


unsigned char ReadKey();//°´¼üº¯Êý
unsigned char ReadDate(void);
void WriteDate(uint Date);
void Clear(void);
void Lcd12864_Init(void);
void Showpicture( unsigned char DotByte,unsigned char DotByte1 ) ;
void LcmPutBMP( uchar *puts );
void LcmReverseBMPP( unsigned char *puts );
void Display8_8(uchar C[],uchar i);
void Display16_16(uchar F[],uchar i);
void Display_character();
void Display_Chinese();
void Showpicture_Advanced(uchar DotByte, uchar DotByte1);
void Showpicture_VerticalWave(uchar DotByte, uchar DotByte1);
void Showpicture_CrossCursorEffect(uchar DotByte, uchar DotByte1);
void LcmPutBMPsjw(uchar *puts) ;
void Showpicture_Advanced2(uchar DotByte, uchar DotByte1);
void Showpicture_Advanced1(uchar DotByte, uchar DotByte1);
void Delay(unsigned int xms);
void LcmPutDots( unsigned char DotByte,unsigned char DotByte1 );
void LcmPutDots1(unsigned char DotByte);
void LcmPutDotsStars(unsigned char pattern);
void LcmPutDotsOddColumns(void);
void LcmPutDotsEvenPages(void);


#endif