/SMG12232ZK标准字符点阵型液晶显示模块的演示程序[C51编程语言][MCS51三线式串行接口方式]
//应用产品: SMG12232ZK标准中文字符点阵型液晶显示模块
//  本演示程序适用于SMG12232ZK液晶显示模块与MCS51系列单片机采用MCS51三线式串行接口
//方式的硬件连线方式。
//  本演示程序包括ST7920兼容芯片的MCS51三线式串行接口方式子程序集,SMG12232系列标准中文
//字符点阵型液晶显示模块的接口子程序集,ST7920兼容芯片的液晶显示控制器的通用子程序
//集,以及SMG12232系列标准中文字符点阵型液晶显示的演示子程序.
//  本演示的内容为,在15列ASCII字符(7.5列中文)X2行的液晶显示屏上显示" SMG12232ZK ",
//"中文字符图形点阵","液晶显示模块","长沙太阳人SUNMAN",所有数字0..9依次全屏显示
//一遍,显示清屏,所有英文字母依次显示,显示自定义字符,图形模式图片显示.
//-------------------------------------------------------------------------------
//创建日期: 2009.11.02
//软件环境: VW For Windows
//硬件环境: AT89C52 MCU,MCS-51系列 MCU,etc.
//创建人: 褚建军(chujianjun@sunman.cn),谭超(tanchao@sunman.cn).
//技术支持: 电话(0731-84167806,84167807),传真(0731-84167807),网址(www.sunman.cn)
//版权信息: 长沙太阳人电子有限公司版权所有,转载请指明出处.
//参考网页: http:/www.sunman.cn/lcm/product/SMG12232ZK.html
//-------------------------------------------------------------------------------
#include <reg51.h>
#include <intrins.h>
//以下为演示程序的函数定义
void exsample(void);                    //演示程序
void exdelay(void);                     //演示延时子程序
void putchar(unsigned int c);           //定位写字符子程序
void putstr(unsigned char code *s);     //显示字符串子程序,字符码为0时退出
void putstrxy(unsigned char cx,unsigned char cy,unsigned char code *s);
void setcharcgram(unsigned char cindex,unsigned char code *s);
                                        //设置自定义字符点阵
void charcursornext(void);              //置字符位置为下一个有效位置子程序
void putimage(unsigned char code *s);   //显示图形子程序
void putsizeimage(unsigned char XSIZE,unsigned char YSIZE,unsigned char code *s);
                                        //显示指定大小的图形子程序
void charlcdfill(unsigned int c);       //整屏显示c代表的字符数据子程序
void glcdfill(unsigned char d);         //整屏显示d代表的字节数据子程序
void glcdpos(void);                     //写入绘图区域内部RAM指针子程序
void charlcdpos(void);                  //设置(CXPOS,CYPOS)内部DDRAM地址的子程序
void lcdreset(void);                    //液晶显示控制器初始化子程序
void delay3ms(void);                    //延时3MS子程序
void lcdon(void);                       //LCD显示开启子程序
void lcdoff(void);                      //LCD显示关闭子程序
void lcdgraphon(void);                  //绘图区域显示开启子程序
void lcdgraphoff(void);                 //绘图区域显示关闭子程序
void lcdwd(unsigned char d);            //送图形数据子程序
void lcdwc(unsigned char c);            //送指令子程序
void transbyte(unsigned char d);        //送1字节数据到液晶显示控制器子程序
void transbit(bit d);                   //送1位数据到液晶显示控制器子程序
//-------------------------------------------------------------------------------
//SMG12232ZK产品引脚说明及演示连线
//PIN1: VSS  [电源地]-------------------VSS
//PIN2: VDD  [电源正极]-----------------VDD(+5V)
//PIN3: NC   [空脚]
//PIN4: CS   [片选信号输入]-------------P3.0
//PIN5: STD  [串行数据输入]-------------P3.1
//PIN6: SCLK [串行移位脉冲输入]---------P3.2
//PIN7: NC   [空脚]
//PIN8: NC   [空脚]
//PIN9: NC   [空脚]
//PIN10:NC   [空脚]
//PIN11:NC   [空脚]
//PIN12:NC   [空脚]
//PIN13:NC   [空脚]
//PIN14:NC   [空脚]
//PIN15:PSB  [接口模式选择 可悬空后直接在液晶显示模块中接地]
//PIN16:RST  [复位端,低电平有效]--------VDD
//PIN17:A    [背光源正极]
//PIN18:K    [背光源负极]
//请参见http://download.sunman.cn/lcm/product/1/SMG12232ZK.pdf
//字符表参见http://download.sunman.cn/lcm/reference/ST7920_GB_CHARACTER_TABLE.pdf
//芯片资料参见http://download.sunman.cn/lcm/reference/ST7920.pdf
//-------------------------------------------------------------------------------
//以下为产品接口引脚在演示程序中的预定义
//用户在编写应用程序时,需按自己的实际硬件连线来重新定义
sbit CSPIN   = P3^0;                    // CS对应单片机引脚
sbit STDPIN  = P3^1;                    // STD对应单片机引脚
sbit SCLKPIN = P3^2;                    // SCLK对应单片机引脚
sbit PSBPIN  = P3^3;                    // PSB对应单片机引脚,也可直接在液晶显示
                                        // 模块中直接硬件短接到地
//-------------------------------------------------------------------------------
//以下CXPOS,CYPOS变量用于指示当前操作字符的位置的预定义
unsigned char CXPOS;                    //列方向地址指针(用于charlcdpos子程序)
unsigned char CYPOS;                    //行方向地址指针(用于charlcdpos子程序)
unsigned char FCHARBUF;                 //上一个显示的ASCII字符

//-------------------------------------------------------------------------------
//以下GXPOS,GYPOS变量用于指示绘图区域RAM的位置的预定义
//用户在编写应用程序时,需按自己的实际软件程序需要来重新定义
unsigned char GXPOS;                    //列方向地址指针(用于glcdpos子程序)
unsigned char GYPOS;                    //行方向地址指针(用于glcdpos子程序)

#define USERCHAR1CODE 0xffff            //自定义字符1代码
#define USERCHAR2CODE 0xfffd            //自定义字符2代码
#define USERCHAR3CODE 0xfffb            //自定义字符3代码
#define USERCHAR4CODE 0xfff9            //自定义字符4代码

unsigned char code CHAR1CGRAMTAB[]={
0x00,0x0f,
0x00,0x7f,
0x01,0xf0,
0x03,0xc0,
0x07,0x0f,
0x0e,0x3f,
0x1c,0xfe,
0x39,0xfc,
0x33,0xfc,
0x73,0xfe,
0x67,0xff,
0x67,0xe7,
0xcf,0xe3,
0xcf,0xe1,
0xcf,0xe4,
0xc0,0x06
};
unsigned char code CHAR2CGRAMTAB[]={
0xf0,0x00,
0xfe,0x00,
0x0f,0x80,
0x03,0xc0,
0xf0,0xe0,
0xfc,0x70,
0x7f,0x38,
0x3f,0x9c,
0x3f,0xcc,
0x7f,0xce,
0xff,0xe6,
0xe7,0xe6,
0xc7,0xf3,
0x87,0xff,
0x27,0xff,
0x60,0x03
};
unsigned char code CHAR3CGRAMTAB[]={
0xc0,0x07,
0xff,0xe7,
0xff,0xe7,
0xcf,0xe7,
0x67,0xe7,
0x67,0xff,
0x73,0xff,
0x33,0xff,
0x39,0xff,
0x1c,0xff,
0x0e,0x3f,
0x07,0x0f,
0x03,0xc0,
0x01,0xf0,
0x00,0x7f,
0x00,0x0f
};
unsigned char code CHAR4CGRAMTAB[]={
0xe0,0x03,
0xe7,0xf3,
0xe7,0xf3,
0xe7,0xf3,
0xe7,0xe6,
0xff,0xe6,
0xff,0xce,
0xff,0xcc,
0xff,0x9c,
0xff,0x38,
0xfc,0x70,
0xf0,0xe0,
0x03,0xc0,
0x0f,0x80,
0xfe,0x00,
0xf0,0x00
};
//-------------------------------------------------------------------------------
//以下为图片库点阵代码
//实际使用时请包含由SUNMAN图形点阵代码生成器.EXE自动生成的imagelib.c文件来替换该
//部分
unsigned char code Img_sunman_32x32[]={32,32,
0x00,0x0f,0xf0,0x00,
0x00,0x7f,0xfe,0x00,
0x01,0xf0,0x0f,0x80,
0x03,0xc0,0x03,0xc0,
0x07,0x0f,0xf0,0xe0,
0x0e,0x3f,0xfc,0x70,
0x1c,0xfe,0x7f,0x38,
0x39,0xfc,0x3f,0x9c,
0x33,0xfc,0x3f,0xcc,
0x73,0xfe,0x7f,0xce,
0x67,0xff,0xff,0xe6,
0x67,0xe7,0xe7,0xe6,
0xcf,0xe3,0xc7,0xf3,
0xcf,0xe1,0x87,0xff,
0xcf,0xe4,0x27,0xff,
0xc0,0x06,0x60,0x03,
0xc0,0x07,0xe0,0x03,
0xff,0xe7,0xe7,0xf3,
0xff,0xe7,0xe7,0xf3,
0xcf,0xe7,0xe7,0xf3,
0x67,0xe7,0xe7,0xe6,
0x67,0xff,0xff,0xe6,
0x73,0xff,0xff,0xce,
0x33,0xff,0xff,0xcc,
0x39,0xff,0xff,0x9c,
0x1c,0xff,0xff,0x38,
0x0e,0x3f,0xfc,0x70,
0x07,0x0f,0xf0,0xe0,
0x03,0xc0,0x03,0xc0,
0x01,0xf0,0x0f,0x80,
0x00,0x7f,0xfe,0x00,
0x00,0x0f,0xf0,0x00
};
//-------------------------------------------------------------------------------

void main(void)
{
    PSBPIN=0;                           //若硬件短接到地后,本条语句可取消
    while(1)
    {   lcdreset();                     //初始化液晶显示控制器
        exsample();                     //演示程序
    }
}
void exsample(void)                     //演示程序
{   unsigned char i;
//0.演示前的准备,LCD液晶显示控制器初始化,所有显示全清零
    lcdreset();                         //液晶显示控制器初始化
    charlcdfill(' ');                   //显示清屏

//1.字符串演示: 在(2,0)字符位置显示" SMG12232ZK "
    putstrxy(2,0," SMG12232ZK ");       //在(2,0)位置开始显示字符串
    exdelay();                          //延时约300mS
    exdelay();                          //延时约300mS

//2.字符串演示: 在(0,1)字符位置显示"中文字符图形点阵"
    putstrxy(0,1,"中文字符图形点阵");   //在(0,1)位置开始显示字符串
    exdelay();                          //延时约300mS
    exdelay();                          //延时约300mS

//3.字符串演示: 在(2,0)字符位置显示"液晶显示模块"
    putstrxy(2,0,"液晶显示模块");       //在(2,0)位置开始显示字符串
    exdelay();                          //延时约300mS
    exdelay();                          //延时约300mS

//4.字符串演示: 在(0,1)字符位置显示"长沙太阳人SUNMAN"
    putstrxy(0,1,"长沙太阳人SUNMAN");   //在(0,1)位置开始显示字符串
    exdelay();                          //延时约300mS
    exdelay();                          //延时约300mS

//5.字符填充演示: 整屏显示0..9 空格一遍
    for(i='0';i<'9'+1;i++)
    {   charlcdfill(i);                 //整屏显示为i的值
        exdelay();                      //延时约300mS
    }
    charlcdfill(' ');                   //显示清屏
    exdelay();                          //延时约300mS

//6.字符演示: 依次显示A..Z 一遍
    for(i='A';i<'Z'+1;i++)              //依次显示A..Z一遍
    {   putchar(i);                     //当前位置显示为i的值
        exdelay();                      //延时约300mS
    }

//7.自定义字符演示: 装入自定义字符点阵发生器
    setcharcgram(0,CHAR1CGRAMTAB);      //装入自定义字符字符点阵
    setcharcgram(1,CHAR2CGRAMTAB);
    setcharcgram(2,CHAR3CGRAMTAB);
    setcharcgram(3,CHAR4CGRAMTAB);      //装入自定义字符字符点阵

//8.自定义字符演示：在(4,0)(5,0)(4,1)(5,1)字符位置显示自定义字符1,2,3,4
    CXPOS=4,CYPOS=0;
    putchar(USERCHAR1CODE);
    exdelay();                          //延时约300mS
    putchar(USERCHAR2CODE);
    exdelay();                          //延时约300mS
    CXPOS=4,CYPOS=1;
    putchar(USERCHAR3CODE);
    exdelay();                          //延时约300mS
    putchar(USERCHAR4CODE);
    exdelay();                          //延时约300mS
    exdelay();                          //延时约300mS

//9.图形模式演示: 在(0,0)点阵位置显示图片
    charlcdfill(' ');                   //显示清屏
    lcdgraphon();                       //开图形模式
    GXPOS=0,GYPOS=0;
    putimage(Img_sunman_32x32);
    exdelay();                          //延时约300mS
    exdelay();                          //延时约300mS
    glcdfill(0);                        //图形区域清屏
    lcdgraphoff();                      //关图形模式
}
void exdelay(void)                      //演示延时子程序
{ unsigned char i,j,k;                  //延时约300mS
  for(i=0;i<30;i++)
    for(j=0;j<64;j++)
      for(k=0;k<51;k++);
}
//-------------------------------------------------------------------------------
//字符操作区域
//12232ZK字符区域定义为:
//   CXPOS(0-15) 对应于第1列到第16列位置,每个字符列位置实际占位8点,总计对应128点
//   由于显示只有122点,所以最后的6点不能显示出来.
//   CYPOS(0-1)对应于第1行到第2行,每个行位置实际占位16点,总计对应32点
//警告:直接设置CXPOS的位置时,请将CXPOS的值设置为偶数,否则显示的字符值会不正确.
//-------------------------------------------------------------------------------
//以下为charlcdfill,putchar,putstr,setcharcgram为ST7920兼容芯片的液晶显示控制器
//的通用子程序,putstrxy为扩充子程序.
//-------------------------------------------------------------------------------
//子程序名称:void charlcdfill(unsigned int c).
//功能:整屏显示A代表的ASCII字符.
//修改日期:2009.10.29
//修改人:chujianjun@sunman.cn,tanchao@sunman.cn
//-------------------------------------------------------------------------------
void charlcdfill(unsigned int c)        //整屏显示A代表的ASCII字符子程序
{   for(CXPOS=CYPOS=0;1;)
    {   putchar(c);                     //定位写字符
        if((CXPOS==0) && (CYPOS==0)) break;
    }
}
//-------------------------------------------------------------------------------
//子程序名称:void putchar(unsigned int c).
//功能:在(XPOS,YPOS)位置写单个字符点阵,若c>128 表示为中文字符,否则为西文字符
//修改日期:2009.8.18
//修改人:chujianjun@sunman.cn,tanchao@sunman.cn
//-------------------------------------------------------------------------------
void putchar(unsigned int c)            //定位写字符子程序
{   if(c>128)                           //字符码大于128表示为汉字
    {   if( (CXPOS&0x1)==1 )            //写汉字时,CXPOS字符位置在奇数位置,则
        {   lcdwd(' ');                 //自动补添一个空格对齐后再显示汉字
            charcursornext();           //置字符位置为下一个有效位置
        }
        charlcdpos();
        if((c&0xff00)==0xff00)          //若高位字节为0FFH则表示为自定义字符
            c=0xffff-c;                 //则转换为ST7920的字符码
        lcdwd(c/256);                   //写高位字符
        charcursornext();
        lcdwd(c&0xff);                  //写低位字符
        charcursornext();
    }
    else                                //字符码小于128表示为ASCII字符
    {   charlcdpos();
        if( (CXPOS&0x1)==1 )            //写ASCII字符时,CXPOS字符位置在奇数位置,则
        {   lcdwd(FCHARBUF);            //重新写高位字符缓冲区内容
            lcdwd(c);
        }
        else
        {   lcdwd(c);                   //写ASCII字符时,CXPOS字符位置在偶数位置,则
            FCHARBUF=c;                 //直接写入,并保存当前字符到高位字符缓冲变量
            lcdwd(' ');                 //同时自动补显一个空格.
        }
        charcursornext();               //置字符位置为下一个有效位置
    }
}
//-------------------------------------------------------------------------------
//子程序名称:void putstr(unsigned char code *s).
//功能:写字符串点阵,若*s=0 表示字符串结束.
//修改日期:2009.8.18
//修改人:chujianjun@sunman.cn,tanchao@sunman.cn
//-------------------------------------------------------------------------------
void putstr(unsigned char code *s)      //显示字符串子程序,字符码为0时退出
{   unsigned int c;
    while(1)
    {   c=*s;
        s++;
        if(c==0) break;
        if(c<128)
            putchar(c);
        else
        {   putchar(c*256+*s);
            s++;
        }
    }
}
//-------------------------------------------------------------------------------
//子程序名称:void putstrxy(unsigned char cx,unsigned char cy,unsigned char *s).
//功能:在(cx,cy)字符位置写字符串.
//修改日期:2009.08.11
//修改人:chujianjun@sunman.cn,tanchao@sunman.cn
//-------------------------------------------------------------------------------
void putstrxy(unsigned char cx,unsigned char cy,unsigned char code *s)
{                                       //在(cx,cy)字符位置写字符串子程序
    CXPOS=cx;                           //置当前X位置为cx
    CYPOS=cy;                           //置当前Y位置为cy
    putstr(s);
}
//-------------------------------------------------------------------------------
//子程序名称:void setcharcgram(unsigned char cindex,unsigned char code *s).
//功能:将*s所指32字节点阵写到自定义字符cindex(0-3)中.
//修改日期:2009.10.29
//修改人:chujianjun@sunman.cn,tanchao@sunman.cn
//-------------------------------------------------------------------------------
void setcharcgram(unsigned char cindex,unsigned char code *s)
{   unsigned char i;
    lcdwc(0x34);                        //扩充指令集,图形模式关闭
    lcdwc(0x02);                        //SR=0 激活CGRAM
    lcdwc(0x30);                        //恢复为基本指令集
    lcdwc( ((cindex&0x3)<<4) | 0x40);   //设置CGRAM地址
    for(i=0;i<32;i++,s++)               //写点阵数据
        lcdwd(*s);
}
//-------------------------------------------------------------------------------
//以下putsizeimage为图形对象的基本子程序,putimage为图形对象的扩充子程序
//-------------------------------------------------------------------------------
//子程序名称:void putsizeimage(unsigned char XSIZE,unsigned YSIZE,
//                             unsigned char code *s)
//功能:在(GXPOS,GYPOS)位置绘制XSIZE列及YISZE行点阵的图形*S(XSIZE仅取8的倍数
//     GXPOS仅取16的倍数).
//修改日期:2009.10.29
//修改人:chujianjun@sunman.cn,tanchao@sunman.cn
//-------------------------------------------------------------------------------
void putsizeimage(unsigned char XSIZE,unsigned char YSIZE,unsigned char code *s)
{  unsigned char lx,ly,x;
   x=GXPOS;
   for(ly=0;ly<YSIZE;ly++,GYPOS++)
   {  for(GXPOS=x,lx=0;lx<XSIZE;)
      {  if((GXPOS&0x08)==0) glcdpos();
         lcdwd(*s);
         s++;
         lx+=8;
         GXPOS+=8;
      }
   }
   GXPOS=x;
}
//-------------------------------------------------------------------------------
//子程序名称:void putimage(unsigned char code *s).
//功能:在(XPOS,YPOS)位置绘制XSIZE[*s]列及YISZE[*(s+1)]行点阵的图形[*(s+2)].
//修改日期:2009.8.18
//修改人:chujianjun@sunman.cn,tanchao@sunman.cn
//-------------------------------------------------------------------------------
void putimage(unsigned char code *s)    //显示图形子程序
{   unsigned char XSIZE,YSIZE;
    XSIZE=*s;
    s++;
    YSIZE=*s;
    s++;
    putsizeimage(XSIZE,YSIZE,s);
}
//-------------------------------------------------------------------------------
//以下为charlcdpos,charcursornext,lcdreset为ST7920兼容芯片的液晶显示控制器的
//15ASCII字符X2行的SMG12232系列标准中文字符点阵型液晶显示模块的接口程序.
//-------------------------------------------------------------------------------
//子程序名称:void charlcdpos(void).
//功能:设置坐标点(CXPOS,CYPOS)位置对应的内部RAM地址.
//修改日期:2009.11.02
//修改人:chujianjun@sunman.cn,tanchao@sunman.cn
//-------------------------------------------------------------------------------
void charlcdpos(void)                   //设置坐标点(CXPOS,CYPOS)内部RAM地址的子程序
{   unsigned char ddaddr;
    if(CXPOS>15) CXPOS=0;               //字符位置CXPOS的有效范围为(0到15)
    CYPOS&=0X1;                         //字符位置CYPOS的有效范围为(0到1)
    ddaddr=CXPOS/2;
    if(CYPOS==0)                        //(第一行)X: 第0----15个字符
        lcdwc(ddaddr|0x80);             //    DDRAM:  80----87H
    else                                //(第二行)X: 第0----15个字符
        lcdwc(ddaddr|0x90);             //    DDRAM:  90----97H
}
//-------------------------------------------------------------------------------
//子程序名称:void charcursornext(void).
//功能:置字符位置为下一个有效位置.
//修改日期:2009.11.02
//修改人:chujianjun@sunman.cn,tanchao@sunman.cn
//-------------------------------------------------------------------------------
void charcursornext(void)               //置字符位置为下一个有效位置子程序
{
    CXPOS++;                            //字符位置加1
    if(CXPOS>15) CXPOS=0;               //字符位置CXPOS的有效范围为(0到15)
    if(CXPOS==0)                        //字符位置CXPOS=0表示要换行
    {   CYPOS++;                        //行位置加1
        CYPOS&=0X1;                     //字符位置CYPOS的有效范围为(0到1)
    }
}
//-------------------------------------------------------------------------------
//子程序名称:void lcdreset(void)
//功能:液晶显示控制器初始化
//修改日期:2009.08.31
//修改人:chujianjun@sunman.cn,tanchao@sunman.cn
//-------------------------------------------------------------------------------
void lcdreset(void)                     //液晶显示控制器初始化子程序
{   lcdwc(0x33);                        //接口模式设置
    delay3ms();                         //延时3MS
    lcdwc(0x30);                        //基本指令集
    delay3ms();                         //延时3MS
    lcdwc(0x30);                        //重复送基本指令集
    delay3ms();                         //延时3MS
    lcdwc(0x01);                        //清屏控制字
    delay3ms();                         //延时3MS
    lcdwc(0x06);                        //显示光标移动设置
    lcdon();                            //开显示
}
void delay3ms(void)                     //延时3MS子程序
{ unsigned char i,j,k;
  for(i=0;i<3;i++)
    for(j=0;j<64;j++)
      for(k=0;k<51;k++);
}
//-------------------------------------------------------------------------------
//子程序名称:void glcdfill(unsigned char d).
//功能:整屏显示d表示的字节数据.
//修改日期:2009.11.02
//修改人:chujianjun@sunman.cn,tanchao@sunman.cn
//-------------------------------------------------------------------------------
void glcdfill(unsigned char d)          //整屏显示d代表的字数据子程序
{   unsigned char i;
    GXPOS=0;
    GYPOS=0;
    while(GYPOS<32)
    {   glcdpos();
        for(i=0;i<16;i++)
            lcdwd(d);
        GYPOS++;
    }
    GYPOS=0;
}
//-------------------------------------------------------------------------------
//子程序名称:void glcdpos(void)
//功能:设置(GXPOS,GYPOS)对应绘图区域内部RAM指针.
//修改日期:2009.11.02
//修改人:chujianjun@sunman.cn,tanchao@sunman.cn
//-------------------------------------------------------------------------------
void glcdpos(void)                      //写入绘图区域内部RAM指针子程序
{   lcdwc(0x36);                        //扩展指令集
    lcdwc((GYPOS&0x1f)|0x80);           //先送Y地址
    lcdwc((GXPOS/16)|0x80);             //再送X地址
    lcdwc(0x30);                        //恢复为基本指令集
}
//-------------------------------------------------------------------------------
//子程序名称:lcdon()
//功能:开启LCD显示
//修改日期:2009.10.28
//修改人:chujianjun@sunman.cn,tanchao@sunman.cn
//-------------------------------------------------------------------------------
void lcdon(void)                        //LCD显示开启子程序
{   lcdwc(0x30);                        //设置为基本指令集
    lcdwc(0x0c);
}
//-------------------------------------------------------------------------------
//子程序名称:lcdoff()
//功能:关闭LCD显示
//修改日期:2009.10.28
//修改人:chujianjun@sunman.cn,tanchao@sunman.cn
//-------------------------------------------------------------------------------
void lcdoff(void)                       //LCD显示关闭子程序
{   lcdwc(0x30);                        //设置为基本指令集
    lcdwc(0x08);
}
//-------------------------------------------------------------------------------
//子程序名称:lcdgraphon()
//功能:开启绘图区域显示
//修改日期:2009.10.28
//修改人:chujianjun@sunman.cn,tanchao@sunman.cn
//-------------------------------------------------------------------------------
void lcdgraphon(void)                  //绘图区域显示开启子程序
{   lcdwc(0x36);
    lcdwc(0x30);                        //恢复为基本指令集
}
//-------------------------------------------------------------------------------
//子程序名称:lcdgraphoff()
//功能:关闭绘图区域显示
//修改日期:2009.10.28
//修改人:chujianjun@sunman.cn,tanchao@sunman.cn
//-------------------------------------------------------------------------------
void lcdgraphoff(void)                  //绘图区域显示关闭子程序
{   lcdwc(0x34);
    lcdwc(0x30);                        //恢复为基本指令集
}
//-------------------------------------------------------------------------------
//以下lcdwc,lcdwd为ST7920兼容芯片的MCS51三线式串行接口方式的基本子程序,transbyte,
//transbit为内部子程序.
//-------------------------------------------------------------------------------
//子程序名称:void lcdwd(unsigned char d).
//功能:向液晶显示控制器写图形数据.
//修改日期:2009.10.29
//修改人:chujianjun@sunman.cn,tanchao@sunman.cn
//-------------------------------------------------------------------------------
void lcdwd(unsigned char d)             //向液晶显示控制器写数据
{
   CSPIN=1;                             //片选使能
   transbyte(0xfa);                     //SYNCODE=0F8H,RW=0,RS=1,D0=0
   transbyte(d&0xf0);                   //送高四位数据,低四位补零
   transbyte((d&0x0f)<<4);              //送低四位数据
   CSPIN=0;                             //片选禁止
}
//-------------------------------------------------------------------------------
//子程序名称:lcdwc(unsigned char c).
//功能:向液晶显示控制器送指令.
//修改日期:2009.10.29
//修改人:chujianjun@sunman.cn,tanchao@sunman.cn
//-------------------------------------------------------------------------------
void lcdwc(unsigned char c)             //向液晶显示控制器送指令
{
   CSPIN=1;                             //片选使能
   transbyte(0xf8);                     //SYNCODE=0F8H,RW=0,RS=0,D0=0
   transbyte(c&0xf0);                   //送高四位数据,低四位补零
   transbyte((c&0x0f)<<4);              //送低四位数据
   CSPIN=0;                             //片选禁止
}
//-------------------------------------------------------------------------------
//子程序名称:void transbyte(unsigned char d).
//功能:送1字节数据到液晶显示控制器.
//修改日期:2009.10.10
//修改人:chujianjun@sunman.cn,tanchao@sunman.cn
//-------------------------------------------------------------------------------
void transbyte(unsigned char d)         //送1字节数据到液晶显示控制器子程序
{   unsigned char i;
    for(i=0;i<8;i++)
    {   if((d&0x80) == 0x80)
            transbit(1);
        else
            transbit(0);
        d<<=1;                          //从高到低位送字节位数据到液晶显示控制器
    }
}
//-------------------------------------------------------------------------------
//子程序名称:void transbit(bit d).
//功能:送1位数据到液晶显示控制器.
//修改日期:2009.10.29
//修改人:chujianjun@sunman.cn,tanchao@sunman.cn
//-------------------------------------------------------------------------------
void transbit(bit d)                    //送1位数据到液晶显示控制器子程序
{  STDPIN = d;                          //先送数据到数据口线DI
   _nop_();
   SCLKPIN = 1;                         //再使时钟口线发一个负脉冲
   _nop_();
   SCLKPIN = 0;
   _nop_();
   SCLKPIN = 1;
}