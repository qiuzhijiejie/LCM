//----------------------------------------------------------------------
/*GUAGDONG YEHUI TECHNOLOGY CO,.LTD.*/
// Module    	: YHBG320240C Series
// Lanuage   	: C51 Code
// Create    	: XIEJIANREN
// Date      	: 2009-06-18
// LCM Drive IC : RA8835
// INTERFACE 	: 8080_8bit
// MCU 			: STC12C5A60AD OR STC12C5A60S2
// VDD			: 5V                 
//***************************************************************************    

#include <INTRINS.H>    
#include <STDIO.H>     
#include <MATH.H>    
#include <ABSACC.H>    
#include <STC12C5A60S2.H>  

#define uchar unsigned char  
#define uint  unsigned int 

sbit _WR   =P3^1; 
sbit _RD   =P3^0;  
sbit CS   =P3^7; 
sbit CD   =P3^3; 
sbit reset=P3^6;

/******************************************************* 
                 Define the instruction code,  parameter 定义指令码、参数
*******************************************************/
#define systemset  0x40  //Initial setting??8 byte parameter string  systemset_P8[]
uchar code 
systemset_P8[] =  {0x30, 0x87, 0x07, 0x27, 0x3f, 0xef, 0x28, 0x00};
                  //P1    P2    P3    P4    P5    P6    P7    P8
#define scroll     0x44  //Set the display starting addresses and display range?? 10 byte parameter string  scroll_P10[]
#define basepart1  0x00
#define basepart2  0x28
#define basepart3  0x50
#define basepart4  0x00
uchar code 
scroll_P10[]   = { 0x00, basepart1, 0xf0,  //1area??10KB??240 row
                   0x00, basepart2, 0xf0,  //2area??10KB??240 row
                   0x00, basepart3,        //3area??10KB
                   0x00, basepart4};       //4area??32KB
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
#define busy       0x40  //Busy signal D6   0100 0000
#define AP (systemset_P8[6])//A row of the display storage unit of the number of bytes
/******************************************************* 
                  	  chinese 中文
*******************************************************/
unsigned char code cgtab[][32]= {
/*--  ????:  ?  --*/
/*--  ????12;  ????????????????????x??=16x16   --*/
0x40,0x40,0x20,0x20,0x27,0xFE,0x09,0x20,0x89,0x20,0x52,0x7C,0x52,0x44,0x16,0xA8,
0x2B,0x98,0x22,0x50,0xE2,0x20,0x22,0x30,0x22,0x50,0x22,0x88,0x23,0x0E,0x22,0x04,

/*--  ????:  ??  --*/
/*--  ????12;  ????????????????????x??=16x16   --*/
0x00,0x00,0x1F,0xF0,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x1F,0xF0,0x10,0x10,
0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x1F,0xF0,0x10,0x10,0x00,0x00,0x00,0x00,

/*--  ????:  ?  --*/
/*--  ????12;  ????????????????????x??=16x16   --*/
0x20,0x90,0x20,0x90,0x27,0xFC,0x20,0x90,0xFB,0xF8,0x22,0x08,0x73,0xF8,0x6A,0x08,
0xA3,0xF8,0xA0,0x40,0x27,0xFE,0x20,0x40,0x20,0xA0,0x20,0x98,0x21,0x0E,0x26,0x04,

/*--  ????:  ??  --*/
/*--  ????12;  ????????????????????x??=16x16   --*/
0x10,0x00,0x19,0xF8,0x11,0x08,0x25,0x08,0x25,0x08,0x79,0xF8,0x09,0x08,0x11,0x08,
0x21,0x08,0x7D,0xF8,0x01,0x08,0x01,0x08,0x0D,0x08,0x73,0xFE,0x00,0x00,0x00,0x00,

};

unsigned char code tab[]={
/*--  ????:  ??  --*/
/*--  ????12;  ????????????????????x??=16x16   --*/
0x01,0x00,0x02,0x00,0x10,0x90,0x10,0x00,0x08,0x10,0x10,0x20,0x02,0x00,0x00,0x00,
0x00,0x80,0x00,0x00,0x00,0x80,0x02,0x00,0x10,0x94,0x13,0xFE,0x1D,0x10,0x10,0x20,

/*--  ????:  ??  --*/
/*--  ????12;  ????????????????????x??=16x16   --*/
0x02,0x00,0x7B,0xF8,0x04,0x80,0x3F,0xF8,0x00,0x80,0x02,0x00,0x11,0x14,0x92,0x02,
0xF0,0x90,0x10,0x20,0xFF,0xFE,0x4A,0x08,0x04,0x80,0x00,0x08,0x3F,0xFC,0x7F,0xFC,

/*--  ????:  ??  --*/
/*--  ????12;  ????????????????????x??=16x16   --*/
0x15,0x18,0x54,0x44,0x10,0x90,0x13,0xFE,0x04,0x00,0x52,0x08,0x08,0x40,0x00,0x08,
0x20,0x00,0x04,0x00,0x5B,0x10,0x58,0x40,0x10,0x10,0xFC,0x20,0x04,0x00,0x53,0xF8,

/*--  ????:  ??  --*/
/*--  ????12;  ????????????????????x??=16x16   --*/
0x08,0x40,0x7F,0xE8,0x20,0x00,0x09,0x00,0x55,0x32,0x13,0xFC,0xFD,0x10,0x10,0x20,
0x0F,0xF0,0x62,0x08,0x10,0x20,0x00,0x08,0x20,0x00,0x11,0x00,0x51,0x52,0xFC,0x80,

/*--  ????:  ??  --*/
/*--  ????12;  ????????????????????x??=16x16   --*/
0x10,0x90,0x10,0x20,0x08,0x10,0x52,0x08,0x20,0x10,0x00,0x08,0x20,0x00,0x21,0x00,
0x91,0x0E,0x28,0xA0,0x38,0x90,0x15,0xFC,0x18,0x10,0x4B,0xF8,0x42,0x08,0x1F,0x88,

/*--  ????:  ??  --*/
/*--  ????12;  ????????????????????x??=16x16   --*/
0x20,0x00,0x3F,0xF8,0x11,0x20,0x29,0x20,0x34,0x10,0x18,0x84,0x2F,0xF0,0x4A,0x44,
0x82,0x06,0x10,0x88,0x20,0x00,0x01,0x00,0x10,0x20,0x29,0xFC,0x50,0x1E,0x30,0x88,

/*--  ????:  ??  --*/
/*--  ????12;  ????????????????????x??=16x16   --*/
0x48,0x10,0x4A,0x48,0x04,0x00,0x10,0x88,0x20,0x00,0x09,0x20,0x13,0xFE,0x28,0x20,
0x53,0xF0,0xD0,0x48,0x88,0x10,0x6A,0x30,0x04,0x40,0x10,0x88,0x20,0x00,0x11,0x10,

/*--  ????:  ??  --*/
/*--  ????12;  ????????????????????x??=16x16   --*/
0x28,0x20,0x28,0x20,0x90,0x10,0x10,0x50,0x0F,0xF0,0x52,0x20,0x08,0x20,0x10,0x88,
0x20,0x00,0x21,0x08,0x24,0x20,0x2B,0xFE,0x10,0x10,0x10,0x20,0x08,0x10,0x42,0x10,
/*--  ????:  ??  --*/
/*--  ????12;  ????????????????????x??=16x16   --*/
0x10,0x20,0x1F,0x88,0x40,0x00,0x41,0x04,0x44,0x20,0x4C,0x20,0x10,0x10,0x10,0x50,
0x08,0x10,0x42,0x88,0x3F,0xF0,0x10,0x88,0x40,0x00,0x05,0x00,0x40,0x20,0x48,0x20,

/*--  ????:  ??  --*/
/*--  ????12;  ????????????????????x??=16x16   --*/
0x10,0x10,0x51,0x88,0x08,0x50,0x43,0x06,0x10,0x10,0x00,0x28,0x80,0x00,0x02,0x00,
0x80,0x20,0x80,0x20,0x10,0x10,0x26,0x06,0x08,0x20,0x42,0x00,0x00,0x00,0x00,0x10,

};
#define  num  sizeof(tab)



#define  width  320  //picture width
#define  height  240  //picture height


unsigned char code tab1[];
unsigned char code tab2[];
unsigned char code tab3[];
/******************************************************* 
                  	Function declaration  功能声明
*******************************************************/
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



void demo_pixel(void);    //show pixels
void demo_line(void);     //show line
void demo_rectangle(void);//show square
void demo_circle(void);   //show circle
void demo_picture(void);  //show picture
void demo_sine(void);     //show sine
void demo_roll(void);     //show scroll 
void demo_hz(void);       //show chinese 
void demo_game(void);     //show moving circle 



/******************************************************* 
                  	Write instruction code  写指令码
*******************************************************/
void write_cmd(unsigned char cmd)
{
 CS = 0;
 _RD = 1;  
 P1 = cmd;
 CD = 1;    //command
 _WR = 0;    //Rising edge of write
 _WR = 1;
 CS = 1;
}
/******************************************************* 
              Write command parameters, display data  写命令参数，显示数据
*******************************************************/
void write_dat(unsigned char dat)
{chkbusy();
 CS = 0;
 _RD = 1;
 P1 = dat;
 CD = 0;    //data
 _WR = 0;    //Rising edge of write
 _WR = 1;
 CS = 1;
}
/******************************************************* 
                  	read data  读取数据
*******************************************************/
uchar read_dat(void)
{ uchar temp=0;
chkbusy();
 CS = 0;
 CD   = 1;
 _WR   = 1;
 _RD   = 0;
 temp = P1;
 _RD   = 1;
  CS = 1;
 return (temp);
}

/******************************************************* 
                      check busy  检查忙碌状态
*******************************************************/
void chkbusy(void)
{
     uchar b;
	 b=1;
	if(b!=0)
 	{
    b=lcd_cmdread();
    b&=0x40; 
 	} 
 
}

/******************************************************* 
                      read status 阅读状态
*******************************************************/
uchar lcd_cmdread(void)
{   
    uchar address_cmd;
    P1=0xff;
    CS = 0;
    CD = 0;
 	_WR = 1;
    _RD = 0;
    address_cmd =P1;
    _RD = 1;
    CS = 1;
  return(address_cmd);
}


/******************************************************* 
                      Delay  延迟
*******************************************************/
void delay(uint a)
{
 uint  b1,c;
 for(b1=a  ;b1>0;b1--);
 for(c=24000;c>0;c--);
}
/******************************************************* 

/******************************************************* 
                LCM  Initialization  LCM初始化
*******************************************************/
void lcd_init(void)
{
 uchar i;
 write_cmd(systemset);//Initialization settings
 for(i=0;i<8 ;i++)
 write_dat(systemset_P8[i]);
 write_cmd(scroll);   //Setting the display start address and the display range
 for(i=0;i<10;i++) 
 write_dat(scroll_P10[i]);
 write_cmd(hdotscr);  //Point displacement amount set
 write_dat(0x00);     //Do not move
 write_cmd(ovlay);    //Display synthesis settings
 write_dat(0x00);     //Synthesis of double, text display, logic or
 show_off();          //display off
 lcd_cls(0);          //clrar dispay
 show_on();           //display on
}
/******************************************************* 
             clear display  清晰的显示屏
               32KB RAM 
           part = 1  1area
           part = 2  2area
           part = 3  3area
           part = 4  4area              
*******************************************************/
void lcd_cls(uchar part)
{
 uint i = 10240;       //10240 = 0x2800  10KB RAM
 write_cmd(csrw);      //Write cursor address
 write_dat(0x00);      //First write the low eight
      if(part == 1) write_dat(0x00);     //1area
 else if(part == 2) write_dat(basepart2);//2area
 else if(part == 3) write_dat(basepart3);//3area
 else if(part == 4) write_dat(basepart4);//4area
 else {i = 32768;   write_dat(0x00);}    //After writing the high eight, All dispay area 32KB RAM
 write_cmd(csrdirr);   //Cursor right direction
 write_cmd(mwrite);    //write data 
 while(i--) write_dat(0x00);//Write 0
}
/******************************************************* 
                 	Display on
*******************************************************/
void show_on(void)
{
 write_cmd(dispon);    //Display on
 write_dat(0x57);      //the cursor 1Hz flashes
}
/******************************************************* 
                  	Display  off
*******************************************************/
void show_off(void)
{
 write_cmd(dispoff);   //Display  off
 write_dat(0x57);      //the cursor 1Hz flashes
}

/******************************************************* 
                      Cursor positioning  光标定位
                x: 0~39 320/8=40
                y: 0~29 240/8=30
                  1 area Text
*******************************************************/
void locate_xy1(unsigned char x, unsigned char y)
{
 unsigned int  addr;
 addr  =(unsigned int)(y) * AP + x;
 write_cmd(csrw);       //Write cursor address
 write_dat((unsigned char)(addr & 0xff));  //First write the low eight
 write_dat((unsigned char)(addr >> 8));  //After writing the high eight
}

/******************************************************* 
                  	 Cursor positioning  光标定位
                  x: 0~319 320
                  y: 0~239 240
                2  area   Graphics
*******************************************************/
void locate_xy2(uint x, uchar y)
{
 uint  addr;
 addr  = (uint)(y) * AP + x/8;
 write_cmd(csrw);         //Write cursor address
 write_dat((uchar)(addr & 0xff));    //First write the low eight
 write_dat((uchar)(addr >> 8) + basepart2); //After writing the high eight
}
/******************************************************* 
                      Cursor positioning  光标定位
                  x: 0~319 320
                  y: 0~239 240
                  2  area      Graphics         
*******************************************************/
void locate_xy22(uint x, uchar y)
{
 uint  addr;
 addr  = (uint)(y) * AP + x;
 write_cmd(csrw);         //Write cursor address
 write_dat((uchar)(addr & 0xff));    //First write the low eight
 write_dat((uchar)(addr >> 8) + basepart2); //After writing the high eight
}
/******************************************************* 
                     set 1 area address   Text  设置1个地区地址文本
*******************************************************/
void set_addr1(uint addr)
{
 addr += basepart1;
 write_cmd(scroll);
 write_dat((uchar)(addr & 0xff));
 write_dat((uchar)(addr >> 8));
}
/******************************************************* 
                  	 set 2 area address   Graphics 设置2个区域地址图形
*******************************************************/
void set_addr2(uint addr)
{
 addr += basepart2;
 write_cmd(scroll);
 write_dat(scroll_P10[0]);
 write_dat(scroll_P10[1]);
 write_dat(scroll_P10[2]);
 write_dat((uchar)(addr & 0xff));
 write_dat((uchar)(addr >> 8));   
}
/******************************************************* 
                Vertical direction scrolling display  垂直方向滚动显示器
        x?? Rolling distance  Text--x  row ??Graphics--x pixels
      dir?? Rolling direction  0--left ??1--right
       part??Scroll area  0--1area?? 1--2area                  	 
*******************************************************/
void roll_h(uint x,bit dir,bit part)
{
 uchar i;
 uint  addr;
 if(dir) addr = x;  //Horizontally to the right
 else addr = 0;     //Horizontally to the left
 while(x--)
 {
  for(i = 0; i < 8; i++)
  {
   write_cmd(hdotscr);
   if(dir) write_dat(7 - i); //Horizontally to the right
   else write_dat(i);        //Horizontally to the left
 delay(50000);
  delay(50000);
  }
  if(dir) addr--;   //Horizontally to the right
  else addr++;      //Horizontally to the left
  if(part) set_addr2(addr); //2area
  else  set_addr1(addr);    //1area
 }
}
/******************************************************* 
                Vertical direction scrolling display 垂直方向滚动显示器
        x?? Rolling distance  Text--x  row ??Graphics--x pixels
      dir?? Rolling direction  0--Up ??1--Down
       part??Scroll area  0--1area?? 1--2area                  	 
*******************************************************/
void roll_v(unsigned char x, bit dir, bit part)
{
 unsigned char i;
 unsigned int  addr = 0;
 for(i = 0; i <= x; i++)
 {
  if(dir) addr = AP * (x - i); //Vertically upwards
  else addr = AP * i;          //Vertically downwards
  if(part) set_addr2(addr);    //2 area
  else  set_addr1(addr);       //1 area 
 delay(50000);
  delay(50000);
 }
}

/******************************************************* 
                      display string 显示字符串
                x: 0~39 320/8=40
                y: 0~29 240/8=30                	 
*******************************************************/
void show_char(unsigned char x, unsigned char y, unsigned char *string)
{
 locate_xy1(x, y);    //Cursor positioning
 write_cmd(csrform);    //Set cursor shape 
 write_dat(0x07);    //Horizontal display width 6
 write_dat(0x07);    //Linear cursor display??hight 8
 write_cmd(csrdirr);    //Cursor right direction
 write_cmd(dispon);    //Display the status is on
 write_dat(0x57);    //Cursor 1Hz blinking
 write_cmd(mwrite);    //write data 
 while(*string) 
 {
  write_dat(*string); 
  string++;
 }
}
/******************************************************* 
                     display chinese (Graphics mode) 显示中文（图形模式）
                 x: 0~39 320/8=40
                 y: 0~29 240/8=30                 	 
*******************************************************/
void show_hzg(uchar x,uchar y,uchar *string,bit inverse)
{
 uchar i, j,k,adr=0;
 write_cmd(csrdirr);    //Cursor right direction
 for(j = 0; j < num/32; j++)
 { 
	for(k=0;k<16;k++)
	{	  locate_xy22(x+adr,y+k);  //Cursor positioning
		  write_cmd(mwrite);   //write data 
		  for(i = 0; i < 2; i++) 
		  {
		   if(inverse) write_dat(~*string);
		   else  write_dat(*string); 
		   string++;
		  }
	}
	adr+=2;
 }
}
/******************************************************* 
					Load cgram 加载 cgram
*******************************************************/
void load_cgram(uchar m,bit inverse)
{
 uchar i, j;
 write_cmd(cgramadr);
 write_dat(0x00);
 write_dat(0x50);  //cgram start address
 write_cmd(csrdirr);
 write_cmd(csrw);
 write_dat(0x00);
 write_dat(0x54);  //cgram 1K RAM end address
 write_cmd(mwrite);
 for(j = 0; j < m; j++)
 {
	 for(i = 0; i < 32; i+=2) 
	  {
	   if(inverse) write_dat(~cgtab[j][i]);
	   else  write_dat(cgtab[j][i]);

	  }
	 for(i = 1; i < 32; i+=2) 
	  { 
	   if(inverse) write_dat(~cgtab[j][i]);
	   else  write_dat(cgtab[j][i]);

	  }
 }
}
/******************************************************* 
                   display chinese (Text mode) 显示中文（文本模式）
              x: 0~39 320/8=40
              y: 0~29 240/8=30
*******************************************************/
void show_hzt(uchar x,uchar y,uchar n_4)
{
 locate_xy1(x, y);
 write_cmd(csrdird);    //Cursor down direction
 write_cmd(mwrite);
 write_dat(n_4);
 write_dat(n_4 + 1);
 locate_xy1(x + 1, y);
 write_cmd(mwrite);
 write_dat(n_4 + 2);
 write_dat(n_4 + 3);
}
/******************************************************* 
                     display pixels 显示器像素
                 x: 0~319 320
                 y: 0~239 240
       attr:Display Properties??0-Drawing   1-clear
*******************************************************/
void show_pixel(uint x,uchar y,bit attr)
{ 
 uchar temp, tempd;
 tempd = 0x80 >> (x & 0x07);
 locate_xy2(x, y);     //Cursor positioning
 write_cmd(mread);
 temp = read_dat();
 if(attr) temp &= ~tempd;
 else  temp |= tempd;
 locate_xy2(x, y);     //Cursor positioning
 write_cmd(mwrite);     //write data 
 write_dat(temp);
}
/******************************************************* 
                    display line 显示线
                 x: 0~319 320
                 y: 0~239 240
       attr:Display Properties??0-Drawing   1-clear
*******************************************************/
void show_line(uint x1,uchar y1,uint x2,uchar y2,bit attr)
{
 int  dx, dy;
 char inc_x, inc_y;
 int  xerr = 0, yerr = 0;    //Initialize variables
 uint i, ds;
    dx = x2 - x1;       //Calculate the coordinates increment
    dy = y2 - y1;
 if(dx > 0) inc_x = 1;     //Set the single-step direction
 else 
 {
  if(dx == 0) {inc_x = 0;     } //Vertical line
  else  {inc_x = -1; dx = -dx;}
 }
 if(dy > 0) inc_y = 1;     //Set the single-step direction
 else
    {
  if(dy == 0) {inc_y = 0;     } //Horizontal line
  else  {inc_y = -1; dy = -dy;}
 }
 if(dx > dy) ds = dx;     //select the basic incremental axis
 else  ds = dy;
 for(i = 0; i <= ds+1; i++)    //drawing line
 {
  show_pixel(x1, y1, attr);   //drawing pixel
  xerr += dx;
  yerr += dy;
  if(xerr > ds)
  {
   xerr -= ds;
   x1   += inc_x;
  }
  if(yerr > ds)
  {
   yerr -= ds;
   y1   += inc_y;
  }
 }
}
/******************************************************* 
                     display square 展示广场
                 x: 0~319 320
                 y? 0~239 240
       attr:Display Properties??0-Drawing   1-clear
*******************************************************/
void show_rectangle(uint x1,uchar y1,uint x2,uchar y2,bit attr)
{
 show_line(x1, y1, x2, y1, attr);
 show_line(x1, y1, x1, y2, attr);
 show_line(x2, y1, x2, y2, attr);
 show_line(x1, y2, x2, y2, attr);
}
/******************************************************* 
                    display circle  显示圈
               x: 0~319 320 
               y: 0~239 240
               r: Radius  0~239 240
       attr:Display Properties??0-Drawing   1-clear
*******************************************************/
void show_circle(uint x,uchar y,uchar r,bit attr)
{
 uchar dx,dy = r; 
 for(dx = 0; dx <= r; dx++)
 {
  while((r * r + 1 - dx * dx) < (dy * dy)) dy--;
  show_pixel(x + dx, y - dy, attr);
  show_pixel(x - dx, y - dy, attr);
  show_pixel(x - dx, y + dy, attr);
  show_pixel(x + dx, y + dy, attr);
  show_pixel(x + dy, y - dx, attr);
  show_pixel(x - dy, y - dx, attr);
  show_pixel(x - dy, y + dx, attr);
  show_pixel(x + dy, y + dx, attr);
 }
}
/******************************************************* 
                   display picture  显示画面
              x: 0~39 320/8=40
              y: 0~239 240
*******************************************************/
void show_pic(uchar x,uchar y,uchar *string,bit inverse)
{
 uchar i, j;
 write_cmd(csrdirr);    //Cursor right direction
 for(j = 0; j < height; j++)
 { 
  locate_xy22(x , y+ j);  //Cursor positioning
  write_cmd(mwrite);   //write data
  for(i = 0; i < width/8; i++) 
  {
   if(inverse) write_dat(~*string);
   else  write_dat(*string); 
   string++;
  }
 }
}
/******************************************************* 
                     display sine   显示正弦
                  y: coordinate  119
       attr:Display Properties??0-Drawing   1-clear
*******************************************************/
void show_sine(uchar y,uchar a,uchar n,bit attr)
{
 uint  x1 = 0, x2;
 uchar y1 = 119, y2;
 for(x2 = 0; x2 < 320; x2++)
 {
  y2 = y + (a * sin(0.0175 * n * x2));
  show_line(x1, y1, x2, y2, attr);
  x1 = x2;
  y1 = y2;
 }
}


/******************************************************* 
				   show pixels  显示像素
*******************************************************/
void demo_pixel(void)
{
 uint  i, j;
 uchar n;
 //lcd_init();
  lcd_cls(0);
 n = 10;
 for(j = 1; j < 121; j += n) 
 {
  for(i = j-n; i < 240-j;   i++) show_pixel(j, i, 0);         //"| "
  for(i = j+1; i < 240-j;   i++) show_pixel(i, 239-j, 0);     //"_"
  for(i = j+1; i < 240-j;   i++) show_pixel(239-j, 239-i, 0); //" |"
  for(i = j+1; i < 240-n-j; i++) show_pixel(239-i, j, 0);     //"??
 }
 for(j = 5; j < 121; j += n) 
 { 
  for(i = j-n; i < 240-j; i++) {show_pixel(j, i, i&0x02);  }       //"| "
  for(i = j+1; i < 240-j; i++) {show_pixel(i, 239-j, i&0x02); }    //"_"
  for(i = j+1; i < 240-j; i++) {show_pixel(239-j, 239-i, i&0x02);} //" |"
  for(i = j+1; i < 240-n-j;i++){show_pixel(239-i, j, i&0x02); }    //"??"
 }
}
/******************************************************* 
					 show line  表演路线
*******************************************************/
void demo_line(void)
{
 //lcd_init();
  lcd_cls(0);
 show_line(0,0,319,239, 0);  //"\"
 show_line(0,239,319,0, 0);  //"_"
 show_line(0,0,319,0, 0);    //"-"
 show_line(0,0,0,239, 0);    //"|"
 show_line(0,239,319,239, 0);//"/"
 show_line(319,0,319,239, 0);//"|"
 show_line(0,0,239,239, 0);
 show_line(0,239,239,0, 0);
 show_line(239,0,239,239, 0);
   delay(50000);
   delay(50000);
   delay(50000);
   delay(50000);
   delay(50000);
   delay(50000);
   delay(50000);
   delay(50000);
}
/******************************************************* 
					show Square  展示广场
*******************************************************/
void demo_rectangle(void)
{
 uchar i;
 //lcd_init();
  lcd_cls(0);
 for(i=0;i<119;i+=5)
 {show_rectangle(i, i, 319-i, 239-i, 0);}
 for(i = 0; i < 119; i += 5)
 {show_rectangle(i, i, 319-i, 239-i, 1);}
 delay(100);
}
/******************************************************* 
					 show circle  展示圈
*******************************************************/
void demo_circle(void)
{
 uchar i, j;
// lcd_init();
 lcd_cls(0);
 show_circle(4,4,3, 0);
 show_circle(315,4,3, 0);
 show_circle(315,235,3, 0);
 show_circle(4,235,3, 0);
 for(i = 1; i< 118; i+=6) 
 { 
  show_circle(159,119,i, 0);
  delay(100);
 }
 j = 1;
 while(j--)
 {
  for(i = 1; i< 118; i+=6) 
  { 
   show_circle(159,119,i, 1);
   delay(100);
  }
  for(i = 118; i > 2; i-=3) 
  { 
   show_circle(159,119,i, 0);
   delay(100); 
  }
 }
}
/******************************************************* 
					 show picture  显示图片
*******************************************************/
void demo_picture(void)
{
// lcd_init();
 lcd_cls(0);
 show_pic (0, 0, tab1, 0);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
 show_pic (0, 0, tab1, 1);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
 show_pic (0, 0, tab2, 0);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
 show_pic (0, 0, tab2, 1);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);

 show_pic (0, 0, tab3, 0);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
 show_pic (0, 0, tab3, 1);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
}
/******************************************************* 
					sine presentation function  正弦表示函数
*******************************************************/
void demo_sine(void)
{
 uchar i;
// lcd_init();
 lcd_cls(0);
 i = 2;
 while(i--)
 { 
  show_sine(119, 80, 5, 0);
  delay(100);
  show_sine(119, 80, 5, 1);
  delay(100);
 }
}
/******************************************************* 
					display scroll  显示滚动
*******************************************************/
void demo_roll(void)
{
// lcd_init();
 lcd_cls(0);
//show_char(0, 0, "EastRising technology  www.buy-display.com ");
 //show_char(0, 3, "!*^_^*!  EastRising technology  www.buy-display.com");
 //show_char(0, 29, "X");
 //show_char(39, 29, "Y");
 //show_char(19, 17, "Z");
 //roll_h(5,0,0);
 //roll_h(5,1,0);
 //roll_v(10,0,0);
 //roll_v(10,1,0);
 //show_char(16, 15, "!*^_^*!");
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);
  delay(50000);

}
/******************************************************* 
					show chinese  表演中文
*******************************************************/
void demo_hz(void)
{
 //lcd_init();
 //lcd_cls(0);
 //show_hzg(0, 0,  tab, 0);
 //show_hzg(0, 32, tab, 1);
 //load_cgram(4,0);
 //show_hzt(0,2,0x80);
 //show_hzt(2,2,0x84);
 //show_hzt(4,2,0x88);
 //show_hzt(6,2,0x8c);
   delay(50000);
   delay(50000);
   delay(50000);
   delay(50000);
   delay(50000);
   delay(50000);
   delay(50000);
   delay(50000);
   delay(50000);
   delay(50000);
   delay(50000);
   delay(50000);
   delay(50000);
   delay(50000);
   delay(50000);
   delay(50000);
}
/******************************************************* 
						show moving circle   显示移动圆圈
*******************************************************/
void demo_game(void)
{uchar cs=100;
 uchar r = 20;    //Radius
 uchar i = 3;    //Stepping variables
 int   x = 30,y = 30;     //Starting position
 int   dx = i,  dy = i;    //Stepping
 int   x1, y1;      //Save the last position
 
 //lcd_init();
 lcd_cls(0);
 while(cs--)
 {
  show_circle(x, y, r, 0);
  x1 = x, y1 = y;     //Save location
  if((x-r) < 0) dx = i;
  if((x+r) > 319) dx = -i;
  if((y-r) < 0) dy = i;
  if((y+r) > 239) dy = -i;
  x += dx;
  y += dy; 
  delay(50000);     
  show_circle(x1, y1, r, 1);  //Disappear before a round
  show_pixel(x1, y1, x&0x01);  //Locus
 }
}
/******************************************************* 
					 The main function  主要功能
*******************************************************/
void main(void)
{P0=P1=P2=P3=0xff;
 delay(10);
 reset=0;
 delay(10);
 reset=1;
 delay(100);
 lcd_init();
 delay(100);
 while(1)
	{
	 demo_picture();   //show picture
	 demo_pixel();     //drawing pixels
	 demo_line();      //drawing line
	 demo_rectangle(); //painted square demo function
	 demo_circle();    //drawing circle
	 demo_sine();      //sine presentation function 
	 demo_hz();        //show chinese 
	 demo_roll();      //display scroll 
	 demo_game();     //show moving circle 
	 }
}




unsigned char code tab1[]={
/*-- ERM320240-1.bmp  --*/
/*--  320x240  --*/
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X40,0X00,0X00,0X18,0X00,0X00,
0X04,0X00,0X00,0X00,0X00,0X00,0X00,0X06,0X00,0X00,0X60,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X18,0X00,0X00,0X01,0X87,0X00,
0X60,0X00,0XE0,0X00,0X00,0X18,0X00,0X00,0X0E,0X00,0X00,0X00,0X03,0X00,0X60,0X06,
0X00,0X00,0X60,0X00,0X00,0X00,0X00,0X00,0X00,0X30,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X1C,0X7F,0XFF,0XC1,0X87,0X18,0X60,0X00,0X60,0X00,0X00,0X18,0X00,0X00,
0X06,0X00,0X03,0XFC,0X03,0X00,0X60,0X06,0X00,0X00,0XE0,0X00,0X1F,0XCF,0XFF,0X00,
0X1C,0X30,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X0F,0X7F,0XFF,0XC1,0X87,0X18,
0X60,0X00,0X70,0X00,0XFF,0XFF,0XFF,0X00,0X07,0X00,0X0F,0XFC,0X83,0X00,0X60,0X06,
0X00,0XFF,0XFF,0XFF,0X19,0XCE,0X07,0X00,0X18,0X38,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X07,0XE0,0X00,0XC1,0X87,0X18,0X63,0XFF,0XFF,0XFC,0XFF,0XFF,0XFF,0X00,
0X02,0X00,0X0D,0XC0,0XE3,0X00,0X60,0X0F,0X00,0XFF,0XFF,0XFF,0X98,0XCC,0X03,0X00,
0X38,0X18,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X03,0X60,0X00,0XC1,0X87,0X18,
0X63,0XFF,0XFF,0XFC,0X00,0X18,0X00,0X3F,0XFF,0XFF,0XC0,0XC0,0XF3,0X00,0XF3,0XFF,
0XFC,0X03,0XC0,0X00,0X18,0XCC,0X03,0X00,0X70,0X1C,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X03,0X18,0X01,0X87,0X18,0X60,0X00,0X60,0X00,0X00,0X18,0X00,0X3F,
0XFF,0XFF,0XC0,0XC0,0X3B,0X03,0XFC,0X0F,0X00,0X03,0X80,0X00,0X19,0X8C,0X03,0X00,
0X70,0X0E,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X20,0X0F,0X1E,0X0F,0XF7,0X18,
0X60,0X00,0X60,0X00,0X00,0X3C,0X00,0X00,0X60,0X00,0X00,0XC0,0X1B,0X00,0X60,0X06,
0X00,0X03,0X00,0X00,0X19,0X8F,0XFF,0X00,0XE0,0X07,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X78,0X1E,0X0F,0X0F,0XF7,0X18,0X60,0X00,0X60,0X00,0X3F,0XFF,0XFC,0X00,
0X60,0X00,0X0F,0XFC,0X03,0X00,0X60,0X06,0X00,0X07,0XFF,0XF8,0X19,0X8F,0XFF,0X01,
0XC0,0X07,0X80,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X3C,0X38,0X03,0X81,0X87,0X18,
0X60,0X00,0XF0,0X00,0X38,0X3C,0X1C,0X00,0X60,0X00,0X0F,0XFD,0X03,0X00,0X60,0X06,
0X00,0X0F,0XFF,0XF8,0X1B,0X0C,0X03,0X03,0X80,0X03,0XC0,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X0F,0X70,0XC1,0X81,0X87,0X18,0X60,0XFF,0XFF,0XF0,0X30,0X18,0X0C,0X00,
0XE0,0X00,0X01,0XC3,0X83,0X00,0X61,0XFF,0XF8,0X1E,0X00,0X18,0X1B,0X0C,0X03,0X07,
0X80,0X01,0XE0,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X06,0X00,0XC0,0X01,0X87,0X18,
0X60,0XFF,0XFF,0XF0,0X30,0X18,0X0C,0X00,0XFF,0XFE,0X01,0XC1,0XC3,0X00,0X67,0XFF,
0XF8,0X3E,0X00,0X18,0X1B,0X8C,0X03,0X0F,0X07,0X00,0XF8,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0XE0,0X01,0X87,0X18,0X60,0XC0,0X60,0X30,0X30,0X18,0X0C,0X00,
0XFF,0XFE,0X01,0XF0,0XE3,0X00,0X7C,0XE0,0X38,0X7E,0X00,0X18,0X19,0X8F,0XFF,0X1E,
0X07,0X00,0X70,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X7F,0XFF,0XC1,0X87,0X18,
0X60,0XC0,0X60,0X30,0X38,0X3C,0X1C,0X00,0XE0,0X06,0X03,0XF8,0X63,0X03,0XF8,0X60,
0X30,0XEF,0XFF,0XF8,0X18,0XCF,0XFF,0X0C,0X0E,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X7F,0XFF,0XC1,0X87,0X18,0X60,0XC0,0X60,0X30,0X3F,0XFF,0XFC,0X00,
0XC0,0X0E,0X03,0XDC,0X03,0X07,0XE0,0X60,0X71,0XEF,0XFF,0XF8,0X18,0XCC,0XC0,0X80,
0X0E,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X06,0X01,0XE0,0X01,0X86,0X18,
0X60,0XC0,0X60,0X30,0X38,0X7E,0X1C,0X00,0XC0,0X0E,0X07,0XCC,0X03,0X06,0X60,0X30,
0X60,0XCE,0X00,0X18,0X18,0XCC,0XE1,0XC0,0X1C,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X0E,0X03,0XF0,0X01,0XFE,0X18,0X60,0XC0,0X60,0X30,0X00,0X7E,0X00,0X01,
0XC0,0X0E,0X06,0XC0,0X1F,0XF8,0X60,0X30,0XE0,0X0E,0X00,0X18,0X18,0XCC,0X63,0X80,
0X18,0X38,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X0E,0X06,0XF8,0X0F,0XF6,0X18,
0X60,0XC0,0X60,0X30,0X00,0XFF,0X00,0X01,0XC0,0X0E,0X0C,0XCF,0XFF,0XF0,0X60,0X19,
0XC0,0X0E,0X00,0X18,0X18,0XCC,0X7F,0X00,0X38,0X18,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X0C,0X0C,0XCE,0X1F,0X86,0X18,0X60,0XC0,0X60,0X30,0X03,0X9B,0X80,0X03,
0X80,0X0E,0X1C,0XC7,0XC3,0X00,0X60,0X1F,0X80,0X0F,0XFF,0XF8,0X18,0XCC,0X3C,0X00,
0X70,0X1C,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X1C,0X1C,0XC7,0X08,0X0E,0X18,
0X60,0XC0,0X60,0X30,0X07,0X18,0XE0,0X03,0X80,0X0C,0X08,0XC0,0X03,0X00,0X60,0X0F,
0X00,0X0F,0XFF,0XF8,0X1F,0XCC,0X1C,0X00,0X60,0X0E,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X1C,0X78,0XC3,0X80,0X0C,0X18,0X60,0XC0,0X67,0XF0,0X1E,0X18,0X78,0X07,
0X00,0X0C,0X00,0XC0,0X03,0X00,0X60,0X0F,0X00,0X0E,0X00,0X18,0X1F,0X8C,0X4E,0X00,
0XC0,0X07,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X18,0XF0,0XC1,0XE0,0X1C,0X18,
0X60,0XC0,0X67,0XF0,0X3C,0X18,0X3E,0X0E,0X00,0X0C,0X00,0XC0,0X03,0X00,0X60,0X3F,
0XC0,0X0E,0X00,0X18,0X18,0X0D,0XCF,0X03,0XE3,0XFF,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X18,0XC0,0XC0,0XC0,0X38,0X18,0X60,0XC0,0X60,0X00,0XF0,0X18,0X0F,0X9C,
0X0E,0X3C,0X00,0XC0,0X03,0X00,0X60,0XF9,0XF0,0X0E,0X00,0X18,0X18,0X0F,0XC3,0XC3,
0XFF,0XFF,0X80,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X38,0X00,0XC0,0X00,0X70,0X00,
0X60,0X00,0X60,0X00,0XE0,0X18,0X07,0X7C,0X0F,0XF8,0X00,0XC0,0X03,0X03,0XE3,0XE0,
0X7E,0X0E,0X03,0XF8,0X18,0X1F,0X01,0XC3,0XFC,0X01,0XC0,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X30,0X00,0XC0,0X00,0X30,0X00,0X60,0X00,0X60,0X00,0X00,0X18,0X00,0X30,
0X07,0XF0,0X00,0XC0,0X03,0X03,0XC7,0X80,0X1C,0X0E,0X03,0XF0,0X18,0X0C,0X00,0X00,
0X00,0X01,0XC0,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X02,0X00,
0X00,0X00,0X00,0X00,0X10,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X3F,0X00,0X00,0X07,
0XFF,0XC1,0XFF,0X00,0X03,0XFE,0X01,0XFF,0XC0,0X01,0XFF,0X80,0X01,0XFF,0X00,0X07,
0XFE,0X00,0X00,0X1F,0X00,0X0F,0XF8,0X00,0X03,0XFF,0XE0,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X3F,0X00,0X00,0X1F,0XFF,0XC1,0XFF,0X80,0X03,0XFE,0X03,0XFF,
0XF0,0X07,0XFF,0XE0,0X03,0XFF,0XC0,0X1F,0XFF,0X80,0X00,0X3F,0X00,0X1F,0XFE,0X00,
0X0F,0XFF,0XE0,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X3F,0X00,0X00,0X3F,
0XFF,0XC1,0XFF,0X80,0X07,0XFE,0X03,0XFF,0XF0,0X0F,0XFF,0XF0,0X07,0XFF,0XE0,0X3F,
0XFF,0XC0,0X00,0X3F,0X00,0X3F,0XFF,0X00,0X1F,0XFF,0XE0,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X3F,0X00,0X00,0X7F,0XFF,0XC1,0XFF,0X80,0X07,0XFE,0X03,0XFF,
0XF8,0X0F,0XFF,0XF8,0X0F,0XFF,0XE0,0X3F,0XFF,0XE0,0X00,0X7F,0X00,0X7F,0XFF,0X00,
0X3F,0XFF,0XE0,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X3F,0X00,0X00,0XFF,
0XFF,0XC1,0XFF,0XC0,0X07,0XFE,0X03,0X83,0XF8,0X0F,0X87,0XF8,0X1F,0XC7,0XF0,0X3E,
0X1F,0XE0,0X00,0XFF,0X00,0XFE,0X3F,0X80,0X7F,0XFF,0XE0,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X3F,0X00,0X01,0XFF,0X00,0XC1,0XFF,0XC0,0X0F,0XFE,0X02,0X01,
0XFC,0X0E,0X03,0XF8,0X1F,0X83,0XF0,0X38,0X0F,0XE0,0X00,0XFF,0X00,0XFC,0X1F,0X80,
0XFF,0X80,0X60,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X3F,0X00,0X03,0XFC,
0X00,0X01,0XFF,0XC0,0X0F,0XFE,0X00,0X01,0XFC,0X08,0X01,0XFC,0X3F,0X83,0XF8,0X20,
0X07,0XF0,0X01,0XFF,0X01,0XFC,0X1F,0XC1,0XFE,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X3F,0X00,0X03,0XF8,0X00,0X01,0XFF,0XE0,0X0F,0XFE,0X00,0X00,
0XFC,0X00,0X01,0XFC,0X3F,0X01,0XF8,0X00,0X07,0XF0,0X01,0XFF,0X01,0XF8,0X0F,0XC1,
0XFC,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X3F,0X00,0X07,0XF0,
0X00,0X01,0XFF,0XE0,0X1F,0XFE,0X00,0X00,0XFC,0X00,0X01,0XFC,0X3F,0X01,0XF8,0X00,
0X07,0XF0,0X03,0XFF,0X01,0XF8,0X0F,0XC3,0XF8,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X3F,0X00,0X07,0XF0,0X00,0X01,0XFB,0XE0,0X1F,0X7E,0X00,0X01,
0XF8,0X00,0X01,0XF8,0X3F,0X01,0XF8,0X00,0X07,0XE0,0X07,0XDF,0X01,0XF8,0X0F,0XC3,
0XF8,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X3F,0X00,0X07,0XE0,
0X00,0X01,0XFB,0XF0,0X3F,0X7E,0X00,0X03,0XF8,0X00,0X01,0XF8,0X3F,0X01,0XF8,0X00,
0X07,0XE0,0X0F,0X9F,0X01,0XF8,0X0F,0XC3,0XF0,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X3F,0X00,0X07,0XE0,0X00,0X01,0XFB,0XF0,0X3E,0X7E,0X00,0XFF,
0XF0,0X00,0X03,0XF8,0X7E,0X01,0XF8,0X00,0X0F,0XE0,0X0F,0X9F,0X03,0XF0,0X0F,0XC3,
0XF0,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X3F,0X00,0X07,0XE0,
0X00,0X01,0XF9,0XF0,0X3E,0X7E,0X00,0XFF,0XE0,0X00,0X07,0XF0,0X7E,0X01,0XFC,0X00,
0X1F,0XC0,0X1F,0X1F,0X03,0XF0,0X0F,0XE3,0XF0,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X3F,0X00,0X0F,0XE0,0X00,0X01,0XF9,0XF8,0X7C,0X7E,0X00,0XFF,
0X80,0X00,0X0F,0XF0,0X7E,0X01,0XFC,0X00,0X3F,0XC0,0X3E,0X1F,0X03,0XF0,0X0F,0XE7,
0XF0,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X3F,0X00,0X0F,0XE0,
0X00,0X01,0XF9,0XF8,0X7C,0X7E,0X00,0XFF,0XF0,0X00,0X1F,0XE0,0X7E,0X01,0XFC,0X00,
0X7F,0X80,0X7C,0X1F,0X03,0XF0,0X0F,0XE7,0XF0,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X3F,0X00,0X0F,0XE0,0X00,0X01,0XF8,0XFC,0X7C,0X7E,0X00,0XFF,
0XF8,0X00,0X7F,0XC0,0X7E,0X01,0XFC,0X01,0XFF,0X00,0XFC,0X1F,0X03,0XF0,0X0F,0XE7,
0XF0,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X3F,0X00,0X0F,0XE0,
0X00,0X01,0XF8,0XFC,0XF8,0X7E,0X00,0X03,0XFC,0X00,0XFF,0X80,0X7E,0X01,0XF8,0X03,
0XFE,0X00,0XF8,0X1F,0X03,0XF0,0X0F,0XC7,0XF0,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X3F,0X00,0X07,0XE0,0X00,0X01,0XF8,0X7C,0XF8,0X7E,0X00,0X01,
0XFC,0X01,0XFF,0X00,0X7F,0X01,0XF8,0X07,0XFC,0X01,0XF0,0X1F,0X83,0XF8,0X0F,0XC3,
0XF0,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X3F,0X00,0X07,0XF0,
0X00,0X01,0XF8,0X7E,0XF8,0X7E,0X00,0X00,0XFC,0X03,0XFC,0X00,0X3F,0X01,0XF8,0X0F,
0XF0,0X01,0XFF,0XFF,0XF1,0XF8,0X0F,0XC3,0XF8,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X3F,0X00,0X07,0XF0,0X00,0X01,0XF8,0X7F,0XF0,0X7E,0X00,0X00,
0XFE,0X07,0XF8,0X00,0X3F,0X01,0XF8,0X1F,0XE0,0X01,0XFF,0XFF,0XF1,0XF8,0X0F,0XC3,
0XF8,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X3F,0X00,0X07,0XF8,
0X00,0X01,0XF8,0X3F,0XF0,0X7E,0X00,0X00,0XFE,0X0F,0XF0,0X00,0X3F,0X01,0XF8,0X3F,
0XC0,0X01,0XFF,0XFF,0XF1,0XF8,0X0F,0XC3,0XFC,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X3F,0X00,0X03,0XFC,0X00,0X01,0XF8,0X3F,0XF0,0X7E,0X00,0X00,
0XFC,0X0F,0XE0,0X00,0X3F,0X03,0XF0,0X3F,0X80,0X01,0XFF,0XFF,0XF1,0XF8,0X1F,0X81,
0XFE,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X3F,0X00,0X03,0XFF,
0X00,0XC1,0XF8,0X3F,0XE0,0X7E,0X04,0X01,0XFC,0X1F,0XC0,0X00,0X1F,0X83,0XF0,0X7F,
0X00,0X00,0X00,0X1F,0X00,0XFC,0X1F,0X81,0XFF,0X80,0X60,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X3F,0XFF,0XE1,0XFF,0XFF,0XC1,0XF8,0X1F,0XE0,0X7E,0X07,0X03,
0XFC,0X1F,0XFF,0XFC,0X1F,0XC7,0XF0,0X7F,0XFF,0XF0,0X00,0X1F,0X00,0XFE,0X3F,0X80,
0XFF,0XFF,0XE0,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X3F,0XFF,0XE0,0XFF,
0XFF,0XC1,0XF8,0X1F,0XE0,0X7E,0X07,0XFF,0XF8,0X1F,0XFF,0XFC,0X1F,0XFF,0XE0,0X7F,
0XFF,0XF0,0X00,0X1F,0X00,0XFF,0XFF,0X00,0X7F,0XFF,0XE0,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X3F,0XFF,0XE0,0X7F,0XFF,0XC1,0XF8,0X1F,0XC0,0X7E,0X07,0XFF,
0XF8,0X1F,0XFF,0XFC,0X0F,0XFF,0XC0,0X7F,0XFF,0XF0,0X00,0X1F,0X00,0X7F,0XFE,0X00,
0X3F,0XFF,0XE0,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X3F,0XFF,0XE0,0X3F,
0XFF,0XC1,0XF8,0X0F,0XC0,0X7E,0X07,0XFF,0XE0,0X1F,0XFF,0XFC,0X07,0XFF,0X80,0X7F,
0XFF,0XF0,0X00,0X1F,0X00,0X3F,0XFC,0X00,0X1F,0XFF,0XE0,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X3F,0XFF,0XE0,0X0F,0XFF,0X81,0XF8,0X0F,0X80,0X7E,0X03,0XFF,
0XC0,0X1F,0XFF,0XFC,0X03,0XFF,0X00,0X7F,0XFF,0XF0,0X00,0X1F,0X00,0X1F,0XF8,0X00,
0X07,0XFF,0XC0,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X60,0X00,0X00,0X00,0X00,0X60,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X70,0X00,0X00,
0X00,0X00,0X00,0X60,0X00,0X00,0X00,0X70,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0XFC,0X60,0X00,0X00,0X00,0XFE,0X60,0X00,0X00,0X00,0X00,0X1E,0X00,0X00,0XC0,
0X00,0X00,0X00,0X00,0X00,0XF0,0X00,0X1F,0XE0,0X00,0X00,0X60,0X00,0X00,0X00,0XF0,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X8C,0X20,0X00,0X00,0X01,0X84,0X20,
0X00,0X00,0X00,0X00,0X33,0X00,0X00,0X40,0X00,0X00,0X02,0X00,0X00,0X30,0X00,0X13,
0X20,0X00,0X00,0X20,0X00,0X00,0X00,0X30,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC1,0X04,0X20,0X00,0X00,0X01,0X04,0X20,0X00,0X00,0X00,0X00,0X61,0X80,0X00,0X00,
0X00,0X00,0X02,0X00,0X00,0X30,0X00,0X13,0X20,0X00,0X00,0X20,0X00,0X00,0X00,0X30,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC1,0X00,0X20,0X00,0X00,0X00,0X08,0X20,
0X00,0X00,0X00,0X00,0X60,0X80,0X00,0X00,0X00,0X00,0X06,0X00,0X00,0X30,0X00,0X03,
0X00,0X00,0X00,0X20,0X00,0X00,0X00,0X30,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC1,0X80,0X2F,0X07,0X86,0XF0,0X08,0X2F,0X07,0X86,0XF0,0X00,0X40,0X9C,0XE3,0XC0,
0X78,0X6F,0X1F,0XC3,0XE0,0X30,0X00,0X03,0X01,0XE0,0X78,0X2F,0X1B,0XC1,0XE0,0X30,
0X1E,0X07,0XE7,0X38,0X00,0X00,0X00,0X03,0XC0,0XE0,0X31,0X0C,0XC7,0X10,0X10,0X31,
0X0C,0XC7,0X10,0X00,0X40,0X8D,0X20,0XC0,0XCC,0X71,0X02,0X06,0X30,0X30,0X00,0X03,
0X03,0X30,0XCC,0X31,0X1C,0X43,0X30,0X30,0X33,0X0C,0XC2,0X10,0X00,0X00,0X00,0X03,
0XC0,0X38,0X21,0X08,0X62,0X10,0X20,0X21,0X08,0X62,0X10,0X00,0X40,0XC6,0X00,0XC0,
0X86,0X21,0X02,0X06,0X10,0X30,0X00,0X03,0X02,0X18,0X84,0X21,0X08,0X46,0X18,0X30,
0X61,0X88,0X41,0X10,0X00,0X00,0X00,0X03,0XC0,0X0C,0X21,0X18,0X62,0X10,0X20,0X21,
0X18,0X62,0X10,0X00,0X40,0X84,0X00,0XC1,0X86,0X21,0X02,0X00,0XF0,0X30,0X00,0X03,
0X06,0X19,0X80,0X21,0X08,0X44,0X08,0X30,0X40,0X88,0X41,0X20,0X00,0X00,0X00,0X03,
0XC0,0X06,0X21,0X1F,0XE2,0X10,0X40,0X21,0X1F,0XE2,0X10,0X00,0X40,0X84,0X00,0XC1,
0XFE,0X21,0X02,0X03,0X90,0X30,0X00,0X03,0X07,0XF9,0X00,0X21,0X08,0X44,0X08,0X30,
0X40,0X8C,0XC1,0X20,0X00,0X00,0X00,0X03,0XC1,0X02,0X21,0X18,0X02,0X10,0X40,0X21,
0X18,0X02,0X10,0X00,0X60,0X84,0X00,0XC1,0X80,0X21,0X02,0X06,0X10,0X30,0X00,0X03,
0X06,0X01,0X80,0X21,0X08,0X44,0X08,0X30,0X40,0X8F,0X80,0X80,0X00,0X00,0X00,0X03,
0XC1,0X02,0X21,0X08,0X02,0X10,0X82,0X21,0X08,0X02,0X10,0X00,0X21,0X84,0X00,0XC0,
0X80,0X21,0X02,0X04,0X10,0X30,0X00,0X03,0X02,0X01,0X82,0X21,0X08,0X46,0X18,0X30,
0X61,0X88,0X00,0XC0,0X00,0X00,0X00,0X03,0XC1,0X84,0X21,0X0C,0X42,0X11,0X86,0X21,
0X0C,0X42,0X10,0X00,0X33,0X04,0X00,0XC0,0XC4,0X21,0X02,0X46,0X34,0X30,0X00,0X03,
0X03,0X10,0XC4,0X21,0X08,0X43,0X30,0X30,0X33,0X0F,0XC0,0X40,0X00,0X00,0X00,0X03,
0XC1,0XF8,0X73,0X87,0X87,0X39,0XFC,0X73,0X87,0X87,0X38,0X00,0X1E,0X1F,0X03,0XF0,
0X78,0X73,0X83,0XC3,0XD8,0XFC,0X00,0X07,0X81,0XE0,0X78,0X73,0X9C,0XE1,0XE0,0XFC,
0X1E,0X10,0X20,0X80,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X18,0X60,0X80,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X0F,0XC7,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X70,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0XE0,
0X00,0X78,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XF8,
0X00,0X00,0X00,0X00,0X00,0X00,0X01,0XE0,0X00,0X78,0X00,0X00,0X03,0XC0,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XF8,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0XE0,
0X00,0X78,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X78,
0X00,0X00,0X00,0X00,0XF0,0X00,0X01,0XE0,0X00,0X78,0X00,0X00,0X03,0XC0,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XF0,0X00,0X01,0XE0,
0X00,0X78,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0XF0,0X00,0X01,0XE0,0X00,0X78,0X00,0X00,0X03,0XC0,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X01,0XE0,0XF0,0XFF,0X07,0X87,
0XF8,0X3C,0X3C,0X00,0X3E,0X03,0XCC,0X78,0X1F,0X01,0XE7,0X83,0XFE,0X1F,0X81,0XE0,
0X00,0X78,0X0F,0X80,0XF3,0XC0,0X00,0X7C,0X03,0XE0,0X3C,0XF0,0X78,0X00,0X00,0X03,
0XC0,0X00,0X01,0XF0,0XF0,0XFF,0X87,0X87,0XFC,0X3C,0X3C,0X01,0XFF,0X83,0XDC,0X78,
0X7F,0XC1,0XEF,0XE7,0XFE,0X7F,0XC1,0XE0,0X00,0X78,0X3F,0XC3,0XFF,0XC0,0X01,0XFE,
0X1F,0XF8,0X3D,0XF9,0XFC,0X00,0X00,0X03,0XC0,0X00,0X00,0XF0,0XF0,0XF7,0X87,0X87,
0XBC,0X3C,0X3C,0X03,0XFF,0XC3,0XFC,0X78,0X7F,0XE1,0XFF,0XE7,0XFE,0X7F,0XE1,0XE0,
0X00,0X78,0X7F,0XC7,0XFF,0XC0,0X03,0XFE,0X3F,0XFC,0X3F,0XFF,0XFE,0X00,0X00,0X03,
0XC0,0X00,0X00,0XF1,0XF8,0XE7,0X8F,0XC7,0X3C,0X7E,0X38,0X03,0XE7,0XE3,0XFC,0X78,
0XF1,0XE1,0XF3,0XE1,0XF0,0X61,0XF1,0XE0,0X00,0X78,0XFC,0XC7,0XCF,0XC0,0X07,0XE6,
0X3E,0X7E,0X3E,0X7F,0X3E,0X00,0X00,0X03,0XC0,0X00,0X00,0XF1,0XF9,0XE7,0X8F,0XCF,
0X3C,0X7E,0X78,0X07,0XC1,0XE3,0XE0,0X79,0XE0,0XF1,0XE1,0XF0,0XF0,0X00,0XF1,0XE0,
0X00,0X79,0XF0,0X0F,0X87,0XC0,0X0F,0X80,0X7C,0X1E,0X3C,0X3E,0X1E,0X00,0X00,0X03,
0XC0,0X00,0X00,0X71,0XF9,0XE3,0X8F,0XCF,0X1C,0X7E,0X78,0X07,0X81,0XE3,0XC0,0X79,
0XE0,0XF1,0XE0,0XF0,0XF0,0X03,0XF1,0XE3,0XF8,0X79,0XE0,0X0F,0X03,0XC0,0X0F,0X00,
0X78,0X1E,0X3C,0X3E,0X1E,0X00,0X00,0X03,0XC0,0X00,0X00,0X7B,0XB9,0XC3,0XDD,0XCE,
0X1E,0XEE,0X70,0X07,0X81,0XF3,0XC0,0X79,0XFF,0XF1,0XE0,0XF0,0XF0,0X3F,0XF1,0XE3,
0XF8,0X79,0XE0,0X0F,0X03,0XC0,0X0F,0X00,0X78,0X1F,0X3C,0X3C,0X1E,0X00,0X00,0X03,
0XC0,0X00,0X00,0X7B,0XBD,0XC3,0XDD,0XEE,0X1E,0XEF,0X70,0X07,0X81,0XF3,0XC0,0X79,
0XFF,0XF1,0XE0,0XF0,0XF0,0X7F,0XF1,0XE3,0XF8,0X79,0XE0,0X0F,0X03,0XC0,0X0F,0X00,
0X78,0X1F,0X3C,0X3C,0X1E,0X00,0X00,0X03,0XC0,0X00,0X00,0X7B,0X9F,0XC3,0XDC,0XFE,
0X1E,0XE7,0XF0,0X07,0X81,0XE3,0XC0,0X79,0XF0,0X01,0XE0,0XF0,0XF0,0XF0,0XF1,0XE0,
0X00,0X79,0XE0,0X0F,0X03,0XC0,0X0F,0X00,0X78,0X1E,0X3C,0X3C,0X1E,0X00,0X00,0X03,
0XC0,0X00,0X00,0X3F,0X1F,0XC1,0XF8,0XFE,0X0F,0XC7,0XF0,0X07,0X81,0XE3,0XC0,0X79,
0XE0,0X01,0XE0,0XF0,0XF0,0XF0,0XF1,0XE0,0X00,0X79,0XF0,0X0F,0X07,0XC0,0X0F,0X80,
0X78,0X1E,0X3C,0X3C,0X1E,0X00,0X00,0X03,0XC0,0X00,0X00,0X3F,0X1F,0X81,0XF8,0XFC,
0X0F,0XC7,0XE1,0XC7,0XC3,0XE3,0XC0,0X79,0XF0,0X01,0XE0,0XF0,0XF0,0XF1,0XF1,0XE0,
0X00,0X79,0XF8,0X0F,0X87,0XC3,0X8F,0XC0,0X7C,0X3E,0X3C,0X3C,0X1E,0X00,0X00,0X03,
0XC0,0X00,0X00,0X3F,0X1F,0X81,0XF8,0XFC,0X0F,0XC7,0XE3,0XE3,0XFF,0XC3,0XC0,0X78,
0XFF,0XE1,0XE0,0XF0,0XFE,0XFF,0XF1,0XE0,0X00,0X78,0XFF,0XC7,0XFF,0XC7,0XC7,0XFE,
0X3F,0XFC,0X3C,0X3C,0X1E,0X00,0X00,0X03,0XC0,0X00,0X00,0X1F,0X0F,0X80,0XF8,0X7C,
0X07,0XC3,0XE3,0XE1,0XFF,0X83,0XC0,0X78,0X7F,0XE1,0XE0,0XF0,0XFE,0XFF,0XF1,0XE0,
0X00,0X78,0X7F,0XC7,0XFF,0XC7,0XC3,0XFE,0X1F,0XF8,0X3C,0X3C,0X1E,0X00,0X00,0X03,
0XC0,0X00,0X00,0X1E,0X0F,0X00,0XF0,0X78,0X07,0X83,0XC1,0XE0,0XFF,0X03,0XC0,0X78,
0X3F,0XC1,0XE0,0XF0,0X7E,0X7E,0XF1,0XE0,0X00,0X78,0X3F,0XC3,0XFB,0XC3,0XC1,0XFE,
0X0F,0XF0,0X3C,0X3C,0X1E,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X0F,0X80,0X1E,0X00,0X78,0X00,0X00,0X3C,0X00,0X00,0X03,0XC0,0X00,0X00,
0X00,0X01,0XF0,0X00,0X00,0X03,0XF0,0X00,0X00,0X00,0X00,0X00,0X00,0XF0,0X03,0XC0,
0X1F,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X3F,0XE0,0XFF,0X81,0XFE,0X03,
0X81,0XFF,0X00,0X1E,0X0F,0XF0,0X00,0XFF,0XE0,0X07,0XFC,0X1F,0XFF,0X8F,0XF8,0X03,
0XCF,0XFE,0X00,0XF8,0X03,0XFC,0X0F,0XF0,0X7F,0XC0,0XFF,0X80,0X00,0X00,0X00,0X03,
0XC0,0X00,0X3F,0XF0,0XFF,0XC1,0XFF,0X03,0X81,0XFF,0X80,0X3E,0X0F,0XF8,0X00,0XFF,
0XF8,0X0F,0XFE,0X1F,0XFF,0X9F,0XF8,0X03,0X8F,0XFF,0X00,0XFC,0X07,0XFE,0X1F,0XF8,
0X7F,0XE0,0XFF,0X80,0X00,0X00,0X00,0X03,0XC0,0X00,0X31,0XF0,0XE7,0XE3,0XCF,0X1F,
0XB1,0XCF,0XC0,0X7E,0X1E,0X78,0X00,0XFF,0XFC,0X1F,0XFF,0X1F,0XFF,0XBF,0X38,0X03,
0X8F,0X3F,0X01,0XFC,0X07,0X9F,0X1E,0X7C,0X63,0XE0,0XFF,0X80,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0XF0,0X81,0XE3,0X87,0X9F,0XF1,0X03,0XC0,0X7E,0X1C,0X3C,0X00,0XF0,
0X7C,0X3E,0X0F,0X80,0XF0,0X3C,0X00,0X07,0X8F,0X0F,0X01,0XFC,0X07,0X0F,0X1C,0X3C,
0X01,0XE0,0XE0,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0XF0,0X01,0XE7,0X87,0X87,
0XE0,0X03,0XC0,0XFE,0X3C,0X3C,0X00,0XF0,0X3E,0X3C,0X07,0X80,0XF0,0X3C,0X00,0X07,
0X0F,0X0F,0X01,0XDE,0X07,0X0F,0X1C,0X3C,0X01,0XE1,0XE0,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0XF0,0X01,0XE7,0X87,0X87,0XC0,0X03,0XC1,0XFE,0X3C,0X3C,0X00,0XF0,
0X1E,0X78,0X07,0XC0,0XF0,0X3E,0X00,0X0F,0X0F,0X0F,0X03,0XDE,0X07,0X0E,0X1C,0X38,
0X01,0XE1,0XE0,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X03,0XE0,0X03,0XC7,0X83,0X8E,
0XE0,0X07,0X81,0XDE,0X3C,0X1C,0X00,0XF0,0X1E,0X78,0X03,0XC0,0XF0,0X1F,0X80,0X0E,
0X0F,0X0F,0X03,0XCE,0X07,0XFE,0X1F,0XF8,0X07,0XC1,0XFE,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X1F,0XC0,0X07,0XC7,0X83,0X8E,0X60,0X0F,0X83,0X9E,0X3C,0X1C,0X00,0XF0,
0X1E,0X78,0X03,0XC0,0XF0,0X1F,0XE0,0X1E,0X0F,0XFE,0X03,0X8F,0X03,0XFC,0X0F,0XF0,
0X3F,0X81,0XFF,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X1F,0XE0,0X0F,0X87,0X83,0X80,
0X00,0X1F,0X07,0X1E,0X3C,0X1C,0X00,0XF0,0X1E,0X78,0X03,0XC0,0XF0,0X0F,0XF0,0X1E,
0X0F,0XFC,0X07,0X8F,0X03,0XFE,0X0F,0XF8,0X3F,0XC1,0XFF,0X80,0X00,0X00,0X00,0X03,
0XC0,0X00,0X07,0XF0,0X1F,0X07,0X83,0X80,0X00,0X3E,0X0F,0X1E,0X3C,0X1C,0X00,0XF0,
0X1E,0X78,0X03,0XC0,0XF0,0X03,0XF8,0X1C,0X0F,0XF8,0X07,0X87,0X07,0XFE,0X1F,0XF8,
0X0F,0XE0,0X07,0XC0,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0XF8,0X3E,0X07,0X87,0X80,
0X00,0X7C,0X1F,0X3E,0X3C,0X3C,0X00,0XF0,0X1E,0X78,0X03,0XC0,0XF0,0X00,0XFC,0X3C,
0X0F,0X7C,0X0F,0XFF,0X8F,0X0F,0X3C,0X3C,0X01,0XF0,0X03,0XC0,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X78,0X7C,0X07,0X87,0X80,0X00,0XF8,0X1F,0XFF,0XBC,0X3C,0X00,0XF0,
0X3E,0X7C,0X07,0XC0,0XF0,0X00,0X3C,0X38,0X0F,0X1E,0X0F,0XFF,0X8F,0X07,0X3C,0X1C,
0X00,0XF0,0X03,0XC0,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X78,0XF0,0X07,0X87,0X80,
0X01,0XE0,0X1F,0XFF,0XBC,0X3C,0X00,0XF0,0X3C,0X3C,0X07,0X80,0XF0,0X00,0X3C,0X78,
0X0F,0X1F,0X0F,0XFF,0XCF,0X07,0X3C,0X1C,0X00,0XF0,0X03,0XC0,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0XF8,0XF0,0X07,0XC7,0X80,0X01,0XE0,0X00,0X1E,0X3E,0X3C,0X00,0XF0,
0XFC,0X3E,0X1F,0X80,0XF0,0X30,0X7C,0X70,0X0F,0X0F,0X1E,0X03,0XCF,0X0F,0X3C,0X3C,
0X01,0XF0,0X07,0X80,0X00,0X00,0X00,0X03,0XC0,0X00,0X3F,0XF1,0XFF,0XE3,0XFF,0X00,
0X03,0XFF,0XC0,0X1E,0X1F,0XF8,0X00,0XFF,0XF8,0X1F,0XFF,0X00,0XF0,0X3F,0XF8,0X70,
0X0F,0X07,0X9E,0X03,0XCF,0XDF,0X3F,0X7C,0X7F,0XE1,0XFF,0X80,0X00,0X00,0X00,0X03,
0XC0,0X00,0X3F,0XF1,0XFF,0XE1,0XFE,0X00,0X03,0XFF,0XC0,0X1E,0X0F,0XF0,0X00,0XFF,
0XF0,0X0F,0XFE,0X00,0XF0,0X3F,0XF8,0XF0,0X0F,0X07,0X9E,0X03,0XE7,0XFE,0X1F,0XF8,
0X7F,0XE1,0XFF,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X3F,0XC1,0XFF,0XE0,0XFC,0X00,
0X03,0XFF,0XC0,0X1E,0X07,0XE0,0X00,0XFF,0XC0,0X07,0XF8,0X00,0XF0,0X3F,0XF0,0XE0,
0X0F,0X03,0XFC,0X01,0XE3,0XFC,0X0F,0XF0,0X7F,0X81,0XFE,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X01,0XE0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0XC0,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X01,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
};


unsigned char code tab2[]={
/*--  320x240  --*/
0XFF,0XFA,0X7F,0XF9,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XEF,0XFF,0XFF,0XEF,0XFF,
0XFF,0XF2,0X0C,0X00,0X00,0X00,0X07,0X00,0XB1,0X6F,0XCE,0X7F,0XCE,0XDB,0X7E,0XF7,
0XDF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF7,0X8F,0X97,0XFB,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XC3,0XC8,0X00,0X00,0X00,0X00,0X00,
0X66,0X00,0XFC,0XBD,0X98,0X10,0X00,0X00,0X00,0X3F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFE,0X7E,0XFB,0XB7,0XFD,0XBF,0XFF,0XFF,0XFF,0XCF,0X7F,0XFE,0XFF,0XFF,0XFF,0XEF,
0XFF,0XF8,0X20,0X44,0X00,0X00,0X00,0X00,0XC0,0X00,0X1C,0X00,0X00,0X00,0X00,0X02,
0X00,0X9F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0X1C,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFE,0X7F,0XFF,0XFF,0XFF,0XBF,0XA0,0X04,0XC0,0X00,0X00,0X00,0X00,
0X00,0X03,0XFF,0X00,0X90,0X00,0X00,0X00,0X00,0X11,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFE,0XFF,0XFF,0XBF,0XBF,0X77,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XEF,0XFF,0XFF,
0XFF,0X80,0X00,0X00,0X00,0X00,0X00,0X20,0X00,0X0F,0XEF,0X00,0X10,0X00,0X00,0X00,
0X00,0X01,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XEF,0XBF,0XDE,0X37,0XFF,0XFF,
0XFF,0XE7,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF0,0X00,0XB0,0X00,0X00,0XBC,0X01,
0XCF,0XCF,0XE7,0X00,0X50,0X00,0X00,0X00,0X00,0X01,0XFF,0XFF,0XF7,0XFF,0XFF,0XFF,
0XFF,0XFD,0XFF,0XFF,0XBF,0XE7,0XFF,0XFF,0XFF,0XE7,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XF8,0X00,0X00,0X00,0X1D,0X01,0X3F,0XFC,0X96,0X05,0X00,0X10,0X00,0X00,0X00,
0X00,0X00,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF7,0XFF,0XFF,0XDF,0XFF,0XFF,0XFF,0XFF,
0XFF,0X47,0XFF,0XFC,0XFF,0XFF,0XFF,0XFF,0XFF,0XC0,0X00,0X00,0X2F,0XFC,0X7F,0X3F,
0XF8,0X02,0X82,0X00,0X10,0X00,0X00,0X00,0X00,0X05,0X5F,0XFF,0XFF,0XFF,0XFF,0XFF,
0XBF,0XFF,0XFE,0XFF,0XFF,0XFF,0XFF,0XFE,0XDE,0X0F,0XFF,0XFF,0XFF,0XFF,0XFF,0X7F,
0XFF,0XC0,0X00,0X0F,0XC0,0XD3,0XDC,0X1F,0XC8,0X00,0X03,0X02,0X90,0X00,0X00,0X00,
0X00,0X00,0XBE,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X7F,0XFF,0XDB,0X7B,0XFF,0XFF,
0XFD,0XAF,0XFD,0XDC,0X7F,0XFF,0XFF,0XFF,0XE7,0XE0,0X03,0XFF,0X8F,0XF8,0XF8,0X67,
0XC0,0X00,0X02,0X00,0X00,0X00,0X00,0X00,0X00,0X06,0X0F,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XDF,0X7F,0XFF,0XFF,0XFF,0XD7,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0X7C,0X7F,0XFF,0XFD,0XB8,0X30,0X07,0XC0,0X00,0X04,0X04,0X00,0X00,0X00,0X00,
0X00,0X80,0X2B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFA,0XFF,0XFB,0XFF,0XEF,0XFF,0XFF,
0XFF,0X77,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X7F,0XFF,0XFE,0XF9,0XB8,0X10,0X07,
0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X11,0XD7,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFC,0XCF,0XFF,0XF7,0XFF,0XFF,0XFF,0XFF,0X9F,0XFB,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0X73,0X38,0X10,0X07,0X80,0X00,0X07,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X0F,0X7F,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFC,0XFF,0XFF,0XFB,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XE7,0X30,0X00,0X17,
0X80,0X00,0X0F,0X20,0X20,0X00,0X00,0X00,0X38,0X00,0X07,0X7F,0XFF,0XFF,0XFF,0XFF,
0XFF,0XF0,0X1C,0XFF,0XEF,0XBF,0XFF,0XFF,0XFF,0XFD,0XFF,0XDF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFC,0XE8,0XB8,0X00,0X03,0XC0,0X00,0X0D,0X00,0X00,0X00,0X00,0X00,
0X40,0X00,0X4F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFA,0XBD,0XFF,0X7F,0XFF,0XFF,0XFF,
0XFF,0XFD,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFD,0X60,0XB0,0X20,0X07,
0X80,0X00,0X0A,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X04,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XF9,0XFF,0XFE,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFD,0X11,0X30,0X00,0X03,0X80,0X10,0X0C,0X00,0X00,0X00,0X00,0X00,
0X00,0X18,0X0B,0XDF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XEF,0X7F,0XFF,0XFF,0XFB,
0XFF,0XFF,0XFD,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFC,0X08,0X30,0X00,0X05,
0X80,0X00,0X0B,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X02,0X77,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XEF,0XFE,0X08,0X01,0XC0,0X01,0X00,0X00,0X06,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X7B,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XDF,0XFF,0XDF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF7,0XFC,0X00,0X00,0X00,0X03,
0X00,0X00,0X00,0X00,0X40,0X00,0X00,0X00,0X00,0X04,0X0E,0X3F,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFE,0XEF,0XFF,0XFF,0XFF,0XFF,0X37,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XC7,0XFC,0X0D,0X00,0X00,0X01,0X40,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X10,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XF7,0XFF,0XFF,0XFF,
0XFF,0XDF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XA7,0XFC,0X02,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X40,0X00,0X00,0X00,0X00,0X00,0X05,0XF3,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XB9,0XFF,0XFF,0XFF,0X7F,0XEF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFD,0X00,0X00,0X00,0X00,0X00,0X00,0X04,0X01,0X00,0X00,0X00,0X00,
0X00,0X00,0X01,0XEF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X7F,0X7F,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XCF,0XFE,0X80,0X00,0X00,0X04,
0X80,0X00,0X00,0X00,0X40,0X00,0X00,0X00,0X00,0X00,0X05,0XEE,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFC,0XFB,0XF9,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XF7,0XFF,0X40,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X40,0X00,0X00,0X00,
0X00,0X00,0X04,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF7,0XFE,0XFA,0XDF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XDF,0XFC,0X00,0X20,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0X01,0X00,0X7F,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFD,0XFF,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XC7,0XFC,0X00,0X00,0X00,0X00,0X80,0X00,0X00,0X00,0XC0,0X00,0X00,0X00,
0X00,0X01,0X00,0X03,0X3F,0XFF,0XFF,0XDF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF6,0X00,0X00,0X00,0X08,
0X00,0X00,0X00,0X00,0XC0,0X00,0X00,0X00,0X00,0X10,0X20,0X00,0X5F,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XBF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XEB,0XFC,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XC0,0X00,0X00,0X00,
0X00,0X00,0X82,0X00,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF7,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XD3,0XFE,0X00,0X00,0X10,0X00,
0X00,0X00,0X04,0X04,0X80,0X00,0X00,0X00,0X00,0X08,0X80,0X00,0X47,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFB,0XFF,0XF7,0XFF,0XFF,0XFB,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XCF,0XF4,0X01,0X00,0X00,0X06,0X80,0X00,0X00,0X04,0XC0,0X00,0X00,0X00,
0X00,0X00,0X08,0X10,0X37,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XEE,0X7F,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XAF,0XFC,0X40,0X20,0X10,0X00,
0X40,0X00,0X00,0X04,0X80,0X00,0X00,0XC0,0X00,0X08,0X00,0X00,0X0F,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0X9F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XEB,0XF6,0X00,0X20,0X04,0X02,0X00,0X00,0X0C,0X00,0X80,0X00,0X08,0X00,
0X00,0X00,0X00,0X00,0X0B,0XFF,0XFF,0XFF,0XFF,0XFF,0X7F,0XFF,0XEF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XEF,0XFB,0X60,0X00,0X00,0X00,
0X00,0X80,0X00,0X00,0X80,0X00,0X00,0X00,0X40,0X00,0X00,0X80,0X12,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFB,0XFF,0XFF,0XFF,0XFF,0XFF,0XFA,0X00,0X00,0X30,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X40,0X20,0X00,0X06,0XBF,0XFF,0XFF,0XFF,0XFF,0X9F,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XBC,0XEB,0XF8,0X02,0X00,0XFF,0XFF,
0XFF,0X27,0XFF,0X00,0X20,0X00,0X00,0X20,0X00,0X80,0X00,0X00,0X01,0X2F,0XFF,0XFF,
0XFF,0XFD,0X29,0XFF,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XF7,0XB3,0XF0,0X10,0X00,0X40,0X00,0X02,0X44,0X8B,0XFF,0XCC,0XC0,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X4B,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XBF,0X37,0XF8,0X00,0X03,0X80,0X00,
0X00,0X00,0X00,0X3E,0X20,0XC4,0XD6,0X17,0X23,0X00,0X00,0X00,0X0F,0XC1,0XFE,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFD,0XE4,0XF8,0X28,0XC0,0X00,0X00,0X04,0X00,0X00,0X3D,0X0C,0X00,0X00,0X00,
0X04,0XFF,0XBF,0XF0,0X10,0X43,0X19,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X87,0X38,0XF0,0X00,0X00,0X00,0X10,
0X04,0XA0,0X00,0X7C,0X00,0X00,0X80,0X00,0X00,0X00,0X00,0X12,0X71,0X9F,0XFF,0XFF,
0XFF,0XFF,0XFF,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XE3,0X25,0XF8,0X20,0X00,0X00,0X20,0X98,0X40,0X20,0X32,0X80,0X0E,0X62,0X00,
0X00,0X00,0X00,0X00,0X10,0X9E,0X7F,0XFF,0XFF,0XFF,0XFF,0X7F,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFC,0X40,0XB0,0X20,0X02,0X18,0X00,
0X02,0X01,0X00,0X88,0X00,0X06,0X40,0X00,0X00,0X00,0X40,0X0A,0X72,0XB2,0X76,0XFF,
0XFF,0XFF,0XFB,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFD,0X79,0XF0,0X80,0X68,0X80,0X20,0X0A,0X00,0X01,0XE9,0X00,0X81,0XB4,0X10,
0X01,0X10,0X00,0X0C,0X20,0X73,0X27,0XFE,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF2,0XA4,0XB5,0X00,0X82,0X00,0X00,
0X00,0X00,0X00,0XE8,0X00,0X13,0X44,0X00,0X40,0X20,0X00,0X51,0X9E,0XAD,0X77,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFD,0X95,0X64,0X20,0X04,0X00,0X0A,0X00,0X00,0X00,0XF1,0XE2,0XD1,0X80,0X00,
0X00,0X00,0X00,0X10,0X31,0X21,0X4F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF7,0X20,0X68,0X00,0X8C,0X22,0X44,
0XC0,0X00,0X00,0XB4,0X05,0X23,0X98,0X00,0X81,0X00,0X00,0X03,0XC2,0X00,0X75,0X7B,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XBF,
0XFF,0XFB,0X7D,0XE4,0X00,0X01,0X68,0X00,0X01,0X00,0X00,0X80,0X14,0X01,0X82,0X40,
0X00,0X02,0X00,0X00,0X88,0X09,0X25,0XFE,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFD,0X86,0XE4,0X60,0X80,0X01,0XD8,
0X14,0X01,0X01,0XF8,0X00,0X21,0XA1,0X00,0X00,0X00,0X00,0X06,0X10,0XC5,0X7F,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0X22,0XE2,0X84,0X01,0XFF,0XF8,0X29,0X00,0X02,0XA0,0XCA,0X05,0X90,0X00,
0X00,0X00,0X00,0X00,0XAA,0X04,0X86,0XBF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0X03,0XFF,0XFF,0XFF,0XFD,0XEF,0XF5,0X61,0XA2,0X00,0X0F,0XFF,0XFF,
0X00,0X00,0X01,0X09,0X04,0X01,0X20,0X80,0X08,0X0C,0X00,0X01,0X38,0X43,0X93,0X1A,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF3,0XE0,0X7F,0XFF,0XFF,0XFD,
0XFF,0XF6,0X81,0XA4,0X00,0X3F,0XF8,0X07,0X80,0X08,0X00,0XA4,0X60,0X01,0X04,0X00,
0X00,0X30,0X00,0X00,0X61,0XD8,0X6D,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0X81,0XF8,0X3F,0XFF,0XFF,0XFF,0XFF,0XD6,0X4D,0X21,0X00,0X3F,0XE0,0X3F,
0XC0,0X60,0X61,0X20,0X00,0X01,0X05,0X20,0X00,0XC0,0X00,0X01,0XAE,0X06,0X09,0X7B,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XF3,0XFC,0X0F,0XFF,0XFF,0XED,
0XFD,0XF6,0XE1,0X70,0X08,0X7D,0XF0,0X3F,0XC6,0X00,0X00,0XE8,0X00,0X01,0X00,0X00,
0X01,0X30,0XC0,0X00,0X24,0X08,0X15,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFD,0XFF,0XFE,0X0F,0XFF,0XFF,0XFE,0XFE,0XE7,0XF3,0XF2,0X00,0XFF,0X38,0XFF,
0XC9,0X80,0X00,0X90,0X40,0X85,0X02,0X80,0X00,0X47,0X00,0X00,0XD5,0X78,0X53,0X2D,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFB,0XF3,0XFF,0X07,0XFF,0XFF,0XFF,
0X7D,0X37,0X6B,0X62,0X40,0XFF,0XBF,0XF7,0XCB,0X00,0X00,0XE0,0X03,0X02,0X00,0X00,
0X00,0X84,0X00,0X02,0X50,0X10,0X00,0X9C,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XF7,0XF0,0X7F,0X07,0XFF,0XFF,0XFF,0XFF,0XFB,0X60,0XA5,0X04,0XFF,0XFF,0XE1,
0XD0,0X20,0X03,0X00,0X30,0X40,0X00,0X22,0X01,0X08,0X08,0X00,0X51,0X21,0X87,0X03,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XEF,0XE0,0X0F,0X07,0XFF,0XFF,0XFF,
0XF5,0XFF,0XD9,0X64,0X14,0XFF,0XF9,0XD9,0XE4,0X04,0X01,0X00,0X28,0X00,0X02,0X00,
0X00,0X80,0X00,0X00,0X86,0X01,0X45,0X7D,0XFB,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XF8,0XF8,0X00,0X07,0XFF,0XFF,0XF7,0XA7,0XFD,0XD5,0XE0,0X20,0XFF,0XFF,0XFF,
0XC1,0X30,0X02,0X42,0X88,0X40,0X04,0X00,0X00,0X05,0X81,0X00,0XA1,0X00,0X43,0X27,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X00,0X0F,0XFF,0XFF,0XEF,
0XEF,0XFF,0XF9,0X4C,0XC0,0X7F,0XFF,0XFF,0XC8,0X04,0X02,0X80,0X00,0X01,0X00,0X12,
0X00,0XC0,0X00,0X00,0X02,0X00,0X8D,0X33,0XFE,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XC0,0X0F,0XFF,0XFF,0XBD,0XA7,0XFF,0XFD,0X72,0X99,0X7F,0XFF,0XFF,
0XC0,0X00,0X00,0X00,0X08,0X02,0X20,0X00,0X01,0X10,0X40,0X00,0X04,0X02,0X02,0X14,
0XFF,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF9,0XFF,0XF8,0X01,0XFF,0XFF,0XFF,
0XBF,0XFF,0XF3,0X62,0X00,0X3F,0XFF,0XFF,0XC1,0X80,0X03,0X40,0X00,0X00,0X00,0X40,
0X00,0X00,0X00,0X08,0X09,0X40,0X10,0X48,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XF0,0X1F,0X82,0X03,0XFF,0XFE,0X7D,0XFF,0XEF,0X48,0XF0,0X9F,0X8F,0XFF,0XFF,
0XC1,0X04,0X02,0X00,0X00,0X05,0X02,0X00,0X00,0X10,0X00,0X11,0X50,0XA0,0X88,0X08,
0XFF,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF8,0X00,0X01,0XE7,0XFF,0XFE,0XFF,
0X7F,0XFD,0XA5,0X79,0X50,0XA9,0XFF,0XFF,0X81,0X6A,0X02,0X00,0X00,0X02,0X00,0X00,
0X40,0X14,0X00,0X12,0X0F,0X08,0X20,0X40,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XF8,0X00,0X07,0XFF,0XFF,0XFE,0X37,0XBF,0XFE,0XED,0XEA,0X22,0X00,0XFF,0XFF,
0X01,0X01,0X00,0X00,0X40,0X04,0X00,0X00,0X00,0X20,0X00,0X24,0X39,0XD0,0X08,0X09,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF4,0XF8,0X7F,0XFF,0XFF,0XFF,0X27,
0XFF,0XFF,0XCF,0XC7,0XC6,0X13,0XFF,0XFF,0XC0,0X40,0X02,0X41,0X01,0X10,0X01,0X00,
0X00,0X01,0X00,0X20,0XE0,0X00,0X25,0X98,0XFF,0XEF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFB,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X1F,0XDE,0XD6,0X07,0XFF,0XFF,
0XF0,0X10,0X01,0X80,0X00,0X22,0X06,0X00,0X00,0X00,0X04,0X40,0X00,0X00,0X95,0X04,
0XFF,0XBF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFB,
0X5F,0XFF,0XBD,0XFC,0X82,0X1F,0XFF,0XFF,0XFC,0X06,0X23,0X8E,0X06,0X00,0X08,0X00,
0X00,0X00,0X40,0X11,0X05,0X80,0X65,0X10,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XBF,0XFF,0XE7,0XD6,0XBB,0X8F,0XFF,0XFF,
0XFF,0XC0,0X1B,0X12,0X01,0X01,0X90,0X00,0X18,0X11,0X04,0X02,0X21,0X01,0X04,0XA7,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF9,
0XFF,0XF7,0X8E,0XFF,0X2A,0X7F,0XFF,0XFF,0XFF,0XE0,0X07,0X38,0X03,0X80,0X00,0X60,
0X8C,0X00,0X04,0X00,0X40,0X43,0X2C,0X02,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XEE,0XFF,0XFF,0XD7,0X6F,0X18,0X7F,0XDF,0XFE,
0X1F,0XF7,0X21,0XE0,0X03,0X80,0XA8,0X04,0X06,0X00,0X49,0X44,0X1E,0X17,0X8A,0X0D,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,
0XFF,0XFF,0XFF,0X5C,0X80,0XFD,0XC0,0XFF,0XEE,0X71,0XB2,0X0E,0X06,0X45,0X11,0X00,
0X24,0X12,0X11,0XB8,0X3E,0X06,0XD5,0XA0,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XBF,0XBF,0XFE,0X00,0X3F,0XC0,0X1C,
0XBF,0X3D,0XFF,0XE0,0X04,0X01,0XE8,0X00,0X41,0XF4,0X47,0X80,0X02,0X8E,0X05,0X8B,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XEF,
0XFF,0XFF,0X57,0XCC,0X40,0X3F,0X40,0X09,0XFF,0XBF,0XFF,0XFD,0X22,0XC3,0XF1,0X00,
0X83,0X08,0X93,0XB4,0XAA,0X29,0X08,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFD,0XDF,0XFF,0XF7,0XEF,0XFE,0X13,0X7F,0X60,0X0E,
0X7F,0XFF,0XFF,0XFE,0X48,0X16,0X7D,0X01,0X87,0X59,0X47,0XA9,0X44,0X18,0X57,0X88,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFC,0X7D,0XF7,0XB2,0X3E,0X60,0X0F,0XFF,0XFF,0XFF,0XFF,0X90,0X0D,0XC8,0XC0,
0X0F,0X7E,0X49,0X46,0XF1,0XD8,0XE3,0XF7,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF7,0XFF,0XB8,0X2F,0XF8,0XF8,0X3E,0X60,0X0F,
0XFF,0XFF,0XFF,0XFF,0XFA,0X0F,0XE1,0XAE,0X0D,0XBC,0XC4,0X7C,0X80,0X10,0X16,0XAD,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFB,0XFF,0XFF,
0XFF,0X71,0X1F,0XFC,0XFC,0X3C,0X60,0X3F,0XFF,0XFF,0XFF,0XFF,0XFC,0XF9,0X9F,0X45,
0X19,0X9D,0X84,0X09,0XE1,0XBA,0XE5,0X77,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFE,0XFF,0XFF,0XFD,0XFF,0XF7,0XFC,0XE3,0XC7,0XFE,0X3F,0XFD,0X60,0XAF,
0XFF,0XFF,0XFF,0XFF,0XFF,0X65,0XCC,0XA8,0X3C,0XFF,0X4B,0XAE,0XC3,0XA4,0XB1,0XBF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFD,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X7F,0XDF,
0XB9,0X83,0X7F,0XFB,0X3F,0XFC,0XE7,0XBF,0XFF,0XFF,0XFF,0XFF,0XFF,0XB2,0X58,0X47,
0X3F,0X8F,0XB9,0X33,0X20,0X61,0XCE,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X7F,0XFF,0X63,0X03,0X7F,0XF9,0X9E,0X7E,0X7F,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFE,0X69,0XF3,0XB9,0XD4,0X1F,0X7D,0X27,0X8D,0X43,0XC3,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XBF,0XFF,0XBF,
0X67,0X05,0X7F,0XFD,0X9F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XF6,0X13,0X1F,
0XE7,0XFF,0XEB,0XBC,0XFD,0X0E,0X0F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XAF,0XFF,0XC2,0X05,0X3F,0XFE,0XCF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFE,0X7F,0XFE,0X78,0XE3,0XF7,0XEE,0XFF,0XFB,0XFD,0XBE,0X77,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF3,0XFF,0XFF,0XFF,0XFF,0XBF,0XFF,0XFF,0XFF,0XFF,
0XC7,0X07,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XD9,0XD7,
0XBF,0XFF,0XFF,0X30,0X7E,0XF8,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFB,0XFF,
0XFB,0X3E,0XFB,0X7F,0XFB,0XFF,0XFB,0XFE,0X84,0X67,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFA,0XC3,0XE5,0XEE,0XFF,0XFF,0X7F,0XFF,0XFB,0X3E,0XF7,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XBF,0XFF,0XFF,0X67,0X7F,0XEE,
0X1F,0X67,0XC3,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFB,0XFE,0X3F,0XBA,0X3F,0XF3,
0X7F,0XFF,0XDF,0XFF,0XFF,0X7F,0X76,0X45,0XFF,0XBF,0XEF,0XFF,0XFF,0XBF,0XF7,0XFF,
0XFB,0XDF,0XF7,0XFF,0XFF,0XFF,0XFF,0XDC,0X07,0X83,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XEF,0X7F,0XBF,0XFF,0XF9,0XFB,0X7F,0XED,0X7E,0XFF,
0XFF,0XFF,0XBF,0XFB,0XFF,0XFE,0XFF,0XFE,0XFD,0XFC,0XDF,0XDF,0XFF,0XFB,0XDD,0XCE,
0X00,0X02,0X3F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFB,0X7F,0X8F,0XBD,0XF8,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XBB,0XFF,0XFF,0XFA,0X7F,0X7F,0XFF,0XEF,0XFD,0XFF,
0X7F,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XCE,0X00,0X00,0X00,0X7E,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XBF,0XEE,0XFD,0XFF,0XF7,0XFF,0XEF,0XFF,0XF7,0XFF,0XF9,0XC7,0X9F,
0XFF,0XFA,0X7F,0XFF,0XFF,0XF7,0XFF,0XFF,0XFF,0XBF,0XFF,0X3F,0XFF,0XE3,0XB7,0XCA,
0X00,0X01,0X01,0XFF,0XEF,0XFF,0XFF,0XFF,0XFF,0XFF,0XEF,0XC4,0XFF,0XDF,0X3F,0XFF,
0XFF,0XEF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,0XFF,0XFF,0XEF,0XEE,0XBF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFD,0X3F,0XF4,0X9F,0XE8,0X00,0X3E,0X01,0XFF,0XFF,0X7F,0XFF,0XFF,
0XFF,0XF4,0X6F,0XE6,0X00,0X7F,0XF7,0XFF,0XBB,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,
0XFF,0XF6,0X7F,0XFA,0XFF,0XFF,0XFF,0XBF,0X71,0XFF,0XFF,0XDF,0XEC,0XFF,0XFF,0XF8,
0X00,0X0F,0X03,0XFF,0XFF,0XFF,0XFF,0XFF,0XF9,0X00,0X07,0XF8,0X00,0X3F,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XDF,0XFF,0XFB,0XFF,0XFF,0XFF,0XFF,0XBF,
0XCF,0XFF,0XFF,0X5F,0XFF,0XEE,0XFF,0XFE,0X00,0X00,0X0F,0XFC,0XFF,0XFF,0XFF,0XFF,
0XC0,0X00,0X03,0XF0,0X00,0X3F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF8,
0X3F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0X70,0XFF,0XF7,0XDF,0XFE,0X5F,0XFF,0XFE,
0X07,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0X00,0X00,0X01,0XF8,0X00,0X1F,0XFF,0XF7,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X8B,0X67,0XFF,0XEF,0XFF,0XFF,0XFF,0XFB,
0X7D,0XFF,0XBF,0X78,0X7F,0XFF,0XFF,0XFF,0X7C,0X0F,0XFF,0XA0,0X7F,0XFF,0XFF,0XFE,
0X00,0X00,0X01,0XF0,0X00,0X1F,0XDF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFD,0XFF,0XFC,0XFF,
0XBF,0XFF,0XDF,0XFF,0XFF,0XFF,0XFF,0XF4,0XF9,0XFF,0XFF,0XFE,0X19,0XFF,0XFF,0XFF,
0XFC,0X7F,0XFF,0XE1,0XF7,0XFF,0XFF,0XFE,0X00,0X00,0X00,0XF9,0XE0,0X1F,0XFF,0XFF,
0XFF,0XFE,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X1F,0X3F,0XFF,0XFF,0XFD,0XFF,0XFF,0XF7,
0XFF,0XDF,0XFF,0XFF,0XFF,0XFF,0XFF,0XEF,0XFC,0X03,0XFF,0XE0,0XFF,0XFF,0XFD,0XFE,
0X00,0XF3,0XE1,0XFF,0XC0,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XDF,0XFF,0XFF,0XFB,
0X7E,0X6F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,0XFF,0XBF,0XFF,
0XDC,0X7F,0XFF,0XC8,0XBF,0XFF,0XE1,0XFC,0X07,0XF3,0XFF,0XB7,0XC0,0X1F,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFD,0X87,0X71,0XF3,0XFF,0XFF,0XFF,0XFF,0XFF,0XBF,
0XF7,0XFF,0XFF,0XEF,0XFF,0XFF,0XEB,0XFF,0XCF,0XFF,0XFF,0X80,0XFF,0XFF,0XFC,0X1C,
0X0B,0X40,0X10,0X2F,0X02,0X1F,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XEF,
0XB5,0XAF,0XFF,0XFF,0XEF,0XFF,0XFF,0XBF,0XFF,0XFF,0XDF,0XBB,0XDF,0XFF,0XFF,0XFF,
0XC3,0XFF,0XFF,0XC0,0XEF,0XFF,0XBF,0XE4,0X00,0X00,0X00,0X3C,0X87,0X5F,0XFF,0XFF,
0XF8,0XF7,0XFF,0XFF,0XFF,0XFF,0XFF,0XC6,0XD5,0X13,0XAF,0XFD,0XB7,0XEF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XDF,0XFF,0XE7,0X7F,0XFD,0XC1,0XFF,0XFF,0XBE,0X47,0XFF,0XF7,0XFC,
0X00,0X00,0X00,0X7F,0X86,0X1F,0XFF,0XFD,0XFF,0XDE,0X3F,0X7F,0XFF,0XFF,0XFF,0X9E,
0XFF,0XEF,0XFF,0XFC,0X11,0XFF,0XFF,0XFF,0XFF,0XFF,0XBF,0XFF,0XFF,0XFF,0XFF,0XFF,
0X80,0XFF,0XFC,0XF3,0XB2,0XFF,0XFF,0X4C,0X00,0X01,0X80,0X64,0XCE,0X1F,0XFF,0XFF,
0XBB,0XFE,0X9F,0XFF,0XFF,0XFF,0XFF,0XFD,0XFD,0XFE,0X9F,0XF8,0X43,0XBF,0XFF,0XFD,
0XFF,0XFE,0X7F,0XDF,0XF5,0XFF,0XFF,0XFE,0X00,0X7F,0XFF,0XBC,0X9B,0X3F,0X13,0XC2,
0X00,0X1F,0XC0,0XC7,0X16,0X4F,0XFB,0XDF,0XFF,0XFF,0XC7,0XFF,0XFF,0XFF,0XC0,0XFF,
0X7F,0XFF,0X1F,0XF6,0X7B,0XFF,0XFF,0XFF,0XFF,0XFF,0XE7,0XF7,0X9B,0X91,0XFC,0X00,
0X00,0X3F,0XFE,0XBE,0X59,0XDF,0XC9,0XA0,0X00,0X00,0X01,0XFE,0XAE,0XAF,0XFF,0XFF,
0XFD,0XFF,0XF2,0XFF,0XFF,0XFA,0X9F,0XFF,0XFF,0XF9,0X3F,0XEB,0X4F,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XC4,0XEF,0XFF,0XE0,0X00,0X00,0X07,0XF8,0X07,0XFD,0XFF,0XCC,0XE7,
0X00,0X0F,0X83,0XD9,0X9C,0X0F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X81,0XEF,0XFF,
0X7B,0X57,0XDB,0XFD,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XBF,0XFF,0XFE,0X00,0X00,
0X00,0X00,0X00,0X03,0XFF,0XFF,0X2E,0XF6,0X40,0X38,0XFF,0XDF,0X8C,0X0F,0XFF,0XFF,
0XF7,0XFF,0XFE,0XFF,0XFB,0XF0,0XEF,0XFF,0XFF,0XF3,0XF0,0XDF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XBC,0XFF,0XF8,0X00,0X00,0X02,0X00,0X00,0X00,0X3F,0XFE,0XFF,0X7D,
0XF0,0X07,0XDF,0X9B,0X8C,0X8F,0XFF,0XFF,0XFF,0XFF,0XFE,0X47,0XFF,0XC3,0X28,0X1F,
0XFF,0XFF,0XEE,0X46,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XBF,0XFF,0XE0,0X00,0X00,
0X0C,0X00,0X00,0X00,0X07,0X7B,0XFF,0XAF,0X7F,0XD1,0XFF,0X3F,0X3C,0X8F,0XFF,0XFF,
0X7F,0XFF,0XFF,0XFF,0X03,0X0B,0XAA,0XE8,0XFF,0XFF,0XFD,0X81,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFE,0XF8,0XC0,0X00,0X00,0X08,0X00,0X00,0X00,0X01,0XFF,0XFF,0X9E,
0XDF,0XC3,0XFE,0XFF,0X88,0XFF,0XFF,0XFF,0XFF,0XFF,0XFC,0X7F,0X1F,0X3F,0XBA,0X57,
0XFF,0XFF,0XDE,0X01,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF7,0XFE,0XFF,0X00,0X00,0X00,
0X20,0X00,0X00,0X00,0X00,0X7F,0XFF,0XDF,0X7F,0XFF,0XFE,0X7F,0X7D,0X9F,0XFF,0XFF,
0XFF,0XFF,0XFB,0XFF,0XF8,0X7F,0XDF,0XFF,0XFF,0XFF,0XDD,0X03,0XFF,0XFF,0XFF,0XFF,
0XFF,0XF7,0XFE,0XFF,0XFE,0X00,0X00,0X00,0XC0,0X00,0X00,0X00,0X00,0X3F,0XFF,0XFF,
0XF9,0XFF,0XFC,0X7F,0XC3,0X5F,0XFF,0XFF,0XFF,0XFF,0XF7,0XFF,0XE7,0XFF,0XFF,0XFF,
0XF7,0XFF,0X3B,0X03,0XFF,0XFF,0XFF,0XFF,0XFD,0XFF,0XFC,0XDF,0XFC,0X00,0X00,0X1F,
0X00,0X00,0X10,0X60,0X00,0X1F,0XFF,0XBF,0XE0,0X7F,0XFE,0XFF,0XFB,0X5F,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XEB,0X7F,0XFF,0X07,0XFF,0XFF,0XFF,0XFF,
0XEF,0XFF,0XFF,0X7F,0XF8,0X00,0X00,0X00,0X00,0X00,0X02,0X00,0X00,0X1F,0XFF,0XBF,
0X80,0X1F,0XFE,0XFF,0XFB,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XE7,0XFF,0XFE,0XA3,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF8,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X0F,0XFF,0XF0,0X00,0X0F,0XFC,0X3E,0XFA,0X7F,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,0XFD,0X64,0X7F,0XFF,0XFF,0XFD,
0X7F,0XFF,0XFF,0XFF,0XF0,0X00,0X00,0X00,0X00,0X00,0X00,0X40,0X00,0X0F,0XFF,0X0F,
0X80,0X00,0X00,0X00,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFB,0XFF,0XFF,0XFF,0XFE,
0XBF,0XFF,0XFE,0XCF,0XFF,0XFF,0XEF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF0,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X07,0XF1,0XFF,0XF0,0X00,0X00,0X01,0X5F,0XFF,0XFF,0XFF,
0XFF,0XF1,0XFF,0XE3,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X9F,0XFF,0XFF,0XF7,0XF7,
0XFF,0XFF,0XFF,0XFF,0XF0,0X0C,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X07,0XE7,0XFF,
0XFF,0X00,0X00,0X03,0XF2,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XCF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0X27,0XFF,0XFF,0XF7,0XFB,0XFF,0XFF,0XFF,0XFF,0XF0,0X0A,0X00,0X00,
0X00,0X00,0X00,0X00,0X22,0X07,0XFF,0XE7,0XFF,0XFE,0X00,0X0F,0XFC,0X7F,0XFF,0XFF,
0XFF,0XFF,0XFF,0XDF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XC7,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XF2,0X20,0X00,0X00,0X00,0X00,0X00,0X00,0X2E,0XF3,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XDF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XCF,0XFD,0XFF,0XF7,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0X0F,0X80,0X00,
0X00,0X00,0X00,0X00,0X3F,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XD7,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFC,0X3B,0X80,0X00,0X00,0X00,0X00,0X00,0X7C,0X7F,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XDF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XDF,0XFF,0XFF,0XFB,0XFF,0XFF,0XFF,0XFF,0XFF,0XFC,0X03,0X80,0X00,
0X00,0X00,0X00,0X00,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFB,0XFF,0XFF,0XDF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFC,0X03,0X80,0X00,0X00,0X00,0X00,0X03,0X7F,0XF7,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XF3,0XFF,0XFF,0XEB,0XFF,0XFF,0XFF,0XFF,0XFF,0XFC,0X01,0X80,0X00,
0X00,0X00,0X00,0X01,0XF9,0XFF,0XFF,0XFF,0XFC,0XFF,0XFE,0XFC,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFA,0XFF,0XFF,0XF9,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFC,0X01,0X80,0X00,0X00,0X00,0X00,0X01,0X78,0XFF,0XFF,0XFF,
0XA0,0X7F,0XF7,0X39,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFD,0XFF,0XFF,0XFF,0XFF,0XFF,0XFC,0X01,0X00,0X00,
0X00,0X00,0X00,0X00,0XF8,0XFF,0XFF,0XFF,0X17,0X9F,0XCD,0XB9,0X7F,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFD,0XFF,
0XFF,0XFF,0XFF,0XFF,0XF8,0X03,0X80,0X00,0X00,0X00,0X00,0X00,0XF8,0XFF,0XFF,0XFF,
0XE4,0X1F,0XD9,0XDF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF8,0X03,0X80,0X00,
0X00,0X00,0X00,0X00,0XF9,0XFF,0XFF,0XFF,0XBF,0XFF,0X67,0XF7,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XF8,0X03,0X80,0X00,0X00,0X00,0X00,0X00,0XF8,0XFF,0XFF,0XFF,
0X20,0X79,0X9F,0XF9,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF8,0X07,0X80,0X00,
0X00,0X00,0X00,0X01,0XF8,0XFF,0XFF,0XFF,0XFF,0XF7,0X3F,0XFB,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XF0,0X07,0X80,0X00,0X00,0X00,0X00,0X01,0XFF,0XFF,0XFF,0XE3,
0XDF,0XF4,0XBF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF0,0X7F,0X80,0X00,
0X00,0X00,0X00,0X01,0XFF,0XFF,0XFF,0XFF,0XDF,0XD0,0X0F,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XE0,0X77,0X80,0X00,0X00,0X00,0X00,0X01,0XFF,0XFF,0XFF,0XED,
0XC0,0X64,0X0F,0XF7,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XE0,0X07,0X80,0X00,
0X00,0X00,0X00,0X11,0XFF,0X3F,0XFF,0XC1,0XF8,0X30,0X1F,0XF2,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XC0,0X07,0X80,0X00,0X00,0X00,0X00,0X11,0XFE,0X1F,0XFF,0XC1,
0XF8,0X61,0X3F,0XF3,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XC0,0X07,0X00,0X00,
0X00,0X00,0X00,0X04,0XF8,0X0F,0XFF,0XFE,0X00,0X18,0X7F,0XE7,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0X80,0X06,0X00,0X00,0X00,0X00,0X00,0X04,0XF0,0X0F,0XFF,0XFC,
0X00,0X00,0X1F,0X8D,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X80,0X0E,0X00,0X00,
0X00,0X00,0X00,0X04,0X70,0X0F,0XFF,0XF1,0X35,0X70,0X9D,0XDF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0X80,0X0C,0X00,0X00,0X00,0X00,0X00,0X08,0X70,0X0F,0XFF,0XFC,
0X00,0X00,0X33,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X00,0X08,0X00,0X00,
0X00,0X00,0X00,0X08,0X78,0X0F,0XFF,0XFF,0XBF,0XFC,0XDF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFB,0XFF,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0X00,0X18,0X00,0X00,0X00,0X00,0X00,0X01,0X78,0X0F,0XFF,0XFF,
0XFF,0XFF,0XBF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFB,0XDF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X00,0X10,0X00,0X40,
0X00,0X00,0X00,0X01,0X38,0X0F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X7F,0XFF,0XFF,
0XFF,0XF7,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0X00,0X10,0X00,0X40,0X00,0X00,0X00,0X01,0X38,0X0F,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFE,0X3F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0X00,0X20,0X00,0X40,
0X00,0X00,0X00,0X02,0X3C,0X0F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFC,0X2F,0XFF,0XBF,
0XFF,0XFF,0XFF,0X3F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFE,0X00,0X60,0X00,0X40,0X00,0X00,0X00,0X02,0X3C,0X0F,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFE,0X3E,0XFF,0X9F,0XEF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XBF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0X00,0X40,0X00,0X40,
0X00,0X80,0X00,0X02,0X3C,0X0F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0X3F,0XF7,0XFF,
0XC7,0XFF,0XFF,0XFF,0XFF,0XFF,0XB7,0XFF,0XDF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFE,0X00,0X80,0X00,0X40,0X00,0X80,0X00,0X02,0X2C,0X0F,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFC,0X3B,0XFF,0XFF,0XFF,0XFF,0XFB,0XFF,0XFF,0XC0,0X76,0XEF,
0X5F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0X00,0X80,0X00,0X00,
0X00,0X80,0X00,0X02,0X3E,0X0F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0X30,0XFF,0XFF,
0XFF,0XFF,0XFF,0XF5,0XFF,0XFF,0XFF,0XFF,0XBF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XDF,0XFF,0XFE,0X00,0X80,0X00,0X00,0X00,0X80,0X00,0X02,0X34,0X0F,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFE,0X1D,0X67,0XFF,0XFF,0X7F,0X3F,0XFF,0XFF,0XFF,0XFF,0XFF,
0XDF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFC,0X03,0X80,0X00,0X00,
0X00,0X80,0X00,0X02,0X3E,0X0F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0X1F,0XCF,0XFF,
0XFC,0XF9,0X8E,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFC,0X01,0X80,0X00,0X00,0X00,0X00,0X00,0X02,0X3E,0X0F,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFE,0X1F,0XFC,0X1F,0XFF,0XFF,0XBF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XBF,0XFF,0XFF,0XFC,0X03,0X00,0X00,0X00,
0X00,0X00,0X00,0X02,0X3E,0X0F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0X13,0XFC,0X3F,
0XFF,0XFF,0XFF,0XFF,0XF6,0XBF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFC,0X03,0X00,0X00,0X00,0X00,0XC0,0X00,0X02,0X3E,0X0F,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFE,0X1F,0XFF,0XFF,0XFF,0XE0,0X00,0XFF,0XF8,0XBF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XDF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFC,0X05,0X00,0X00,0X00,
0X00,0XC0,0X00,0X06,0X3E,0X0F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0X0F,0XFF,0XFF,
0XEE,0XF0,0X00,0X77,0XFF,0X3F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XBF,0XFF,0XFD,
0X9F,0XFF,0XFF,0XFC,0X05,0X00,0X00,0X00,0X00,0XC0,0X00,0X06,0X3C,0X1F,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0X0F,0XFF,0XFF,0XFB,0XBB,0X07,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XDF,0XDF,0XFF,0XFF,0X7E,0XFF,0XDF,0XFC,0X01,0X00,0X00,0X00,
0X00,0XE0,0X00,0X06,0X3C,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFC,0X0F,0XFF,0XFF,
0XFE,0X3F,0XFF,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0X27,0XFC,0XFF,0XFF,0XDF,0XFF,
0XFF,0X7F,0XFF,0XFC,0X00,0X80,0X00,0X00,0X00,0XE0,0X00,0X06,0X18,0X3F,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFC,0X0F,0XFF,0XFF,0XFF,0XCF,0XFF,0XF5,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XF7,0X8C,0XC3,0XDF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFC,0X00,0X00,0X00,0X00,
0X00,0XE0,0X00,0X06,0X10,0X3F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF8,0X0F,0XFF,0XFF,
0XFF,0XDF,0XFF,0XFB,0XFF,0XFF,0XF9,0XFF,0X5F,0XF1,0XF0,0X0C,0XC7,0XFF,0XFF,0XE1,
0XD7,0XFF,0XFF,0XFC,0X00,0X00,0X00,0X00,0X00,0XE0,0X00,0X06,0X00,0X3F,0X9F,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFE,0X0F,0XFF,0XFF,0XFF,0XFB,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,
0X81,0XFB,0XF3,0XEE,0X8B,0XFF,0XFF,0X09,0XFF,0XFF,0XFF,0XFC,0X00,0X00,0X00,0X00,
0X00,0XC0,0X00,0X06,0X00,0X3F,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0X1C,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XEF,0XFF,0XFF,0XCF,0XEF,0X8F,0XBB,0X3F,0XFF,0X03,
0XFF,0XBF,0X1F,0XFC,0X00,0X00,0X00,0X00,0X00,0XC0,0X00,0X06,0X40,0X3F,0X80,0X7F,
0XFF,0XFF,0XFF,0XFF,0XFE,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XEF,0XFF,
0X7F,0XCF,0XFF,0XFF,0XFF,0XFF,0XF8,0X72,0XFC,0XF7,0XFF,0XF0,0X08,0X00,0X00,0X00,
0X00,0XE0,0X00,0X02,0X40,0X3F,0X80,0X07,0XFF,0XFF,0XFF,0XFF,0XFE,0X1F,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFC,0XF3,0XFF,0X9F,0X87,0XFF,0X87,0XFF,0XFC,0X7F,0XFF,
0XFF,0XFF,0XFF,0XF8,0X08,0X00,0X00,0X00,0X00,0XC0,0X00,0X02,0X44,0X1F,0X80,0X01,
0XFF,0XFF,0XFF,0XFF,0XFE,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XDF,0XF8,0X92,0X7F,0X1F,0XE7,0XFF,0XFF,0XFF,0XFF,0XFF,0XF8,0X00,0X00,0X00,0X00,
0X00,0XE0,0X00,0X02,0X8C,0X3F,0X80,0X01,0XFF,0XFF,0XFF,0XFF,0XFC,0X5F,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X8E,0XB8,0X2F,0XDF,0XFF,0XFE,
0XFF,0XFF,0XFF,0XFA,0X00,0X00,0X00,0X00,0X00,0XE0,0X00,0X02,0X8C,0X3F,0XF0,0X01,
0X07,0XFF,0XFF,0XFF,0XFC,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFC,0X3A,
0XFE,0XFF,0XFF,0XFA,0X7F,0X3F,0XFF,0XFF,0X9F,0XFF,0XFF,0XF8,0X00,0X00,0X00,0X00,
0X00,0XE0,0X00,0X03,0XAE,0X7F,0XF0,0X01,0X03,0XFF,0X7F,0XFF,0XE4,0XDF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XC2,0XFF,0XFF,0XFF,0XE4,0X3F,0XFF,0XFF,0XDF,
0XE7,0XFF,0XFF,0XFC,0X00,0X00,0X00,0X00,0X00,0XF0,0X00,0X03,0X1C,0X7F,0XB8,0X01,
0X03,0XF8,0X3F,0XFF,0XCC,0X9F,0XFF,0XFF,0XFB,0XFF,0XFF,0XFE,0X7F,0XFF,0XFF,0XE7,
0X77,0XFF,0XFF,0X99,0X5F,0XFF,0XFF,0XB8,0XFD,0XFF,0XFF,0X3E,0X00,0X00,0X00,0X00,
0X00,0X60,0X00,0X03,0XFC,0XFF,0X9C,0X00,0X01,0XF8,0X3E,0XFB,0X8D,0XBF,0XFF,0XFF,
0XEF,0XFF,0XEF,0XFC,0XFF,0XFF,0XFF,0XCE,0X07,0XFF,0XFF,0XFB,0X2F,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XC8,0X00,0X00,0X00,0X00,0XE0,0X00,0X02,0X59,0XFF,0X9C,0X00,
0X01,0XFC,0X3C,0XF0,0X07,0X3F,0XFF,0XFF,0XFF,0XFE,0X7B,0XF9,0XFF,0XFF,0XFF,0X9F,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XE8,0X00,0X00,0X00,
0X00,0X70,0X00,0X02,0XD3,0XFF,0XD9,0X00,0X00,0XFC,0X3C,0X80,0X07,0X3F,0XFF,0XFF,
0XFF,0XF9,0XFD,0XF7,0XFF,0XFF,0XFF,0X3F,0X3F,0XF8,0X7F,0XBF,0XFD,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0X90,0X00,0X00,0X00,0X00,0X70,0X00,0X02,0X07,0XFF,0XE8,0X00,
0X00,0XFC,0X1C,0X00,0X07,0X7F,0XFF,0XFF,0XFF,0XE3,0XFD,0XF7,0XFF,0XFF,0XFC,0XFF,
0XFF,0XF4,0XFC,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X80,0X00,0X00,0X00,
0X00,0XF0,0X00,0X02,0X1F,0XFF,0XE8,0X00,0X00,0XE4,0X3C,0X00,0X06,0XFF,0XFF,0XFF,
0XFF,0XE7,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,0XF9,0XFB,0XFF,0XFF,0XFF,0XFF,0XFF,
0XF0,0X3F,0XFF,0XFF,0X80,0X00,0X00,0X00,0X00,0XE0,0X00,0X03,0X0B,0XFF,0XF0,0X00,
0X00,0X74,0X38,0X00,0X03,0XFF,0XFF,0XFF,0XFF,0XEF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,
0XFF,0XFC,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X80,0X00,0X00,0X00,
0X00,0XE0,0X00,0X03,0X83,0XFF,0XFC,0X00,0X00,0X74,0X38,0X00,0X07,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XEF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XDF,0XFF,0X80,0X00,0X00,0X00,0X00,0XE0,0X00,0X00,0X03,0XFF,0XF0,0X00,
0X00,0X74,0X38,0X00,0X07,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XDF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XEF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF7,0XDB,0X3F,0XFF,0X00,0X00,0X00,0X00,
0X00,0XE8,0X00,0X01,0X03,0XFF,0XF0,0X00,0X00,0X74,0X74,0X00,0X03,0XFF,0XFF,0XDB,
0XFF,0XF0,0X00,0X1F,0XFE,0X00,0X03,0XFF,0XFF,0XE0,0X00,0X1F,0XFF,0X00,0X01,0XFF,
0XF0,0X00,0X0F,0XFF,0X00,0X00,0X00,0X00,0X00,0X50,0X00,0X01,0X03,0XFF,0XF0,0X00,
0X00,0X74,0X32,0X02,0X33,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XF8,0X00,0XDF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X00,0X00,0X00,0X00,
0X00,0XE0,0X30,0X00,0X01,0XFF,0XE0,0X00,0X00,0X74,0X20,0X0D,0XF3,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0X00,0X00,0X00,0X00,0X00,0XF0,0X01,0XC0,0X01,0XFF,0XE0,0X00,
0X00,0X64,0X20,0X0F,0X33,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X00,0X00,0X00,0X00,
0X00,0XFC,0X00,0X06,0X01,0XFF,0XE0,0X00,0X00,0X6C,0X00,0X00,0XF3,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0X00,0X00,0X0F,0XF9,0XFF,0XE7,0XFF,0XFF,0X01,0XFF,0XC0,0X00,
0X00,0X4C,0X00,0X00,0X03,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0X00,0X00,0X0F,0XFF,
0XFF,0XC3,0XFF,0XFE,0X01,0XFF,0XC0,0X00,0X00,0X0C,0X00,0X00,0X01,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XC0,0X01,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0X01,0XFF,0XC0,0X00,
0X00,0X0C,0X00,0X00,0X01,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XC0,0X0F,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0X81,0XFF,0XC0,0X00,0X00,0X0C,0X00,0X00,0X01,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFC,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XC0,0X00,
0X00,0X1C,0X00,0X00,0X01,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XC0,0X00,0X00,0X3C,0X00,0X00,0X01,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF7,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XC0,0X00,
0X00,0X7C,0X00,0X00,0X01,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X7F,0XFF,0XFF,0XFF,0XDF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XC0,0X00,0X00,0XFC,0X00,0X00,0X01,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XDF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XC0,0X00,
0X03,0XFC,0X00,0X00,0X01,0XFF,0XFF,0XFF,0XFF,0XFF,0XFD,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XEF,0XFF,0XFF,0XFF,0XFF,0XFD,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X7F,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XC0,0X00,0X07,0X7C,0X00,0X00,0X01,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X7F,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFC,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XC0,0X00,
0X07,0XFC,0XC0,0X00,0X01,0XFF,0XFF,0XFF,0XFF,0XFF,0XFB,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFD,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFC,0X3F,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XC0,0X00,0X06,0XFC,0X00,0X00,0X01,0XFF,0XFF,0XFF,
0XFF,0XFF,0XF7,0XFF,0X3F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFC,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XC0,0X00,
0X0F,0XFE,0X00,0X00,0X01,0XFF,0XFF,0XEF,0XFF,0XFF,0XFF,0XD1,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFB,0XFC,0X1F,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X80,0X00,0X1F,0XFE,0X00,0X00,0X39,0XFF,0XFF,0XFF,
0XFF,0XFF,0XF1,0X5F,0XBF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFC,0X0F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X80,0X00,
0X3F,0XFF,0X00,0X00,0X61,0XFF,0XF7,0XFF,0XFF,0XFF,0XFF,0XFE,0X6F,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFC,0X07,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X80,0X00,0X7F,0XFF,0X00,0X00,0XE1,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XE9,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFB,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFC,0X07,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X80,0X01,
0XFF,0XFF,0X80,0X01,0XC1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF2,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X87,0XFF,0XFF,0XFF,0XF8,0X07,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X80,0X07,0XFF,0XFF,0XC0,0X03,0X81,0XFF,0XFF,0X7F,
0XFF,0XFF,0XFE,0XFF,0XFF,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0X38,0X07,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X80,0X0F,
0XFF,0XFF,0XF0,0X07,0X01,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X4F,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XD8,0X07,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X00,0X03,0XFF,0XFF,0XD8,0X0C,0X03,0XFF,0XFF,0XFE,
0XFF,0XFF,0XFF,0X7F,0XFE,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFC,0X07,0XFF,0XFF,0X1F,0XFF,0XFF,0XFF,0XFF,0X00,0X03,
0XFF,0XFF,0X80,0X1C,0X03,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XF7,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFC,0X03,0XFF,
0XFE,0X1F,0XFF,0XFF,0XFF,0XFF,0X00,0X0F,0X3F,0XFF,0X80,0X00,0X03,0XFF,0XFF,0XFE,
0X7F,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XF3,0XFF,0XFF,0XFF,0XFC,0X03,0XFF,0XFE,0X07,0XFF,0XFF,0XFF,0XFF,0X00,0X1F,
0XE7,0XFF,0X00,0X00,0X03,0XFF,0XFF,0XFC,0X7F,0XFF,0XFF,0XFF,0XFF,0XEF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFC,0X01,0XFF,
0XFC,0X17,0XFF,0XFF,0XFF,0XFF,0X00,0X1F,0XC7,0XFF,0X00,0X00,0X07,0XFF,0XFF,0XFD,
0XFF,0XFF,0XFF,0XFF,0XFF,0XEF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFC,0X00,0XFF,0XFC,0X1F,0XFF,0XFF,0XFF,0XFE,0X00,0X1F,
0X87,0XFF,0X00,0X00,0X07,0XFF,0XFF,0XFB,0XFF,0XFF,0XAF,0XBF,0XFE,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XD6,0X01,0XFF,
0XFC,0X1F,0XFF,0XFF,0XFF,0XFE,0X00,0X3E,0X07,0XFF,0X80,0X00,0X0F,0XFF,0XFF,0XFF,
0XFF,0XFE,0X15,0XDF,0XFF,0XFF,0XFB,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X01,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFC,0X3C,0X03,0XFF,0XF8,0X1F,0XFF,0XFF,0XFF,0XFE,0X00,0X78,
0X07,0XFF,0X80,0X00,0X0F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0X50,0X1F,0XFF,0XE0,0X01,0XFF,0XF0,0XC7,0XFF,0XFF,0XFF,0XFE,0X00,0XFF,
0XF8,0X0F,0XFF,0XFF,0XFF,0XFE,0X00,0X60,0X0F,0XFF,0XC0,0X00,0X0F,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFB,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X00,0XFF,0XF8,0X1F,0XFF,0XFF,0XFF,0XFE,0X00,0X00,
0X0F,0XFF,0XC0,0X00,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X00,0X7F,
0XFC,0X0F,0XFF,0XFF,0XFF,0XFC,0X00,0X00,0X0F,0XFF,0XC0,0X00,0X2F,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X00,0X7F,0XFC,0X0F,0XFF,0XFF,0XFF,0XFC,0X00,0X00,
0X0F,0XFF,0XC0,0X00,0X27,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X80,0X7F,
0XFC,0X07,0XFF,0XFF,0XFF,0XFC,0X00,0X00,0X1F,0XFF,0XC0,0X40,0X37,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X80,0X7F,0XFC,0X07,0XFF,0XFF,0XFF,0XFC,0X01,0XF0,
0X1F,0XFF,0XC0,0X00,0X77,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X80,0X7F,
0XFC,0X07,0XFF,0XFF,0XFF,0XFC,0X03,0XFF,0X1F,0XFF,0X00,0X00,0XE7,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XC0,0X3F,0XFC,0X07,0XFF,0XFF,0XFF,0XF8,0X3F,0XF0,
0X3F,0XFE,0X08,0X01,0XCF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0X7D,0XFF,0XFF,0XFF,0XFF,0XC0,0X1F,
0XF8,0X0F,0XFF,0XFF,0XFF,0XF8,0X00,0X00,0X7F,0XFC,0X00,0X07,0X8F,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XE0,0X0F,0XF8,0X0F,0XFF,0XFF,0XFF,0XF8,0X00,0X01,
0XFF,0XFC,0X00,0X0C,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XE0,0X07,
0XE0,0X05,0XFF,0XFF,0XFF,0XF8,0X00,0X03,0XFF,0XFC,0X00,0X30,0X1F,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XE0,0X03,0XE0,0X01,0XFF,0XFF,0XFF,0XFC,0X00,0X08,
0X7F,0XFE,0X00,0X80,0X3F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFD,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XE0,0X01,
0XC0,0X01,0XFF,0XFF,0XFF,0XFF,0X00,0X03,0XFF,0XFF,0XC0,0X00,0X7F,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X87,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFD,0XFF,0XFF,0XFF,0XFF,0XFF,0XC0,0X01,0XC0,0X00,0XFF,0XFF,0XFF,0XFF,0XAF,0XFF,
0XFF,0XFF,0XFF,0XDF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFD,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFD,0XFF,0XFF,0XFF,0XFF,0XFF,0X80,0X00,
0X80,0X00,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,
0X5E,0XFF,0XFC,0X73,0XFF,0X90,0X00,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFD,0XFF,0XFF,0XFF,0XFF,0XBF,0X80,0X00,0X40,0X00,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XB8,0X17,0XF0,0X00,0X86,0X00,0X00,0X03,
0XFF,0XFF,0XFB,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFB,0XFF,0XFF,0XCF,0X80,0X00,
0XC0,0X00,0X7F,0XFF,0XFF,0XFB,0XFF,0X7F,0XFF,0XFE,0X6B,0XFF,0XFF,0XFF,0XFF,0XE9,
0X00,0X01,0XCF,0X60,0X00,0X0A,0X00,0X00,0X7F,0XFE,0X0F,0XCF,0XFF,0XFF,0X5F,0X0A,
0XF6,0XFE,0X7E,0XFF,0XFF,0XF7,0X00,0X00,0XC0,0X00,0X3F,0XFF,0XFF,0XF7,0XFF,0XFF,
0XFF,0XFE,0X37,0XFF,0XFF,0XFF,0XD0,0X63,0XC8,0X01,0XFF,0XE0,0X00,0XB0,0X00,0X00,
0X7F,0XFF,0X67,0XFF,0XFF,0XFF,0XF8,0X00,0X0E,0X78,0XFF,0X54,0X03,0XBD,0X00,0X00,
0XC0,0X00,0X3F,0XFF,0XFF,0XFF,0X1F,0X71,0XFF,0XFE,0X1F,0XF3,0XFF,0XFC,0X6F,0XFF,
0XFF,0XFE,0X3F,0XFC,0X7C,0X7A,0X08,0X16,0XCC,0X00,0XFF,0XF0,0X01,0XFF,0X80,0X02,
0X02,0X7E,0XDD,0XB8,0X0B,0XFF,0X00,0X00,0XC0,0X00,0X1F,0XFF,0XFF,0XDF,0X3D,0XE3,
0XFF,0XFF,0X1B,0XF1,0XEF,0XFF,0XFF,0XF8,0XFE,0XF0,0X70,0X38,0X70,0XFE,0X9E,0X7E,
0X00,0X00,0X78,0X00,0X00,0X0F,0X00,0X00,0X01,0X7A,0XE0,0XA0,0X3F,0XEE,0X00,0X01,
0XE0,0X00,0X0F,0XFF,0XFF,0XBF,0XFF,0XE7,0XFF,0XFF,0X8F,0XCF,0X87,0X1F,0X60,0X60,
0XF1,0XE0,0XE0,0X61,0XE0,0XC7,0X3C,0XEC,0X00,0X00,0X60,0X00,0X01,0X8C,0X00,0X00,
0X01,0X60,0X03,0X04,0XFE,0X7E,0X00,0X00,0XE0,0X00,0X07,0XFF,0XFF,0XBF,0XFD,0XDF,
0XEF,0XF7,0X34,0X08,0X1A,0X78,0XE0,0XC3,0X83,0XC0,0X82,0XC1,0XC1,0XEC,0X38,0XF0,
0X00,0X03,0XC0,0X00,0X04,0X1E,0X00,0X20,0X01,0X60,0XFD,0XC1,0X31,0XDC,0X00,0X01,
0XF0,0X00,0X03,0XFF,0XFF,0XFE,0X7F,0XDF,0XBB,0XCE,0X33,0X40,0X2F,0XE3,0XE1,0XC7,
0X1F,0XA1,0X0F,0X8F,0X87,0XEE,0X78,0X70,0XC0,0X3F,0XCB,0XF7,0XC1,0XBE,0XFD,0XFF,
0XC3,0X87,0XE3,0X96,0XC6,0X3C,0X00,0X01,0XF0,0X00,0X01,0XFF,0XF7,0XE0,0X3F,0X9E,
0X7F,0XDE,0XF3,0X28,0X7D,0XC7,0XE1,0XCF,0X2F,0XE6,0X1F,0X9F,0X07,0XDE,0XE4,0X00,
0X8F,0XCC,0X8F,0XEC,0X97,0X07,0X83,0X8E,0X0E,0X0C,0X4C,0X1E,0X18,0XF0,0X00,0X01,
0XF8,0X00,0X00,0XFF,0XEE,0X37,0XD0,0X1E,0XF3,0XDE,0XF1,0XE3,0XFE,0XDF,0XFF,0XFF,
0X9F,0XE4,0X38,0X1F,0X06,0X00,0XC0,0X08,0X1E,0X3E,0X40,0X44,0X3C,0X3C,0X0C,0X20,
0X78,0X03,0X38,0XFC,0X63,0X80,0X00,0X01,0XF8,0X00,0X00,0XF3,0XDE,0X07,0XE0,0XFF,
0X0C,0X3F,0X08,0X07,0XF0,0X70,0X1C,0X02,0X00,0XC0,0X00,0X00,0X00,0X00,0X0E,0X01,
0X71,0XFB,0X0B,0X18,0X61,0XF8,0X71,0XC1,0XC0,0XCE,0X61,0XF0,0XC7,0X30,0X00,0X01,
0XF8,0X03,0XE0,0X7F,0XE1,0XFF,0XFF,0XC1,0XB0,0X80,0XFF,0X0F,0XFF,0XC0,0XC0,0X60,
0X0F,0X0D,0X00,0X00,0X02,0X83,0X00,0XF0,0X9F,0X3C,0X1C,0XC7,0X8F,0X87,0XC7,0X8F,
0X03,0X9C,0XE3,0XE1,0X9E,0XE0,0X00,0X00,0XF8,0X07,0XFF,0XFF,0XFC,0X3F,0XFF,0XE0,
0X0E,0X1C,0X07,0XFF,0XFF,0XF0,0X38,0X04,0X00,0XE3,0XC0,0X00,0X01,0XE1,0XC0,0X9A,
0X7C,0XF8,0X79,0X9E,0X3C,0X1F,0X1F,0X1C,0X0F,0X39,0XCF,0XC6,0X3F,0XE0,0X3F,0X80,
0XF8,0X0F,0XFF,0XFC,0X1F,0XC0,0XFC,0XFE,0X01,0X8C,0X01,0XFF,0X3F,0X1C,0X18,0X01,
0XE0,0X78,0XFC,0X00,0X60,0XF8,0X78,0X7D,0XF1,0XE1,0XF6,0X7C,0XF9,0XF8,0X1C,0X7C,
0X5F,0X03,0XFF,0XDD,0X80,0XE0,0X7F,0XC1,0X8E,0X1F,0XFF,0X9F,0XC0,0X78,0X7F,0X0F,
0X41,0XE6,0X00,0XFF,0X8E,0X0C,0X0E,0X50,0XFC,0X3E,0X3F,0X81,0XC8,0X7E,0X7F,0X70,
0XC7,0XC7,0X04,0XF3,0XE3,0XF8,0X03,0X0F,0X03,0XC0,0X00,0X0C,0X00,0X3F,0XFF,0XFF,
0XE3,0XFF,0XFF,0X8F,0XF8,0X3E,0XBF,0X97,0XFF,0XF3,0XD0,0X7F,0XCF,0X07,0X0F,0X30,
0XFF,0X1F,0XBF,0XC3,0XE0,0X7F,0XBC,0X7E,0X06,0X01,0X01,0XC0,0X40,0X78,0X00,0XE3,
0X81,0XF0,0X1C,0X03,0XC8,0X1F,0XFF,0XFF,0XF8,0XFF,0XFF,0XCF,0XEC,0X7F,0XBF,0XFF,
0XFF,0XF3,0XC4,0X7F,0XEF,0XFF,0XC7,0X20,0XFC,0X1F,0X8F,0XD7,0XBF,0X3F,0X00,0X37,
0X03,0XE0,0X40,0X30,0X38,0X1C,0X00,0X71,0XE0,0XF4,0X1E,0XC1,0XE2,0X1F,0XFF,0XFF,
0XFF,0X7F,0XFF,0XEF,0XFE,0XFB,0XFF,0XF0,0X07,0XFF,0X70,0X03,0X16,0XA0,0XFC,0X48,
0X68,0X1C,0X0B,0X0C,0X04,0X06,0X1F,0XB8,0X01,0XF0,0X71,0X7C,0X1C,0X0F,0X00,0X71,
0XF0,0X7E,0X8F,0XF0,0XFE,0X07,0XFF,0XCF,0XFD,0XFF,0XFE,0X1A,0X1F,0X87,0XE1,0XE0,
0XFE,0X83,0XE0,0XFF,0X7F,0X81,0XD1,0XF1,0XF1,0X78,0X1E,0X1E,0X1C,0X1C,0X3F,0XF8,
};


unsigned char code tab3[]={
/*--  320x240  --*/ 
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X18,0X00,0X10,0X0C,0X00,
0X00,0X08,0X00,0X01,0X00,0X00,0X20,0X01,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X07,0XFF,0XFC,0X00,0X1C,0X0C,0X00,0X00,0X0C,0X00,0X01,0XFF,0XFF,0XF0,0X03,
0X80,0X06,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0X00,0X18,0X00,0X0E,0X18,0X00,
0X00,0X0C,0X00,0X01,0X80,0X00,0X60,0X07,0X0F,0XFF,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X03,0X00,0X18,0X00,0X06,0X10,0X40,0X00,0X0C,0X00,0X01,0X80,0X03,0X60,0X06,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XFF,0XF8,0X03,0XFF,0XFF,0XE0,
0X00,0X0C,0X00,0X01,0XBF,0XFF,0XE0,0X0C,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X03,0X00,0X18,0X00,0X00,0XC0,0X00,0X00,0X0C,0X02,0X01,0X80,0XC0,0X60,0X18,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0X00,0X18,0X00,0X00,0XC0,0X00,
0X1F,0XFF,0XFF,0X01,0X80,0XC0,0X60,0X30,0X80,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X07,0XFF,0XF8,0X00,0X00,0XC1,0X80,0X18,0X0C,0X06,0X01,0X80,0XC0,0X60,0X61,
0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X07,0X00,0X18,0X00,0XFF,0XFF,0XC0,
0X18,0X0C,0X06,0X01,0X80,0XC0,0X60,0X01,0X80,0X01,0X80,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X03,0X00,0X00,0XC0,0X00,0X18,0X0C,0X06,0X01,0X80,0XC6,0X60,0X03,
0X3F,0XFF,0X80,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X7F,0XFF,0XFF,0X80,0X00,0XC0,0X00,
0X18,0X0C,0X06,0X01,0X9F,0XFF,0X60,0X07,0X00,0X60,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X0C,0X18,0X00,0X00,0X00,0XC0,0X30,0X18,0X0C,0X06,0X01,0X80,0XC0,0X60,0X06,
0X00,0X60,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X0C,0X18,0X02,0X07,0XFF,0XFF,0XF8,
0X18,0X0C,0X06,0X01,0X80,0XD8,0X60,0X0F,0X00,0X60,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X0F,0XFF,0XFF,0X00,0X00,0X80,0X00,0X18,0X0C,0X06,0X01,0X80,0XDC,0X60,0X1E,
0X00,0X60,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X0C,0X19,0X06,0X00,0X01,0XC0,0X00,
0X1F,0XFF,0XFE,0X01,0X80,0XCE,0X60,0X36,0X00,0X60,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X0C,0X19,0X0E,0X00,0X01,0XC0,0X40,0X18,0X0C,0X06,0X01,0X80,0XC6,0X60,0X66,
0X00,0X60,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X0F,0XF8,0X8C,0X03,0XFF,0XFF,0XE0,
0X10,0X0C,0X06,0X01,0X80,0XC4,0X60,0X46,0X00,0X60,0X01,0XC0,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X0C,0X18,0X9C,0X00,0X01,0XC0,0X00,0X00,0X0C,0X00,0X01,0X80,0XC3,0X60,0X06,
0X00,0X60,0X01,0XE0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X0C,0X18,0XD8,0X00,0X03,0XA0,0X00,
0X00,0X0C,0X00,0X01,0XFF,0XFF,0XE0,0X06,0X00,0X60,0X01,0XE0,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X0C,0X3F,0X70,0X00,0X03,0X30,0X00,0X00,0X0C,0X00,0X01,0X80,0X00,0X60,0X06,
0X00,0X60,0X01,0XE0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X0F,0XF8,0X70,0X00,0X06,0X18,0X00,
0X00,0X0C,0X00,0X01,0X80,0X00,0X60,0X06,0X00,0X60,0X00,0X60,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X7F,0X98,0X78,0X00,0X0E,0X0E,0X00,0X00,0X0C,0X00,0X01,0XFF,0XFF,0XE0,0X06,
0X00,0X60,0X00,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X78,0X18,0XDC,0X00,0X1C,0X07,0X80,
0X00,0X0C,0X00,0X01,0X80,0X00,0X60,0X06,0X00,0X60,0X01,0X80,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X20,0X19,0X0F,0XC0,0X38,0X03,0XFC,0X00,0X0C,0X00,0X01,0X80,0X00,0X70,0X06,
0X07,0XE0,0X01,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X1E,0X03,0X01,0XE0,0X00,0XF8,
0X00,0X0C,0X00,0X01,0X00,0X00,0X00,0X06,0X01,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X18,0X00,0X07,0X00,0X00,0X30,0X00,0X08,0X00,0X00,0X00,0X00,0X00,0X06,
0X00,0X80,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X08,0X00,0X01,0X00,0X00,0X20,
0X00,0X01,0X00,0X00,0X00,0X10,0X00,0X01,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X0C,0X00,0X01,0XFF,0XFF,0XF0,0X00,0X01,0X80,0X01,0X00,0X38,0X00,0X01,
0XC7,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X0C,0X00,0X01,0X80,0X00,0X60,
0X00,0X63,0X80,0X01,0XC4,0XE0,0X00,0X03,0X87,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X0C,0X00,0X01,0X80,0X03,0X60,0X7F,0XF3,0X00,0X00,0XC7,0X00,0X10,0X03,
0X06,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X0C,0X00,0X01,0XBF,0XFF,0XE0,
0X00,0XE3,0X00,0X00,0XC6,0X0F,0XF8,0X03,0X0C,0X00,0X00,0X38,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X0C,0X02,0X01,0X80,0XC0,0X60,0X00,0XC3,0X01,0X00,0XC6,0X0C,0X30,0X06,
0X0C,0X01,0X00,0X38,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X1F,0XFF,0XFF,0X01,0X80,0XC0,0X60,
0X40,0XC7,0XFF,0X80,0X06,0X0C,0X30,0X06,0X1F,0XFF,0X80,0X38,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X18,0X0C,0X06,0X01,0X80,0XC0,0X60,0X20,0XC6,0X03,0X80,0X06,0X0C,0X30,0X0C,
0X18,0X03,0X80,0X38,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X18,0X0C,0X06,0X01,0X80,0XC0,0X60,
0X11,0X84,0XC3,0X00,0X46,0X0C,0X30,0X0E,0X30,0X82,0X00,0X38,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X18,0X0C,0X06,0X01,0X80,0XC6,0X60,0X19,0X8C,0XE6,0X07,0XE6,0X0C,0X30,0X1E,
0X30,0XE4,0X00,0X38,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X18,0X0C,0X06,0X01,0X9F,0XFF,0X60,
0X0F,0X88,0XE4,0X00,0XC6,0X0C,0X30,0X1E,0X60,0XC0,0X00,0X38,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X18,0X0C,0X06,0X01,0X80,0XC0,0X60,0X07,0X18,0XE0,0X00,0XC6,0X0C,0X30,0X36,
0X40,0XC0,0X00,0X38,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X18,0X0C,0X06,0X01,0X80,0XD8,0X60,
0X07,0X10,0XE0,0X00,0XC6,0X0C,0X30,0X66,0X0C,0XC0,0X00,0X18,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X18,0X0C,0X06,0X01,0X80,0XDC,0X60,0X07,0X80,0XE0,0X00,0XC6,0X3C,0X30,0X46,
0X1C,0XC8,0X00,0X10,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X1F,0XFF,0XFE,0X01,0X80,0XCE,0X60,
0X07,0X81,0XA0,0X00,0XC7,0XCC,0X30,0X86,0X18,0XC4,0X00,0X10,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XFE,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X18,0X0C,0X06,0X01,0X80,0XC6,0X60,0X0D,0XC1,0XB0,0X00,0XC7,0X8C,0X30,0X06,
0X18,0XC6,0X00,0X10,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X1F,0XFF,0X80,0X00,0X00,0X00,0X00,0X00,0X00,0X10,0X0C,0X06,0X01,0X80,0XC4,0X60,
0X0C,0XE1,0XB0,0X00,0XC7,0X0D,0XF0,0X06,0X30,0XC3,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X7F,0XFF,0XE0,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X0C,0X00,0X01,0X80,0XC3,0X60,0X18,0XE3,0X10,0X00,0XC2,0X0C,0X60,0X06,
0X30,0XC3,0X80,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0XFF,0XFF,0XF8,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X0C,0X00,0X01,0XFF,0XFF,0XE0,
0X10,0X63,0X18,0X00,0XC0,0X0C,0X00,0X06,0X20,0XC1,0X80,0X38,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0XFF,0XFF,0XFE,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X0C,0X00,0X01,0X80,0X00,0X60,0X20,0X66,0X1C,0X03,0XE0,0X0C,0X00,0X06,
0X60,0XC1,0XC0,0X7C,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,
0XFF,0XFF,0XFF,0X80,0X00,0X00,0X00,0X00,0X00,0X00,0X0C,0X00,0X01,0X80,0X00,0X60,
0X40,0X0E,0X0C,0X07,0X30,0X0C,0X00,0X06,0XC0,0XC0,0XC0,0X7C,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X07,0XFF,0XFF,0XFF,0XE0,0X00,0X00,0X00,0X00,
0X00,0X00,0X0C,0X00,0X01,0XFF,0XFF,0XE0,0X80,0X0C,0X06,0X0E,0X1E,0X08,0X00,0X06,
0X80,0XC0,0X80,0X38,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X0F,
0XFF,0XFF,0XFF,0XF8,0X00,0X00,0X00,0X00,0X00,0X00,0X0C,0X00,0X01,0X80,0X00,0X60,
0X00,0X30,0X07,0X84,0X07,0XFF,0XFC,0X07,0X00,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X0F,0XFF,0XFF,0XFF,0XE0,0X00,0X00,0X00,0X00,
0X00,0X00,0X0C,0X00,0X01,0X80,0X00,0X70,0X00,0X60,0X03,0XC0,0X00,0XFF,0XF0,0X06,
0X0F,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X3F,0XFF,0XC0,0X00,0X0F,
0XFF,0XFF,0XFF,0XF8,0X00,0X00,0X00,0X00,0X00,0X00,0X0C,0X00,0X01,0X00,0X00,0X00,
0X01,0X80,0X00,0X00,0X00,0X00,0X00,0X06,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X03,0XFF,0XFF,0XFC,0X00,0X1F,0XFF,0XFF,0XFF,0XFC,0X00,0X00,0X00,0X00,
0X00,0X00,0X08,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X04,
0X01,0X80,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X1F,0XFF,0XFF,0XFF,0X80,0X1F,
0XFF,0XFF,0XFF,0XFC,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0XFF,0XFF,0XFF,0XFF,0XE0,0X1F,0XFF,0XFF,0X80,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XFF,0XFF,0XFF,0XFF,0XF8,0X1F,
0XFF,0XFC,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X0F,0XFF,0XFF,0XFF,0XFF,0XFE,0X1F,0XFF,0XF0,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,0X1F,
0XFF,0XE0,0X00,0X03,0XF8,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XDF,0XFF,0XC0,0X00,0X7F,0XFF,0XC0,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0X80,0X07,0XFF,0XFF,0XF0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X03,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X00,0X3F,0XFF,0XFF,0XFC,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X07,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFE,0X00,0XFF,0XFF,0XFF,0XFF,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X0F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFC,0X03,0XFF,0XFF,0XFF,0XFF,0X80,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XF8,0X0F,0XFF,0XFF,0XFF,0XFF,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF8,0X1F,0XFF,0XFF,0XFF,0XFF,0XE0,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XF8,0X7F,0XFF,0XFF,0XFF,0XFF,0XF0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X3F,0XC0,0X01,0XFF,0XFF,0XFF,0XFF,0XFF,0XF8,0XFF,0XFF,0XFF,0XFF,0XFF,0XF8,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X1E,0X00,0X00,0X07,0XFF,0XFF,0XFF,0XFF,
0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFC,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X7F,0XFF,0XFF,0XFF,0XF3,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X07,0XFF,0XFF,0XFF,
0XE7,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0XFF,0XFF,0XFF,0XE7,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X0F,0XFF,0XFF,
0XEF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X03,0XFF,0XFF,0XFF,0XFF,0XFF,0XF8,0X00,0X00,0X7F,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0XFF,0XFF,
0XFF,0XFF,0XFF,0X80,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X07,0XFF,0XFF,0XFF,0XFF,0XE0,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X1F,0XFF,0XFF,
0XFF,0XFF,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X7F,0XFF,0XFF,0XFF,0XF8,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XFF,0XFF,0XFF,
0XFF,0XF0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X03,0XFF,0XFF,0XFF,0XFF,0XF8,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X07,0XFF,0XFF,0XFF,
0XFF,0XF8,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X1F,0XFF,0XFF,0XFF,0XFF,0XF0,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X3F,0XFF,0XFF,0XFF,
0XFF,0XF0,0X00,0X00,0X00,0X1F,0XFF,0XFF,0XFE,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X7F,0XFF,0XFF,0XFF,0XFF,0XE0,0X00,0X00,0X03,0XFF,0XFF,0XFF,
0XFF,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XFF,0XFF,0XFF,0XFF,0XFC,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XFF,0XFF,0XFF,0XFF,
0XFF,0XC0,0X00,0X00,0XFF,0XFF,0XFF,0XFF,0XFF,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,
0X0F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XE0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X03,0XFF,0XFF,0XFF,0XFF,0XFF,0X80,0X00,0X3F,0XFF,0XFF,0XFF,0XFF,
0XFF,0XE0,0X00,0X00,0X00,0X00,0X07,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF8,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X07,0XFF,0XFF,0XFF,0XFF,
0XFF,0X00,0X00,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF0,0X00,0X00,0X00,0X1F,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF8,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X0F,0XFF,0XFF,0XFF,0X3F,0X00,0X00,0X01,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XF8,0X00,0X00,0X00,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF8,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X1F,0XFF,0XFF,0XF8,0X3E,
0X00,0X00,0X03,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFC,0X00,0X00,0X00,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFC,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X7F,0XFF,0XFF,0X80,0X7E,0X00,0X00,0X07,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFE,0X00,0X00,0X00,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFC,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XFF,0XFF,0XF0,0X00,0X7E,
0X00,0X00,0X07,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X00,0X00,0X00,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFC,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X7F,0XFF,0X00,0X00,0X7E,0X00,0X00,0X0F,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0X80,0X00,0X01,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFC,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XFF,0XE0,0X00,0X00,0XFE,
0X00,0X00,0X0F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X80,0X00,0X01,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFC,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0XFC,0X00,0X00,0X00,0XFC,0X00,0X00,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XC0,0X00,0X00,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF0,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XFC,
0X00,0X00,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XC0,0X00,0X00,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X80,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XFC,0X00,0X00,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XC0,0X00,0X00,0X3F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0XFC,
0X00,0X00,0X3F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XC0,0X00,0X00,0X03,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XF0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X01,0XFC,0X00,0X00,0X3F,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XC0,0X00,0X00,0X00,0X00,0X1F,0XFF,0XF0,0X00,0X1F,0XF0,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0XFC,
0X00,0X00,0X3F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XC0,0X00,0X00,0X00,0X00,0X00,
0X00,0XF8,0X00,0X1F,0XF8,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XF8,0X00,0X00,0X3F,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XC0,0X00,0X00,0X00,0X00,0X00,0X01,0XFE,0X00,0X1F,0XF8,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XF8,
0X00,0X00,0X3F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X80,0X00,0X00,0X00,0X00,0X00,
0X03,0XFE,0X00,0X3F,0XF8,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XF8,0X00,0X00,0X3F,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XFF,0X00,0X3F,0XFC,0X00,0X00,0X00,
0X00,0X00,0X00,0X0C,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X07,0XF8,
0X00,0X00,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X00,0X00,0X01,0XFE,0X00,0X00,
0X07,0XFF,0X00,0X3F,0XFC,0X00,0X00,0X00,0X00,0X00,0X00,0X0C,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X07,0XF8,0X00,0X00,0X0F,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFE,0X00,0X00,0X03,0XFF,0XF8,0X00,0X07,0XFF,0X80,0X3F,0XFC,0X00,0X00,0X00,
0X00,0X00,0X1F,0XFF,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X07,0XF8,
0X00,0X00,0X07,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF8,0X00,0X00,0X0F,0XFF,0XFE,0X00,
0X07,0XFF,0X80,0X3F,0XFC,0X00,0X00,0X00,0X00,0X01,0XFF,0XFF,0XFC,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X07,0XF8,0X00,0X00,0X03,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XF0,0X00,0X00,0X3F,0XFF,0XFF,0X00,0X07,0XFF,0X80,0X3F,0XFC,0X00,0X00,0X00,
0X00,0X0F,0XFF,0XFF,0XFF,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X0F,0XF8,
0X00,0X00,0X01,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XE0,0X00,0X00,0X7F,0XFF,0XFF,0X00,
0X07,0XFF,0XC0,0X3F,0XFC,0X00,0X00,0X00,0X00,0X3F,0XFF,0XFF,0XFF,0XF0,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X0F,0XF0,0X00,0X00,0X00,0X7F,0XFF,0XFF,0XFF,0XFF,
0XFF,0X80,0X00,0X00,0X7F,0XFF,0XFF,0X80,0X0F,0XFF,0XC0,0X3F,0XFE,0X00,0X00,0X00,
0X00,0XFF,0XFF,0XFF,0XFF,0XFE,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X0F,0XF0,
0X00,0X00,0X00,0X3F,0XFF,0XFF,0XFF,0XFF,0XF8,0X00,0X00,0X00,0XFF,0XFF,0XFF,0X80,
0X0F,0XFF,0XC0,0X3F,0XFE,0X00,0X00,0X00,0X01,0XFF,0XFF,0XFF,0XFF,0XFF,0X80,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X1F,0XF0,0X00,0X00,0X00,0X0F,0XFF,0XFF,0XFF,0XFF,
0X00,0X00,0X00,0X01,0XFF,0XFF,0XFF,0XE0,0X0F,0XFF,0XC0,0X3F,0XFC,0X00,0X00,0X00,
0X07,0XFF,0XFF,0XFF,0XFF,0XFF,0XE0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X1F,0XF0,
0X00,0X00,0X00,0X03,0XFF,0XFF,0XFF,0XF0,0X00,0X00,0X00,0X03,0XFF,0XFF,0XFF,0XF0,
0X0F,0XFF,0XC0,0X3F,0XFC,0X00,0X00,0X00,0X0F,0XFF,0XFF,0XFF,0XFF,0XFF,0XF8,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X1F,0XF0,0X0F,0X80,0X00,0X00,0X00,0XFC,0X00,0X00,
0X00,0X00,0X00,0X03,0XFF,0XFF,0XFF,0XF8,0X0F,0XFF,0XC0,0X7F,0XFE,0X00,0X3F,0X00,
0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X1F,0XF0,
0XFF,0XF0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X07,0XFF,0XFF,0XFF,0XFC,
0X0F,0XFF,0XC0,0X7F,0XFE,0X01,0XFF,0XC0,0X3F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X3F,0XF3,0XFF,0XFC,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X07,0XFF,0XFF,0XFF,0XFE,0X0F,0XFF,0XE0,0X7F,0XFE,0X07,0XFF,0XF0,
0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X3F,0XFF,
0XFF,0XFE,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X07,0XFF,0XFF,0XFF,0XFE,
0X0F,0XFF,0XE0,0X7F,0XFE,0X07,0XFF,0XF8,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XE0,
0X00,0X00,0X00,0X00,0X00,0X00,0X3F,0XFF,0XFF,0XFF,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X0F,0XFF,0XFF,0XFF,0XFF,0X0F,0XFF,0XE0,0X7F,0XFC,0X0F,0XFF,0XF8,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF0,0X00,0X00,0X00,0X00,0X00,0X00,0X3F,0XFF,
0XFF,0XFF,0X80,0X00,0X00,0X00,0X00,0X00,0X20,0X00,0X00,0X0F,0XFF,0XFF,0XFF,0XFF,
0X0F,0XFF,0XE0,0X7F,0XFC,0X1F,0XFF,0XFD,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF8,
0X00,0X00,0X00,0X00,0X00,0X00,0X7F,0XFF,0XFF,0XFF,0XC0,0X00,0X00,0X00,0X00,0XFF,
0XFC,0X00,0X00,0X0F,0XFF,0XFF,0XFF,0XFF,0X9F,0XFF,0XE0,0X7F,0XFC,0X3F,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF8,0X00,0X00,0X00,0X00,0X00,0X00,0X7F,0XFF,
0XFF,0XFF,0XC0,0X00,0X00,0X00,0X7F,0XFF,0XFF,0XE0,0X00,0X0F,0XFF,0XFF,0XFF,0XFF,
0X9F,0XFF,0XE0,0X7F,0XFE,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF8,
0X00,0X00,0X00,0X00,0X00,0X00,0X7F,0XFF,0XFF,0XFF,0XE0,0X00,0X00,0X1F,0XFF,0XFF,
0XFF,0XFC,0X00,0X0F,0XFF,0XFF,0XFF,0XFF,0X9F,0XFF,0XE0,0XFF,0XFE,0X7F,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFC,0X00,0X00,0X00,0X00,0X00,0X00,0X7F,0XFF,
0XFF,0XFF,0XE0,0X00,0X1F,0XFF,0XFF,0XFF,0XFF,0XFE,0X00,0X0F,0XFF,0XFF,0XFF,0XFF,
0XDF,0XFF,0XE0,0X7F,0XFE,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFC,
0X00,0X00,0X00,0X00,0X00,0X00,0X7F,0XFF,0XFF,0XFF,0XF0,0X3F,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0X00,0X07,0XFF,0XFF,0XFF,0XFF,0XDF,0XFF,0XE0,0XFF,0XFE,0XFF,0XFF,0XFF,
0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFC,0X00,0X00,0X00,0X00,0X00,0X00,0XFF,0XFF,
0XF8,0XFF,0XF0,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X00,0X07,0XFF,0XFF,0XFF,0XFF,
0XDF,0XFF,0XE0,0XFF,0XFF,0XFF,0XFF,0XFF,0X01,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFC,
0X00,0X00,0X00,0X00,0X00,0X00,0XFF,0XFF,0XE0,0X7F,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0X80,0X03,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XE0,0XFF,0XFF,0XFF,0XFF,0XFF,
0X80,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XF8,0X00,0X00,0X00,0X00,0X00,0X00,0XFF,0XFF,
0XC0,0X3F,0XF3,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X80,0X01,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XE0,0XFF,0XFF,0XFF,0XFF,0XFF,0X80,0X1F,0XFF,0XFF,0XDF,0XFF,0XF0,0X40,
0X00,0X00,0X00,0X00,0X00,0X00,0XFF,0XFF,0XC0,0X3F,0XFB,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0X80,0X01,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XE0,0XFF,0XFF,0XFF,0XFF,0XFF,
0X80,0X00,0X07,0XFF,0X83,0XFF,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XFF,0XFF,
0X80,0X3F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XC0,0X00,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XE0,0XFF,0XFF,0XFF,0XFF,0XFF,0X80,0X00,0X00,0X07,0X80,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X01,0XFF,0XFF,0X80,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XC0,0X00,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XE0,0XFF,0XFF,0XFF,0XFF,0XFF,
0X00,0X00,0X00,0X07,0X80,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0XFF,0XFF,
0X00,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XC0,0X00,0X7F,0XFF,0XFF,0XFF,
0XFF,0XFF,0XE0,0XFF,0XFF,0XFF,0XFF,0XFF,0X00,0X00,0X00,0X07,0X80,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X01,0XFF,0XFF,0X00,0X3F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XC0,0X00,0X3F,0XFF,0XFF,0XFF,0XFF,0XFF,0XE0,0XFF,0XFE,0XFF,0XFF,0XFF,
0X00,0X00,0X00,0X07,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0XFF,0XFF,
0X00,0X3F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X80,0X00,0X3F,0XFF,0XFF,0XFF,
0XFF,0XFF,0XE0,0XFF,0XFE,0XFF,0XFF,0XFE,0X00,0X00,0X00,0X07,0XC0,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X01,0XFF,0XFF,0X80,0X7F,0XFB,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFE,0X00,0X00,0X1F,0XFF,0XFF,0XFF,0X9F,0XFF,0XE0,0XFF,0XFE,0X7F,0XFF,0XFE,
0X00,0X00,0X00,0X07,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0XFF,0XFF,
0XC0,0X7F,0XF9,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFC,0X00,0X00,0X07,0XFF,0XFF,0XFE,
0X1F,0XFF,0XE0,0XFF,0XFE,0X3F,0XFF,0XFC,0X00,0X00,0X00,0X07,0XC0,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X01,0XFF,0XFF,0XE1,0XFF,0XF8,0X7F,0XFF,0XFF,0XFF,0XFF,
0XFF,0XE0,0X00,0X00,0X03,0XFF,0XFF,0XF8,0X1F,0XFF,0XE0,0XFF,0XFE,0X1F,0XFF,0XFC,
0X00,0X00,0X00,0X07,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0XFF,0XFF,
0XFF,0XFF,0XF0,0X07,0XFF,0XFF,0XFF,0XFF,0XF8,0X00,0X00,0X00,0X01,0XFF,0XFF,0XC0,
0X1F,0XFF,0XF0,0XFF,0XFC,0X0F,0XFF,0XF0,0X00,0X00,0X00,0X07,0XC0,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X01,0XFF,0XFF,0XFF,0XFF,0XF0,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X3F,0XFE,0X00,0X1F,0XFF,0XF0,0XFF,0XFC,0X07,0XFF,0XC0,
0X00,0X00,0X00,0X07,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0XFF,0XFF,
0XFF,0XFF,0XE0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X1F,0XFF,0XF0,0XFF,0XFC,0X01,0XFF,0X00,0X00,0X00,0X00,0X07,0XC0,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X01,0XFF,0XFF,0XFF,0XFF,0XE0,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X1F,0XFF,0XE0,0XFF,0XFE,0X00,0X38,0X00,
0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XFF,0XFF,
0XFF,0XFF,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X1F,0XFF,0XE0,0XFF,0XFC,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X03,0XFF,0XFF,0XFF,0XFF,0X80,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X1F,0XFF,0XE0,0X7F,0XFC,0X00,0X00,0X00,
0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XFF,0XFF,
0XFF,0XFF,0X00,0X00,0X00,0X00,0X00,0X00,0XFF,0XFF,0XFF,0X5E,0X00,0X00,0X00,0X00,
0X1F,0XFF,0XE0,0X7F,0XFC,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X03,0XFF,0XFF,0XFF,0XFE,0X00,0X00,0X00,0X01,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XE0,0X00,0X00,0X00,0X1F,0XFF,0XE0,0X7F,0XFC,0X00,0X00,0X00,
0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XFF,0XFF,
0XFF,0XFC,0X00,0X00,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF8,0X00,0X00,0X00,
0X1F,0XFF,0XE0,0X7F,0XFC,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X01,0XFF,0XFF,0XFF,0XE0,0X00,0X7F,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFC,0X00,0X00,0X00,0X1F,0XFF,0XE0,0X7F,0XFC,0X00,0X00,0X00,
0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X3F,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X00,0X00,0X00,
0X1F,0XFF,0XE0,0X7F,0XFC,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XE0,0X00,0X00,0X00,
0X00,0X03,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0X00,0X00,0X00,0X1F,0XFF,0XE0,0X7F,0XFC,0X00,0X00,0X00,
0X00,0X00,0X00,0X03,0XE0,0X00,0X00,0X00,0X00,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X80,0X00,0X00,
0X1F,0XFF,0XE0,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XE0,0X00,
0X00,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0X80,0X00,0X00,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X00,0X03,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X80,0X00,0X03,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XE0,
0X07,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XC0,0X09,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF0,0X07,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF8,
0X0F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF8,0X0F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF8,
0X0F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFC,0X0F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFC,
0X0F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFC,0X0F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFC,
0X07,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF8,0X03,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XE0,
0X00,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X80,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X3F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFC,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X07,0XFF,0XFF,0XF0,
0XF0,0X7F,0XFF,0XE0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X0F,0XFB,0X3F,0XE0,0X07,0XFC,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0XF0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X3F,0XFF,0X1F,0XC0,0X0F,0XFE,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X07,0XF0,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X7F,0XFF,0X1F,0X80,0X0F,0XFE,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X7F,0XE0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XFF,0XFF,0X9F,0X80,0X07,0XFC,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X1F,0XE0,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X01,0XFF,0XE7,0X9F,0X80,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X1F,0XE0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XFF,0XC1,0X9F,0X80,0X1F,0XFE,0X00,
0X00,0X07,0XEF,0XC0,0X00,0X00,0X00,0X00,0X07,0XF8,0X1F,0XE0,0X00,0X00,0X7F,0X00,
0X00,0X00,0X00,0XFF,0X01,0XFE,0X60,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X03,0XFF,0XC0,0X1F,0X7F,0X07,0XF8,0XFF,0XFE,0X1F,0XFF,0X80,0X03,0XFF,0X87,0XFF,
0X1F,0XFC,0X1F,0XE0,0XFF,0X41,0XFF,0XC7,0XFF,0X9F,0X83,0XFF,0X83,0XFF,0XE0,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XFF,0X80,0X1F,0XFF,0XC3,0XF8,0X7F,
0XFF,0X1F,0XFF,0X80,0X03,0XFE,0X31,0XFE,0X3F,0XFF,0X1F,0XE1,0XFF,0XE3,0XFF,0XE3,
0XFF,0XFF,0XC7,0XFF,0XE7,0XFF,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X07,0XFF,0XC0,0X1F,0XFF,0XC3,0XF8,0X3F,0XFF,0X3F,0XFF,0X80,0X01,0XFE,0X39,0XFE,
0X7C,0XFF,0X1F,0XE3,0XFF,0XE7,0XCF,0XF1,0XFF,0XFF,0XCF,0X9F,0XE7,0XF3,0XC0,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X07,0XFF,0XC0,0X1F,0XFF,0XE3,0XF8,0X3F,
0XFF,0XBF,0XBF,0X80,0X01,0XFE,0X7B,0XFC,0X78,0XFF,0X1F,0XE7,0XFF,0XE7,0X8F,0XF1,
0XFF,0XFF,0XEF,0X1F,0XE7,0XF1,0X80,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X07,0XFF,0XF3,0XCF,0XFF,0XE7,0XF8,0X3F,0XFF,0XFF,0X9F,0X80,0X00,0XFE,0XFF,0XFC,
0X79,0XFF,0X1F,0XC7,0XFE,0X6F,0X9F,0XF1,0XFF,0XFF,0XEF,0X3F,0XE7,0XFE,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XFF,0XFF,0XCF,0XFF,0XE7,0XF8,0X3F,
0XFF,0XFF,0XDF,0X80,0X00,0XFF,0XFF,0XFC,0XFF,0XFF,0X1F,0XCF,0XFE,0X0F,0XFF,0XF1,
0XFF,0XFF,0XFF,0XFF,0XE3,0XFF,0XE0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X03,0XFF,0XFF,0XCF,0XFF,0XE7,0XF8,0X3F,0XFF,0XFF,0XFF,0X80,0X00,0XFF,0XFF,0XF8,
0XFF,0XFF,0X1F,0XCF,0XFE,0X0F,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XE1,0XFF,0XF8,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XFF,0XFF,0XCF,0XFF,0XE7,0XF8,0X7F,
0XFF,0XFF,0XFF,0X80,0X00,0X7F,0XFF,0XF8,0XFF,0XFE,0X1F,0XCF,0XFF,0X7F,0XFF,0XF1,
0XFF,0XFF,0XFF,0XFF,0XC0,0X1F,0XF8,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X01,0XFF,0XFF,0X8F,0XDF,0XC7,0XFC,0X7F,0X7F,0X7F,0XFF,0X80,0X00,0X7F,0XFF,0XF8,
0X7F,0XF9,0X9F,0XCF,0XFF,0XF7,0XFF,0XF1,0XFF,0XFF,0XCF,0XFF,0X37,0X0F,0XF8,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0XFF,0XFF,0X8F,0X8F,0XC7,0XFC,0X7E,
0X3F,0X3F,0XFF,0X80,0X00,0X7F,0XFF,0XF0,0X7F,0XC7,0X9F,0XC7,0XFF,0XF7,0XFF,0XF1,
0XF3,0XE7,0XCF,0XF8,0XF7,0XDF,0XF8,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0XFF,0XFD,0X8F,0X8F,0X87,0XFC,0X7E,0X3E,0X3F,0XFF,0X80,0X00,0X3F,0X9F,0XF0,
0X3F,0XFF,0X9F,0XC7,0XFF,0XF3,0XFF,0XE1,0XF3,0XE7,0X87,0XFF,0XFF,0XFF,0XF0,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X7F,0XF9,0X8F,0X9F,0X07,0XFC,0X7E,
0X7E,0X1F,0XFF,0X80,0X00,0X3F,0X8F,0XF0,0X1F,0XFF,0X9F,0XC3,0XFF,0XB1,0XFF,0XC1,
0XF7,0XEF,0X83,0XFF,0XFF,0XFF,0XE0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X1F,0XE0,0X3F,0XFF,0XDF,0XFF,0XFF,0X7F,0X0F,0XDF,0XC0,0X00,0X7F,0XDF,0XF8,
0X0F,0XF8,0XFF,0XE0,0XFE,0X00,0XFF,0X07,0XF8,0X1F,0XC1,0XFF,0X1C,0X7F,0X80,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
};
