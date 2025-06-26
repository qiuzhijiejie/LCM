#ifndef __320240_H__
#define __320240_H__

#include <INTRINS.H>    
//#include <STDIO.H>     
#include <MATH.H>    
#include <ABSACC.H>    
#include <REGX52.H>

#define  num  sizeof(tab)
#define  width  320  //picture width
#define  height  240  //picture height

#define uchar unsigned char  
#define uint  unsigned int 

/******************************************************* 
Define the instruction code,  parameter 定义指令码、参数
*******************************************************/
#define systemset  0x40  //Initial setting??8 byte parameter string  systemset_P8[]

#define scroll     0x44  //Set the display starting addresses and display range?? 10 byte parameter string  scroll_P10[]
#define basepart1  0x00
#define basepart2  0x28
#define basepart3  0x50
#define basepart4  0x00

#define sleepin    0x53  //Sleep mode setting      Subsequent parameter 0 byte
#define csrdirr    0x4c  //The cursor moves right  Subsequent parameter 0 byte
#define csrdirl    0x4d  //The cursor moves left  Subsequent parameter 0 byte
#define csrdiru    0x4e  //The cursor moves up Subsequent parameter 0 byte
#define csrdird    0x4f  //The cursor moves down Subsequent parameter 0 byte
#define dispoff    0x58  //Display off        Subsequent parameter 1 byte
#define dispon     0x59  //Display on    Subsequent parameter 1 byte
#define hdotscr    0x5a  //Displacement volume setting     Subsequent parameter 1 byte
#define ovlay      0x5b  //Display synthesis settings     Subsequent parameter 1 byte
#define cgramadr   0x5c  //CGRAM start  addresses  Subsequent parameter 2 byte
#define csrform    0x5d  //Set cursor shape     Subsequent parameter 2 byte
#define csrw       0x46  //Set cursor address     Subsequent parameter 2 byte
#define csrr       0x47  //Read cursor address    Subsequent parameter 2 byte
#define mwrite     0x42  //Display data writing     Subsequent parameter N byte
#define mread      0x43  //Display data reading     Subsequent parameter N byte
#define busy       0x40  //Busy signal D6   0500 0000
#define AP (systemset_P8[6])//A row of the display storage unit of the number of bytes

sbit _WR   =P3^1; 
sbit _RD   =P3^0;  
sbit CS   =P3^5; 
sbit CD   =P3^3; 
sbit reset=P3^2;
sbit LCD_KEY=P2^0;
void chkbusy(void);
uchar lcd_cmdread(void);
void write_cmd(unsigned char cmd);
void write_dat(unsigned char dat);
uchar read_dat(void);
void lcd_cls(uchar part);           //clear display??32KB RAM 
void show_on(void);                 //display on
void show_off(void);                //dispay off
void locate_xy1(uchar x, uchar y);  //1area cursor location
void locate_xy2(uint  x, uchar y);  //2area cursor location
void locate_xy22(uint  x, uchar y); //2area cursor location2
void set_addr1(uint addr);          //set 1 area address
void set_addr2(uint addr);          //set 2 area address
void roll_h(uint x, bit dir, bit part);         //Horizontal scrolling
void roll_v(uchar x, bit dir, bit part);        //Vertical scrolling
void show_char(uchar x, uchar y, uchar *string);//display string
void load_cgram(uchar m, bit inverse);          //load cgram
void show_hzg(uchar x, uchar y, uchar *string, bit inverse);//Display chinese graphics mode
void show_hzt(uchar x, uchar y, uchar n_4);     //display chinese  text mode
void show_pixel(uint x, uchar y, bit attr);     //drawing pixel
void show_line(uint x1, uchar y1, uint x2, uchar y2, bit attr);     //drawing line
void show_rectangle(uint x1, uchar y1, uint x2, uchar y2, bit attr);//drawing square
void show_circle(uint x, uchar y, uchar r, bit attr);               //drawing circle
void show_pic(uchar x, uchar y, uchar *string, bit inverse);        //display picture
void show_sine(uchar y, uchar a, uchar n, bit attr);                //drawing sine
void lcd_init(void);
void delay(uint ms);
void demo_pixel(void);    //show pixels
void demo_line(void);     //show line
void demo_rectangle(void);//show square
void demo_circle(void);   //show circle
void demo_picture(void);  //show picture
void demo_sine(void);     //show sine
void demo_roll(void);     //show scroll 
void demo_hz(void);       //show chinese 
void demo_game(void);     //show moving circle 
void DisplayDots(unsigned num1, unsigned num2);		
void Display_FullCGROM();
uint ReadKey1(); // 按键检测函数
#endif//__320240_H__