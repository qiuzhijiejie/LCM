/**************************************************
*****************12864158    9PIN SPI FOR Arduino*************
*****3----3SCK,   4----4SDA,      5---5RST,
      6---6DC,      7----7CS
*****
本模块自带字库型号：
*********ST7565

90度旋转显示，图片需要更改取模方式

演示代码仅演示16*15汉字旋转
********************************************************/
#define SPI_SDA_0  digitalWrite(3,LOW)             
#define SPI_SDA_1  digitalWrite(3,HIGH) 
#define SPI_SCK_0  digitalWrite(4,LOW)               
#define SPI_SCK_1  digitalWrite(4,HIGH)
#define SPI_DC_0  digitalWrite(5,LOW)               
#define SPI_DC_1  digitalWrite(5,HIGH)
#define SPI_RST_0  digitalWrite(6,LOW)               
#define SPI_RST_1  digitalWrite(6,HIGH)
#define SPI_CS_0  digitalWrite(7,LOW)               
#define SPI_CS_1  digitalWrite(7,HIGH)
#define SPI_CS2_0  digitalWrite(9,LOW)               
#define SPI_CS2_1  digitalWrite(9,HIGH)
#define SPI_FSO    digitalRead(8)

void  IO_init(void )
{
pinMode(3,OUTPUT);//设置数字脚为输出
pinMode(4,OUTPUT);//设置数字脚为输出
pinMode(5,OUTPUT);//设置数字脚为输出
pinMode(6,OUTPUT);//设置数字脚为输出
pinMode(7,OUTPUT);//设置数字脚为输出
pinMode(9,OUTPUT);//设置数字脚为输出
pinMode(8,INPUT);//设置数字脚为输入
}


#define LCD_DIS_CHINA_ROT  90        //选择屏幕旋转参数，不旋转0，旋转90度：90，旋转180度：180
//#define LCD_DIS_CHINA_ROT  180
//#define LCD_DIS_CHINA_ROT  0

#define LCD_COLUMN_NUMBER 128
#define LCD_LINE_NUMBER 64

  #if (LCD_DIS_CHINA_ROT == 180) 
  #define LCD_COLUMN_OFFSET 0         //旋转180度需要加4个偏移量
  #else 
  #define LCD_COLUMN_OFFSET 4
  #endif
  
#define LCD_PAGE_NUMBER (LCD_LINE_NUMBER/8)
#define LCD_COMMAND_NUMBER  13

const unsigned char *point;
unsigned char READ_BACK[32];                    ////一个汉字最大返回32个字节
unsigned char CHAR_DATA[32];
const unsigned char  LCD_init_cmd[LCD_COMMAND_NUMBER]=
{
        0xe2, //软复位

  0x2c, //升压步聚1
  
  0x2e, //升压步聚2
  
  0x2f, //升压步聚3
  
  0x26, //粗调对比度，可设置范围0x20～0x27
  0x81, //微调对比度
  0x18, //微调对比度的值，可设置范围0x00～0x3f
  0xa2, //1/9 偏压比（bias）
  #if (LCD_DIS_CHINA_ROT == 180) 
  0xA0, //列扫描顺序：从左到右
  0xc0, //行扫描顺序：反序
  #else 
  0xA1, //列扫描顺序：从左到右
  0xc8, //行扫描顺序：反序
  #endif
  0xa6, //正显
  0x40, //起始行：第一行开始
  0xaf, //开显示
};

const unsigned char  picture_tab[]={
/*------------------------------------------------------------------------------
;  列行式，低位在前，阴码
;  宽×高（像素）: 128×64
------------------------------------------------------------------------------*/
    

};

void delay_us(unsigned int _us_time)
{       
  unsigned char x=0;
  for(;_us_time>0;_us_time--)
  {
    x++;
  }
}

/*************SPI配置函数*******************
SCL空闲时低电平，第一个上升沿采样
模拟SPI
******************************************/

/**************************SPI模块发送函数************************************************

 *************************************************************************/
void SPI_SendByte(unsigned char byte)
{
  
  unsigned char counter;
   
  for(counter=0;counter<8;counter++)
  { 
    SPI_SCK_0;
    delay_us(1);
    if((byte&0x80)==0)
    {
      SPI_SDA_0;
      delay_us(1);
    }
    else SPI_SDA_1;
    byte=byte<<1;
  delay_us(1);
    SPI_SCK_1;  
  delay_us(1);
  SPI_SCK_0;
    
  } 
}

void LCD_send_cmd(unsigned char o_command)
  {
    SPI_DC_0;
    delay_us(1);
    SPI_CS_0;
    SPI_SendByte(o_command);
    SPI_CS_1;
   
    //SPI_DC_1;
  }
void LCD_send_data(unsigned char o_data)
  { 
    SPI_DC_1;
    delay_us(1);
    SPI_CS_0;
    SPI_SendByte(o_data);
    SPI_CS_1;
    
   }
void Column_set(unsigned char column)
  {
  column+=LCD_COLUMN_OFFSET;
    LCD_send_cmd(0x10|(column>>4));    //设置列地址高位
    LCD_send_cmd(0x00|(column&0x0f));   //设置列地址低位  
       
  }
void Page_set(unsigned char page)
  {
    LCD_send_cmd(0xb0+page);
  }
void LCD_clear(void)
  {
    unsigned char page,column;
    for(page=0;page<LCD_PAGE_NUMBER;page++)             //page loop
      { 
          Page_set(page);
          Column_set(0);    
          for(column=0;column<LCD_COLUMN_NUMBER;column++)  //column loop
            {
              LCD_send_data(0x00);
            }
      }
  }
void LCD_full(void)
  {
    unsigned char page,column;
    for(page=0;page<LCD_PAGE_NUMBER;page++)             //page loop
      { 
        Page_set(page);
        Column_set(0);    
  for(column=0;column<LCD_COLUMN_NUMBER;column++)  //column loop
          {
            LCD_send_data(0xff);
          }
      }
  }
void LCD_init(void)
  {
    unsigned char i;
    for(i=0;i<LCD_COMMAND_NUMBER;i++)
      {
        LCD_send_cmd(LCD_init_cmd[i]);
      }
  }

void Picture_display(const unsigned char *ptr_pic)
  {
    unsigned char page,column;
    for(page=0;page<(LCD_LINE_NUMBER/8);page++)        //page loop
      { 
  Page_set(page);
  Column_set(0);    
  for(column=0;column<LCD_COLUMN_NUMBER;column++)  //column loop
          {
            LCD_send_data(*ptr_pic++);
          }
      }
  }
void Picture_ReverseDisplay(const unsigned char *ptr_pic)
{
    unsigned char page,column,data;
    for(page=0;page<(LCD_LINE_NUMBER/8);page++)        //page loop
      { 
  Page_set(page);
  Column_set(0);    
  for(column=0;column<LCD_COLUMN_NUMBER;column++)  //column loop
          {
            data=*ptr_pic++;
            data=~data;
            LCD_send_data(data);
          }
      }
  }

