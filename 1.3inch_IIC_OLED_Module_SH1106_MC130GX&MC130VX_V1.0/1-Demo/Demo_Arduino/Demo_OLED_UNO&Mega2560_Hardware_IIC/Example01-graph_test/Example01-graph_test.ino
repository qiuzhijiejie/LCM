/***********************************************************************************
*This program is a demo of how to use some U8glib functions to draw some graph.
*This demo was made for OLED modules with a screen resolution of 128x64 pixels.
*This program requires the U8glib library.

* File                : Example01-graph_test.ino
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
**********************************************************************************/
#include "U8glib.h" ////U8g lib

U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_NONE);  // I2C / TWI 

void prepare(void) 
{
  u8g.setFont(u8g_font_6x10);
  u8g.setFontRefHeightExtendedText();
  u8g.setDefaultForegroundColor();
  u8g.setFontPosTop();
}

//draw rectangle
void test_rectangle(void)
{
   int i; 
   u8g.firstPage();  
   do 
   {
     for(i = 0;i<u8g.getWidth()/2;i+=4)
     { 
        u8g.drawFrame(i,i/2,u8g.getWidth()-2*i,u8g.getHeight()-i);
     } 
   }while(u8g.nextPage());   
/*   delay(1000);
   u8g.firstPage();  
   do 
   {
     for(i = 0;i<u8g.getWidth()/2;i+=4) 
     { 
       if((i/4)%2)
       {
          u8g.setDefaultForegroundColor();  
       }
       else
       {
          u8g.setDefaultBackgroundColor();   
       }
        u8g.drawBox(i,i/2,u8g.getWidth()-2*i,u8g.getHeight()-i);
     } 
   }while(u8g.nextPage()); */    
}

//draw circle
void test_circle(void)
{
   int i; 
   u8g.setDefaultForegroundColor(); 
   u8g.firstPage();  
   do 
   {
     for(i = 0;i<u8g.getHeight()/2;i+=4) 
     { 
        u8g.drawCircle(u8g.getWidth()/2-1,u8g.getHeight()/2-1,u8g.getHeight()/2-i-1);
     } 
   }while(u8g.nextPage());   
   delay(1000);
   u8g.firstPage();  
   do 
   {
     for(i = 0;i<u8g.getHeight()/2;i+=4) 
     { 
       if((i/4)%2)
       {
          u8g.setDefaultForegroundColor();  
       }
       else
       {
          u8g.setDefaultBackgroundColor();   
       }
        u8g.drawDisc(u8g.getWidth()/2-1,u8g.getHeight()/2-1,u8g.getHeight()/2-i);
     } 
   }while(u8g.nextPage());  
}

//draw round rectangle
void test_round_rectangle(void)
{
  int i; 
  u8g.setDefaultForegroundColor();
   u8g.firstPage();  
   do 
   {
     for(i = 0;i<u8g.getWidth()/2-8;i+=4)
     { 
        u8g.drawRFrame(i,i/2,u8g.getWidth()-2*i,u8g.getHeight()-i,5);
     } 
   }while(u8g.nextPage());   
/*   delay(1000);
   u8g.firstPage();  
   do 
   {
     for(i = 0;i<u8g.getWidth()/2-8;i+=4) 
     { 
       if((i/4)%2)
       {
          u8g.setDefaultForegroundColor();  
       }
       else
       {
          u8g.setDefaultBackgroundColor();   
       }
        u8g.drawRBox(i,i/2,u8g.getWidth()-2*i,u8g.getHeight()-i,5);
     } 
   }while(u8g.nextPage()); */
}

void draw_triangle(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned char x3, unsigned char y3)
{
  u8g.drawLine(x1,y1,x2,y2);  
  u8g.drawLine(x2,y2,x3,y3); 
  u8g.drawLine(x1,y1,x3,y3); 
}

void test_triangle(void)
{
   int i; 
   u8g.setDefaultForegroundColor();
   u8g.firstPage();  
   do 
   {
     for(i = 0;i<u8g.getWidth()/4;i+=4)
     { 
        draw_triangle(u8g.getWidth()/2-u8g.getWidth()/2/2-1+i, u8g.getHeight()-1-i, u8g.getWidth()/2-1, i,u8g.getWidth()/2+u8g.getWidth()/2/2-1-i,u8g.getHeight()-1-i);
     } 
   }while(u8g.nextPage());   
   delay(1000);
   u8g.firstPage();  
   do 
   {
     for(i = 0;i<u8g.getWidth()/4;i+=4) 
     { 
       if((i/4)%2)
       {
          u8g.setDefaultForegroundColor();  
       }
       else
       {
          u8g.setDefaultBackgroundColor();   
       }
        u8g.drawTriangle(u8g.getWidth()/2-u8g.getWidth()/2/2-1+i, u8g.getHeight()-1-i, u8g.getWidth()/2-1, i,u8g.getWidth()/2+u8g.getWidth()/2/2-1-i,u8g.getHeight()-1-i);
     } 
   }while(u8g.nextPage());     
}

//draw lines
void draw_line(void)
{
   int i; 
   u8g.setDefaultForegroundColor();
   u8g.firstPage();  
   do 
   {
     for(i = 4;i<u8g.getHeight();i+=4)
     { 
        u8g.drawHLine(0,i,u8g.getWidth());
     } 
     for(i = 4;i<u8g.getWidth();i+=4)
     { 
        u8g.drawVLine(i,0,u8g.getHeight());
     }
   }while(u8g.nextPage());   
   delay(1000);
   for(i = 0;i<=32;i+=2) //X from 0~128 
   {
      u8g.firstPage();  
      do 
      {
        u8g.drawLine(i, 0, 0, 127);
        u8g.drawLine(i*2, 0, 0, 127);
        u8g.drawLine(i*3, 0, 0, 127);
        u8g.drawLine(i*4, 0, 0, 127);
        u8g.drawLine(u8g.getWidth()-i, 0, 127, 127);
        u8g.drawLine(u8g.getWidth()-i*2, 0, 127, 127);
        u8g.drawLine(u8g.getWidth()-i*3, 0, 127, 127);
        u8g.drawLine(u8g.getWidth()-i*4, 0, 127, 127);
        u8g.drawLine(u8g.getWidth()/2+i, 0, 63, 127);
        u8g.drawLine(u8g.getWidth()/2+i*2, 0, 63, 127); 
        u8g.drawLine(u8g.getWidth()/2-i, 0, 63, 127);
        u8g.drawLine(u8g.getWidth()/2-i*2, 0, 63, 127);       
      }while(u8g.nextPage());       
   } 
}

void setup() 
{
    prepare();
}

void loop() 
{
  //loop
  prepare();
  test_rectangle();
  delay(1000);
  test_circle();
  delay(1000);
  test_round_rectangle();
  delay(1000);
  test_triangle();
  delay(1000);
  draw_line();
  delay(1000);
}
