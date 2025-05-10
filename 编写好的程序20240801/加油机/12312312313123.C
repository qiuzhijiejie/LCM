//SMS1806D数码笔段型液晶显示模块的演示程序[C51]
//  本演示程序是在SMS1806D软件接口子程序集基础上开发出来的.通过简单设置SMS1806D中
//显示对象Num1..Num18数字及Z1..Z18标志,调用transram程序,就可以将要显示的数字及相
//应的标志在SMS1806D数码笔段型液晶显示模块上显示出来.通过使用本程序,加深理解SMS18
//06D软件接口子程序的使用方法,以及使用的的简洁性,用户上层程序的可移植性.
//  本演示的内容为,在SMS1806D液晶显示屏中所有数字显示0..9 空格一遍,所有数字依次显
//示8,所有标志依次显示,LCD所有数字重复显示0..9 空格一遍,所有数字依次显示8,所有标
//志依次隐藏,LCD液晶显示控制器进入睡眠状态.
//-------------------------------------------------------------------------------
//创建日期: 2009.08.08
//软件环境: VW For Windows
//硬件环境: AT89C52 MCU,MCS-51系列 MCU,etc.
//创建人: 褚建军(chujianjun@sunman.cn),谭超(tanchao@sunman.cn).
//技术支持: 电话(0731-84167806,84167807),传真(0731-84167807),网址(www.sunman.cn)
//版权信息: 长沙太阳人电子有限公司版权所有,转载请指明出处.
//参考网页: http:/www.sunman.cn/lcm/product/SMS1806D.html
//-------------------------------------------------------------------------------
#include <reg51.h>
#include <intrins.h>
//以下为演示程序的函数定义
void exsample(void);                    //演示程序
void exdelay(void);                     //演示延时子程序
void numfill(unsigned char d);          //所有数字填充子程序
void zfill(bit d);                      //所有Z标志填充子程序
void decodetolcd1(void);                //液晶显示控制器1译码子程序
void decodetolcd1(void);                //液晶显示控制器2译码子程序
void transram(void);                    //LCD显示刷新子程序
void lcdwc(unsigned char c);            //送控制字到液晶显示控制器子程序
void lcdsleep(void);                    //LCD液晶显示控制器进入睡眠状态子程序
void transbyte(unsigned char d);        //送1字节数据到液晶显示控制器子程序
void transbit(bit d);                   //送1位数据到液晶显示控制器子程序
//-------------------------------------------------------------------------------
//SMS1806D产品引脚说明
//PIN1: CS2 [片选2信号输入]
//PIN2: CS1 [片选1信号输入]
//PIN3: CLK [串行移位脉冲输入]
//PIN4: DI  [串行数据输入]
//PIN5: VSS [电源地]
//PIN6: VDD [电源正极]
//-------------------------------------------------------------------------------
//以下为产品接口引脚在演示程序中的预定义
//用户在编写应用程序时,需按自己的实际硬件连线来重新定义
sbit CS2PIN  = P1^0;                    //CS2对应单片机引脚
sbit CS1PIN  = P1^1;                    //CS1对应单片机引脚
sbit CLKPIN  = P1^2;                    //CLK对应单片机引脚
sbit DIPIN   = P1^3;                    //DI对应单片机引脚
//-------------------------------------------------------------------------------
//以下Num1_Ram至Num18_Ram为数字1至18在演示程序中的RAM地址预定义
//用户在编写应用程序时,需按自己的实际软件程序需要来重新定义
#define NUMBERS 18                      //显示数字的个数
unsigned char Num1_Ram;                 //数字1
unsigned char Num2_Ram;                 //数字2
unsigned char Num3_Ram;                 //数字3
unsigned char Num4_Ram;                 //数字4
unsigned char Num5_Ram;                 //数字5
unsigned char Num6_Ram;                 //数字6
unsigned char Num7_Ram;                 //数字7
unsigned char Num8_Ram;                 //数字8
unsigned char Num9_Ram;                 //数字9
unsigned char Num10_Ram;                //数字10
unsigned char Num11_Ram;                //数字11
unsigned char Num12_Ram;                //数字12
unsigned char Num13_Ram;                //数字13
unsigned char Num14_Ram;                //数字14
unsigned char Num15_Ram;                //数字15
unsigned char Num16_Ram;                //数字16
unsigned char Num17_Ram;                //数字17
unsigned char Num18_Ram;                //数字18
//-------------------------------------------------------------------------------
//以下Z1_Ram至Z18_Ram,Z1_BMASK至Z18_BMASK为Z标志1至18在演示程序中的RAM及BIT的预定义
//用户在编写应用程序时,需按自己的实际软件程序需要来重新定义
#define ZNUMBERS 18                     //显示标志的个数
unsigned char Z1_Ram _at_ 0x32;         //Z1[左箭头1]
#define  Z1_BMASK  0x01                 //[Z1_Ram].0
unsigned char Z2_Ram _at_ 0x32;         //Z2[左箭头2]
#define  Z2_BMASK  0x02                 //[Z2_Ram].1
unsigned char Z3_Ram _at_ 0x32;         //Z3[左箭头3]
#define  Z3_BMASK  0x04                 //[Z3_Ram].2
unsigned char Z4_Ram _at_ 0x32;         //Z4[右箭头1]
#define  Z4_BMASK  0x08                 //[Z4_Ram].3
unsigned char Z5_Ram _at_ 0x32;         //Z5[右箭头2]
#define  Z5_BMASK  0x10                 //[Z5_Ram].4
unsigned char Z6_Ram _at_ 0x32;         //Z6[右箭头3]
#define  Z6_BMASK  0x20                 //[Z6_Ram].5
unsigned char Z7_Ram _at_ 0x32;         //Z7[H2]
#define  Z7_BMASK  0x40                 //[Z7_Ram].6
unsigned char Z8_Ram _at_ 0x32;         //Z8[H3]
#define  Z8_BMASK  0x80                 //[Z8_Ram].7
unsigned char Z9_Ram _at_ 0x33;         //Z9[H4]
#define  Z9_BMASK  0x01                 //[Z9_Ram].0
unsigned char Z10_Ram _at_ 0x33;        //Z10[H5]
#define  Z10_BMASK 0x02                 //[Z10_Ram].1
unsigned char Z11_Ram _at_ 0x33;        //Z11[H8]
#define  Z11_BMASK 0x04                 //[Z11_Ram].2
unsigned char Z12_Ram _at_ 0x33;        //Z12[H9]
#define  Z12_BMASK 0x08                 //[Z12_Ram].3
unsigned char Z13_Ram _at_ 0x33;        //Z13[H10]
#define  Z13_BMASK 0x10                 //[Z13_Ram].4
unsigned char Z14_Ram _at_ 0x33;        //Z14[H11]
#define  Z14_BMASK 0x20                 //[Z14_Ram].5
unsigned char Z15_Ram _at_ 0x33;        //Z15[H14]
#define  Z15_BMASK 0x40                 //[Z15_Ram].6
unsigned char Z16_Ram _at_ 0x33;        //Z16[H15]
#define  Z16_BMASK 0x80                 //[Z16_Ram].7
unsigned char Z17_Ram _at_ 0x34;        //Z17[H16]
#define  Z17_BMASK 0x01                 //[Z17_Ram].0
unsigned char Z18_Ram _at_ 0x34;        //Z18[H17]
#define  Z18_BMASK 0x02                 //[Z18_Ram].1
//-------------------------------------------------------------------------------
void main(void)                         //主程序
{
  while(1)
  {
    exsample();                         //调用演示程序
  }
}
void exsample(void)                     //演示程序
{ unsigned char i;
//0.演示前的准备,将LCD液晶显示所有标志及数字全清零
  zfill(0);                             //所有Z标志清零
  numfill(0);                           //所有数字清零

//1.数字演示: 所有数字显示0..9 空格一遍
  for(i=0;i<11;i++)
  {
    numfill(i);                         //数字缓冲区全部填充为i的值
    transram();                         //LCD显示刷新
    exdelay();                          //延时300mS
  }

//2.数字演示: 所有数字依次显示8
  Num1_Ram=8;                           //数字1显示8
  transram();                           //LCD显示刷新
  exdelay();                            //延时300mS

  Num2_Ram=8;                           //数字2显示8
  transram();                           //LCD显示刷新
  exdelay();                            //延时300mS

  Num3_Ram=8;                           //数字3显示8
  transram();                           //LCD显示刷新
  exdelay();                            //延时300mS

  Num4_Ram=8;                           //数字4显示8
  transram();                           //LCD显示刷新
  exdelay();                            //延时300mS

  Num5_Ram=8;                           //数字5显示8
  transram();                           //LCD显示刷新
  exdelay();                            //延时300mS

  Num6_Ram=8;                           //数字6显示8
  transram();                           //LCD显示刷新
  exdelay();                            //延时300mS

  Num7_Ram=8;                           //数字7显示8
  transram();                           //LCD显示刷新
  exdelay();                            //延时300mS

  Num8_Ram=8;                           //数字8显示8
  transram();                           //LCD显示刷新
  exdelay();                            //延时300mS

  Num9_Ram=8;                           //数字9显示8
  transram();                           //LCD显示刷新
  exdelay();                            //延时300mS

  Num10_Ram=8;                          //数字10显示8
  transram();                           //LCD显示刷新
  exdelay();                            //延时300mS

  Num11_Ram=8;                          //数字11显示8
  transram();                           //LCD显示刷新
  exdelay();                            //延时300mS

  Num12_Ram=8;                          //数字12显示8
  transram();                           //LCD显示刷新
  exdelay();                            //延时300mS

  Num13_Ram=8;                          //数字13显示8
  transram();                           //LCD显示刷新
  exdelay();                            //延时300mS

  Num14_Ram=8;                          //数字14显示8
  transram();                           //LCD显示刷新
  exdelay();                            //延时300mS

  Num15_Ram=8;                          //数字15显示8
  transram();                           //LCD显示刷新
  exdelay();                            //延时300mS

  Num16_Ram=8;                          //数字16显示8
  transram();                           //LCD显示刷新
  exdelay();                            //延时300mS

  Num17_Ram=8;                          //数字17显示8
  transram();                           //LCD显示刷新
  exdelay();                            //延时300mS

  Num18_Ram=8;                          //数字18显示8
  transram();                           //LCD显示刷新
  exdelay();                            //延时300mS

//3.标志演示: 所有标志依次显示
  Z1_Ram|=Z1_BMASK;                     //Z1:左箭头1
  transram();                           //LCD显示刷新
  exdelay();                            //延时300mS

  Z2_Ram|=Z2_BMASK;                     //Z2:左箭头2
  transram();                           //LCD显示刷新
  exdelay();                            //延时300mS

  Z3_Ram|=Z3_BMASK;                     //Z3:左箭头3
  transram();                           //LCD显示刷新
  exdelay();                            //延时300mS

  Z4_Ram|=Z4_BMASK;                     //Z4:右箭头1
  transram();                           //LCD显示刷新
  exdelay();                            //延时300mS

  Z5_Ram|=Z5_BMASK;                     //Z5:右箭头2
  transram();                           //LCD显示刷新
  exdelay();                            //延时300mS

  Z6_Ram|=Z6_BMASK;                     //Z6:右箭头3
  transram();                           //LCD显示刷新
  exdelay();                            //延时300mS

  Z7_Ram|=Z7_BMASK;                     //Z7:H2
  transram();                           //LCD显示刷新
  exdelay();                            //延时300mS

  Z8_Ram|=Z8_BMASK;                     //Z8:H3
  transram();                           //LCD显示刷新
  exdelay();                            //延时300mS

  Z9_Ram|=Z9_BMASK;                     //Z9:H4
  transram();                           //LCD显示刷新
  exdelay();                            //延时300mS

  Z10_Ram|=Z10_BMASK;                   //Z10:H5
  transram();                           //LCD显示刷新
  exdelay();                            //延时300mS

  Z11_Ram|=Z11_BMASK;                   //Z11:H8
  transram();                           //LCD显示刷新
  exdelay();                            //延时300mS

  Z12_Ram|=Z12_BMASK;                   //Z12:H9
  transram();                           //LCD显示刷新
  exdelay();                            //延时300mS

  Z13_Ram|=Z13_BMASK;                   //Z13:H10
  transram();                           //LCD显示刷新
  exdelay();                            //延时300mS

  Z14_Ram|=Z14_BMASK;                   //Z14:H11
  transram();                           //LCD显示刷新
  exdelay();                            //延时300mS

  Z15_Ram|=Z15_BMASK;                   //Z15:H14
  transram();                           //LCD显示刷新
  exdelay();                            //延时300mS

  Z16_Ram|=Z16_BMASK;                   //Z16:H15
  transram();                           //LCD显示刷新
  exdelay();                            //延时300mS

  Z17_Ram|=Z17_BMASK;                   //Z17:H16
  transram();                           //LCD显示刷新
  exdelay();                            //延时300mS

  Z18_Ram|=Z18_BMASK;                   //Z18:H17
  transram();                           //LCD显示刷新
  exdelay();                            //延时300mS

//4.数字演示: 所有数字重复显示0..9 空格一遍
  for(i=0;i<11;i++)
  {
    numfill(i);                         //数字缓冲区全部填充为i的值
    transram();                         //LCD显示刷新
    exdelay();                          //延时300mS
  }

//5.数字演示: 所有数字依次显示8
  Num1_Ram=8;                           //数字1显示8
  transram();                           //LCD显示刷新
  exdelay();                            //延时300mS

  Num2_Ram=8;                           //数字2显示8
  transram();                           //LCD显示刷新
  exdelay();                            //延时300mS

  Num3_Ram=8;                           //数字3显示8
  transram();                           //LCD显示刷新
  exdelay();                            //延时300mS

  Num4_Ram=8;                           //数字4显示8
  transram();                           //LCD显示刷新
  exdelay();                            //延时300mS

  Num5_Ram=8;                           //数字5显示8
  transram();                           //LCD显示刷新
  exdelay();                            //延时300mS

  Num6_Ram=8;                           //数字6显示8
  transram();                           //LCD显示刷新
  exdelay();                            //延时300mS

  Num7_Ram=8;                           //数字7显示8
  transram();                           //LCD显示刷新
  exdelay();                            //延时300mS

  Num8_Ram=8;                           //数字8显示8
  transram();                           //LCD显示刷新
  exdelay();                            //延时300mS

  Num9_Ram=8;                           //数字9显示8
  transram();                           //LCD显示刷新
  exdelay();                            //延时300mS

  Num10_Ram=8;                          //数字10显示8
  transram();                           //LCD显示刷新
  exdelay();                            //延时300mS

  Num11_Ram=8;                          //数字11显示8
  transram();                           //LCD显示刷新
  exdelay();                            //延时300mS

  Num12_Ram=8;                          //数字12显示8
  transram();                           //LCD显示刷新
  exdelay();                            //延时300mS

  Num13_Ram=8;                          //数字13显示8
  transram();                           //LCD显示刷新
  exdelay();                            //延时300mS

  Num14_Ram=8;                          //数字14显示8
  transram();                           //LCD显示刷新
  exdelay();                            //延时300mS

  Num15_Ram=8;                          //数字15显示8
  transram();                           //LCD显示刷新
  exdelay();                            //延时300mS

  Num16_Ram=8;                          //数字16显示8
  transram();                           //LCD显示刷新
  exdelay();                            //延时300mS

  Num17_Ram=8;                          //数字17显示8
  transram();                           //LCD显示刷新
  exdelay();                            //延时300mS

  Num18_Ram=8;                          //数字18显示8
  transram();                           //LCD显示刷新
  exdelay();                            //延时300mS

//6.标志演示: 所有标志依次隐藏
  Z1_Ram&=(0xff-Z1_BMASK);              //Z1:左箭头1
  transram();                           //LCD显示刷新
  exdelay();                            //延时300mS

  Z2_Ram&=(0xff-Z2_BMASK);              //Z2:左箭头2
  transram();                           //LCD显示刷新
  exdelay();                            //延时300mS

  Z3_Ram&=(0xff-Z3_BMASK);              //Z3:左箭头3
  transram();                           //LCD显示刷新
  exdelay();                            //延时300mS

  Z4_Ram&=(0xff-Z4_BMASK);              //Z4:右箭头1
  transram();                           //LCD显示刷新
  exdelay();                            //延时300mS

  Z5_Ram&=(0xff-Z5_BMASK);              //Z5:右箭头2
  transram();                           //LCD显示刷新
  exdelay();                            //延时300mS

  Z6_Ram&=(0xff-Z6_BMASK);              //Z6:右箭头3
  transram();                           //LCD显示刷新
  exdelay();                            //延时300mS

  Z7_Ram&=(0xff-Z7_BMASK);              //Z7:H2
  transram();                           //LCD显示刷新
  exdelay();                            //延时300mS

  Z8_Ram&=(0xff-Z8_BMASK);              //Z8:H3
  transram();                           //LCD显示刷新
  exdelay();                            //延时300mS

  Z9_Ram&=(0xff-Z9_BMASK);              //Z9:H4
  transram();                           //LCD显示刷新
  exdelay();                            //延时300mS

  Z10_Ram&=(0xff-Z10_BMASK);            //Z10:H5
  transram();                           //LCD显示刷新
  exdelay();                            //延时300mS

  Z11_Ram&=(0xff-Z11_BMASK);            //Z11:H8
  transram();                           //LCD显示刷新
  exdelay();                            //延时300mS

  Z12_Ram&=(0xff-Z12_BMASK);            //Z12:H9
  transram();                           //LCD显示刷新
  exdelay();                            //延时300mS

  Z13_Ram&=(0xff-Z13_BMASK);            //Z13:H10
  transram();                           //LCD显示刷新
  exdelay();                            //延时300mS

  Z14_Ram&=(0xff-Z14_BMASK);            //Z14:H11
  transram();                           //LCD显示刷新
  exdelay();                            //延时300mS

  Z15_Ram&=(0xff-Z15_BMASK);            //Z15:H14
  transram();                           //LCD显示刷新
  exdelay();                            //延时300mS

  Z16_Ram&=(0xff-Z16_BMASK);            //Z16:H15
  transram();                           //LCD显示刷新
  exdelay();                            //延时300mS

  Z17_Ram&=(0xff-Z17_BMASK);            //Z17:H16
  transram();                           //LCD显示刷新
  exdelay();                            //延时300mS

  Z18_Ram&=(0xff-Z18_BMASK);            //Z18:H17
  transram();                           //LCD显示刷新
  exdelay();                            //延时300mS

//7.节电演示: LCD液晶显示控制器进入睡眠状态,电流小于1uA
  lcdsleep();                           //LCD液晶显示控制器进入睡眠状态
  exdelay();                            //延时300mS

}
void exdelay(void)                      //演示延时子程序
{ unsigned char i,j,k;                  //延时约300mS
  for(i=0;i<30;i++)
    for(j=0;j<64;j++)
      for(k=0;k<51;k++);
}
//-------------------------------------------------------------------------------
//以下为SMS1806D软件接口子程序集[C51]
//本软件接口子程序集将显示定义成Num1..Num18数字对象及Z1..Z18标志对象,用户只需要
//在应用时将Num1..Num18数字赋值及Z1..Z18标志置1,调用transram程序,就可以将要显示
//的数字及相应的标志在SMS1806D数码笔段型液晶显示模块上显示出来.
//-------------------------------------------------------------------------------
//以下numfill,zfill,decodetolcd1,decodetolcd2为三线式串行硬件接口(B型)[两个芯片]
//的SMS1806D液晶显示模块的基本子程序,NUMCODE1TAB,NUMCODE2TAB为SMS1806D液晶显示模
//块的数字七段译码表
//用户可根据实际使用的标志及数字情况进行程序的删减或合并,地址映射表请参见
//http://www.sunman.cn/document/product/chinese/usermanual/SMS1806D.pdf
//-------------------------------------------------------------------------------
//子程序名称:void numfill(unsigned char d).
//功能:将数字缓冲区填充为d代表的数字.
//修改日期:2009.08.08
//修改人:chujianjun@sunman.cn,tanchao@sunman.cn
//-------------------------------------------------------------------------------
void numfill(unsigned char d)           //所有数字填充子程序
{
  Num1_Ram=d;                           //1
  Num2_Ram=d;                           //2
  Num3_Ram=d;                           //3
  Num4_Ram=d;                           //4
  Num5_Ram=d;                           //5
  Num6_Ram=d;                           //6
  Num7_Ram=d;                           //7
  Num8_Ram=d;                           //8
  Num9_Ram=d;                           //9
  Num10_Ram=d;                          //10
  Num11_Ram=d;                          //11
  Num12_Ram=d;                          //12
  Num13_Ram=d;                          //13
  Num14_Ram=d;                          //14
  Num15_Ram=d;                          //15
  Num16_Ram=d;                          //16
  Num17_Ram=d;                          //17
  Num18_Ram=d;                          //18
}
//-------------------------------------------------------------------------------
//子程序名称:void zfill(bit d).
//功能:将标志缓冲区中所有标志填充为d代表的内容(d为0时,标志不显示).
//修改日期:2009.08.08
//修改人:chujianjun@sunman.cn,tanchao@sunman.cn
//-------------------------------------------------------------------------------
void zfill(bit d)                       //所有Z标志填充子程序
{
  if(d)
  {                                     //d不为零,则所有标志置位
    Z1_Ram|=Z1_BMASK;                   //Z1:左箭头1
    Z2_Ram|=Z2_BMASK;                   //Z2:左箭头2
    Z3_Ram|=Z3_BMASK;                   //Z3:左箭头3
    Z4_Ram|=Z4_BMASK;                   //Z4:右箭头1
    Z5_Ram|=Z5_BMASK;                   //Z5:右箭头2
    Z6_Ram|=Z6_BMASK;                   //Z6:右箭头3
    Z7_Ram|=Z7_BMASK;                   //Z7:H2
    Z8_Ram|=Z8_BMASK;                   //Z8:H3
    Z9_Ram|=Z9_BMASK;                   //Z9:H4
    Z10_Ram|=Z10_BMASK;                 //Z10:H5
    Z11_Ram|=Z11_BMASK;                 //Z11:H8
    Z12_Ram|=Z12_BMASK;                 //Z12:H9
    Z13_Ram|=Z13_BMASK;                 //Z13:H10
    Z14_Ram|=Z14_BMASK;                 //Z14:H11
    Z15_Ram|=Z15_BMASK;                 //Z15:H14
    Z16_Ram|=Z16_BMASK;                 //Z16:H15
    Z17_Ram|=Z17_BMASK;                 //Z17:H16
    Z18_Ram|=Z18_BMASK;                 //Z18:H17
  }
  else
  {
    Z1_Ram&=(0xff-Z1_BMASK);            //Z1:左箭头1
    Z2_Ram&=(0xff-Z2_BMASK);            //Z2:左箭头2
    Z3_Ram&=(0xff-Z3_BMASK);            //Z3:左箭头3
    Z4_Ram&=(0xff-Z4_BMASK);            //Z4:右箭头1
    Z5_Ram&=(0xff-Z5_BMASK);            //Z5:右箭头2
    Z6_Ram&=(0xff-Z6_BMASK);            //Z6:右箭头3
    Z7_Ram&=(0xff-Z7_BMASK);            //Z7:H2
    Z8_Ram&=(0xff-Z8_BMASK);            //Z8:H3
    Z9_Ram&=(0xff-Z9_BMASK);            //Z9:H4
    Z10_Ram&=(0xff-Z10_BMASK);          //Z10:H5
    Z11_Ram&=(0xff-Z11_BMASK);          //Z11:H8
    Z12_Ram&=(0xff-Z12_BMASK);          //Z12:H9
    Z13_Ram&=(0xff-Z13_BMASK);          //Z13:H10
    Z14_Ram&=(0xff-Z14_BMASK);          //Z14:H11
    Z15_Ram&=(0xff-Z15_BMASK);          //Z15:H14
    Z16_Ram&=(0xff-Z16_BMASK);          //Z16:H15
    Z17_Ram&=(0xff-Z17_BMASK);          //Z17:H16
    Z18_Ram&=(0xff-Z18_BMASK);          //Z18:H17
  }
}
//-------------------------------------------------------------------------------
//数据表名称:NUMCODE1TAB.
//功能:将数字转换为七段码.
//修改日期:2009.08.08
//修改人:chujianjun@sunman.cn,tanchao@sunman.cn
//-------------------------------------------------------------------------------
//数码笔段定义:  8字的最上面一笔段为A笔段,顺时针方向依次为BCDEF笔段,中间的是G笔段
//数码笔段顺序:  D76543210
//                XCBADEGF
unsigned char code NUMCODE1TAB[]={0x7D,0x60,0x3E,0x7A,  //数字0,1,2,3
                                  0x63,0x5B,0x5F,0x70,  //数字4,5,6,7
                                  0x7F,0x7B,0x00,0x02,  //数字8,9,字符空格,字符-
                                  0x1D,0x37,0x1F,0x17}; //字符C,字符P,字符E,字符F
//-------------------------------------------------------------------------------
//数据表名称:NUMCODE2TAB.
//功能:将数字转换为七段码.
//修改日期:2009.08.08
//修改人:chujianjun@sunman.cn,tanchao@sunman.cn
//-------------------------------------------------------------------------------
//数码笔段顺序:  D76543210
//                DEGFXCBA
unsigned char code NUMCODE2TAB[]={0xD7,0x06,0xE3,0xA7,  //数字0,1,2,3
                                  0x36,0xB5,0xF5,0x07,  //数字4,5,6,7
                                  0xF7,0xB7,0x00,0x20,  //数字8,9,字符空格,字符-
                                  0xD1,0x73,0xF1,0x71}; //字符C,字符P,字符E,字符F
//-------------------------------------------------------------------------------
//子程序名称:void decodetolcd1(void).
//功能:按地址映射表字节顺序,所有数据译码后送LCD液晶显示控制器1.
//修改日期:2009.08.08
//修改人:chujianjun@sunman.cn,tanchao@sunman.cn
//-------------------------------------------------------------------------------
//地址映射表请参见
//http://www.sunman.cn/document/product/chinese/usermanual/SMS1806D.pdf
void decodetolcd1(void)                 //按地址映射表字节顺序,所有数据译码后送
{ unsigned char buf;                    //LCD液晶显示控制器1子程序
//LCDBUF+0 缓冲区第1个字节译码
  buf=0x00;                             //当前译码数据置初始值
                                        //需译码的BIT为ZCBADEGF位
  buf|=NUMCODE1TAB[Num7_Ram]&0x7F;      //第7个8字的CBADEGF笔段译码
  if((Z2_Ram&Z2_BMASK) != 0 )           //第2个标志[左箭头2]译码
    buf|=0x80;                          
  transbyte(buf);                       //将当前译码数据送LCD液晶显示控制器

//LCDBUF+1 缓冲区第2个字节译码
  buf=0x00;                             //当前译码数据置初始值
                                        //需译码的BIT为ZCBADEGF位
  buf|=NUMCODE1TAB[Num8_Ram]&0x7F;      //第8个8字的CBADEGF笔段译码
  if((Z11_Ram&Z11_BMASK) != 0 )         //第11个标志[H8]译码
    buf|=0x80;                          
  transbyte(buf);                       //将当前译码数据送LCD液晶显示控制器

//LCDBUF+2 缓冲区第3个字节译码
  buf=0x00;                             //当前译码数据置初始值
                                        //需译码的BIT为ZCBADEGF位
  buf|=NUMCODE1TAB[Num9_Ram]&0x7F;      //第9个8字的CBADEGF笔段译码
  if((Z12_Ram&Z12_BMASK) != 0 )         //第12个标志[H9]译码
    buf|=0x80;                          
  transbyte(buf);                       //将当前译码数据送LCD液晶显示控制器

//LCDBUF+3 缓冲区第4个字节译码
  buf=0x00;                             //当前译码数据置初始值
                                        //需译码的BIT为ZCBADEGF位
  buf|=NUMCODE1TAB[Num1_Ram]&0x7F;      //第1个8字的CBADEGF笔段译码
  if((Z1_Ram&Z1_BMASK) != 0 )           //第1个标志[左箭头1]译码
    buf|=0x80;                          
  transbyte(buf);                       //将当前译码数据送LCD液晶显示控制器

//LCDBUF+4 缓冲区第5个字节译码
  buf=0x00;                             //当前译码数据置初始值
                                        //需译码的BIT为ZCBADEGF位
  buf|=NUMCODE1TAB[Num2_Ram]&0x7F;      //第2个8字的CBADEGF笔段译码
  if((Z7_Ram&Z7_BMASK) != 0 )           //第7个标志[H2]译码
    buf|=0x80;                          
  transbyte(buf);                       //将当前译码数据送LCD液晶显示控制器

//LCDBUF+5 缓冲区第6个字节译码
  buf=0x00;                             //当前译码数据置初始值
                                        //需译码的BIT为ZCBADEGF位
  buf|=NUMCODE1TAB[Num3_Ram]&0x7F;      //第3个8字的CBADEGF笔段译码
  if((Z8_Ram&Z8_BMASK) != 0 )           //第8个标志[H3]译码
    buf|=0x80;                          
  transbyte(buf);                       //将当前译码数据送LCD液晶显示控制器

//LCDBUF+6 缓冲区第7个字节译码
  buf=0x00;                             //当前译码数据置初始值
                                        //需译码的BIT为ZCBADEGF位
  buf|=NUMCODE1TAB[Num4_Ram]&0x7F;      //第4个8字的CBADEGF笔段译码
  if((Z9_Ram&Z9_BMASK) != 0 )           //第9个标志[H4]译码
    buf|=0x80;                          
  transbyte(buf);                       //将当前译码数据送LCD液晶显示控制器

//LCDBUF+7 缓冲区第8个字节译码
  buf=0x00;                             //当前译码数据置初始值
                                        //需译码的BIT为ZCBADEGF位
  buf|=NUMCODE1TAB[Num5_Ram]&0x7F;      //第5个8字的CBADEGF笔段译码
  if((Z10_Ram&Z10_BMASK) != 0 )         //第10个标志[H5]译码
    buf|=0x80;                          
  transbyte(buf);                       //将当前译码数据送LCD液晶显示控制器

//LCDBUF+8 缓冲区第9个字节译码
  buf=0x00;                             //当前译码数据置初始值
                                        //需译码的BIT为ZCBADEGF位
  buf|=NUMCODE1TAB[Num6_Ram]&0x7F;      //第6个8字的CBADEGF笔段译码
  if((Z4_Ram&Z4_BMASK) != 0 )           //第4个标志[右箭头1]译码
    buf|=0x80;                          
  transbyte(buf);                       //将当前译码数据送LCD液晶显示控制器
}
//-------------------------------------------------------------------------------
//子程序名称:void decodetolcd2(void).
//功能:按地址映射表字节顺序,所有数据译码后送LCD液晶显示控制器2.
//修改日期:2009.08.08
//修改人:chujianjun@sunman.cn,tanchao@sunman.cn
//-------------------------------------------------------------------------------
//地址映射表请参见
//http://www.sunman.cn/document/product/chinese/usermanual/SMS1806D.pdf
void decodetolcd2(void)                 //按地址映射表字节顺序,所有数据译码后送
{ unsigned char buf;                    //LCD液晶显示控制器2子程序

//LCDBUF+9 缓冲区第10个字节译码
  buf=0x00;                             //当前译码数据置初始值
                                        //需译码的BIT为DEGFZCBA位
  buf|=NUMCODE2TAB[Num12_Ram]&0xF7;     //第12个8字的DEGFCBA笔段译码
  if((Z5_Ram&Z5_BMASK) != 0 )           //第5个标志[右箭头2]译码
    buf|=0x08;                          
  transbyte(buf);                       //将当前译码数据送LCD液晶显示控制器

//LCDBUF+10 缓冲区第11个字节译码
  buf=0x00;                             //当前译码数据置初始值
                                        //需译码的BIT为DEGFZCBA位
  buf|=NUMCODE2TAB[Num11_Ram]&0xF7;     //第11个8字的DEGFCBA笔段译码
  if((Z14_Ram&Z14_BMASK) != 0 )         //第14个标志[H11]译码
    buf|=0x08;                          
  transbyte(buf);                       //将当前译码数据送LCD液晶显示控制器

//LCDBUF+11 缓冲区第12个字节译码
  buf=0x00;                             //当前译码数据置初始值
                                        //需译码的BIT为DEGFZCBA位
  buf|=NUMCODE2TAB[Num10_Ram]&0xF7;     //第10个8字的DEGFCBA笔段译码
  if((Z13_Ram&Z13_BMASK) != 0 )         //第13个标志[H10]译码
    buf|=0x08;                          
  transbyte(buf);                       //将当前译码数据送LCD液晶显示控制器

//LCDBUF+12 缓冲区第13个字节译码
  buf=0x00;                             //当前译码数据置初始值
                                        //需译码的BIT为DEGFZCBA位
  buf|=NUMCODE2TAB[Num18_Ram]&0xF7;     //第18个8字的DEGFCBA笔段译码
  if((Z6_Ram&Z6_BMASK) != 0 )           //第6个标志[右箭头3]译码
    buf|=0x08;                          
  transbyte(buf);                       //将当前译码数据送LCD液晶显示控制器

//LCDBUF+13 缓冲区第14个字节译码
  buf=0x00;                             //当前译码数据置初始值
                                        //需译码的BIT为DEGFZCBA位
  buf|=NUMCODE2TAB[Num17_Ram]&0xF7;     //第17个8字的DEGFCBA笔段译码
  if((Z18_Ram&Z18_BMASK) != 0 )         //第18个标志[H17]译码
    buf|=0x08;                          
  transbyte(buf);                       //将当前译码数据送LCD液晶显示控制器

//LCDBUF+14 缓冲区第15个字节译码
  buf=0x00;                             //当前译码数据置初始值
                                        //需译码的BIT为DEGFZCBA位
  buf|=NUMCODE2TAB[Num16_Ram]&0xF7;     //第16个8字的DEGFCBA笔段译码
  if((Z17_Ram&Z17_BMASK) != 0 )         //第17个标志[H16]译码
    buf|=0x08;                          
  transbyte(buf);                       //将当前译码数据送LCD液晶显示控制器

//LCDBUF+15 缓冲区第16个字节译码
  buf=0x00;                             //当前译码数据置初始值
                                        //需译码的BIT为DEGFZCBA位
  buf|=NUMCODE2TAB[Num15_Ram]&0xF7;     //第15个8字的DEGFCBA笔段译码
  if((Z16_Ram&Z16_BMASK) != 0 )         //第16个标志[H15]译码
    buf|=0x08;                          
  transbyte(buf);                       //将当前译码数据送LCD液晶显示控制器

//LCDBUF+16 缓冲区第17个字节译码
  buf=0x00;                             //当前译码数据置初始值
                                        //需译码的BIT为DEGFZCBA位
  buf|=NUMCODE2TAB[Num14_Ram]&0xF7;     //第14个8字的DEGFCBA笔段译码
  if((Z15_Ram&Z15_BMASK) != 0 )         //第15个标志[H14]译码
    buf|=0x08;                          
  transbyte(buf);                       //将当前译码数据送LCD液晶显示控制器

//LCDBUF+17 缓冲区第18个字节译码
  buf=0x00;                             //当前译码数据置初始值
                                        //需译码的BIT为DEGFZCBA位
  buf|=NUMCODE2TAB[Num13_Ram]&0xF7;     //第13个8字的DEGFCBA笔段译码
  if((Z3_Ram&Z3_BMASK) != 0 )           //第3个标志[左箭头3]译码
    buf|=0x08;                          
  transbyte(buf);                       //将当前译码数据送LCD液晶显示控制器
}
//-------------------------------------------------------------------------------
//以下transram,transbyte,lcdsleep为三线式串行硬件接口(B型)[两个芯片]的基本子程序,
//transbit,lcdwc为内部子程序.
//-------------------------------------------------------------------------------
//子程序名称:void transram(void).
//功能:LCD显示刷新.
//修改日期:2009.08.08
//修改人:chujianjun@sunman.cn,tanchao@sunman.cn
//-------------------------------------------------------------------------------
#define CMDOFF      0                   //控制器掉电(控制字)
#define CMDON       0x80                //控制器上电(控制字)
#define CMDLCDOFF   0x40                //控制器液晶关闭(控制字)
#define CMDLCDON    0xc0                //控制器液晶显示(控制字)
#define CMDB3C4     0x94                //控制器液晶模式设置(控制字)
void transram(void)                     //LCD显示刷新子程序
{                                       //显示模式设置
    lcdwc(CMDOFF);                      //将液晶显示控制器关闭后打开
    lcdwc(CMDLCDOFF);                   //再置显示模式为1/3偏压,1/4占空比
    lcdwc(CMDON);
    lcdwc(CMDLCDON);
    lcdwc(CMDB3C4);
                                        //送数据前准备操作
    CS1PIN=1;                           //CS1PIN执行一个下降沿
    CS1PIN=0;
    transbit(1);                        //数据连续先送101000000系列后再开始送数
    transbyte(0x2);
                                        //开始送数
    decodetolcd1();                     //按地址映射表字节顺序,所有数据译码后送
                                        //LCD液晶显示控制器1
    CS1PIN=1;                           //送数完成后将CS1PIN置为低电平

                                        //送数据前准备操作
    CS2PIN=1;                           //CS2PIN执行一个下降沿
    CS2PIN=0;
    transbit(1);                        //数据连续先送101000000系列后再开始送数
    transbyte(0x2);
                                        //开始送数
    decodetolcd2();                     //按地址映射表字节顺序,所有数据译码后送
                                        //LCD2液晶显示控制器
    CS2PIN=1;                           //送数完成后将CS2PIN置为低电平
}
//-------------------------------------------------------------------------------
//子程序名称:void lcdwc(unsigned char c).
//功能:送控制字到液晶显示控制器.
//修改日期:2009.08.08
//修改人:chujianjun@sunman.cn,tanchao@sunman.cn
//-------------------------------------------------------------------------------
void lcdwc(unsigned char c)             //送控制字到液晶显示控制器子程序
{
    CS1PIN=1;                           //CS1PIN执行一个下降沿
    CS1PIN=0;
    CS2PIN=1;                           //CS2PIN执行一个下降沿
    CS2PIN=0;
    transbit(1);                        //数据连续先送100系列后再开始送控制字
    transbit(0);
    transbit(0);
    transbyte(c);                       //送控制字
    transbit(0);                        //送1位0
    CS2PIN=1;                           //送数完成后将CS2PIN置为高电平
    CS1PIN=1;                           //送数完成后将CS1PIN置为高电平
}
//-------------------------------------------------------------------------------
//子程序名称:void lcdsleep(void).
//功能:液晶显示控制器进入睡眠来减少功耗.
//修改日期:2009.08.08
//修改人:chujianjun@sunman.cn,tanchao@sunman.cn
//-------------------------------------------------------------------------------
void lcdsleep(void)                     //液晶显示控制器进入睡眠来减少功耗子程序
{
    lcdwc(CMDLCDOFF);                   //将液晶显示关闭
    lcdwc(CMDOFF);                      //LCD液晶显示控制器进入睡眠状态,此时LCD
}                                       //液晶显示控制器的电流小于1uA
//-------------------------------------------------------------------------------
//子程序名称:void transbyte(unsigned char d).
//功能:送1字节数据到液晶显示控制器.
//修改日期:2009.08.08
//修改人:chujianjun@sunman.cn,tanchao@sunman.cn
//-------------------------------------------------------------------------------
void transbyte(unsigned char d)         //送1字节数据到液晶显示控制器子程序
{   unsigned char i;
    for(i=0;i<8;i++)
    {   if((d&0x1) == 0x1)
            transbit(1);
        else
            transbit(0);
        d>>=1;                          //从低到高位送字节位数据到液晶显示控制器
    }
}
//-------------------------------------------------------------------------------
//子程序名称:void transbit(bit d).
//功能:送1位数据到液晶显示控制器.
//修改日期:2009.08.08
//修改人:chujianjun@sunman.cn,tanchao@sunman.cn
//-------------------------------------------------------------------------------
void transbit(bit d)                    //送1位数据到液晶显示控制器子程序
{  DIPIN = d;                           //先送数据到数据口线DI
   _nop_();
   _nop_();
   CLKPIN = 1;                          //再使时钟口线发一个负脉冲
   _nop_();
   _nop_();
   CLKPIN = 0;
   _nop_();
   _nop_();
   CLKPIN = 1;
}