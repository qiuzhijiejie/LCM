#include "320240.h"
#include "PIC.h"
uchar code scroll_P10[]   = { 0x00, basepart1, 0xf0,  //1area??10KB??240 row
                   0x00, basepart2, 0xf0,  //2area??10KB??240 row
                   0x00, basepart3,        //3area??10KB
                   0x00, basepart4};       //4area??32KB

				   				   
uchar code systemset_P8[] =  {0x30, 0x87, 0x07, 0x27, 0x3f, 0xef, 0x28, 0x00};
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
void delay(uint ms)
{
    uint i, j;
    for(i = 0; i < ms; i++)
        for(j = 0; j < 120; j++);  // 这个120是根据主频调整的
}



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
	//	demo_line();
	delay(10);
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
// show_line(0,0,319,239, 0);  //"\"
// show_line(0,239,319,0, 0);  //"_"
	
 show_line(0,0,319,0, 0);    //"-"
 show_line(0,0,0,239, 0);    //"|"
 show_line(0,239,319,239, 0);//"/"
 show_line(319,0,319,239, 0);//"|"
	
 show_line(1,1,318,1, 0);    //"-"
 show_line(1,1,1,238, 0);    //"|"
 show_line(1,238,318,238, 0);//"/"
 show_line(318,1,318,238, 0);//"|"	
	
// show_line(0,0,239,239, 0);
// show_line(0,239,239,0, 0);
// show_line(239,0,239,239, 0);
   delay(500);

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
 delay(500);
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
  delay(500);
 }
 j = 1;
 while(j--)
 {
  for(i = 1; i< 118; i+=6) 
  { 
   show_circle(159,119,i, 1);
   delay(500);
  }
  for(i = 118; i > 2; i-=3) 
  { 
   show_circle(159,119,i, 0);
   delay(500); 
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
// show_pic (0, 0, tab1, 0);
//	delay(10);
//demo_line();
 show_pic (0, 0, tab2, 0);

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
  delay(500);
  show_sine(119, 80, 5, 1);
  delay(500);
 }
}
/******************************************************* 
display scroll  显示滚动
*******************************************************/
void demo_roll(void)
{
// lcd_init();
 lcd_cls(0);
show_char(0, 0, "EastRising technology  www.buy-display.com ");
show_char(0, 3, "!*^_^*!  EastRising technology  www.buy-display.com");
show_char(0, 29, "X");
show_char(39, 29, "Y");
show_char(19, 17, "Z");
roll_h(5,0,0);
roll_h(5,1,0);
roll_v(10,0,0);
roll_v(10,1,0);
show_char(16, 15, "!*^_^*!");
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
// lcd_init();
 lcd_cls(0);
 show_hzg(0, 0,  tab, 0);
 show_hzg(0, 32, tab, 1);
 load_cgram(4,0);
 show_hzt(0,2,0x80);
 show_hzt(2,2,0x84);
 show_hzt(4,2,0x88);
 show_hzt(6,2,0x8c);
 delay(50000);
}
/******************************************************* 
show moving circle   显示移动圆圈
*******************************************************/
void demo_game(void)
{uchar cs=500;
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
  show_circle(x1, y1, r, 1);   //Disappear before a round
  show_pixel(x1, y1, x&0x01);  //Locus
 }
}


void DisplayDots(unsigned num1, unsigned num2)
{
    uchar y;
    uint i;

    for (y = 0; y < 240; y++)  // RW8835 支持 240 行
    {
        locate_xy2(0, y);       // 设置当前行的起始位置为 X=0

        write_cmd(mwrite);      // 进入图形写入模式

        for (i = 0; i < 40; i++) // 每行写 320 像素 / 8 = 40 字节
        {
            if (y % 2 == 1)
                write_dat(num1);  // 奇数行：写满（点亮）
            else
                write_dat(num2);  // 偶数行：清零（熄灭）
        }
    }
}


uint ReadKey1() // 按键检测函数
{
    if (LCD_KEY == 1)           
	{  // 按键被按下
        delay(10);  // 简单的去抖动处理
        if (LCD_KEY == 1) 
		{  // 检查按键是否仍然按下
            while (LCD_KEY == 1);  // 等待按键释放
            return 1;  // 返回按键被按下
        }
    }
    return 0;  // 按键没有被按下
}

void DisplayFullCGROM(void)
{
    uchar x, y;
    uchar charCode = 0x01;  // CGROM 从 0x01 开始，0x00 是空格

    for (y = 0; y < 30; y++)      // 30 行（240 / 8 = 30）
    {
        for (x = 0; x < 40; x++)  // 40 列（320 / 8 = 40）
        {
            locate_xy1(x, y);        // 文字区定位
            write_cmd(mwrite);       // 写入模式
            write_dat(charCode);     // 写入 CGROM 字符码

            charCode++;
            if (charCode == 0x00)    // 跳过 0x00（一般是空格）
                charCode = 0x01;
        }
    }

show_char(0, 0, "!*^_^*!ABCDEFG12345678909876543");
show_char(1, 3, "1234567890EDCWFRWEGTEHRTYJHUMQ234675634");
show_char(7, 5, "1234567890EDCWFRWEGTEHRTYJHUMQ2");
show_char(1, 6, "1234567890EDCWFRWEGTEHRTYJHUMQ21234567890EDCWFRWEGTEHRTYJHUMQ2");	
show_char(23, 9, "1234567890EDCWFRWEGTEHRTYJHUMQ21234567890EDCWFRWEGTEHRTYJHUMQ2");	
show_char(38, 11, "1234567890EDCWFRWEGTEHRTYJHUMQ21234567890EDCWFRWEGTEHRTYJHUMQ2");	
show_char(37, 15, "1234567890EDCWFRWEGTEHRTYJHUMQ21234567890EDCWFRWEGTEHRTYJHUMQ2");
show_char(0, 18, "1234567890EDCWFRWEGTEHRTYJHUMQ21234567890EDCWFRWEGTEHRTYJHUMQ2");	
show_char(0, 24, "1234567890EDCWFRWEGTEHRTYJHUMQ21234567890EDCWFRWEGTEHRTYJHUMQ2");
}
