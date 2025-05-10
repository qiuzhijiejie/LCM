#include<reg51.h>
#include<intrins.h>
#define uint unsigned int
#define uchar unsigned char
//========================================================

sbit RS=P3^0;
sbit RW=P3^2;
sbit E=P3^1;
//========================================================
void INTI_ST7066U();
void write_com(uchar Command_byte);
void write_data(uchar Data8_byte);
void delay_ms(int t);
void delay(int t);
void all_disp();
void chara_disp();
void digi_disp();
void cha_disp();
//========================================================
void main()
{
  E=1;
  INTI_ST7066U();
  while(1)
{   all_disp();
   delay_ms(1000);
  chara_disp();
   delay_ms(1000);
   digi_disp();
  delay_ms(1000);
  cha_disp();
  delay_ms(1000);
}
}

void INTI_ST7066U()
{  
   write_com(0x3F);	// FUCT SET   DL=1  N=F=1
   delay(2);       
    write_com(0x0E);     //DISLY ON  D=1  C=1  B=0
   delay(2);
   write_com(0x06);      // MODE SET  I/D=1  S=0
   delay(2);
   write_com(0x01);     // CLEAR DISLY
   delay(2);
}

void write_com(uchar Command_byte)
{
   RS=0;
   RW=0;
   E=0;
   P1=Command_byte;
   delay(2);
   E=1;
   delay(2);
   E=0;
}
   

void write_data(uchar Data8_byte)
{   RS=1;
   RW=0;
   E=0;
   P1=Data8_byte;
   delay(2);
   E=1;
   delay(2);
   E=0;
}

void delay(int t)
{
  int i,j;
  for(i=0;i<=t;i++)
  for(j=0;j<10;j++)
 ;
}

void delay_ms(int t)
{   int i,j;
    for(i=0;i<=t;i++)
    for(j=0;j<125;j++)
;
}

void all_disp()
{   int i;
     write_com(0x80);
     for(i=0;i<20;i++)
     write_data(0xFF);
      write_com(0xC0);
     for(i=0;i<20;i++)
     write_data(0xFF);
}

void chara_disp()
{    
   write_com(0x80);
    write_data(0x44);
    write_data(0x4F);
    write_data(0x54);
    write_data(0x11);
    write_data(0x4D); 
    write_data(0x41);
    write_data(0x54);
    write_data(0x52);
    write_data(0x49);
    write_data(0x58);
    write_data(0x11);
    write_data(0x4C);
    write_data(0x43); 
    write_data(0x11);
    write_data(0x4D);
    write_data(0x4F);
    write_data(0x55);
    write_data(0x44);
    write_data(0x4C);
    write_data(0x45);
  
  write_com(0xC0);  
    write_data(0x2A);
    write_data(0x2A);
    write_data(0x2A);
    write_data(0x2A);
    write_data(0x2A); 
    write_data(0x2A);
    write_data(0x2A);
    write_data(0x54);
    write_data(0x4D);
    write_data(0x2D);
    write_data(0x32);
    write_data(0x30);
    write_data(0x32); 
    write_data(0x2A);
    write_data(0x2A);
    write_data(0x2A);
    write_data(0x2A);
    write_data(0x2A); 
    write_data(0x2A);
    write_data(0x2A);
}

void digi_disp()
{ int i;
       write_com(0x80);  
        write_data(0x30);
        write_data(0x31);
	     write_data(0x32);
		 write_data(0x33);
		 write_data(0x34);
		 write_data(0x35);
		 write_data(0x36);
		 write_data(0x37);
		 write_data(0x38);
		 write_data(0x39);
    write_data(0x41);
    write_data(0x42);
	 write_data(0x43);
	write_data(0x44);
      write_data(0x45);
    write_data(0x46);
    write_data(0x47);
    write_data(0x48);
    write_data(0x49);
    write_data(0x4A);
  write_com(0xC0);  
  for(i=0x61;i<=0x6F;i++)
   write_data(i);  
  for(i=0x70;i<=0x74;i++)
   write_data(i);  
}
void cha_disp()
{
  int i;
  write_com(0x80);
  for(i=0;i<20;i++)
  write_data(0xDB);
  write_com(0xC0);
  for(i=0;i<20;i++)
  write_data(0xDB);
}
   