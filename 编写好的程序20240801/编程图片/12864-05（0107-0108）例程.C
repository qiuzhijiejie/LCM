//,单片机:89S52,STC完全兼容
//晶振:12M,编译软件:KEIL 7.06
//并行连接方式,P2.0-RS,P2.1-RW,P2.2-E

#include<reg52.h>  
#include <intrins.h>
sbit RS=P2^0; sbit RW=P2^1; sbit E=P2^2;  
sbit stop=P3^2;
sbit CSB=P2^4;   //右半屏片选(高有效）
sbit CSA=P2^3;   //左半屏片选(高有效）

typedef unsigned int uint;
typedef unsigned char uchar;
uchar m1,z,z1,d,d1,s,s1,s10,s100;
uchar code ascii_table_8x16[95][16];
uchar code ascii_table_5x8[95][5];


 
const uchar delay=250;  //延时时间常数
static void Wait1ms(void)//延迟1 ms
{
  uchar cnt=0;
  while (cnt<delay) cnt++;
}
//延迟n ms 
void WaitNms(int n)
{
    uchar i;
    for(i=1;i<=n;i++)
   	Wait1ms();
}

/*判断是否为忙?*/
void Busy(void)
{
	uchar temp;
    RS=0;
    RW=1;
	P1=0xff;
	while(1)
  {
    E=1;
	temp=P1; 					//读状态字
    E=0;
	if ((temp&0x80)==0) 
    break;  				    //判断忙标志是否为0
  }	
}
void wrcomL(uchar command)   //写左半屏指令    
{   
    Busy();
 
	CSA=1;
    CSB=0;
	RW=0;
	RS=0;
	P1=command;
	E=1;
	E=0;

}
void wrcomR(uchar command) //写右半屏指令
{
    Busy();
    CSA=0;
	CSB=1;

	RW=0;
	RS=0;
	P1=command;
	E=1;
	E=0;

}
void wrdataL(uchar DATA) //写左半屏数据
{
    Busy();
 
	CSA=1;
	CSB=0;
	RW=0;
	RS=1;
	P1=DATA;
	E=1;
	E=0;

}
void wrdataR(uchar DATA) //写左半屏数据
{
    Busy();
 
	CSA=0;
	CSB=1;
	RW=0;
	RS=1;
	P1=DATA;
	E=1;
	E=0;

}


void dispsameone(uchar k)//把一个字节内容填满全屏
{
  uchar i;
  uchar j;
  wrcomL(0xc0);//设置C0为首行
  wrcomL(0xb8);//设置第一页
  wrcomL(0x40);//设置第一列

  wrcomR(0xc0);//设置C0为首行
  wrcomR(0xb8);//设置第一页
  wrcomR(0x40);//设置第一列
  for(j=0;j<8;j++)//再写下一行
 {wrcomR(j+0xB8);
  wrcomL(j+0xB8);
  for(i=0;i<64;i++)//写满一行
  {
  wrdataL(k);//指针自动加一
  wrdataR(k);
  }
 }
}
void dispsametow(uchar k,q)//把两个紧靠的字节内容填满全屏
{
  uchar i;
  uchar j;
  wrcomL(0xc0);
  wrcomL(0xb8);
  wrcomL(0x40);

  wrcomR(0xc0);
  wrcomR(0xb8);
  wrcomR(0x40);
  for(j=0;j<8;j++)//再写下一行
 {wrcomR(j+0xB8);
  wrcomL(j+0xB8);
  for(i=0;i<32;i++)//写满一行
  {
  wrdataL(k);//指针自动加一
  wrdataL(q);//指针自动加一
  wrdataR(k);
  wrdataR(q);
  }
 }
}

void display_string_8x16(uchar hh,uint page,uint column,uchar *text)
{
uint i=0,j,k,n;
	while(text[i]>0x00)
	{
		if((text[i]>=0x20)&&(text[i]<=0x7e))
		{
			j=text[i]-0x20;
			for(n=0;n<2;n++)
			{				
				wrcomL(0xc0);wrcomR(0xc0);
				wrcomL(0xb8+page+n);wrcomR(0xb8+page+n);
				if(column<64)
				{
					wrcomL(0x40+column);
					for(k=0;k<8;k++)
					if(hh==0)
					{
					wrdataL(ascii_table_8x16[j][k+8*n]);					
					}
					else
					{
					wrdataL(~(ascii_table_8x16[j][k+8*n]));					
					}
				}
				else
				{
					wrcomR(0x40+(column-64));
					for(k=0;k<8;k++)
					if(hh==0)
					{wrdataR(ascii_table_8x16[j][k+8*n]);					
					}
					else
					{wrdataR(~(ascii_table_8x16[j][k+8*n]));					
					}
				}				
			}
			i++;
			column+=8;
		}
	else
	i++;
	}
}
 
void display_string_8x16_t(uchar hh,uint page,uint column,uchar text)
{
	uint j,k,n;
	j=text+16;
	for(n=0;n<2;n++)
	{				
		wrcomL(0xc0);wrcomR(0xc0);
		wrcomL(0xb8+page+n);wrcomR(0xb8+page+n);
		if(column<64)
		{
			wrcomL(0x40+column);
			for(k=0;k<8;k++)
			if(hh==0)
			{
			wrdataL(ascii_table_8x16[j][k+8*n]);					
			}
			else
			{
			wrdataL(~(ascii_table_8x16[j][k+8*n]));					
			}
		}
		else
		{
			wrcomR(0x40+(column-64));
			for(k=0;k<8;k++)
			if(hh==0)
			{wrdataR(ascii_table_8x16[j][k+8*n]);					
			}
			else
			{wrdataR(~(ascii_table_8x16[j][k+8*n]));					
			}
		}				
	}
		
} 
 
 
void display_string_5x8(uint page,uint column,uchar *text)
{
uint i=0,j,k;
	while(text[i]>0x00)
	{
	if((text[i]>=0x20)&&(text[i]<0x7e))	
	{
		j=text[i]-0x20;
		wrcomL(0xc0);wrcomR(0xc0);
		wrcomL(0xb8+page);wrcomR(0xb8+page);
		if(column<64)
		{
			wrcomL(0x40+column);
			for(k=0;k<5;k++)					
			{wrdataL(ascii_table_5x8[j][k]);
			}
		}	
		else
		{
			wrcomR(0x40+(column-64));
			for(k=0;k<5;k++)					
			{wrdataR(ascii_table_5x8[j][k]);
			}
		}	
		i++;
		column+=6;							
	}	
	else
	i++;
	}
}

//显示128x64 点阵边框
void display_bk()
{
uint i,j;
//左框
for(j=0;j<8;j++)
	{
	wrcomL(0xb8+j);
	wrcomL(0x40+0);
	wrdataL(0xff); 
	wrdataL(0xff); 
	}
	//右框
	for(j=0;j<8;j++)
	{
	wrcomR(0xb8+j);
	wrcomR(0x40+62);
	wrdataR(0xff); 
	wrdataR(0xff); 
	}
	//上框
	wrcomL(0xb8+0);wrcomR(0xb8+0);
	wrcomL(0x40+2);wrcomR(0x40+0);	
	for (i=0;i<62;i++)
	{
	wrdataL(0x03); 
	wrdataR(0x03); 
	}
	//下框
	wrcomL(0xb8+7);wrcomR(0xb8+7);
	wrcomL(0x40+2);wrcomR(0x40+0);	
	for (i=0;i<62;i++)
	{
	wrdataL(0xC0); 
	wrdataR(0xC0); 
	}
}

//显示16x16 点阵图像、汉字、生僻字或16x16 点阵的其他图标
void display_graphic_16x16(uchar hh,uchar page,uchar column,uchar *dp )
{
uint i,j;
	for(j=0;j<2;j++)
	{
		wrcomL(0xb8+page+j);wrcomR(0xb8+page+j);
		if(column<64)
		{
		wrcomL(0x40+column);
			for (i=0;i<16;i++)
			{
			if(hh==0) wrdataL(*dp);
			else wrdataL(~(*dp)); //写数据到LCD,每写完一个8 位的数据后列地址自动加1
			dp++;
			}
		}
		else
		{
		wrcomR(0x40+(column-64));
			for (i=0;i<16;i++)
			{
			if(hh==0) wrdataR(*dp);
			else wrdataR(~(*dp)); //写数据到LCD,每写完一个8 位的数据后列地址自动加1
			dp++;
			}
		}			
	}
}

void lcdint(void)
{
wrcomR(0x3e);//显示关
wrcomR(0x3f);//显示开
wrcomL(0x3e);
wrcomL(0x3f);
wrcomL(0xC0);//首行
wrcomR(0xC0);//首行
}

void ini_int1(void) 
{ 
EA=1; 
EX0=1;//允许外部INT0的中断 
IT0=1;// 允许中断
}

 int scankey1() interrupt 0 using 3 //使用外部中断1,寄存器组3
{ 
while(P3^2==0){for(;;);}
   IE1=0;//中断标志清零    
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~程序从这里开始
void main(void)
{
ini_int1();//开中断
for(m1=0;m1<50;m1++)
	{
	lcdint();
	dispsameone(0x00);//清屏
	display_string_8x16(0,0,0,"0123456789ABCDEF");
	display_string_8x16(0,2,0,"HUIJINGKEJI12864");
	display_string_5x8(4,4,"0123456789ABCDEFGHIJ");
	display_string_5x8(5,4,"**TEL:0755-23146001*");
	display_string_5x8(6,4,"**FAX:0755-23146002*");
	display_string_5x8(7,4,".www.huijinglcm.com.");
	WaitNms(250);
	WaitNms(150);
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~1,（5*8和8*16字符显示）

	dispsameone(0x00);//清屏
	display_graphic_16x16(0,2,32,hui1); 	
	display_graphic_16x16(0,2,48,jing1);
	display_graphic_16x16(0,2,64,ke1); 
	display_graphic_16x16(0,2,80,ji1); 
	display_string_8x16(0,4,40,"128*64");
	display_bk();
	WaitNms(250);
	WaitNms(150);
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~2,（自编边框/16*16汉字显示 ）

	dispsameone(0x00);//清屏
	dispsameone(0xFF);//清屏
	display_graphic_16x16(1,2,32,hui1); 	
	display_graphic_16x16(1,2,48,jing1); 	
	display_graphic_16x16(1,2,64,ke1); 	
	display_graphic_16x16(1,2,80,ji1); 	
	display_string_8x16(1,4,40,"128*64");
	WaitNms(250);
	WaitNms(150);
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~3,（灵活的反白显示演示）

	dispsameone(0x55);//显示横扫描1
	WaitNms(150);
	WaitNms(150);
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~4,扫横1
	dispsameone(0xaa);//显示横扫描2
	WaitNms(150);
	WaitNms(150);
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~5,扫横2
	dispsametow(0xff,0x00);//显示竖扫描1
	WaitNms(150);
	WaitNms(150);
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~6,扫竖1
	dispsametow(0x00,0xff);//显示竖扫描2
	WaitNms(150);
	WaitNms(150);
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~7,扫竖2
	dispsametow(0xaa,0x55);//显示点扫描1
	WaitNms(150);
	WaitNms(150);
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~8,扫点1
	dispsametow(0x55,0xaa);//显示点扫描2
	WaitNms(150);
	WaitNms(150);
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~9,扫点2
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~10,时钟演示
	dispsameone(0x00);//清屏
	display_graphic_16x16(1,0,0,hui1); 	
	display_graphic_16x16(1,2,0,jing1); 
	display_string_8x16(1,5,0,"TEL:755-23146001");
	display_string_5x8(7,4,".www.huijinglcm.com.");
	display_string_5x8(0,40,"<<<<<<<<<<<<<<");
	display_string_5x8(3,40,"<<<<<<<<<<<<<<");
	for(z=0;z<10;z++)
	{
		display_string_8x16_t(0,1,40,z);		
		for(z1=0;z1<10;z1++)
		{	
			display_string_8x16_t(0,1,48,z1);		
			for(d=0;d<6;d++)
			{	
				display_string_8x16_t(0,1,56,10);	
				display_string_8x16_t(0,1,64,d);	
				for(d1=0;d1<10;d1++)
				{	
					display_string_8x16_t(0,1,72,d1);	
					for(s=0;s<6;s++)
					{					
						display_string_8x16_t(0,1,80,10);	
						display_string_8x16_t(0,1,88,s);		
						for(s1=0;s1<10;s1++)
						{
							display_string_8x16_t(0,1,96,s1);			
							WaitNms(5);////延时 x ms	
							display_string_8x16_t(0,1,104,10);	
							for(s10=0;s10<10;s10++)
							{
								display_string_8x16_t(0,1,112,s10);				
								WaitNms(5);////延时 x ms	
								for(s100=0;s100<10;s100++)
								{	
								display_string_8x16_t(0,1,120,s100);	
								WaitNms(5);////延时 x ms	
								}
							}
						}
					}
				}
			}	
		}	
	}	
			
	
	
}


