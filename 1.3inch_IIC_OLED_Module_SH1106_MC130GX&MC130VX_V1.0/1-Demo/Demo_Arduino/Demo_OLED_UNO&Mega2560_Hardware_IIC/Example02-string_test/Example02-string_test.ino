/****************************************************************************
*This program is a demo of display string.
*This demo was made for OLED modules with a screen resolution of 128x64 pixels.
*This program requires the U8glib library.

* File                : Example02-string_test.ino
* Hardware Environment: Arduino UNO&Mega2560
* Build Environment   : Arduino

*Set the pins to the correct ones for your development shield or breakout board.
*This demo use the BREAKOUT BOARD only and use these IIC data lines to the LCD,
*pin usage as follow:
*            SCL SDA
*       Uno  A5  A4  
*   Mega2560 21  20  

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
*******************************************************************************/
#include "U8glib.h" //U8g lib
#include "cfont.h"

U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_NONE);  // I2C / TWI 

//string for right and left
const char str1[] = "abcdefghik";
const char str2[] = "ABCDEFGHIK";
const char str3[] = "1234567890";

//string for down and up
const char str4[] = "1234567890";
const char str5[] = "ABCDEFGHIK";
const char str6[] = "abcdefghik";

//prepare for drawing string
void prepare(void) 
{
  u8g.setFont(u8g_font_8x13B);
  u8g.setFontRefHeightExtendedText();
  u8g.setDefaultForegroundColor();
  u8g.setFontPosTop();
}

void show_text(void)
{
    u8g.firstPage(); 
    do 
   { 
      u8g.drawStr(28,2,"OLED TEST");
      u8g.drawStr(40,17,"SH1106");
      u8g.drawStr(20,32,"1.3\" 64X128");
      u8g.drawStr(4,47,"www.lcdwiki.com");
    }while(u8g.nextPage()); 
    delay(1500);
}

//show string moving to right
void string_moving_to_right(void)
{
   int i; 
   for(i = -16;i<=32;i+=2) //X from -128~128 
   {
      u8g.firstPage();  
      do 
      {
        u8g.drawStr(0,0,"RIGHT");
        u8g.drawStr(i*4,13,(char *)str1); 
        u8g.drawStr(i*4,26,(char *)str2);
        u8g.drawStr(i*4,39,(char *)str3); 
      }while(u8g.nextPage());       
   }
}

//show string moving to left
void string_moving_to_left(void)
{
   int i; 
   for(i = 32;i>=-32;i-=2) //X from -128~128 
   {
      u8g.firstPage();  
      do 
      {
        u8g.drawStr(0,0,"LEFT");
        u8g.drawStr(i*4,13,(char *)str1);
        u8g.drawStr(i*4,26,(char *)str2);
        u8g.drawStr(i*4,39,(char *)str3); 
      }while(u8g.nextPage());       
   }
}

//show string moving to down 
void string_moving_to_down(void)
{
   int i;  
   for(i = -16;i<=32;i+=2) //X from -64~64 
   {
      u8g.firstPage();  
      do 
      {
        u8g.drawStr(0,0,"DOWN");
        u8g.drawStr90(58,i*4,(char *)str4);
        u8g.drawStr90(71,i*4,(char *)str5);
        u8g.drawStr90(84,i*4,(char *)str6);
      }while(u8g.nextPage());       
   }
}

//show string moving to up 
void string_moving_to_up(void)
{
   int i; 
   for(i = 32;i>=-32;i-=2) //X from -64~64 
   {
      u8g.firstPage();  
      do 
      {
        u8g.drawStr(0,0,"UP");
        u8g.drawStr90(58,i*4,(char *)str4);
        u8g.drawStr90(71,i*4,(char *)str5);
        u8g.drawStr90(84,i*4,(char *)str6);
      }while(u8g.nextPage());       
   }
}

void show_rotation(void)
{
    int i; 
    u8g.setFont(u8g_font_unifont);
    for(i=0;i<4;i++)
    {
       switch(i)
       {
          case 0:
            u8g.undoRotation();
            break;
          case 1:
            u8g.setRot90();
            break;
          case 2:
            u8g.setRot180();
            break;
          case 3: 
            u8g.setRot270();
            break;
       } 
      u8g.firstPage();  
      do 
      {
        u8g.drawStr(10, 10, "hello!");
      }while(u8g.nextPage()); 
      delay(1500);
    }
}

//show zoom
void display_zoom(void)
{
    int i; 
    u8g.setFont(u8g_font_9x18);
    for(i=0;i<4;i++)
    {
       switch(i)
       {
          case 0:
            u8g.setFontPosTop();
            break;
          case 1:
            u8g.setFontPosBottom();
            break;
          case 2:
            u8g.setFontPosBaseline();
            break;
          case 3: 
            u8g.setFontPosCenter();
            break;
       } 
      u8g.firstPage();  
      do 
      {
         u8g.drawStr(u8g.getWidth()>>1, u8g.getHeight()>>1, "FR");
         u8g.drawStr90(u8g.getWidth()>>1, u8g.getHeight()>>1, "FR");
         u8g.drawStr180(u8g.getWidth()>>1, u8g.getHeight()>>1, "FR");
         u8g.drawStr270(u8g.getWidth()>>1, u8g.getHeight()>>1, "FR");
      }while(u8g.nextPage()); 
      delay(500);
    }
     for(i=0;i<4;i++)
    {
       switch(i)
       {
          case 3:
            u8g.setFontPosTop();
            break;
          case 2:
            u8g.setFontPosBottom();
            break;
          case 1:
            u8g.setFontPosBaseline();
            break;
          case 0: 
            u8g.setFontPosCenter();
            break;
       } 
      u8g.firstPage();  
      do 
      {
         u8g.drawStr(u8g.getWidth()>>1, u8g.getHeight()>>1, "FR");
         u8g.drawStr90(u8g.getWidth()>>1, u8g.getHeight()>>1, "FR");
         u8g.drawStr180(u8g.getWidth()>>1, u8g.getHeight()>>1, "FR");
         u8g.drawStr270(u8g.getWidth()>>1, u8g.getHeight()>>1, "FR");
      }while(u8g.nextPage()); 
      delay(500);
    } 
}

void ShowFont16(unsigned char x,unsigned char y,unsigned char *s)
{
  unsigned char i,j,k,tmp;
  unsigned short num;
  num = sizeof(cfont16)/sizeof(typFNT_GB16);
  for(i=0;i<num;i++)
  {
    if((pgm_read_byte(&cfont16[i].Index[0])==*s)&&(pgm_read_byte(&cfont16[i].Index[1])==*(s+1)))
    {
      for(j=0;j<32;j++)
      {
        tmp = pgm_read_byte(&cfont16[i].Msk[j]);
        for(k=0;k<8;k++)
        {
          if(tmp&(0x80>>k))
          {
              u8g.drawPixel(x+(j%2)*8+k, y+j/2); 
          }
        }
      } 
      break;
    } 
  }
}

void ShowFont24(unsigned char x,unsigned char y,unsigned char *s)
{
  unsigned char i,j,k,tmp;
  unsigned short num;
  num = sizeof(cfont24)/sizeof(typFNT_GB24);
  for(i=0;i<num;i++)
  {
    if((pgm_read_byte(&cfont24[i].Index[0])==*s)&&(pgm_read_byte(&cfont24[i].Index[1])==*(s+1)))
    {
      for(j=0;j<72;j++)
      {
        tmp = pgm_read_byte(&cfont24[i].Msk[j]);
        for(k=0;k<8;k++)
        {
          if(tmp&(0x80>>k))
          {
            u8g.drawPixel(x+(j%3)*8+k, y+j/3);
          }
        }
      } 
      break;
    } 
  }
}

void ShowFont32(unsigned char x,unsigned char y,unsigned char *s)
{
  unsigned char i,j,k,tmp;
  unsigned short num;
  num = sizeof(cfont32)/sizeof(typFNT_GB32);
  for(i=0;i<num;i++)
  {
    if((pgm_read_byte(&cfont32[i].Index[0])==*s)&&(pgm_read_byte(&cfont32[i].Index[1])==*(s+1)))
    {
      for(j=0;j<128;j++)
      {
        tmp = pgm_read_byte(&cfont32[i].Msk[j]);
        for(k=0;k<8;k++)
        {
          if(tmp&(0x80>>k))
          {
            u8g.drawPixel(x+(j%4)*8+k, y+j/4);
          }
        }
      } 
      break;
    } 
  }
}

void ShowCHinese(unsigned char x,unsigned char y,unsigned char hsize,unsigned char *str)
{ 
  while(*str!='\0')
  {
    if(hsize == 16)
    {
      ShowFont16(x,y,str);
    }
    else if(hsize == 24)
    {
      ShowFont24(x,y,str);
    }
    else if(hsize == 32)
    {
      ShowFont32(x,y,str);
    }
    else
    {
      return;
    }
    x+=hsize;
    if(x>u8g.getWidth()-hsize)
    {
      x=0;
      y+=hsize;
    }
    str+=3;
  } 
}

void setup() 
{
    Serial.begin(9600);
  prepare(); 
}

void loop() 
{
  //show moving string loop
  prepare();
  u8g.undoRotation();
  show_text();
  delay(2000);
  string_moving_to_right();
  delay(150);
  string_moving_to_left();
  delay(150);
  string_moving_to_down();
  delay(150);
  string_moving_to_up();
  delay(1000);
  u8g.firstPage(); 
  do
  {
    ShowCHinese(16,24,16,"全动电子技术");
  }while(u8g.nextPage());  
  delay(1000);
  u8g.firstPage(); 
  do
  {
    ShowCHinese(16,20,24,"全动电子");
  }while(u8g.nextPage()); 
  delay(1000);
  u8g.firstPage(); 
  do
  {
    ShowCHinese(0,16,32,"全动电子");
  }while(u8g.nextPage()); 
  delay(1000);
  display_zoom();
  delay(1000);
  show_rotation();
  delay(1000);
}
