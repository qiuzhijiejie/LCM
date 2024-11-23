#include "reg51.h"
#include "picture.h"
sbit   KEY1  = P2^0;

#define set_0   0x01
#define set_1   0x02
#define clr_0   0xFE
#define clr_1   0xFD

#define IIC_SCK_0  P3 &=clr_0            
#define IIC_SCK_1  P3|=set_0       
#define IIC_SDA_0  P3&=clr_1            
#define IIC_SDA_1  P3|=set_1

#define OLED_COLUMN_NUMBER 128
#define OLED_LINE_NUMBER 64
#define OLED_PAGE_NUMBER (OLED_LINE_NUMBER/8)
#define OLED_COLUMN_OFFSET 0
#define OLED_LINE_OFFSET 0

extern const unsigned char  *point1;
extern const unsigned char  *point2;
extern const unsigned char  *point3;
extern const unsigned char  *point4;
extern const unsigned char  *point5;
extern unsigned char ACK=0;

unsigned char code OLED_init_cmd[25]=
{
       0xAE,//关闭显示
	
       0xD5,//设置时钟分频因子,震荡频率
       0x80,  //[3:0],分频因子;[7:4],震荡频率

       0xA8,//设置驱动路数
       0X3F,//默认(1/64)
	
       0xD3,//设置显示偏移
       0X00,//默认为0
	
       0x40,//设置显示开始行 [5:0],行数.
	
       0x8D,//电荷泵设置
       0x10,//bit2，开启/关闭
       0x20,//设置内存地址模式
       0x02,//[1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认10;
       0xA1,//段重定义设置,bit0:0,0->0;1,0->127;  A1
	
       0xC8,//设置COM扫描方向;bit3:0,普通模式;1,重定义模式 COM[N-1]->COM0;N:驱动路数 (C0 翻转显示) C8
	   
       0xDA,//设置COM硬件引脚配置
       0x12,//[5:4]配置  
	   
       0x81,//对比度设置
       0xf0,//1~255;默认0X7F (亮度设置,越大越亮)
	   
       0xD9,//设置预充电周期
       0x71,//[3:0],PHASE 1;[7:4],PHASE 2;
	   
       0xDB,//设置VCOMH 电压倍率
       0x00,//[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;
	   
       0xA4,//全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)
	   
       0xA6,//设置显示方式;bit0:1,反相显示;0,正常显示 
       
       0xAF,//开启显示   




};


void delay_us(unsigned int _us_time)
{       
  unsigned char x=0;
  for(;_us_time>0;_us_time--)
  {
    x++;
  }
}

void delay_ms(unsigned int _ms_time)
  {
    unsigned int i,j;
    for(i=0;i<_ms_time;i++)
    {
    for(j=0;j<900;j++)
      {;}
    }
  }
/**************************IIC模块发送函数************************************************

 *************************************************************************/
//写入  最后将SDA拉高，以等待从设备产生应答
void IIC_write(unsigned char date)
{
	unsigned char i, temp;
	temp = date;
			
	for(i=0; i<8; i++)
	{	IIC_SCK_0;
		if ((temp&0x80)==0)
         IIC_SDA_0;
         else IIC_SDA_1;
		temp = temp << 1;
						  //最少250ns延时
		IIC_SCK_1;
		
	}
	IIC_SCK_0;
	
	IIC_SDA_1;
	
	IIC_SCK_1;
	
	 ACK =0;
	IIC_SCK_0;
	

}
//启动信号
//SCL在高电平期间，SDA由高电平向低电平的变化定义为启动信号
void IIC_start()
{
	IIC_SDA_1;
	
	IIC_SCK_1;
					   //所有操作结束释放SCL	
	IIC_SDA_0;
	
	IIC_SCK_0;
	
    IIC_write(0x78);
        
}

//停止信号
//SCL在高电平期间，SDA由低电平向高电平的变化定义为停止信号
void IIC_stop()
{
	IIC_SDA_0;
	
	IIC_SCK_1;
	
	IIC_SDA_1;
	
}

void OLED_send_cmd(unsigned char o_command)
  {
    
    IIC_start();
    IIC_write(0x00); 
    IIC_write(o_command);
    IIC_stop();
    
  }
  
void OLED_send_data(unsigned char o_data)
  { 
    IIC_start();
    IIC_write(0x40);
    IIC_write(o_data);
    IIC_stop();
   }
  
void Column_set(unsigned char column)
  {
    column+=OLED_COLUMN_OFFSET;
    OLED_send_cmd(0x10|(column>>4));    //设置列地址高位
    OLED_send_cmd(0x00|(column&0x0f));   //设置列地址低位  
    	 
  }
  
void Page_set(unsigned char page)
  {
    OLED_send_cmd(0xb0+page);
  }
  
void OLED_clear(void)
  {
    unsigned char page,column;
    for(page=0;page<OLED_PAGE_NUMBER;page++)             //page loop
      { 
          Page_set(page);
          Column_set(0);	  
          for(column=0;column<OLED_COLUMN_NUMBER;column++)	//column loop
            {
              OLED_send_data(0x00);
            }
      }
  }

    
 void OLED_full(void)
  {
    unsigned char page,column;
    for(page=0;page<OLED_PAGE_NUMBER;page++)             //page loop
      { 
        Page_set(page);
        Column_set(0);	  
	for(column=0;column<OLED_COLUMN_NUMBER;column++)	//column loop
          {
            OLED_send_data(0xff);
          }
      }
  } 
void OLED_init(void)
  {
    unsigned char i;
    for(i=0;i<25;i++)
      {
        OLED_send_cmd(OLED_init_cmd[i]);
      }
  }

void Picture_display(const unsigned char *ptr_pic)
  {
    unsigned char page,column;
    for(page=0;page<OLED_PAGE_NUMBER;page++)        //page loop
      { 
	Page_set(page);
	Column_set(0);	  
	for(column=0;column<OLED_COLUMN_NUMBER;column++)	//column loop
          {
            OLED_send_data(*ptr_pic++);
          }
      }
  }
void Picture_ReverseDisplay(const unsigned char *ptr_pic)
{
    unsigned char page,column,dis_data;
    for(page=0;page<OLED_PAGE_NUMBER;page++)        //page loop
      { 
	Page_set(page);
	Column_set(0);	  
	for(column=0;column<OLED_COLUMN_NUMBER;column++)	//column loop
          {
            dis_data=*ptr_pic++;
            dis_data=~dis_data;
            OLED_send_data(dis_data);
          }
      }
  }

void IO_init(void)
{
  P2|=0xFF;                        
  

}
 unsigned int ReadKey1() // 按键检测函数
{
    if (KEY1 == 0) 
	{  // 按键被按下
        delay_ms(1);  // 简单的去抖动处理
        if (KEY1 == 0) 
		{  // 检查按键是否仍然按下
            while (KEY1 == 0);  // 等待按键释放
            return 1;  // 返回按键被按下
        }
    }
    return 0;  // 按键没有被按下
}



void display1(void)
{	
		point1= &picture_tab1[0];
		Picture_display(point1);
		delay_ms(1);		
		Picture_ReverseDisplay(point1);	 
		delay_ms(1);
}	

void display2(void){	
		point2= &picture_tab2[0];//logo
		Picture_display(point2);
		delay_ms(1);		
		Picture_ReverseDisplay(point2);	 
		delay_ms(1);  	
}
void display3(void)
{
	  
		point3= &picture_tab3[0];
		Picture_display(point3);
		delay_ms(1);		
		Picture_ReverseDisplay(point3);	 
		delay_ms(1);  
}

void display4(void)
{
		point4= &picture_tab4[0];
		Picture_display(point4);
		delay_ms(1);		
		Picture_ReverseDisplay(point4);	 
		delay_ms(1);  	
}
void display5(void)
{
		point5= &picture_tab5[0];
		Picture_display(point5);
		delay_ms(1);		
		Picture_ReverseDisplay(point5);	 
		delay_ms(1);  	
}



void OLED_dotted_pattern(void) 
{	
	unsigned char page, column;
    for (column = 0; column < OLED_COLUMN_NUMBER; column++) 
	{  // 遍历列
        Column_set(column);  // 设置当前列
        for (page = 0; page < OLED_PAGE_NUMBER; page++) 
		{  // 遍历页（从上到下填充一列）
            Page_set(page);  // 设置当前页
            
            // 检查列和页的组合，并设置不同的图案形成麻点效果
            if ((column + page) % 2 == 0) 
			{
                OLED_send_data(0xaa);  // 交替使用 0x55
            } else 
			{
                OLED_send_data(0x55);  // 或者使用 0xAA
            }
        }

        // 刷新当前列的显示，避免下一列与当前列内容重叠
        OLED_send_cmd(0xAF);  // 可能需要发送的显示刷新命令 (示例值)
    }
}

void OLED_dotted_Reversepattern(void) 
{
	unsigned char page, column;
    for (column = 0; column < OLED_COLUMN_NUMBER; column++) 
	{
		// 遍历列
        Column_set(column);  // 设置当前列
        for (page = 0; page < OLED_PAGE_NUMBER; page++) 
		{  // 遍历页（从上到下填充一列）
            Page_set(page);  // 设置当前页
            
            // 检查列和页的组合，并设置不同的图案形成麻点效果
            if ((column + page) % 2 == 0) 
			{
                OLED_send_data(0x55);  // 交替使用 0x55
            } else 
			{
                OLED_send_data(0xAA);  // 或者使用 0xAA
            }
        }

        // 刷新当前列的显示，避免下一列与当前列内容重叠
        OLED_send_cmd(0xAF);  // 可能需要发送的显示刷新命令 (示例值)
    }
}