#include "oled.h"
#include <intrins.h>

void delay_ms(unsigned int ms)
{                         
	unsigned int a;
	while(ms)
	{
		a=1800;
		while(a--);
		ms--;
	}
	return;
}

// Normal/Reverse Display
void OLED_ColorTurn(u8 i)
{
	if(i==0)
	{
		OLED_WR_Byte(0xA6,OLED_CMD);//Normal Display P37-10.1.10
	}
	if(i==1)
	{
		OLED_WR_Byte(0xA7,OLED_CMD);//Reverse Display P37-10.1.10
	}
}

// Display Vertically
void OLED_DisplayTurn(u8 i)
{
	if(i==0)
	{
		OLED_WR_Byte(0xC8,OLED_CMD);//remapped mode. Scan from COM[N-1] to COM0 P31- Table4
		OLED_WR_Byte(0xA1,OLED_CMD);//column address 127 is mapped to SEG0 P31- Table4
	}
	if(i==1)
	{
		OLED_WR_Byte(0xC0,OLED_CMD);//normal mode (RESET) Scan from COM0 to COM[N –1]  P31- Table4
		OLED_WR_Byte(0xA0,OLED_CMD);//column address 0 is mapped to SEG0 (RESET) P31- Table4
	}
}

//开启OLED显示 
void OLED_DisPlay_On(void)
{
	OLED_WR_Byte(0x8D,OLED_CMD);//Charge Pump Setting P3/6
	OLED_WR_Byte(0x14,OLED_CMD);//Enable Charge Pump P3/6
	OLED_WR_Byte(0xAF,OLED_CMD);//Display ON P37-10.1.12
}

//关闭OLED显示 
void OLED_DisPlay_Off(void)
{
	OLED_WR_Byte(0x8D,OLED_CMD);//Charge Pump Setting P3/6
	OLED_WR_Byte(0x10,OLED_CMD);//Disable charge pump(RESET) 
	OLED_WR_Byte(0xAE,OLED_CMD);//Display OFF P37-10.1.12
}


//向SSD1306写入一个字节。
//mode:数据/命令标志 0,表示命令;1,表示数据;
void OLED_WR_Byte(u8 dat,u8 cmd)
{	
	u8 i;			  
	if(cmd)
	{
	  OLED_DC_Set();//Write data 7.1.3 
	}
	else 
	{
	  OLED_DC_Clr();//Write command 7.1.3	
	}	  
	
	OLED_CS_Clr();//拉低CS引脚,开始传输
	
	for(i=0; i<8; i++)
	{			  
		OLED_SCL_Clr();//时钟信号置低
		if(dat & 0x80)//检查最高位（MSB）是否为1
		{
			OLED_SDA_Set();//数据位为1,SDA置高
		}
		else 
		{
			OLED_SDA_Clr();//数据位为0,SDA置低
		}
		OLED_SCL_Set();//时钟信号置高,数据被OLED读取
		dat <<= 1;     //数据左移一位,准备发送下一位
	}
				 		  
	OLED_CS_Set();//CS 引脚置高，表示结束通信
	OLED_DC_Set();//DC 引脚恢复到默认状态（通常置高）   	  
}

//清屏函数
void OLED_Clear(void)
{
	u8 i,n;
	for(i=0;i<8;i++)
	{
	   OLED_WR_Byte(0xB0+i,OLED_CMD);//Set Page Start Address 10.1.13
	   OLED_WR_Byte(0x10,OLED_CMD);  //Upper start column address 10.1.3
	   OLED_WR_Byte(0x02,OLED_CMD);  //设置列地址的低4位
	   for(n=0;n<128;n++)
		 {
			 OLED_WR_Byte(0x00,OLED_DATA);//清除所有数据
		 }
  }
}

//设置起始地址
//x:Column0-127 y:Page0-7
void OLED_address(u8 x,u8 y)
{
	x+=2;
	OLED_WR_Byte(0xB0+y,OLED_CMD);              //设置页地址
	OLED_WR_Byte(((x&0xF0)>>4)|0x10,OLED_CMD);  //upper start column address 4
	OLED_WR_Byte((x&0x0F),OLED_CMD);            //设置列地址的低4位
}

//显示128x64点阵图像
void OLED_Display_128x64(u8 *dp)
{
	u8 i,j;
	for(i=0;i<8;i++)
	{
		OLED_address(0,i);
		for(j=0;j<128;j++)
		{
			OLED_WR_Byte(*dp,OLED_DATA); //写数据到OLED,每写完一个8位的数据后列地址自动加1
			dp++;
    }
  }
}

//显示16x16点阵图像、汉字、生僻字或16x16点阵的其他图标
//x:Column0-127 y:Page0-7
void OLED_Display_16x16(u8 x,u8 y,u8 *dp)
{
	u8 i,j;
	for(j=0;j<2;j++)
	{
		OLED_address(x,y);
		for(i=0;i<16;i++)
		{
			OLED_WR_Byte(*dp,OLED_DATA);  //写数据到OLED,每写完一个8位的数据后列地址自动加1
			dp++;
    }
		y++;
  }
}

//显示8x16点阵图像、ASCII, 或8x16点阵的自造字符、其他图标
//x:Column0-127 y:Page0-7
void OLED_Display_8x16(u8 x,u8 y,u8 *dp)
{
	u8 i,j;
	for(j=0;j<2;j++)
	{
		OLED_address(x,y);
		for(i=0;i<8;i++)
		{
			OLED_WR_Byte(*dp,OLED_DATA);   //写数据到LCD,每写完一个8位的数据后列地址自动加1
			dp++;
    }
		y++;
  }
}

//显示5*7点阵图像、ASCII, 或5x7点阵的自造字符、其他图标
//x:Column0-127 y:Page0-7
void OLED_Display_5x7(u8 x,u8 y,u8 *dp)
{
	u8 i;
  OLED_address(x,y);
	for(i=0;i<6;i++)
	{
		OLED_WR_Byte(*dp,OLED_DATA);
		dp++;
  }
}

//送指令到晶联讯字库IC
void Send_Command_to_ROM(u8 dat)
{
	u8 i;
	for(i=0;i<8;i++)
	{
		OLED_SCL_Clr();
		if(dat&0x80)
		{
			OLED_SDA_Set();
    }
		else
		{
			OLED_SDA_Clr();
    }
		dat<<=1;
		OLED_SCL_Set();
  }
}

//从晶联讯字库IC中取汉字或字符数据（1个字节）
u8 Get_data_from_ROM(void)
{
	u8 i,read=0;
	for(i=0;i<8;i++)
	{
		OLED_SCL_Clr();
		read<<=1;
		if(OLED_READ_FS0)
		{
			read++;
    }
		OLED_SCL_Set();
  }
	return read;
}


//从相关地址（addrHigh：地址高字节,addrMid：地址中字节,addrLow：地址低字节）中连续读出DataLen个字节的数据到 pbuff的地址
//连续读取
void OLED_get_data_from_ROM(u8 addrHigh,u8 addrMid,u8 addrLow,u8 *pbuff,u8 DataLen)
{
	u8 i;
	OLED_ROM_CS_Clr();
	Send_Command_to_ROM(0x03);
	Send_Command_to_ROM(addrHigh);
	Send_Command_to_ROM(addrMid);
	Send_Command_to_ROM(addrLow);
	for(i=0;i<DataLen;i++)
	{
		*(pbuff+i)=Get_data_from_ROM();
  }
	OLED_ROM_CS_Set();
}


u32 fontaddr=0;
void OLED_Display_GB2312_string(u8 x,u8 y,u8 *text)
{
	u8 i=0;
	u8 addrHigh,addrMid,addrLow;
	u8 fontbuf[32];
	while(text[i]>0x00)
	{
		if((text[i]>=0xb0)&&(text[i]<=0xf7)&&(text[i+1]>=0xa1))
		{
			//国标简体（GB2312）汉字在晶联讯字库IC中的地址由以下公式来计算：
			//Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1)+ 846)*32+ BaseAdd;BaseAdd=0
			//由于担心8位单片机有乘法溢出问题，所以分三部取地址
			fontaddr=(text[i]-0xb0)*94;
			fontaddr+=(text[i+1]-0xa1)+846;
			fontaddr=fontaddr*32;
			
			addrHigh=(fontaddr&0xff0000)>>16;   //地址的高8位,共24位
			addrMid=(fontaddr&0xff00)>>8;       //地址的中8位,共24位
			addrLow=(fontaddr&0xff);            //地址的低8位,共24位
			
			OLED_get_data_from_ROM(addrHigh,addrMid,addrLow,fontbuf,32);
			//取32个字节的数据，存到"fontbuf[32]"
			OLED_Display_16x16(x,y,fontbuf);
			//显示汉字到LCD上，y为页地址，x为列地址，fontbuf[]为数据
			x+=16;
			i+=2;
    }
		else if((text[i]>=0xa1)&&(text[i]<=0xa3)&&(text[i+1]>=0xa1))
		{
			
			fontaddr=(text[i]-0xa1)*94;
			fontaddr+=(text[i+1]-0xa1);
			fontaddr=fontaddr*32;
			
			addrHigh=(fontaddr&0xff0000)>>16;
			addrMid=(fontaddr&0xff00)>>8;
			addrLow=(fontaddr&0xff);
			
			OLED_get_data_from_ROM(addrHigh,addrMid,addrLow,fontbuf,32);
			OLED_Display_16x16(x,y,fontbuf);
			x+=16;
			i+=2;
    }
		else if((text[i]>=0x20)&&(text[i]<=0x7e))
		{
			unsigned char fontbuf[16];
			fontaddr=(text[i]-0x20);
			fontaddr=(unsigned long)(fontaddr*16);
			fontaddr=(unsigned long)(fontaddr+0x3cf80);
			
			addrHigh=(fontaddr&0xff0000)>>16;
			addrMid=(fontaddr&0xff00)>>8;
			addrLow=fontaddr&0xff;
			
			OLED_get_data_from_ROM(addrHigh,addrMid,addrLow,fontbuf,16);
			OLED_Display_8x16(x,y,fontbuf);
			x+=8;
			i+=1;
    }
		else 
			i++;
  }
}

void OLED_Display_string_5x7(u8 x,u8 y,u8 *text)
{
	u8 i=0;
	u8 addrHigh,addrMid,addrLow;
	while(text[i]>0x00)
	{
		if((text[i]>=0x20)&&(text[i]<=0x7e))
		{
			u8 fontbuf[8];
			fontaddr=(text[i]-0x20);
			fontaddr=(unsigned long)(fontaddr*8);
			fontaddr=(unsigned long)(fontaddr+0x3bfc0);
			
			addrHigh=(fontaddr&0xff0000)>>16;
			addrMid=(fontaddr&0xff00)>>8;
			addrLow=fontaddr&0xff;
			
			OLED_get_data_from_ROM(addrHigh,addrMid,addrLow,fontbuf,8);
			OLED_Display_5x7(x,y,fontbuf);
			x+=6;
			i+=1;
    }
		else 
			i++;
  }
}

//显示2个数字
//x,y :起点坐标
//num1:要显示的小数
//len :数字的位数
void OLED_ShowNum(u8 x,u8 y,float num1,u8 len)
{
	u8 i;
	u32 t,num;
	x=x+len*8+8;//要显示的小数最低位的横坐标
	num=num1*100;//将小数左移两位并转化为整数
	OLED_Display_GB2312_string(x-24,y,".");//显示小数点
	for(i=0;i<len;i++)
	{
		t=num%10;//取个位数的数值
		num=num/10;//将整数右移一位
		x-=8;
		if(i==2){x-=8;}//当显示出来两个小数之后，空出小数点的位置
		switch(t)
		{
			case 0 :OLED_Display_GB2312_string(x,y,"0");break;
			case 1 :OLED_Display_GB2312_string(x,y,"1");break;
			case 2 :OLED_Display_GB2312_string(x,y,"2");break;
			case 3 :OLED_Display_GB2312_string(x,y,"3");break;
			case 4 :OLED_Display_GB2312_string(x,y,"4");break;
			case 5 :OLED_Display_GB2312_string(x,y,"5");break;
			case 6 :OLED_Display_GB2312_string(x,y,"6");break;
			case 7 :OLED_Display_GB2312_string(x,y,"7");break;
			case 8 :OLED_Display_GB2312_string(x,y,"8");break;
			case 9 :OLED_Display_GB2312_string(x,y,"9");break;
		}
	}
}

//OLED的初始化
void OLED_Init(void)
{
	OLED_CS_Set();
	OLED_ROM_CS_Set();
	delay_ms(200);
	
	OLED_WR_Byte(0xAE,OLED_CMD); /*display off*/ 
	OLED_WR_Byte(0x02,OLED_CMD); /*set lower column address*/ 
	OLED_WR_Byte(0x10,OLED_CMD); /*set higher column address*/ 
	OLED_WR_Byte(0x40,OLED_CMD); /*set display start line*/ 
	OLED_WR_Byte(0xB0,OLED_CMD); /*set page address*/
	OLED_WR_Byte(0x81,OLED_CMD); /*contract control*/ 
	OLED_WR_Byte(0xcf,OLED_CMD); /*128*/ 
	OLED_WR_Byte(0xA1,OLED_CMD); /*set segment remap*/ 
	OLED_WR_Byte(0xA6,OLED_CMD); /*normal / reverse*/ 
	OLED_WR_Byte(0xA8,OLED_CMD); /*multiplex ratio*/ 
	OLED_WR_Byte(0x3F,OLED_CMD); /*duty = 1/64*/ 
	OLED_WR_Byte(0xad,OLED_CMD); /*set charge pump enable*/ 
	OLED_WR_Byte(0x8b,OLED_CMD); /* 0x8B 内供 VCC */ 
	OLED_WR_Byte(0x33,OLED_CMD); /*0X30---0X33 set VPP 9V */ 
	OLED_WR_Byte(0xC8,OLED_CMD); /*Com scan direction*/ 
	OLED_WR_Byte(0xD3,OLED_CMD); /*set display offset*/ 
	OLED_WR_Byte(0x00,OLED_CMD); /* 0x20 */ 
	OLED_WR_Byte(0xD5,OLED_CMD); /*set osc division*/ 
	OLED_WR_Byte(0x80,OLED_CMD); 
	OLED_WR_Byte(0xD9,OLED_CMD); /*set pre-charge period*/ 
	OLED_WR_Byte(0x1f,OLED_CMD); /*0x22*/ 
	OLED_WR_Byte(0xDA,OLED_CMD); /*set COM pins*/ 
	OLED_WR_Byte(0x12,OLED_CMD); 
	OLED_WR_Byte(0xdb,OLED_CMD); /*set vcomh*/ 
	OLED_WR_Byte(0x40,OLED_CMD);
	OLED_Clear();
	OLED_WR_Byte(0xAF,OLED_CMD); /*display ON*/
}


void Delay(unsigned int time)
{
	  unsigned int i,j;
	  for(i=0;i<time;i++)
	     for(j=0;j<20000;j++)
		 {;}
}


void Ins_trans(unsigned char command)
{
    unsigned char counter;
	  OLED_CS=0;
	  OLED_DC=0;
    for(counter=0;counter<8;counter++)
       {
	     OLED_SCL=0;
         OLED_SDA=(command&0x80)>>7;
         command=command<<1;
	       _nop_();
	     OLED_SCL=1;
	     //  _nop_();
	     _nop_();
	     OLED_SCL=0;
        }
	OLED_DC=1;
   OLED_CS=1;
 }

 void Data_trans(unsigned char command)
{
    unsigned char counter;
	  OLED_CS=0;
	  OLED_DC=1;
    for(counter=0;counter<8;counter++)
       {
         OLED_SCL=0;
	     OLED_SDA=(command&0x80)>>7;
			command=command<<1;
	       _nop_();
	     OLED_SCL=1;
	      // _nop_();
	     _nop_();
	     OLED_SCL=0;
       }  
	OLED_DC=1;
   OLED_CS=1;
 }


 
void Column_set(unsigned char column)
{
	     Ins_trans(0x10|(column>>4));
	     Ins_trans(0x00|(column&0x0f));
}


void Page_set(unsigned char page)
{
	    Ins_trans(0xb0+page);
}


void Screen_FillClear(unsigned char FC)
{
   unsigned char page,column;
  for(page=0;page<8;page++)             //page loop
  {
	   Page_set(page);
	   Column_set(2);
	   for(column=0;column<128;column++)	//column loop
		{
		  Data_trans(FC);
		}
  }
}



void OLED_fullCOL(unsigned char oddData, unsigned char evenData)
{
    unsigned char page, column;
    for (page = 0; page < 8; page++) 
    { 
        Page_set(page);
        Column_set(0);  
        for (column = 0; column < 130; column++) // column loop
        {
         
            if (column % 2 == 0)
            {
                Data_trans(oddData); 
            }
            else
            {
                Data_trans(evenData); 
            }
        }
    }
}


void Picture_display(unsigned char *ptr_pic)
        {
	  unsigned char page,column;
 	  for(page=0;page<(64/8);page++)        //page loop
	     {
		   Page_set(page);
		   Column_set(2);
		   for(column=0;column<128;column++)	//column loop
		      {
		     Data_trans(*ptr_pic++);
		  }
		 }
	  Delay(15);
	}

	
void Initial()
{
		Ins_trans(0xAE);//--turn off oled panel

		Ins_trans(0x02);//--set low column address
		Ins_trans(0x10);//--set high column address

		Ins_trans(0x40);//--set start line address

		Ins_trans(0xB0);//--set page address

		Ins_trans(0x81);//--set contrast control register
		Ins_trans(0xFF);
	
		Ins_trans(0xA1);//--set segment re-map 127 to 0   a0:0 to seg127
		Ins_trans(0xA4);//--set normal display
		Ins_trans(0xA6);//--set indication display
		Ins_trans(0xC8);//--set com(N-1)to com0  c0:com0 to com(N-1)

		Ins_trans(0xA8);//--set multiples ratio(1to64)
		Ins_trans(0x3F);//--1/64 duty

		Ins_trans(0xD5);//--set display clock divide ratio/oscillator frequency
		Ins_trans(0x80);//--set divide ratio

		Ins_trans(0xD3);//--set display offset
		Ins_trans(0x00);//--not offset

		Ins_trans(0xAD);//--OLED_DC-OLED_DC ON/OFF Mode Set
		Ins_trans(0x8B);//--8A:OLED_DC-OLED_DC is disable,8B:OLED_DC-OLED_DC will be turned on when display on.(POR)

		Ins_trans(0xDA);//--set com pins hardware configuration
		Ins_trans(0x12);

		Ins_trans(0xDB);//--set vcomh
		Ins_trans(0x40);

		Ins_trans(0xD9);//--set charge period
		Ins_trans(0xF1);

		Ins_trans(0xAF);//--turn on oled panel
}

//==================display a piture of 16*16 dots================
void display_zifu_16x16(char page,char column,char *dp)
{
	char i,j;
	for(j=0;j<2;j++)
	{
		
 	 	Page_set(page+j);  	 	
		Column_set(column); 
		for (i=0;i<16;i++)
		{	
			Data_trans(*dp);		
			dp++;
		}
	}
}

void display_zifu_16x8(char page,char column,char *dp)
{
	char i,j;
	for(j=0;j<2;j++)
	{
		
 	 	Page_set(page+j);  	 	
		Column_set(column); 
		for (i=0;i<8;i++)
		{	
			Data_trans(*dp);		
			dp++;
		}
	}
}




unsigned int ReadKey() 
{
    if (KEY == 0) 
	{ 
        Delay(1);  
        if (KEY == 0) 
		{  
            while (KEY == 0);  
            return 1; 
        }
    }
    return 0;  
}


void PerformScreenFillSequence() 
{
    Screen_FillClear(0xAA);
    Delay(70);
    Screen_FillClear(0x55);
    Delay(70);
    OLED_fullCOL(0xFF, 0x00);
    Delay(70);
    OLED_fullCOL(0x00, 0xFF);
    Delay(70);
    OLED_fullCOL(0xAA, 0x55);
    Delay(70);
    OLED_fullCOL(0x55, 0xAA);
}