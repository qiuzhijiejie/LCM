#include <INTRINS.H>   
#include <reg52.h>
#include "lcd12832.h"
#include "picture.h"
sbit RS   = P3^0;             
sbit RW   = P3^1;                  
sbit E    = P3^2;              
sbit KEY     = P2^0; 

unsigned char ROW,COL;             
        

void charlcdfill(unsigned int c)        //整屏显示A代表的ASCII字符子程序
{   for(ROW=COL=0;1;)
    {   putchar(c);                     //定位写字符
        if((ROW==0) && (COL==0)) break;
    }
}

void putchar(unsigned int c)            //定位写字符子程序
{   unsigned char d;
    if(c>128)                           //字符码大于128表示为汉字
    {   if( (ROW&0x1)==1 )            //写汉字时,ROW字符位置在奇数位置,则
        {   lcdwd(' ');                 //自动补添一个空格对齐后再显示汉字
            charcursornext();           //置字符位置为下一个有效位置
        }
        charlcdpos();
        if((c&0xff00)==0xff00)          //若高位字节为0FFH则表示为自定义字符
            c=0xffff-c;                 //则转换为ST7920的字符码
        lcdwd(c/256);                   //写高位字符
        charcursornext();
        lcdwd(c&0xff);                  //写低位字符
        charcursornext();
    }
    else                                //字符码小于128表示为ASCII字符
    {   charlcdpos();
        if( (ROW&0x1)==1 )            //写ASCII字符时,ROW字符位置在奇数位置,则
        {   d=lcdrd();                  //读高位字符
            charlcdpos();
            lcdwd(d);                   //重新写高位字符
            lcdwd(c);
        }
        else
        {   lcdwd(c);                   //写ASCII字符时,ROW字符位置在偶数位置,则
            lcdwd(' ');                 //直接写入,同时自动补显一个空格.
        }
        charcursornext();               //置字符位置为下一个有效位置
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
    ROW=cx;                           //置当前X位置为cx
    COL=cy;                           //置当前Y位置为cy
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
   x=ROW;
   for(ly=0;ly<YSIZE;ly++,COL++)
   {  for(ROW=x,lx=0;lx<XSIZE;)
      {  if((ROW&0x08)==0) glcdpos();
         lcdwd(*s);
         s++;
         lx+=8;
         ROW+=8;
      }
   }
   ROW=x;
}

void putimage(unsigned char code *s)    //显示图形子程序
{   unsigned char XSIZE,YSIZE;
    XSIZE=*s;
    s++;
    YSIZE=*s;
    s++;
    putsizeimage(XSIZE,YSIZE,s);
}

void charlcdpos(void)                   //设置坐标点(ROW,COL)内部RAM地址的子程序
{  unsigned char ddaddr;
   ROW&=0xf;                          //X位置范围(0到15)
   ddaddr=ROW/2;
    if(COL==0)                        //(第一行)X: 第0----15个字符
        lcdwc(ddaddr|0x80);             //    DDRAM:  80----87H
    else if(COL==1)                   //(第二行)X: 第0----19个字符
        lcdwc(ddaddr|0x90);             //    DDRAM:  90----07H
    else if(COL==2)                   //(第三行)X: 第0----19个字符
        lcdwc(ddaddr|0x88);             //    DDRAM:  88----8FH
    else                                //(第四行)X: 第0----19个字符
        lcdwc(ddaddr|0x98);             //    DDRAM:  98----9FH
}

void charcursornext(void)               //置字符位置为下一个有效位置子程序
{
    ROW++;                            //字符位置加1
    ROW&=0x0f;                        //字符位置ROW的有效范围为(0到15)
    if(ROW==0)                        //字符位置ROW=0表示要换行
    {   COL++;                        //行位置加1
        COL&=0X3;                     //字符位置COL的有效范围为(0到3)
    }
}

void lcdreset(void)                     //液晶显示控制器初始化子程序
{   lcdwc(0x33);                        //接口模式设置
    delay3ms();                         //延时3MS
    lcdwc(0x30);                        //基本指令集
    delay3ms();                         //延时3MS
    lcdwc(0x30);                        //重复送基本指令集
    delay3ms();                         //延时3MS
    lcdwc(0x01);                        //清屏控制字
    delay3ms();                         //延时3MS
    lcdon();                            //开显示
}
void delay3ms(void)                     //延时3MS子程序
{ unsigned char i,j,k;
  for(i=0;i<3;i++)
    for(j=0;j<64;j++)
      for(k=0;k<51;k++);
}

void glcdpos(void)                      //写入绘图区域内部RAM指针子程序
{   
    lcdwc(0x36);                        //扩展指令集
    lcdwc((COL & 0x1F) | 0x80);       //送Y地址 (Y范围调整到 0~31)
    lcdwc((ROW / 16) | 0x80);         //送X地址 (删除对 COL 的 32 检查逻辑)
    lcdwc(0x30);                        //恢复为基本指令集
}

void lcdon(void)                        //LCD显示开启子程序
{   lcdwc(0x30);                        //设置为基本指令集
    lcdwc(0x0c);
}

void lcdoff(void)                       //LCD显示关闭子程序
{   lcdwc(0x30);                        //设置为基本指令集
    lcdwc(0x08);
}

void lcdgraphon(void)                  //绘图区域显示开启子程序
{   lcdwc(0x36);
    lcdwc(0x30);                        //恢复为基本指令集
}

void lcdgraphoff(void)                  //绘图区域显示关闭子程序
{   lcdwc(0x34);
    lcdwc(0x30);                        //恢复为基本指令集
}

unsigned char lcdrd(void)               //从液晶显示控制器读数据
{   unsigned char d;
    lcdwaitidle();                      //ST7920液晶显示控制器忙检测
    RS=1;                            //RS=1 RW=1 E=高脉冲
    RW=1;
    E=1;
    _nop_();
    d=P1;
    E=0;
    return d;
}

void lcdwd(unsigned char d)             //向液晶显示控制器写数据
{   lcdwaitidle();                      //ST7920液晶显示控制器忙检测
    P1=d;
    RS=1;                            //RS=1 RW=0 E=高脉冲
    RW=0;
    E=1;
    _nop_();
    E=0;
}

void lcdwc(unsigned char c)             //向液晶显示控制器送指令
{   lcdwaitidle();                      //ST7920液晶显示控制器忙检测
    P1=c;
    RS=0;                            //RS=0 RW=0 E=高脉冲
    RW=0;
    E=1;
    _nop_();
    E=0;
}

void lcdwaitidle(void)                  //控制器忙检测子程序
{   unsigned char i;
    P1=0xff;
    RS=0;
    RW=1;
    E=1;
    for(i=0;i<20;i++)                   //
        if( (P1&0x80) != 0x80 ) break;  //D7=0 空闲退出
    E=0;
}

void exdelay(void)                      //演示延时子程序
{ unsigned char i,j,k;                  //延时约300mS
  for(i=0;i<30;i++)
    for(j=0;j<64;j++)
      for(k=0;k<51;k++);
}



void glcdfill(unsigned char d)          //整屏显示d代表的字数据子程序
{   unsigned char i;
    ROW=0;
    COL=0;
    while(COL<32)
    {   glcdpos();
        for(i=0;i<16;i++)
            lcdwd(d);
        COL++;
    }
    COL=0;
}

void glcdfill_with_params(unsigned char oddData, unsigned char evenData)
{
    unsigned char i;
    ROW = 0;
    COL = 0;

    while (COL < 32)  // 遍历屏幕的 32 行
    {
        glcdpos();  // 设置当前行的显示地址

        for (i = 0; i < 16; i++)  // 遍历当前行的16字节
        {
            if (COL % 2 == 0)  // 偶数行
            {
                lcdwd(evenData);  // 写入偶数行数据
            }
            else  // 奇数行
            {
                lcdwd(oddData);  // 写入奇数行数据
            }
        }

        COL++;  // 切换到下一行
    }
    COL = 0;  // 重置 Y 坐标
}

unsigned char key()
{
	if(KEY==0)
	{
		exdelay();
		while(KEY==0);
		return 1;
	}
	return 0;
}

void HandleKey() 
{
	unsigned char mode;
	
		if (key()) 
	  {  
		mode++;
		mode %= 7;			
		switch (mode) 
	  {
        case 0:            
			glcdfill_with_params(0xAA,0xAA);break;
        case 1:
            glcdfill_with_params(0x55,0xAA);break;
        case 2:
            glcdfill_with_params(0xAA,0x55);break;
        case 3:
            glcdfill_with_params(0x00,0xFF);break;
        case 4:
            glcdfill_with_params(0xFF,0x00);break;
        case 5:
            glcdfill_with_params(0x55,0x55);break;
		case 6:
			glcdfill_with_params(0xFF,0xFF);break;

		}
	}
}

void DisplayPatterns() 
{
    static int step = 0; // 当前显示图案步骤
    switch (step) 
    {
        case 0: glcdfill_with_params(0x00,0xFF); exdelay(); break;
        case 1: glcdfill_with_params(0x55,0xAA); exdelay(); break;
        case 2: glcdfill_with_params(0xAA,0xAA); exdelay(); break;
        case 3: glcdfill_with_params(0xAA,0x55); exdelay(); break;
        case 4: glcdfill_with_params(0x55,0x55); exdelay(); break;
        case 5: glcdfill_with_params(0xFF,0x00); exdelay(); break;
        case 6: glcdfill_with_params(0xFF,0xFF); exdelay(); break;
        case 7: lcdreset();charlcdfill(' ');putstrxy(0,0,"烨辉科技有限公司");putstrxy(2,1,"LCM12832_02 "); exdelay(); 
    }
    step++;
    if (step > 7) step = 0; // 循环
}
