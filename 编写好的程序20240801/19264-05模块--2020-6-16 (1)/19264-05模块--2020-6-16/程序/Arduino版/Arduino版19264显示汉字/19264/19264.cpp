#include "19264.h"

//向显示屏发送数据函数
//C=0表示发送的是显示数据 C=1表示发送的是指令代码
void LCD19264::WriteData(unsigned char data, unsigned char C)
{
  unsigned char i,dat;                        //i用于循环写入8位数据，dat用于缓存数据
  digitalWrite(CS,LOW);                       //使能片选
  if(C==1)                                    //判断要写入的是指令还是数据
  {
    digitalWrite(CD,HIGH);                    //等于1 就是数据
  }
  else
  {
    digitalWrite(CD,LOW);                     //等于0 就是指令
  }
  dat=data;                                   //缓存数据
  for(i=0;i<8;i++)                            //循环发送8位数据
  {
    if((dat&0x80)==0x80)                      //判断最高位是否为1
    {
      digitalWrite(SDA,HIGH);                 //为1 拉高总线写入1
    }
    else
    {                
      digitalWrite(SDA,LOW);                  //为0 拉低总线写入0
    }
    digitalWrite(SCK,LOW);                    //产生一个时钟
    delayMicroseconds(2);
    
    digitalWrite(SCK,HIGH);                   //时钟结束
    delayMicroseconds(2);
    
    dat<<=1;                                  //左移一位 准备写入下一位
  }
  digitalWrite(CS,HIGH);                      //写入完毕，取消片选
}

//显示屏初始化函数
void LCD19264::Init_lcd(void)
{
  pinMode(CS,OUTPUT);                         //设置显示屏片选引脚为输出
  pinMode(CD,OUTPUT);                         //设置显示屏指令数据引脚为输出
  pinMode(RST,OUTPUT);                        //设置显示屏复位引脚为输出
  pinMode(SDA,OUTPUT);                        //设置显示屏数据引脚为输出
  pinMode(SCK,OUTPUT);                        //设置显示屏时钟引脚为输出
  digitalWrite(CS,HIGH);                      //初始化显示屏片选引脚为高
  digitalWrite(CD,HIGH);                      //初始化显示屏指令数据引脚为高
  digitalWrite(RST,HIGH);                     //初始化显示屏复位引脚为高
  digitalWrite(SDA,HIGH);                     //初始化显示屏数据引脚为高
  digitalWrite(SCK,HIGH);                     //初始化显示屏时钟引脚为高
  
  delay(50);                                  //上电延时 等待电源稳定
  
  digitalWrite(RST,LOW);                      //复位显示屏
  delay(3);
  digitalWrite(RST,HIGH);                     //复位完成
  delay(6);                                   //注意：此延时必须要有且保持5毫秒左右
  
  LCD19264::WriteData(0xe2,0);                //显示屏复位指令
  LCD19264::WriteData(0xa3,0);                //设置帧速率[A0: 76fps, A1b: 95fps, A2b: 132fps, A3b: 168fps(fps: frame-per-second)] 
  LCD19264::WriteData(0xeb,0);                //设置LCD偏置比(亮度设置)
  LCD19264::WriteData(0x2f,0);                //显示屏功耗设置
  LCD19264::WriteData(0xc2,0);                //设置LCD映射控制
  LCD19264::WriteData(0x81,0);                //设置SEG偏置电压(对比度) 双字节指令
  LCD19264::WriteData(180,0);                 //设置SEG偏置电压(对比度) 双字节指令
  LCD19264::WriteData(0xaf,0);                //开启显示指令
}

//图片显示函数
void LCD19264::Dispgraphic(unsigned char D)
{
  unsigned char i,j;                          //用于循环写入显示数据
  for(i=0;i<8;i++)                            //总共写入8页的数据
  {
    LCD19264::WriteData(0x40,0);              //设置滚动线
    LCD19264::WriteData(0xb0|i,0);            //设置页地址
    
    LCD19264::WriteData(0x10,0);              //设置列地址 （双字节指令）
    LCD19264::WriteData(0x00,0);              //设置列地址为0从第一列开始写数据
    
    for(j=0;j<192;j++)                        //准备写入192列数据
    {
      LCD19264::WriteData(D,1);               //写入显示数据
    }
  }
}

//16*16汉字显示函数
//y:页地址  l:列地址  *p:要显示汉字的指针
void LCD19264::Display_Chinese(unsigned char y,unsigned char l,unsigned char *p)
{
  unsigned char i,j;                        //用于循环写入图片数据
  for(i=0;i<2;i++)                          //总共写入8页的数据
  {
    WriteData((0xb0|i)+y,0);                //设置页地址
    
    WriteData(0x10+(l>>4&0x0F),0);          //设置列地址（双字节指令）
    WriteData(l&0x0F,0);                    //设置列地址为0从第一列开始写数据
    
    for(j=0;j<16;j++)                       //准备写入16列数据
    {
      WriteData(*p++,1);                    //写入显示数据
    }
  }
}
