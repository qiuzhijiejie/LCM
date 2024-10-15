#ifndef __LCD12864_H__
#define __LCD12864_H__

void Delay(unsigned char xms);	
void Trans_Bit(bit d); 
void Trans_Byte(unsigned char d);
void Write_Command(unsigned char c);
void Write_Data(unsigned char e);
void Write_Status(unsigned char e);
void Read_Data(unsigned char e);
void Graphic_Display_ON(void);
void Graphic_Display_OFF(void);
void Display_ON(void);
void Display_OFF(void);
void Display_Clear(void);
void Return_Home(void);
void Set_DrawingPosition(void);
void FillScreen(unsigned char DATA) ;




void Lcd12864spi_init(void);

void moveToNextCharPosition(void) ;

void setLcdCursorPosition(void)  ;

void drawimge(unsigned char code *imgeData);

void drawImageWithSize(unsigned char width, unsigned char height, unsigned char code *imgeData);


void setchartoCGRAM(unsigned char charindex ,unsigned char code *pattern);


void DisplaystringAtposition(unsigned char x ,unsigned char y ,unsigned char code*s);


void DisplayString(unsigned char code *str) ;


void putchar(unsigned int c);


void charlcdfill(unsigned int c) ;


void  Versawhite_on(void);

void  Versawhite_off(void);

unsigned int ReadKey1();


void FillScreenWithPattern(unsigned char pattern);
void FillCheckerboardPattern(void);
void FillVerticalStripePattern(void);
void FillHorizontalStripePattern(void);
void FillDiagonalLinePattern(void);
void DisplaySequence(void);




#endif