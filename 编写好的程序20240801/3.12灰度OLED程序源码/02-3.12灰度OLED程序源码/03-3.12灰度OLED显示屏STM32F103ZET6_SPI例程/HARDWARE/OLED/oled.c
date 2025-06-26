#include "oledfont.h"
#include "delay.h"
#include "oled.h"

//******************************************************************************
//    函数说明：OLED写入一个指令
//    入口数据：dat 数据
//    返回值：  无
//******************************************************************************
void OLED_WR_Bus(u8 dat)
{
	u8 i;
	OLED_CS_Clr();
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
		OLED_SCL_Set();
		dat<<=1;   
	}				 		  
	OLED_CS_Set();
}

//******************************************************************************
//    函数说明：OLED写入一个指令
//    入口数据：reg 指令
//    返回值：  无
//******************************************************************************
void OLED_WR_REG(u8 reg)
{	  
	OLED_DC_Clr();		  
  OLED_WR_Bus(reg);
  OLED_DC_Set();	
}

//******************************************************************************
//    函数说明：OLED写入一个数据
//    入口数据：dat 数据
//    返回值：  无
//******************************************************************************
void OLED_WR_Byte(u8 dat)
{	  
  OLED_WR_Bus(dat);
}

//******************************************************************************
//    函数说明：OLED显示列的起始终止地址
//    入口数据：a  列的起始地址
//              b  列的终止地址
//    返回值：  无
//******************************************************************************
void Column_Address(u8 a,u8 b)
{
	OLED_WR_REG(0x15);       // Set Column Address
	OLED_WR_Byte(0x1c+a);
	OLED_WR_Byte(0x1c+b);
}


//******************************************************************************
//    函数说明：OLED显示行的起始终止地址
//    入口数据：a  行的起始地址
//              b  行的终止地址
//    返回值：  无
//******************************************************************************
void Row_Address(u8 a,u8 b)
{
	OLED_WR_REG(0x75);       // Row Column Address
	OLED_WR_Byte(a);
	OLED_WR_Byte(b);
	OLED_WR_REG(0x5C);    //写RAM命令
}


//******************************************************************************
//    函数说明：OLED填充函数
//    入口数据：xstr,ystr:填充函数起始坐标
//              xend,yend:填充函数结束坐标
//              color:填充的颜色值
//    返回值：  无
//******************************************************************************
void OLED_Fill(u16 xstr,u8 ystr,u16 xend,u8 yend,u8 color)
{
	u8 x,y;
	xstr/=4;
	xend/=4;
	Column_Address(xstr,xend-1);
	Row_Address(ystr,yend-1);
	for(x=xstr;x<xend;x++)
	{
		for(y=ystr;y<yend;y++)
		{
			OLED_WR_Byte(color);
			OLED_WR_Byte(color);
    }
  }
}

//******************************************************************************
//    函数说明：OLED显示汉字
//    入口数据：x,y :起点坐标
//              *s  :要显示的汉字串
//              sizey 字符高度 
//              mode  0:正常显示；1：反色显示
//    返回值：  无
//******************************************************************************
void OLED_ShowChinese(u8 x,u8 y,u8 *s,u8 sizey,u8 mode)
{
	while(*s!=0)
	{
		if(sizey==16) OLED_ShowChinese16x16(x,y,s,sizey,mode);
		else if(sizey==24) OLED_ShowChinese24x24(x,y,s,sizey,mode);
		else if(sizey==32) OLED_ShowChinese32x32(x,y,s,sizey,mode);
		else return;
		s+=2;
		x+=sizey;
	}
}
//******************************************************************************
//    函数说明：OLED显示汉字
//    入口数据：x,y :起点坐标
//              *s  :要显示的汉字
//              sizey 字符高度 
//              mode  0:正常显示；1：反色显示
//    返回值：  无
//******************************************************************************
void OLED_ShowChinese16x16(u8 x,u8 y,u8 *s,u8 sizey,u8 mode)
{
	u8 i,j,k,DATA=0;
	u16 HZnum;//汉字数目
	u16 TypefaceNum;//一个字符所占字节大小
	TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;//字符所占字节数
	HZnum=sizeof(tfont16)/sizeof(typFNT_GB16);	//统计汉字库数目
	Column_Address(x/4,x/4+sizey/4-1);
	Row_Address(y,y+sizey-1);
	for(k=0;k<HZnum;k++)
	{
		if ((tfont16[k].Index[0]==*(s))&&(tfont16[k].Index[1]==*(s+1)))
		{ 	
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<4;j++)
				{
					if(tfont16[k].Msk[i]&(0x01<<(j*2+0)))
					{
						DATA=0xf0;
					}
					if(tfont16[k].Msk[i]&(0x01<<(j*2+1)))
					{
						DATA|=0x0f;
					}
					if(mode)
					{
						OLED_WR_Byte(~DATA);
					}else
					{
						OLED_WR_Byte(DATA);
					}
					DATA=0;
				}
			}
		}				  	
		continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
	}
} 

//******************************************************************************
//    函数说明：OLED显示汉字
//    入口数据：x,y :起点坐标
//              *s  :要显示的汉字
//              sizey 字符高度 
//              mode  0:正常显示；1：反色显示
//    返回值：  无
//******************************************************************************
void OLED_ShowChinese24x24(u8 x,u8 y,u8 *s,u8 sizey,u8 mode)
{
	u8 i,j,k,DATA=0;
	u16 HZnum;//汉字数目
	u16 TypefaceNum;//一个字符所占字节大小
	TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;//字符所占字节数
	HZnum=sizeof(tfont24)/sizeof(typFNT_GB24);	//统计汉字库数目
	Column_Address(x/4,x/4+sizey/4-1);
	Row_Address(y,y+sizey-1);
	for(k=0;k<HZnum;k++)
	{
		if ((tfont24[k].Index[0]==*(s))&&(tfont24[k].Index[1]==*(s+1)))
		{ 	
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<4;j++)
				{
					if(tfont24[k].Msk[i]&(0x01<<(j*2+0)))
					{
						DATA=0xf0;
					}
					if(tfont24[k].Msk[i]&(0x01<<(j*2+1)))
					{
						DATA|=0x0f;
					}
					if(mode)
					{
						OLED_WR_Byte(~DATA);
					}else
					{
						OLED_WR_Byte(DATA);
					}
					DATA=0;
				}
			}
		}				  	
		continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
	}
} 

//******************************************************************************
//    函数说明：OLED显示32x32汉字
//    入口数据：x,y :起点坐标
//              *s  :要显示的汉字
//              sizey 字符高度 
//              mode  0:正常显示；1：反色显示
//    返回值：  无
//******************************************************************************
void OLED_ShowChinese32x32(u8 x,u8 y,u8 *s,u8 sizey,u8 mode)
{
	u8 i,j,k,DATA=0;
	u16 HZnum;//汉字数目
	u16 TypefaceNum;//一个字符所占字节大小
	TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;//字符所占字节数
	HZnum=sizeof(tfont32)/sizeof(typFNT_GB32);	//统计汉字库数目
	Column_Address(x/4,x/4+sizey/4-1);
	Row_Address(y,y+sizey-1);
	for(k=0;k<HZnum;k++)
	{
		if ((tfont32[k].Index[0]==*(s))&&(tfont32[k].Index[1]==*(s+1)))
		{ 	
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<4;j++)
				{
					if(tfont32[k].Msk[i]&(0x01<<(j*2+0)))
					{
						DATA=0xf0;
					}
					if(tfont32[k].Msk[i]&(0x01<<(j*2+1)))
					{
						DATA|=0x0f;
					}
					if(mode)
					{
						OLED_WR_Byte(~DATA);
					}else
					{
						OLED_WR_Byte(DATA);
					}
					DATA=0;
				}
			}
		}				  	
		continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
	}
} 
//******************************************************************************
//    函数说明：OLED显示字符函数 
//    此函数适用范围：字符宽度是4的倍数  字符高度是宽度的2倍
//    入口数据：x,y   起始坐标
//              chr   要写入的字符
//              sizey 字符高度 
//              mode  0:正常显示；1：反色显示
//    返回值：  无
//******************************************************************************
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 sizey,u8 mode)
{
	u8 c,i,k,m,t=4,size2,data1,DATA=0;
	size2=(sizey/16+((sizey%16)?1:0))*sizey;
	c=chr-' ';//得到偏移后的值
	Column_Address(x/4,x/4+sizey/8-1);
	Row_Address(y,y+sizey-1);
	for(i=0;i<size2;i++)
	{
		if(sizey==16)
		{
			data1=ascii_1608[c][i];//8x16 ASCII码
		}
		else if(sizey==24)
		{
			data1=ascii_2412[c][i];//12x24 ASCII码
		}
		else if(sizey==32)
		{
			data1=ascii_3216[c][i];//16x32 ASCII码
		}
//			else if(sizey==xx)               //如果需要添加新的字体，只需要将xx替换成所需汉字高度
//			{                                //然后新建数组ascii_xxxx[][]并将取好的模放入其中
//				data=ascii_xxxx[c][i];
//			}
		if(sizey%16)
		{
			m=sizey/16+1;
			if(i%m) t=2;
			else t=4;
		}
		for(k=0;k<t;k++)
		{
			if(data1&(0x01<<(k*2+0)))
			{
				DATA=0xf0;
			}
			if(data1&(0x01<<(k*2+1)))
			{
				DATA|=0x0f;
			}
			if(mode)
			{
				OLED_WR_Byte(~DATA);
			}else
			{
				OLED_WR_Byte(DATA);
			}
			DATA=0;
		}
  }
}
//******************************************************************************
//    函数说明：OLED显示字符串
//    入口数据：x,y  起始坐标
//              *dp   要写入的字符
//              sizey 字符高度 
//              mode  0:正常显示；1：反色显示
//    返回值：  无
//******************************************************************************
void OLED_ShowString(u8 x,u8 y,u8 *dp,u8 sizey,u8 mode)
{
	while(*dp!='\0')
	{
	  OLED_ShowChar(x,y,*dp,sizey,mode);
		dp++;
		x+=sizey/2;
	}
}


//******************************************************************************
//    函数说明：m^n
//    入口数据：m:底数 n:指数
//    返回值：  result
//******************************************************************************
u32 oled_pow(u8 m,u8 n)
{
	u32 result=1;
	while(n--)result*=m;    
	return result;
}


//******************************************************************************
//    函数说明：OLED显示变量
//    入口数据：x,y :起点坐标	 
//              num :要显示的变量
//              len :数字的位数
//              sizey 字符高度 
//              mode  0:正常显示；1：反色显示
//    返回值：  无
//******************************************************************************
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 sizey,u8 mode)
{         	
	u8 t,temp;
	u8 enshow=0;
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(sizey/2)*t,y,' ',sizey,mode);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(sizey/2)*t,y,temp+'0',sizey,mode); 
	}
}

//******************************************************************************
//    函数说明：显示灰度图片
//    入口数据：x,y :起点坐标
//              length 图片长度
//              width  图片宽度
//              BMP[] :要显示图片
//              mode  0:正常显示；1：反色显示
//    返回值：  无
//******************************************************************************
void OLED_DrawBMP(u8 x,u8 y,u16 length,u8 width,const u8 BMP[],u8 mode)
{
	u16 i,num;
	length=(length/4+((length%4)?1:0))*4;
	num=length/2*width;
	x/=4;
	length/=4;
	Column_Address(x,x+length-1);
	Row_Address(y,y+width-1);
	for(i=0;i<num;i++)
	{
		if(mode)
		{
			OLED_WR_Byte(~BMP[i]);
		}else
		{
			OLED_WR_Byte(BMP[i]);
		}
	}
}

//******************************************************************************
//    函数说明：显示单色图片
//    入口数据：x,y :起点坐标
//              length 图片长度 
//              width  图片宽度
//              BMP[] :要显示图片
//              mode  0:正常显示；1：反色显示
//    返回值：  无
//******************************************************************************
void OLED_DrawSingleBMP(u8 x,u8 y,u16 length,u8 width,const u8 BMP[],u8 mode)
{
	u8 k,DATA=0;
	u16 i,num;
	length=(length/8+((length%8)?1:0))*8;
	num=length*width/8;
	x/=4;
	length/=4;
	Column_Address(x,x+length-1);
	Row_Address(y,y+width-1);
	for(i=0;i<num;i++)
	{
		for(k=0;k<4;k++)
		{
			if(BMP[i]&(0x01<<(k*2+0)))
			{
				DATA=0xf0;
			}
			if(BMP[i]&(0x01<<(k*2+1)))
			{
				DATA|=0x0f;
			}
			if(mode)
			{
				OLED_WR_Byte(~DATA);
			}else
			{
				OLED_WR_Byte(DATA);
			}
			DATA=0;
		}
	}
}

//******************************************************************************
//    函数说明：OLED屏初始化
//    入口数据：无
//    返回值：  无
//******************************************************************************
void OLED_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOG,ENABLE);//使能PORTA~E,PORTG时钟
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
  GPIO_Init(GPIOG, &GPIO_InitStructure);//初始化
	GPIO_SetBits(GPIOG,GPIO_Pin_12);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_15 ;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
  GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化
	GPIO_SetBits(GPIOD,GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_15);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
 	GPIO_Init(GPIOE, &GPIO_InitStructure);	  //初始化GPIOE10
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 	 //上拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
 	GPIO_Init(GPIOE, &GPIO_InitStructure);	  //初始化GPIOE8
	GPIO_SetBits(GPIOE,GPIO_Pin_10);
	
	OLED_RES_Clr();	
	delay_ms(10);	
	OLED_RES_Set();

	OLED_WR_REG(0xfd);  /*Command Lock*/ 
	OLED_WR_Byte(0x12);
	
	OLED_WR_REG(0xae); //Sleep In 
	
	OLED_WR_REG(0xb3);	//Set Display Clock Divide Ratio/Oscillator Frequency 
	OLED_WR_Byte(0x91);
	
	OLED_WR_REG(0xca);	//Set Multiplex Ratio 
	OLED_WR_Byte(0x3f);
	
	OLED_WR_REG(0xa2);	//Set Display Offset 
	OLED_WR_Byte(0x00);	//
	
	OLED_WR_REG(0xa1);	//Set Display Start Line 
	OLED_WR_Byte(0x00);	//
	
	OLED_WR_REG(0xa0);	//Set Re-Map $ Dual COM Line Mode
	if(USE_HORIZONTAL)
	{
	  OLED_WR_Byte(0x14);
	}
	else
	{
	  OLED_WR_Byte(0x06);
	}
	
	OLED_WR_REG(0xB5);	//Set GPIO 
	OLED_WR_Byte(0x00);
		
	OLED_WR_REG(0xab);	//Function Selection
	OLED_WR_Byte(0x01);	//
	
	OLED_WR_REG(0xb4);	//Enable External VSL 
	OLED_WR_Byte(0xa0);	//
	OLED_WR_Byte(0xfd);	//
	
	OLED_WR_REG(0xc1);	//Set Contrast Current
	OLED_WR_Byte(0xff);	
	
	OLED_WR_REG(0xc7);	//Master Contrast Current Control
	OLED_WR_Byte(0x0f);	//

	OLED_WR_REG(0xb9);	//Select Default Linear Gray Scale Table 
	
	OLED_WR_REG(0xb1);	//Set Phase Length
	OLED_WR_Byte(0xe2);	
	
	OLED_WR_REG(0xd1);	//Enhance Driving Scheme Capability 
	OLED_WR_Byte(0x82);	
	OLED_WR_Byte(0x20);	

	OLED_WR_REG(0xbb);	//Set Pre-Charge Voltage 
	OLED_WR_Byte(0x1f);	

	OLED_WR_REG(0xb6);	//Set Second Pre-Charge Period 
	OLED_WR_Byte(0x08);	

	OLED_WR_REG(0xbe);	//Set VCOMH Deselect Level 
	OLED_WR_Byte(0x07);		

	OLED_WR_REG(0xa6);	//Set Display Mode
	
	OLED_Fill(0,0,256,64,0x00);   //Clear Screen

	OLED_WR_REG(0xaf);	//Sleep Out
}



