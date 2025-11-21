// 驱动IC是:ST7525 

#include <reg52.h>
//#include <STC15F2K60S2.H>
#include <intrins.h>
#include <Chinese_code.h>

//=============================
sbit cs1=P1^0;		/*对应LCD的CS引脚*/
sbit rs=P3^2;		  /*对应LCD的RS引脚*/

sbit sclk=P1^3;   //对应LCD的SCK(D0)
sbit sid=P1^2;    //对应LCD的SDA(D1)
sbit reset=P1^1;	/*对应LCD的RST引脚*/
sbit key=P2^0;
//=============================
#define DataBus P1

void delay_us(int i); 
void delay(int i); 


//写指令到LCD模块
void transfer_command(int data1)   
{
	char i;
	cs1=0;
	rs=0;
	for(i=0;i<8;i++)
	{
		sclk=0;
		if(data1&0x80) sid=1;
		else sid=0;
		sclk=1;
		data1=data1<<=1;
	}
	cs1=1;
}

//写数据到LCD模块
void transfer_data(int data1)
{
	char i;
	cs1=0;
	rs=1;
	for(i=0;i<8;i++)
	{
		sclk=0;
		if(data1&0x80) sid=1;
		else sid=0;
		sclk=1;
		data1=data1<<=1;
	}
	cs1=1;
}
//延时1
void delay(int i)               
{
 int j,k;
 for(j=0;j<i;j++)
 for(k=0;k<110;k++);
}

//延时2
void delay_us(int i)               
{
 int j,k;
 for(j=0;j<i;j++)
 for(k=0;k<10;k++);
}

void waitkey()
{
repeat:	
	if(key==1)goto repeat;
	else delay(2500);
}

//LCD模块初始化
//=======对比度设置值0xa5=======//
void initial_lcd()
{
	reset=0;        //低电平复位
	delay(100);
	reset=1;		    //复位完毕
	delay(200);        
	transfer_command(0xe2);	 //软复位
	delay(200);
	transfer_command(0x2f);  //打开内部升压
	delay(200);
	transfer_command(0xa0);  //
	transfer_command(0x81);  //微调对比度V0-XV0=10.21v
	transfer_command(0x79);  //微调对比度的值，可设置范围0x00～0xFF
	transfer_command(0xeb);  //1/9偏压比（bias）
	transfer_command(0xc4);  //行列扫描顺序：从上到下 
//	transfer_command(0xc6);  //行列扫描顺序：从上到下 
	transfer_command(0xaf);  //开显示	
}


void lcd_address(uchar page,uchar column)
{
	column=column-1;  							//我们平常所说的第1列，在LCD驱动IC里是第0列。所以在这里减去1.
	page=page-1;
	transfer_command(0xb0+page);   			//设置页地址。每页是8行。一个画面的64行被分成8个页。我们平常所说的第1页，在LCD驱动IC里是第0页，所以在这里减去1
	transfer_command(((column>>4)&0x0f)+0x10);	//设置列地址的高4位
	transfer_command(column&0x0f);				//设置列地址的低4位
}

//全屏清屏
void clear_screen()
{
	unsigned char i,j;
 	for(i=0;i<8;i++)
	{
		lcd_address(1+i,1);
		for(j=0;j<192;j++)
		{
			transfer_data(0x00);
		}
	}
}

void display_graphic_192x64(uchar *dp)
{
	uchar i,j;
 	for(i=0;i<8;i++)
	{
		lcd_address(i+1,1);
		for(j=0;j<192;j++)
		{
			transfer_data(*dp);
			dp++;
		}
	}
}	

//==================display a piture of 128*64 dots================
void full_display(uchar data_left,uchar data_right)
{
	int i,j;
	for(i=0;i<8;i++)
	{
		lcd_address(i+1,1);
		for(j=0;j<96;j++)
		{
			transfer_data(data_left);   
			transfer_data(data_right);    			 
		}
	}          
}

//显示32x32点阵图像、汉字、生僻字或32x32点阵的其他图标
void display_graphic_32x32(uchar page,uchar column,uchar *dp)
{
	uchar i,j;
	for(j=0;j<4;j++)
	{
		lcd_address(page+j,column);
		for (i=0;i<32;i++)
		{	
			transfer_data(*dp);		//写数据到LCD,每写完一个8位的数据后列地址自动加1
			dp++;
		}
	}
}

//显示16x16点阵图像、汉字、生僻字或16x16点阵的其他图标
void display_graphic_16x16(uchar page,uchar column,uchar *dp)
{
	uchar i,j;
	for(j=0;j<2;j++)
	{
		lcd_address(page+j,column);
		for (i=0;i<16;i++)
		{	
			transfer_data(*dp);		//写数据到LCD,每写完一个8位的数据后列地址自动加1
			dp++;
		}
	}
}

//写入一组16x16点阵的汉字字符串（字符串表格中需含有此字）
//括号里的参数：(页，列，汉字字符串）
void display_string_16x16(uchar page,uchar column,uchar reverse, uchar *text)
{
	uchar i,j,k,data1;
	uint address;
	j = 0;
	while(text[j] != '\0')
	{
		i=0;
		address=1;
		while(Chinese_text_16x16[i]> 0x7e )
		{
			if(Chinese_text_16x16[i] == text[j])
			{
				if(Chinese_text_16x16[i+1] == text[j+1])
				{
					address = i*16;
					break;
				}
			}
			i +=2;
		}
		if(column>191)
		{
			column =0;
			page +=2;
		}
		if(address !=1)
		{
			for(k=0;k<2;k++)
			{
				lcd_address(page+k,column);
				for(i=0;i<16;i++)
				{
					if(reverse==1) data1=~Chinese_code_16x16[address];
					else data1=Chinese_code_16x16[address];
					transfer_data(data1);
					address++;				
				}
			}
			j +=2;
		}
		else
		{
			for(k=0;k<2;k++)
			{
				lcd_address(page+k,column);
				for(i=0;i<16;i++)
				{
					if(reverse==0) transfer_data(0x00);
					else transfer_data(0xff);			
				}
			}
			j++;
		}
		column +=16;
	}
}



//显示8x16点阵图像、ASCII, 或8x16点阵的自造字符、其他图标
void display_graphic_8x16(uchar page,uchar column,uchar *dp)
{
	uchar i,j;
	for(j=0;j<2;j++)
	{
		lcd_address(page+j,column);
		for (i=0;i<8;i++)
		{	
			transfer_data(*dp);					//写数据到LCD,每写完一个8位的数据后列地址自动加1
			dp++;
		}
	}
}

//显示8x16的点阵的字符串，括号里的参数分别为（页,列，字符串指针）
void display_string_8x16_2(uchar page,uchar column,uchar reverse, uchar *text)
{
	uchar data1;
	uint i=0,j,k,n;

	while(text[i]>0x00)
	{
		if((text[i]>=0x20)&&(text[i]<=0x7e))
		{
			j=text[i]-0x20;
			for(n=0;n<2;n++)
			{
				lcd_address(page+n,column);
				for(k=0;k<8;k++)
				{
					if(reverse==1) data1=~ascii_table_8x16[j][k+8*n];
					else data1=ascii_table_8x16[j][k+8*n];
					transfer_data(data1);
				}
				if(reverse==0)	transfer_data(0x00);
				else	transfer_data(0xff);
			}
			i++;
			column+=8;
		}
		else
		i++;
		
		if(column>127)
		{
			column=0;
			page+=2;
 		}	
	}	
}



void display_string_8x16(uint page,uint column,uchar *text)
{
	uint i=0,j,k,n;
	while(text[i]>0x00)
	{	
		if((text[i]>=0x20)&&(text[i]<=0x7e))
		{
			j=text[i]-0x20;
			for(n=0;n<2;n++)
			{			
				lcd_address(page+n,column);
				for(k=0;k<8;k++)
				{
					transfer_data(ascii_table_8x16[j][k+8*n]);//显示5x7的ASCII字到LCD上，y为页地址，x为列地址，最后为数据
				}
			}
			i++;
			column+=8;
			
		}
		else
		i++;
	}
}


//显示一串5x8点阵的字符串
//括号里的参数分别为（页，列，是否反显，数据指针）
void display_string_5x8(uint page,uint column,uchar reverse,uchar *text)
{
	uchar i=0,j,k,data1;
	while(text[i]>0x00)
	{	
		if((text[i]>=0x20)&&(text[i]<=0x7e))
		{
			j=text[i]-0x20;
			lcd_address(page,column);
			for(k=0;k<5;k++)
			{
				if(reverse==1)	data1=~ascii_table_5x8[j][k];
				else	data1=ascii_table_5x8[j][k];
				transfer_data(data1);
			}
			if(reverse==1)	transfer_data(0xff);
			else	transfer_data(0x00);
			i++;
			column+=6;
		}
		else
		i++;
	}
}

void display_string_5x8_1(uint page,uint column,uchar *text)
{
	uint i=0,j,k;
	while(text[i]>0x00)
	{	
		if((text[i]>=0x20)&&(text[i]<0x7e))
		{
			j=text[i]-0x20;
			lcd_address(page,column);
			for(k=0;k<5;k++)
			{
				transfer_data(ascii_table_5x8[j][k]);//显示5x7的ASCII字到LCD上，y为页地址，x为列地址，最后为数据
			}
			i++;
			column+=6;
		}
		else
		i++;
	}
}


void main(void)
{  
	P1=0x00;
	P1=0x00;   //P1配置为准双向
	P2=0x00;
	P2=0x00;   //P2配置为准双向
	P3=0x00;
	P3=0x00;   //P3配置为准双向	
//	BM0=1;
//	BM1=1;
	
	while(1)
	{
		initial_lcd();
		clear_screen();   
		full_display(0xff,0xff);
		waitkey();
		clear_screen();    							//clear all dots
//		display_graphic_192x64(bmp2); 
//		waitkey();
//		clear_screen();    							//clear all dots
		display_graphic_192x64(bmp1); 
		waitkey();
		clear_screen();    
		display_string_5x8(1,1,1,"             MENU                ");	//显示5x8点阵的字符串，括号里的参数分别为（页，列，是否反显，数据指针）
		display_string_5x8(3,1,0,"    Select>>>>");
		display_string_5x8(3,100,1,"1.Graphic      ");	    
		display_string_5x8(4,100,0,"2.Chinese        " );
		display_string_5x8(5,100,0,"3.Movie          ");
		display_string_5x8(6,100,0,"4.Contrast       ");
		display_string_5x8(7,100,0,"5.Mirror         ");
		display_string_5x8(8,1,1,"    PRE    USER     DEL     NEW     ");
		display_string_5x8(8,59,0," ");
		display_string_5x8(8,94,0," ");
		display_string_5x8(8,97+48,0," "); 
		waitkey();
        clear_screen();
		display_string_8x16_2(1,1,1,"--");
		display_string_16x16(1,17,1,"→粉尘测试      ");
		display_string_16x16(3,33,0,"一般测试");
		display_string_16x16(5,33,0,"校准模式");
		display_string_16x16(7,33,0,"充电模式 ↓");
		waitkey();
		clear_screen(); 
		display_graphic_32x32(1,49,cheng1); 			//在第1页，第49列显示单个汉字"成"
		display_graphic_32x32(1,89,gon); 			//在第1页，第49列显示单个汉字"成"		
		display_graphic_16x16(6,1,zhuang1); 		//在第5页，第1列显示单个汉字"状"
		display_graphic_16x16(6,(1+16),tai1); 		//在第5页，第17列显示单个汉字"态"
//		display_graphic_8x16(6,(1+16*2),mao_hao); 	//在第5页，第25列显示单个字符":"
		display_graphic_16x16(6,(1+16*2+8),shi1); 	//在第5页，第41列显示单个汉字"使"
		display_graphic_16x16(6,(1+16*3+8),yong1);	//在第5页，第49列显示单个汉字"用"
		display_graphic_8x16(6,(89),num0); 			//在第5页，第89列显示单个数字"0"
		display_graphic_8x16(6,(89+8*1),num0); 		//在第5页，第97列显示单个数字"0"
		display_graphic_8x16(6,(89+8*2),mao_hao); 	//在第5页，第105列显示单个字符":"
		display_graphic_8x16(6,(89+8*3),num0); 		//在第5页，第113列显示单个数字"0"
		display_graphic_8x16(6,(89+8*4),num0); 		//在第5页，第121列显示单个数字"0"
		waitkey();
		clear_screen();    								//clear all dots		
		display_string_8x16(1,1,"(<\"0123456abt~`!@#$%^\">)");//在第1页，第1列显示字符串
		display_string_8x16(3,1,"{[(<\" '&*|\\@#_-+=' \">)]}");//在第*页，第*列显示字符串
		display_string_5x8_1(5,1,"[!#$%&'()*+,-./0123456789:;<=>?]");
		display_string_5x8_1(6,1,"[ABCDEFGHIJKLMNOPQRSTUVWXYZabcd]");
		display_string_5x8_1(7,1,"(abcdefghijklmnopqrstuvwxyzabcd)");
		display_string_5x8_1(8,1,"{[(<\" '&*|\\@abcde012#_-+=' \">)]}");
		waitkey();
		full_display(0x55,0xaa);
		waitkey();
		full_display(0xaa,0x55);
		waitkey();
		full_display(0xff,0x00);
		waitkey();
		full_display(0x00,0xff);
		waitkey();
		full_display(0x55,0x55);
		waitkey();
		full_display(0xaa,0xaa);
		waitkey();
		full_display(0xff,0xff);
		waitkey();		
		

	}
}


