#include "stm32f10x.h"                  // Device header
#include "oled242.h"
unsigned char ACK=0;

/*******时钟初始化******************/
void SYS_init(unsigned char PLL)
{
 
	RCC->APB1RSTR = 0x00000000;   //复位结束			 
	RCC->APB2RSTR = 0x00000000; 	  
  	RCC->AHBENR   = 0x00000014;  //睡眠模式闪存和SRAM时钟使能.其他关闭.	  
  	RCC->APB2ENR  = 0x00000000;  //外设时钟关闭.			   
  	RCC->APB1ENR  = 0x00000000;   
	RCC->CR |= 0x00000001;       //使能内部高速时钟HSION
	
	RCC->CFGR &= 0xF8FF0000;     //复位SW[1:0],HPRE[3:0],PPRE1[2:0],PPRE2[2:0],ADCPRE[1:0],MCO[2:0]					 
	RCC->CR &= 0xFEF6FFFF;       //复位HSEON,CSSON,PLLON
	RCC->CR &= 0xFFFBFFFF;       //复位HSEBYP	   	  
	RCC->CFGR &= 0xFF80FFFF;     //复位PLLSRC, PLLXTPRE, PLLMUL[3:0] and USBPRE
	while(((RCC->CFGR>>2)& 0x03 )!=0x00); 	
	RCC->CIR = 0x00000000;       //关闭所有中断		 
	//配置向量表				  
  
//	SCB->VTOR = 0x08000000|(0x0 & (u32)0x1FFFFF80);//设置NVIC的向量表偏移寄存器
	
 	RCC->CR|=0x00010000;  //外部高速时钟使能HSEON
	while(((RCC->CR>>17)&0x00000001)==0);//等待外部时钟就绪
	RCC->CFGR=0X00000400; //APB1=DIV2;APB2=DIV1;AHB=DIV1;
	PLL-=2;//抵消2个单位
	RCC->CFGR|=PLL<<18;   //设置PLL值 2~16
	RCC->CFGR|=1<<16;	  //PLLSRC ON 
	
	RCC->CR|=0x01000000;  //PLLON
	while(!(RCC->CR>>25));//等待PLL锁定
	RCC->CFGR|=0x00000002;//PLL作为系统时钟	 
	while(((RCC->CFGR>>2)&0x03)!=0x02);   //等待PLL作为系统时钟设置成功
	
}

void IO_init(void)
{
	RCC->APB2ENR|=1<<3;    //使能PORTB时钟 
	GPIOB->CRL&=0X000FFFFF;				//将B567口配置为通用推挽输出,最大50MH
	GPIOB->CRL|=0X33300000;	
	GPIOB->ODR=0XFFFF;
}
void delay_us(unsigned int _us_time)
{       
  unsigned char x=0;
  for(;_us_time>0;_us_time--)
  {
    x++;x++;x++;x++;x++;x++;x++;x++;x++;x++;x++;x++;
	  x++;x++;x++;x++;x++;x++;x++;x++;x++;x++;x++;x++;
	  x++;x++;x++;x++;x++;x++;x++;x++;x++;x++;x++;x++;
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
		delay_us(1);			  
		IIC_SCK_1;
		delay_us(1);
		
	}
	IIC_SCK_0;
	delay_us(1);
	IIC_SDA_1;
	delay_us(1);
	IIC_SCK_1;
//								不需要应答
//	if (READ_SDA==0)
//		ACK = 1;
//	else ACK =0;
	delay_us(1);
	IIC_SCK_0;
	delay_us(1);
	

}
//启动信号
//SCL在高电平期间，SDA由高电平向低电平的变化定义为启动信号
void IIC_start()
{
	IIC_SDA_1;
	delay_us(1);
	IIC_SCK_1;
	delay_us(1);				   //所有操作结束释放SCL	
	IIC_SDA_0;
	delay_us(3);
	IIC_SCK_0;
	
        IIC_write(0x78);
        
}

//停止信号
//SCL在高电平期间，SDA由低电平向高电平的变化定义为停止信号
void IIC_stop()
{
	IIC_SDA_0;
	delay_us(1);
	IIC_SCK_1;
	delay_us(3);
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
	column = column+OLED_COLUMN_OFFSET;
    OLED_send_cmd(0x10|(column>>4));    //设置列地址高位
    OLED_send_cmd(0x00|(column&0x0f));   //设置列地址低位  
    	 
  }
void Page_set(unsigned char page)
  {
	page = page+(OLED_LINE_OFFSET/8);
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
  

  void OLED_HX(void)
  {
    unsigned char page,column;
    for(page=0;page<OLED_PAGE_NUMBER;page++)             //page loop
      { 
        Page_set(page);
        Column_set(0);	  
	for(column=0;column<OLED_COLUMN_NUMBER;column++)	//column loop
          {
            OLED_send_data(0x55);
          }
      }
  }
  
  void OLED_HX1(void)
  {
    unsigned char page,column;
    for(page=0;page<OLED_PAGE_NUMBER;page++)             //page loop
      { 
        Page_set(page);
        Column_set(0);	  
	for(column=0;column<OLED_COLUMN_NUMBER;column++)	//column loop
          {
            OLED_send_data(0xAA);
          }
      }
  }  
  
void OLED_init(void)
  {
    unsigned char i;
    for(i=0;i<OLED_CMD_NUMBER;i++)
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
    unsigned char page,column,data;
    for(page=0;page<OLED_PAGE_NUMBER;page++)        //page loop
      { 
	Page_set(page);
	Column_set(0);	  
	for(column=0;column<OLED_COLUMN_NUMBER;column++)	//column loop
          {
            data=*ptr_pic++;
            data=~data;
            OLED_send_data(data);
          }
      }
  }

 
const unsigned char  OLED_init_cmd[OLED_CMD_NUMBER]=
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
