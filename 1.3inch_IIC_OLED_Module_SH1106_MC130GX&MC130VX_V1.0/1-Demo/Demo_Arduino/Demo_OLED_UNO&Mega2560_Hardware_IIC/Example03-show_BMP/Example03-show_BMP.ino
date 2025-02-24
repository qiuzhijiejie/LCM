/*******************************************************************************
*This program is a demo of how to show a BMP picture to the screen.
*This demo was made for OLED modules with a screen resolution of 128x64 pixels.
*This program requires the U8glib library.

* File                : Example03-show_BMP.ino
* Hardware Environment: Arduino UNO&Mega2560
* Build Environment   : Arduino

*Set the pins to the correct ones for your development shield or breakout board.
*This demo use the BREAKOUT BOARD only and use these IIC data lines to the LCD,
*pin usage as follow:
*             SCL SDA
*        Uno  A5  A4  
*    Mega2560 21  20  

*Remember to set the pins to suit your display module!
*
* @attention
*
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
* TIME. AS A RESULT, QD electronic SHALL NOT BE HELD LIABLE FOR ANY
* DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
* FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE 
* CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
***********************************************************************************/
#include "U8glib.h" //U8g lib
#include "BMP.h"

U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_NONE);  // I2C / TWI 

void show_bmp(void)
{
    u8g.firstPage();  
    do 
    {
        u8g.drawXBMP(0, 0, 128, 64, BMP1);
    } while(u8g.nextPage());
    delay(1000);
    u8g.firstPage();  
    do 
    {
        u8g.drawXBMP(0, 0, 128, 64, BMP2);
    } while(u8g.nextPage());
    delay(1000);
        u8g.firstPage();  
    do 
    {
        u8g.drawXBMP(0, 0, 128, 64, BMP3);
    } while(u8g.nextPage());
    delay(1000); 
}

void show_menu()
{
    u8g.drawHLine(0,10,u8g.getWidth());
    u8g.drawVLine(u8g.getWidth()/2-1,11,u8g.getHeight()-10);
    u8g.drawHLine(u8g.getWidth()/2-1,10+(u8g.getHeight()-10)/2-1,u8g.getWidth()/2);
    u8g.setFont(u8g_font_6x10);
    u8g.setFontPosTop();
    u8g.drawStr(0, 0, "2018-09-26");
    u8g.drawStr(78, 0, "Saturday");
    u8g.drawStr(14, u8g.getHeight()-1-10, "Cloudy");
    u8g.drawStr(u8g.getWidth()/2-1+2, 13, "TEMP");
    u8g.drawCircle(u8g.getWidth()-1-16,27,2);
    u8g.drawStr(u8g.getWidth()-1-11, 27, "C");    
    u8g.drawStr(u8g.getWidth()/2-1+2, 39, "PM2.5");
    u8g.drawStr(u8g.getWidth()/2-1+32, 54, "ug/m3");
    u8g.setFont(u8g_font_timR14r);
    u8g.setFontPosTop();
    u8g.drawStr(u8g.getWidth()/2-1+24, 21, "");
    u8g.drawStr(u8g.getWidth()/2-1+4, 50, "");
    u8g.drawXBMP(6, 16, 51, 32, BMP4);
}

void set_num(unsigned char x,unsigned char y,int num)
{
   char buf[32]={0};
   u8g.setFont(u8g_font_timR14r);
   u8g.setFontPosTop();
   sprintf(buf,"%d",num);
   u8g.drawStr(x, y, buf);
}

void setup() 
{
 
}

void loop() {
    int i;
  // show bmp picture loop
    show_bmp();
    delay(1000);
    for(i=0;i<15;i++)
    {
      u8g.firstPage();  
      do
      {
        show_menu();  
        set_num(u8g.getWidth()/2-1+24, 21,25+i);
        set_num(u8g.getWidth()/2-1+4, 50,88+i);      
      }while(u8g.nextPage());
      delay(500);
    }
    delay(1000);
}
