#include <reg51.H>
#include <intrins.h>
#include <chinese_code.h>


sbit  reset=P3^5;          //
sbit	scl=P1^7;            //
sbit	sda=P1^6;            //
sbit  key=P2^0;            //

#define uchar unsigned char 
#define uint unsigned int
#define ulong unsigned long

void delay(int i)
{
	int j,k;
	for(j=0;j<i;j++)
	for(k=0;k<110;k++);	
}

//等待一个按键
void waitkey()
{
 repeat:
  	if (key==1) goto repeat;
	else;
		delay(1500);
}

void transfer(int data1)
{
	int i;
	for(i=0;i<8;i++)
   {
		scl=0;
		if(data1&0x80) sda=1;
		else sda=0;
		scl=1;
	 	scl=0;
	 	data1=data1<<1;
   }
		sda=0;
		scl=1;
		scl=0;
}

void start_flag()
{
	scl=1;		/*START FLAG*/
	sda=1;		/*START FLAG*/
	sda=0;		/*START FLAG*/
}

void stop_flag()
{
	scl=1;		/*STOP FLAG*/
	sda=0;		/*STOP FLAG*/		
	sda=1;		/*STOP FLAG*/
}

//写命令到液晶显示模块
void transfer_command(uchar com)   
{
	start_flag(); 
	transfer(0x78); 
	transfer(0x00); 
	transfer(com); 
	stop_flag(); 
}

//写数据到液晶显示模块
void transfer_data(uchar dat)
{
	start_flag(); 
	transfer(0x78); 
	transfer(0x40); 
	transfer(dat); 
	stop_flag(); 
} 

void initial_lcd()
{
	reset=0;
	delay(500);
	reset=1;
	delay(100);
	
	transfer_command(0x2c);
	delay(200);
	transfer_command(0x2e);
	delay(200);
	transfer_command(0x2f);
	delay(10);
	
	transfer_command(0xae);		//显示关
	transfer_command(0x38);		//模式设置
	transfer_command(0xb8);		//85HZ
	transfer_command(0xc8);		//行扫描顺序	
	transfer_command(0xa0);		//列扫描顺序
	
	transfer_command(0x44);		//Set initial COM0 register
	transfer_command(0x00);
	transfer_command(0x40);		//Set initial display line register
	transfer_command(0x00);

	transfer_command(0xab);
	transfer_command(0x67);
	transfer_command(0x27);		//粗调对比度，可设置范围0x20～0x27
	transfer_command(0x81);		//微调对比度
	transfer_command(0x28);		//微调对比度的值，可设置范围0x00～0x3f

	transfer_command(0x56);		//0x56  1/11 bias
	transfer_command(0xf3);
	transfer_command(0x04);
	transfer_command(0x93);
    
	transfer_command(0xaf);		//显示开

}


void lcd_address(uchar page,uchar column)
{
	column=column;
	page=page-1;
	transfer_command(0xb0+page);
	transfer_command(((column>>4)&0x0f)+0x10);
	transfer_command(column&0x0f);
}

void clear_screen()
{	
	uchar i,j;
	for(j=0;j<16;j++)
	{
		lcd_address(j+1,0);
		for(i=0;i<128;i++)
		{
			transfer_data(0x00);
		
			transfer_data(0x00);

		}
	}
}


void full_display(unsigned char dots)
{	
	uchar i,j;
	for(j=0;j<16;j++)
	{
		lcd_address(j+1,0);
		for(i=0;i<128;i++)
		{
			transfer_data(dots);
		
			transfer_data(dots);

		}
	}
}





//显示8x16的点阵的字符串，括号里的参数分别为（页,列，字符串指针）
void display_string_8x16(uchar page,uchar column,uchar *text)
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
					transfer_data(ascii_table_8x16[j][k+8*n]);
					transfer_data(ascii_table_8x16[j][k+8*n]);
				}
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

//写入一组16x16点阵的汉字字符串（字符串表格中需含有此字）
//括号里的参数：(页，列，汉字字符串）
void display_string_16x16(uchar page,uchar column,uchar *text)
{
	uchar i,j,k;
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
		if(column>127)
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
					transfer_data(Chinese_code_16x16[address]);
					transfer_data(Chinese_code_16x16[address]);
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
					transfer_data(0x00);
					transfer_data(0x00);
					
				}
			}
			j++;
		}
		column +=16;
	}
}

//显示16x16点阵的汉字或者ASCII码8x16点阵的字符混合字符串
//括号里的参数：(页，列，字符串）
void display_string_8x16_16x16(uchar page,uchar column,uchar *text)
{
	uchar temp[3];
	uchar i=0;
	while(text[i] !='\0')
	{
		if(text[i]>0x7e)
		{
			temp[0]=text[i];
			temp[1]=text[i+1];
			temp[2]='\0';														//汉字为两个字节
			display_string_16x16(page,column,temp); //显示汉字
			column +=16;
			i +=2;
			if(column>127)
			{
				column =0;
				page +=2;
			}
		}
		else
		{
			temp[0]=text[i];
			temp[1]='\0';														//字母占一个字节
			display_string_8x16(page,column,temp);  //显示字母
			column +=8;
			i++;
			if(column>127)
			{
				column =0;
				page +=2;
			}
		}
	}
}

void display_32x32(uchar page,uchar column,uchar *dp)
{
	int i,j;
	for(j=0;j<4;j++)
	{
		lcd_address(page+j,column);
		for(i=0;i<32;i++)
		{
			transfer_data(*dp);
			transfer_data(*dp);
			dp++;
		}
	}
	
}

void display_graphic(uchar *dp)
{
	int i,j;
	for(j=0;j<16;j++)
	{
		lcd_address(j+1,0);
		for(i=0;i<128;i++)
		{
			transfer_data(*dp);
			transfer_data(*dp);
			dp++;	
		}
	}
}


// 切黑白模式（1字节对应纵向8点）
void st7571_set_bw_mode(void)
{
    transfer_command(0x7B); // Enter Extension Command Set 3
    transfer_command(0x11); // Set Color Mode: B/W
    transfer_command(0x00); // Return to normal command set
}


// 显式逐列写：偶数列写 dots_even，奇数列写 dots_odd
void explicit_column_test(unsigned char dots_even, unsigned char dots_odd)
{
    unsigned char page, col;
    for(page = 1; page <= 16; page++)        // page 1..16
    {
        for(col = 0; col < 128; col++)       // col 0..127
        {
            lcd_address(page, col);          // 明确设置到 page,col
            if((col & 1) == 0)
                transfer_data(dots_even);    // 偶数列
            else
                transfer_data(dots_odd);     // 奇数列
        }
    }
}

// 切回灰度模式（默认模式）
void st7571_set_gray_mode(void)
{
    transfer_command(0x7B); // 进入扩展命令集 3
    transfer_command(0x12); // Set Color Mode: 4 gray scale
    transfer_command(0x00); // 回到普通命令集
}

void main(void)
{
	initial_lcd();
	while(1)
	{
		st7571_set_gray_mode();
		display_graphic(bmp2);
		delay(3000);
		clear_screen();
		
		display_graphic(bmp3);
		display_string_16x16(5,32,"中国制造");
		display_string_8x16(8,16,"Made in China");
		display_string_8x16(10,8,"Dots:128 x 128");
		display_string_8x16(12,32,"IC:ST571");
		delay(3000);
		
		st7571_set_bw_mode();
		explicit_column_test(0x00, 0xFF);
		delay(3000);
		explicit_column_test(0xff, 0x00);
		delay(3000);
		explicit_column_test(0x55, 0xaa);
		delay(3000);
		explicit_column_test(0xaa, 0x55);
		delay(3000);	

		explicit_column_test(0xaa, 0xaa);
		delay(3000);
		explicit_column_test(0x55, 0x55);
		delay(3000);		
		clear_screen();		

		
		
//		display_32x32(1,16,jing32);
//		display_32x32(1,48,lian32);
//		display_32x32(1,80,xun32);



	}
}