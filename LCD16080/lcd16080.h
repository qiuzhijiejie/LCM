#ifndef __lcd16080_H__
#define __lcd16080_H__



void lcdwaitBusy(void);
void lcdwaitBusy(void);
void lcdWriteData(unsigned char Data) ;
unsigned char lcdReadData(void);
void lcdWriteCommand2(unsigned char command, unsigned char param1, unsigned char param2);
void lcdWriteCommand1(unsigned char command, unsigned char param);
void lcdWriteCommand(unsigned char command);
unsigned char lcdReadGraphicData(void);
void lcdWriteGraphicData(unsigned char Data);
void lcdInitialize(void);
void lcdSetPosition(void);
void lcdFill(unsigned char Data)  ;
void rect(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, bit b);
void lineto(unsigned char x1, unsigned char y1, bit b);
void line(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, bit b);
void drawPoint(bit isEnabled);
void drawdrawPoint(bit isEnabled);
void displayImage(unsigned char code *imageData);
void displaySizedImage(unsigned char imageWidth, unsigned char imageHeight, unsigned char code *imageData);
void putstr(unsigned char code *s);
void putchar(unsigned int c);
void FillHorizontalStripePattern(void);
unsigned int ReadKey1() ;
void Delay(unsigned char xms);
void FillHorizontalStripePattern1(void);
void FillHorizontalStripePattern2(void);
void draw_more_nested_rectangles();
void circle(unsigned char x_center, unsigned char y_center, unsigned char radius, bit b);
void drawNestedCircles(unsigned char x_center, unsigned char y_center, unsigned char max_radius, bit b) ;


#endif 
