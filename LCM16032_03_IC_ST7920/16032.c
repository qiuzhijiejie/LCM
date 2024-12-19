#include <REGX52.H>
#include "16032.h"
#include "picture.h"
#include <intrins.h>

sbit RS   = P3^0;                    // RS对应单片机引脚
sbit RW   = P3^1;                    // RW对应单片机引脚
sbit E    = P3^2;                    // E对应单片机引脚
sbit KEY=P2^0;			//按键

unsigned char Column; //列方向地址指针(用于setLCDCursor子程序)
unsigned char row;    //行方向地址指针(用于setLCDCursor子程序)

void lcdwaitidle(void)       //控制器忙检测子程序
{   unsigned char i;
    P1=0xff;
    RS=0;
    RW=1;
    E=1;
    for(i=0;i<20;i++)//这个循环最多检查20次  
	{                
        if( (P1&0x80)!= 0x80 )
		{
			break;  //D7=0 空闲退出
		}
	}
    E=0;
}


unsigned char lcdrd(void)  //从液晶显示控制器读数据
{   unsigned char d;
    lcdwaitidle();                  
    RS=1;                        
    RW=1;				   //RW 1: read
    E=1;
    _nop_();
    d=P1;
    E=0;
    return d;
}

void lcdwd(unsigned char d) //向液晶显示控制器写数据
{   lcdwaitidle();            
    P1=d;
    RS=1;                   //RS 1:select data write, read       
    RW=0;					//RW 0: write
    E=1;
    _nop_();
    E=0;
}

void lcdwc(unsigned char c)  //向液晶显示控制器送指令
{   lcdwaitidle();           
    P1=c;
    RS=0;           		 //RS 0:select instruction write,        
    RW=0;
    E=1;
    _nop_();
    E=0;
}


void setLCDCursor(void)           //写入绘图区域内部RAM指针子程序
{   
	lcdwc(0x36);             //扩展指令集
    lcdwc((row&0x1f)|0x80);  //先送Y地址,范围是0~31
    lcdwc((Column/16)|0x80); //再送X地址
    lcdwc(0x30);             //恢复为基本指令集
}

void lcdon(void)    //LCD显示开启子程序
{   
	lcdwc(0x30);    //设置为基本指令集
    lcdwc(0x0c);
}

void lcdoff(void)   //LCD显示关闭子程序
{   lcdwc(0x30);    //设置为基本指令集
    lcdwc(0x08);
}

void lcdgraphon(void) //绘图区域显示开启子程序
{   lcdwc(0x36);
    lcdwc(0x30);      //恢复为基本指令集
}

void lcdgraphoff(void) //绘图区域显示关闭子程序
{   lcdwc(0x34);
    lcdwc(0x30);       //恢复为基本指令集
}

void lcdreset(void)                     //液晶显示控制器初始化子程序
{   lcdwc(0x33);                        //接口模式设置
    delay3ms();                         //延时3MS
    lcdwc(0x30);                        //基本指令集
    delay3ms();                         //延时3MS
    lcdwc(0x01);                        //清屏控制字
    delay3ms();                         //延时3MS
    lcdwc(0x06);                        //显示光标移动设置
    lcdon();                            //开显示
}

void delay3ms(void)                     //延时3MS子程序
{ unsigned char i,j,k;
  for(i=0;i<3;i++)
    for(j=0;j<64;j++)
      for(k=0;k<51;k++);
}



void fillLCD_OddColumn(unsigned char oddData, unsigned char evenData)  
{
    unsigned char i; 
    while (row < 32)   
    {
        setLCDCursor();   
        if (row % 2 == 0)   
        {
            for (i = 0; i < 20; i++)
                lcdwd(evenData);   
        }
        else   
        {
            for (i = 0; i < 20; i++)
                lcdwd(oddData);   
        }
        row++;   
    }
    row = 0;   
}

void fillLCD(unsigned char d) //整屏显示d代表的字数据子程序
{   
	unsigned char i;
    while(row<32)
    {   
		setLCDCursor();
        for(i=0;i<20;i++)
		{
            lcdwd(d);
		}
        row++;
    }
    row=0;
}



void exdelay(void)                      //演示延时子程序
{ unsigned char i,j,k;                  //延时约300mS
  for(i=0;i<30;i++)
    for(j=0;j<64;j++)
      for(k=0;k<51;k++);
}


void charlcdfill(unsigned int c)//整屏显示ST7920内部的CGROMA程序
{   for(Column=row=0;1;)
    {   putchar(c);                
		c+=1;
        if((Column==0) && (row==0)) break;
    }
}

void Clean(unsigned int c)        
{  
	for(;;)
    {   putchar(c);            
        if((Column==0) && (row==0)) break;
    }
}

void putchar(unsigned int c) //定位写字符子程序
{   unsigned char d;
    if(c>128)                 //字符码大于128表示为汉字
    {   if( (Column&0x1)==1 ) //写汉字时,Column字符位置在奇数位置,则
        {   lcdwd(' ');       //自动补添一个空格对齐后再显示汉字
            charcursornext(); //置字符位置为下一个有效位置
        }
        charlcdpos();
        if((c&0xff00)==0xff00)//若高位字节为0FFH则表示为自定义字符
            c=0xffff-c;       //则转换为ST7920的字符码
        lcdwd(c/256);         //写高位字符
        charcursornext();
        lcdwd(c&0xff);        //写低位字符
        charcursornext();
    }
    else                      //字符码小于128表示为ASCII字符
    {   charlcdpos();
        if( (Column&0x1)==1 ) //写ASCII字符时,Column字符位置在奇数位置,则
        {   d=lcdrd();        //读高位字符
            charlcdpos();
            lcdwd(d);         //重新写高位字符
            lcdwd(c);
        }
        else
        {   lcdwd(c);         //写ASCII字符时,Column字符位置在偶数位置,则
            lcdwd(' ');       //直接写入,同时自动补显一个空格.
        }
        charcursornext();     //置字符位置为下一个有效位置
    }
}

void putstr(unsigned char code *s)      //显示字符串子程序,字符码为0时退出
{   unsigned int c;
    while(1)
    {   c=*s;
        s++;
        if(c==0) break;
        if(c<128)
            putchar(c);
        else
        {   putchar(c*256+*s);
            s++;
        }
    }
}

void putstrxy(unsigned char cx,unsigned char cy,unsigned char code *s)
{                                       //在(cx,cy)字符位置写字符串子程序
    Column=cx;                           //置当前X位置为cx
    row=cy;                           //置当前Y位置为cy
    putstr(s);
}

void setcharcgram(unsigned char cindex,unsigned char code *s)
{   unsigned char i;
    lcdwc(0x34);                        //扩充指令集,图形模式关闭
    lcdwc(0x02);                        //SR=0 激活CGRAM
    lcdwc(0x30);                        //恢复为基本指令集
    lcdwc( ((cindex&0x3)<<4) | 0x40);   //设置CGRAM地址
    for(i=0;i<32;i++,s++)               //写点阵数据
    lcdwd(*s);
}


void putsizeimage(unsigned char XSIZE,unsigned char YSIZE,unsigned char code *s)
{  unsigned char lx,ly,x;
   x=Column;
   for(ly=0;ly<YSIZE;ly++,row++)
   {  for(Column=x,lx=0;lx<XSIZE;)
      {  if((Column&0x08)==0) setLCDCursor();
         lcdwd(*s);
         s++;
         lx+=8;
         Column+=8;
      }
   }
   Column=x;
}

void putimage(unsigned char code *s)    //显示图形子程序
{   unsigned char XSIZE,YSIZE;
    XSIZE=*s;
    s++;
    YSIZE=*s;
    s++;
    putsizeimage(XSIZE,YSIZE,s);
}

void charlcdpos(void)       //设置坐标点(Column,row)内部RAM地址的子程序
{   unsigned char ddaddr;
    if(Column>19) Column=0; //字符位置Column的有效范围为(0到19)
    row&=0X1;               //字符位置row的有效范围为(0到1)
    ddaddr=Column/2;
    if(row==0)              //(第一行)X: 第0----19个字符
        lcdwc(ddaddr|0x80); // DDRAM:  80----89H
    else                    //(第二行)X: 第0----19个字符
        lcdwc(ddaddr|0x90); //    DDRAM:  90----99H
}

void charcursornext(void)    //置字符位置为下一个有效位置子程序
{
    Column++;                //字符位置加1
    if(Column>19) Column=0;  //字符位置Column的有效范围为(0到19)
    if(Column==0)            //字符位置Column=0表示要换行
    {   row++;               //行位置加1
        row&=0X1;            //字符位置row的有效范围为(0到1)
    }
}

unsigned char ReadKey() //按键函数
{
    if (KEY == 0) 
		{  // 按键被按下
			exdelay();  // 简单的去抖动处理
			if (KEY == 0) 
				{
					while (KEY == 0);  // 等待按键释放
					return 1;  // 返回按键被按下
				}
    }
    return 0;  // 返回按键没有被按下
}


void HandleKey() 
{
	unsigned char mode;
	
		if (ReadKey()) 
	  {  
		mode++;
		mode %= 7;			
		switch (mode) 
	  {
        case 0:            
			fillLCD_OddColumn(0xAA,0xAA);exdelay(); break;
        case 1:
            fillLCD_OddColumn(0x55,0xAA);exdelay();break;
        case 2:
            fillLCD_OddColumn(0xAA,0x55);exdelay();break;
        case 3:
            fillLCD_OddColumn(0x00,0xFF);exdelay();break;
        case 4:
            fillLCD_OddColumn(0xFF,0x00);exdelay();break;
        case 5:
            fillLCD_OddColumn(0x55,0x55);exdelay();break;
		case 6:
			fillLCD_OddColumn(0xFF,0xFF);exdelay();break;
		}
	}
}

void DisplayPatterns() 
{
    static int step = 0; // 当前显示图案步骤
    switch (step) 
    {
        case 0: fillLCD_OddColumn(0x00,0xFF);exdelay(); break;
        case 1: fillLCD_OddColumn(0x55,0xAA);exdelay(); break;
        case 2: fillLCD_OddColumn(0xAA,0xAA);exdelay(); break;
        case 3: fillLCD_OddColumn(0xAA,0x55);exdelay(); break;
        case 4: fillLCD_OddColumn(0x55,0x55);exdelay(); break;
        case 5: fillLCD_OddColumn(0xFF,0x00);exdelay(); break;
        case 6: fillLCD_OddColumn(0xFF,0xFF);exdelay(); break;
        case 7: fillLCD_OddColumn(0x00, 0x00);putstrxy(0,0,"广东烨辉科技有限公司");putstrxy(3,1," LCM16032_03 "); exdelay(); break;
    }
    step++;
    if (step > 7) step = 0; // 循环
}