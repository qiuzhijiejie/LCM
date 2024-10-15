#ifndef __picture_H__
#define __picture_H__
extern unsigned int code picture [];
extern unsigned int code picture2[];
extern unsigned int code picture3[];
extern unsigned int code picture4[];
extern unsigned int code picture5[];
extern unsigned int code picture6[];
unsigned char code Img_sunman_32[];

unsigned char code EnglishCharDot[];
unsigned char code EnglishCode[];
unsigned int  code ChineseCode[];
unsigned char code ChineseCharDot[];


#endif




#if 0
//SMG240128A标准图形点阵型液晶显示模块的扩展演示程序[C51编程语言][MCS51模拟口线方式]
//应用产品: SMG240128A标准图形点阵型液晶显示模块
//  本演示程序适用于SMG240128A液晶显示模块与MCS51系列单片机采用MCS51模拟口线的
//硬件连线方式。
//  本演示程序包括T6963C兼容芯片的MCS51模拟口线方式子程序集,T6963C兼容芯片的240128
//液晶显示模块的基本子程序,240128系列绘图子程序集,240128系列图形子程序集,240128系
//列精简中英文字库子程序集,以及SMG240128系列标准图形点阵型液晶显示的扩展演示子程序.
//  本演示的内容为,在240列X128行的点阵液晶显示屏上清屏,图片演示,英文字符串演示,中文
//字符串演示,反白显示字符串,绘制矩形,写点,绘制直线,全屏显示.
//-------------------------------------------------------------------------------
//创建日期: 2009.08.17
//软件环境: VW For Windows
//硬件环境: AT89C52 MCU,MCS-51系列 MCU,etc.
//创建人: 褚建军(chujianjun@sunman.cn),谭超(tanchao@sunman.cn).
//技术支持: 电话(0731-84167806,84167807),传真(0731-84167807),网址(www.sunman.cn)
//版权信息: 长沙太阳人电子有限公司版权所有,转载请指明出处.
//参考网页: http:/www.sunman.cn/lcm/product/SMG240128A.html
//-------------------------------------------------------------------------------
#include <reg51.h>
#include <intrins.h>
//以下为演示程序的函数定义
void exsample(void);                    //演示程序
void exdelay(void);                     //演示延时子程序
void putchar(unsigned int c);           //定位写字符子程序
void putstr(unsigned char code *s);     //显示字符串子程序,字符码为0时退出
unsigned char code *getchinesecodepos(unsigned int ac);
unsigned char code *getenglishcodepos(unsigned char ac);
void putimage(unsigned char code *s);   //显示图形子程序
void putsizeimage(unsigned char XSIZE,unsigned char YSIZE,unsigned char code *s);
                                        //显示指定大小的图形子程序
void point(bit b);
void line(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,bit b);
void lineto(unsigned char x1,unsigned char y1,bit b);//绘制(XPOS,YPOS)-(X1,Y1)的直线
void rect(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,bit b);
void lcdfill(unsigned char d);          //整屏显示d代表的字节数据子程序
void lcdpos(void);                      //设置坐标点(XPOS,YPOS)内部RAM地址的子程序
void lcdreset(void);                    //液晶显示控制器初始化子程序
void lcdwd(unsigned char d);            //送图形数据子程序
unsigned char lcdrd(void);              //读图形数据子程序
void lcdwc(unsigned char c);            //送指令子程序
void lcdwc2(unsigned char c,unsigned char d);//送2字节指令子程序
void lcdwc3(unsigned char c,unsigned char d1,unsigned char d2);//送3字节指令子程序
unsigned char lcdrdata(void);           //读数据子程序
void lcdwdata(unsigned char d);         //送数据子程序
void lcdwaitidle(void);                 //忙检测子程序
//-------------------------------------------------------------------------------
//SMG240128A产品引脚说明及演示连线
//PIN1: FG   [构造地]-------------------VSS
//PIN2: VSS  [电源地]-------------------VSS
//PIN3: VDD  [电源正极]-----------------VDD(+5V)
//PIN4: Vo   [LCD 偏压输入]-------------接10K的可调电阻2K电阻到VSS及VEE
//                                      Vo=VEE时为显示最深
//PIN5: WR   [写信号 低有效]------------P3.1
//PIN6: RD   [读信号 低有效]------------P3.2
//PIN7: CE   [使能信号 低有效]----------P3.3
//PIN8: CD   [数据/命令选择端（H/L）]---P3.0
//PIN9: NC   [空脚]---------------------不连线
//PIN10:RST  [复位端 低有效]------------VDD
//PIN11:D0   [Data I/O]-----------------P1.0
//PIN12:D1   [Data I/O]-----------------P1.1
//PIN13:D2   [Data I/O]-----------------P1.2
//PIN14:D3   [Data I/O]-----------------P1.3
//PIN15:D4   [Data I/O]-----------------P1.4
//PIN16:D5   [Data I/O]-----------------P1.5
//PIN17:D6   [Data I/O]-----------------P1.6
//PIN18:D7   [Data I/O]-----------------P1.7
//PIN19:FS   [字体点阵]-----------------VSS
//PIN20:VEE  [LCD负压输出]--------------提供液晶负电压.
//PIN21:BLA  [背光源正极]---------------见使用说明书.
//PIN22:BLK  [背光源负极]---------------VSS
//注:8031的晶振频率为12MHz.
//请参见http://download.sunman.cn/lcm/product/1/SMG240128A.pdf
//-------------------------------------------------------------------------------
//以下为产品接口引脚在演示程序中的预定义
//用户在编写应用程序时,需按自己的实际硬件连线来重新定义
sbit CDPIN=P3^0;                        // C/D对应单片机引脚
sbit CEPIN=P3^3;                        // CE对应单片机引脚
sbit WRPIN=P3^1;                        // WR对应单片机引脚
sbit RDPIN=P3^2;                        // RD对应单片机引脚
//-------------------------------------------------------------------------------
//以下XPOS,YPOS变量用于指示当前操作点的位置的预定义
//用户在编写应用程序时,需按自己的实际软件程序需要来重新定义
unsigned char XPOS;                     //列方向LCD点阵位置指针
unsigned char YPOS;                     //行方向LCD点阵位置指针
bit CharImageReverse;                   //字符及图形的反显控制,0(正常显示),1(反显)

#define XDOTS   240                     //图形空间X方向大小
#define YDOTS   128                     //图形空间X方向大小
//-------------------------------------------------------------------------------
//以下为精简版中英文字库
//实际使用时请包含由Sunman精简版中英文字符库代码生成器.exe自动生成的charlib.c文
//文件来替换该部分
//精简版中英文字符库常量定义
#define ENGLISHCHARNUMBER  8       //精简版英文字符库中的英文字符的个数
#define CHINESECHARNUMBER  11      //精简版中文字符库中的中文字符的个数
#define ENGLISHCHARSIZE    8       //英文字符X方向显示点的个数
#define CHINESECHARSIZE    16      //中文字符X方向及中英文字符Y方向显示点的个数
#define ENGLISHCHARDOTSIZE 16      //单个英文字符点阵的字节数
#define CHINESECHARDOTSIZE 32      //单个中文字符点阵的字节数

unsigned char code EnglishCode[]={
0x2e,    //字符001:[.]
0x41,    //字符002:[A]
0x43,    //字符003:[C]
0x4d,    //字符004:[M]
0x4e,    //字符005:[N]
0x53,    //字符006:[S]
0x55,    //字符007:[U]
0x57     //字符003:[W]
};

unsigned int code ChineseCode[]={
0xb3a4,  //字符001:[长]
0xb5e7,  //字符002:[电]
0xb9ab,  //字符003:[公]
0xc8cb,  //字符004:[人]
0xc9b3,  //字符005:[沙]
0xcbbe,  //字符006:[司]
0xccab,  //字符007:[太]
0xcfde,  //字符008:[限]
0xd1f4,  //字符009:[阳]
0xd3d0,  //字符010:[有]
0xd7d3   //字符011:[子]
};

unsigned char code EnglishCharDot[]={
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,   //字符001:[.]
0x00,0x00,0x00,0x00,0x60,0x60,0x00,0x00,
0x00,0x00,0x00,0x10,0x10,0x18,0x28,0x28,   //字符002:[A]
0x24,0x3c,0x44,0x42,0x42,0xe7,0x00,0x00,
0x00,0x00,0x00,0x3e,0x42,0x42,0x80,0x80,   //字符003:[C]
0x80,0x80,0x80,0x42,0x44,0x38,0x00,0x00,
0x00,0x00,0x00,0xee,0x6c,0x6c,0x6c,0x6c,   //字符004:[M]
0x54,0x54,0x54,0x54,0x54,0xd6,0x00,0x00,
0x00,0x00,0x00,0xc7,0x62,0x62,0x52,0x52,   //字符005:[N]
0x4a,0x4a,0x4a,0x46,0x46,0xe2,0x00,0x00,
0x00,0x00,0x00,0x3e,0x42,0x42,0x40,0x20,   //字符006:[S]
0x18,0x04,0x02,0x42,0x42,0x7c,0x00,0x00,
0x00,0x00,0x00,0xe7,0x42,0x42,0x42,0x42,   //字符007:[U]
0x42,0x42,0x42,0x42,0x42,0x3c,0x00,0x00,
0x00,0x00,0x00,0xd6,0x92,0x92,0x92,0x92,   //字符003:[W]
0xaa,0xaa,0x6c,0x44,0x44,0x44,0x00,0x00
};

unsigned char code ChineseCharDot[]={
0x08,0x00,0x08,0x10,0x08,0x20,0x08,0x40,   //字符001:[长]
0x09,0x80,0x0e,0x00,0x08,0x00,0xff,0xfe,
0x0a,0x00,0x09,0x00,0x08,0x80,0x08,0x40,
0x09,0x30,0x0a,0x1c,0x0c,0x08,0x08,0x00,
0x01,0x00,0x01,0x00,0x01,0x00,0x3f,0xf8,   //字符002:[电]
0x21,0x08,0x21,0x08,0x3f,0xf8,0x21,0x08,
0x21,0x08,0x21,0x08,0x3f,0xf8,0x21,0x08,
0x01,0x02,0x01,0x02,0x00,0xfe,0x00,0x00,
0x00,0x00,0x04,0x40,0x06,0x40,0x04,0x40,   //字符003:[公]
0x08,0x20,0x08,0x10,0x10,0x18,0x22,0x0e,
0x43,0x04,0x02,0x00,0x04,0x20,0x08,0x10,
0x11,0xf8,0x3f,0x18,0x10,0x10,0x00,0x00,
0x01,0x00,0x01,0x80,0x01,0x00,0x01,0x00,   //字符004:[人]
0x01,0x00,0x01,0x00,0x02,0x80,0x02,0x80,
0x04,0x80,0x04,0x40,0x08,0x60,0x08,0x30,
0x10,0x18,0x20,0x0e,0x40,0x04,0x00,0x00,
0x20,0x40,0x10,0x40,0x10,0x40,0x01,0x50,   //字符005:[沙]
0x41,0x48,0x2a,0x44,0x0a,0x42,0x14,0x40,
0x10,0x44,0x20,0x44,0xe0,0x08,0x20,0x10,
0x20,0x20,0x20,0xc0,0x23,0x00,0x2c,0x00,
0x00,0x00,0x3f,0xfc,0x00,0x04,0x00,0x04,   //字符006:[司]
0x7f,0xe4,0x00,0x04,0x00,0x04,0x3f,0xc4,
0x20,0x44,0x20,0x44,0x3f,0xc4,0x20,0x44,
0x20,0x04,0x00,0x14,0x00,0x08,0x00,0x00,
0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,   //字符007:[太]
0x7f,0xfe,0x01,0x00,0x01,0x00,0x01,0x00,
0x02,0x80,0x02,0x40,0x04,0x20,0x05,0x10,
0x08,0x98,0x10,0xcc,0x20,0x86,0x40,0x04,
0x00,0x00,0xfb,0xf8,0x92,0x08,0x93,0xf8,   //字符008:[限]
0xa2,0x08,0xa2,0x08,0x93,0xf8,0x8a,0x80,
0x8a,0x48,0xaa,0x50,0x92,0x20,0x82,0x20,
0x82,0x10,0x82,0x8e,0x83,0x04,0x82,0x00,
0x00,0x00,0x7d,0xfc,0x45,0x04,0x49,0x04,   //字符009:[阳]
0x49,0x04,0x51,0x04,0x49,0xfc,0x45,0x04,
0x45,0x04,0x45,0x04,0x69,0x04,0x51,0x04,
0x41,0xfc,0x41,0x04,0x40,0x00,0x40,0x00,
0x01,0x00,0x01,0x00,0x7f,0xfe,0x02,0x00,   //字符010:[有]
0x04,0x00,0x0f,0xf0,0x18,0x10,0x28,0x10,
0x4f,0xf0,0x88,0x10,0x08,0x10,0x0f,0xf0,
0x08,0x10,0x08,0x90,0x08,0x70,0x08,0x20,
0x00,0x00,0x3f,0xf0,0x00,0x20,0x00,0x40,   //字符011:[子]
0x00,0x80,0x01,0x00,0x01,0x00,0x01,0x04,
0xff,0xfe,0x01,0x00,0x01,0x00,0x01,0x00,
0x01,0x00,0x01,0x00,0x05,0x00,0x02,0x00
};
//-------------------------------------------------------------------------------
//以下为图片库点阵代码
//实际使用时请包含由SUNMAN图形点阵代码生成器.EXE自动生成的imagelib.c文件来替换该
//部分
unsigned char code Img_sunman_32[]={32,32,
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
{   while(1)
    {   lcdreset();                     //初始化液晶显示控制器
        exsample();                     //演示程序
    }
}
void exsample(void)                     //演示程序
{
//0.演示前的准备,将LCD液晶显示全部清空
    CharImageReverse=0;                 //反显关闭
    lcdfill(0);                         //清屏

//1.图片测试: 在(16,48)位置显示32点阵SUNMAN图标
    XPOS=16;
    YPOS=48;
    putimage(Img_sunman_32);            //写数据到当前LCDRAM地址中
    exdelay();                          //延时约600mS

//2.字符串测试: 在(84,48)位置显示"WWW.SUNMAN.CN"字符串.
    XPOS=84;
    YPOS=48;
    putstr("WWW.SUNMAN.CN");
    exdelay();                          //延时约600mS

//3.字符串测试: 在(48,64)位置显示"长沙太阳人电子有限公司"字符串.
    XPOS=48;
    YPOS=64;
    putstr("长沙太阳人电子有限公司");
    exdelay();                          //延时约600mS

//3.反显测试: 在(48,64)位置反显"长沙太阳人电子有限公司"字符串.
    XPOS=48;
    YPOS=64;
    CharImageReverse=1;
    putstr("长沙太阳人电子有限公司");
    CharImageReverse=0;
    exdelay();                          //延时约600mS

//3.绘图测试: RECT(15,47)-(224,80),画矩形.
    rect(15,47,224,80,1);
    exdelay();                          //延时约600mS

//4.绘图测试: point(15,85),画点.
    XPOS=15;
    YPOS=85;
    point(1);
    exdelay();                          //延时约600mS

//5.绘图测试: line(15,85)-(119,120),画线.
    line(15,85,119,120,1);
    exdelay();                          //延时约600mS

//6.绘图测试: lineto(224,85),画线.
    lineto(224,85,1);
    exdelay();                          //延时约600mS

//7.绘图测试: lineto(15,85),画线.
    lineto(15,85,1);
    exdelay();                          //延时约600mS

//8.全屏测试: 所有坐标点全部显示.
    lcdfill(0);                        //全显
    exdelay();                          //延时约600mS

}
void exdelay(void)                      //演示延时子程序
{ unsigned char i,j,k;                  //延时约600mS
  for(i=0;i<60;i++)
    for(j=0;j<64;j++)
      for(k=0;k<51;k++);
}
//-------------------------------------------------------------------------------
//以下putchar为字符对象的基本子程序,putstr为字符对象的扩充子程序,getchinesecodepos,
//getenglishcodepos为字符对象的辅助子程序.
//-------------------------------------------------------------------------------
//子程序名称:void putchar(unsigned int c).
//功能:在(XPOS,YPOS)位置写单个字符点阵,若c>128 表示为中文字符,否则为西文字符
//修改日期:2009.8.18
//修改人:chujianjun@sunman.cn,tanchao@sunman.cn
//-------------------------------------------------------------------------------
void putchar(unsigned int c)            //定位写字符子程序
{   if(c>128)
        putsizeimage(CHINESECHARSIZE,CHINESECHARSIZE,getchinesecodepos(c));
    else
        putsizeimage(ENGLISHCHARSIZE,CHINESECHARSIZE,getenglishcodepos(c));
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
//子程序名称:unsigned char code *getchinesecodepos(unsigned char ac).
//功能:根据当前中文字符码查表后计算得到当前中文字符码的字库点阵位置.
//修改日期:2009.8.18
//修改人:chujianjun@sunman.cn,tanchao@sunman.cn
//-------------------------------------------------------------------------------
unsigned char code *getchinesecodepos(unsigned int ac)
{   unsigned int min,max,mid,midc;
    min=0;
    max=CHINESECHARNUMBER-1;
    while(1)
    {   if(max-min<2)
        {   if(ChineseCode[min]==ac)
                mid=min;
            else if(ChineseCode[max]==ac)
                mid=max;
            else
                mid=0;
            break;
        }
        mid=(max+min)/2;
        midc=ChineseCode[mid];
        if(midc==ac)
            break;
        else if(midc>ac)
            max=mid-1;
        else
            min=mid+1;
    }
    return ChineseCharDot+mid*CHINESECHARDOTSIZE;
}
//-------------------------------------------------------------------------------
//子程序名称:unsigned char code *getenglishcodepos(unsigned char ac).
//功能:根据当前ASCII字符码查表后计算得到当前ASCII字符码的字库点阵位置.
//修改日期:2009.8.18
//修改人:chujianjun@sunman.cn,tanchao@sunman.cn
//-------------------------------------------------------------------------------
unsigned char code *getenglishcodepos(unsigned char ac)
{   unsigned char min,max,mid,midc;
    min=0;
    max=ENGLISHCHARNUMBER-1;
    while(1)
    {   if(max-min<2)
        {   if(EnglishCode[min]==ac)
                mid=min;
            else if(EnglishCode[max]==ac)
                mid=max;
            else
                mid=0;
            break;
        }
        mid=(max+min)/2;
        midc=EnglishCode[mid];
        if(midc==ac)
            break;
        else if(midc>ac)
            max=mid-1;
        else
            min=mid+1;
    }
    return EnglishCharDot+mid*ENGLISHCHARDOTSIZE;
}
//-------------------------------------------------------------------------------
//以下putsizeimage为图形对象的基本子程序,putimage为图形对象的扩充子程序
//-------------------------------------------------------------------------------
//子程序名称:void putsizeimage(unsigned char XSIZE,unsigned YSIZE,
//                             unsigned char code *s)
//功能:在(XPOS,YPOS)位置绘制XSIZE列及YISZE行点阵的图形*S.
//修改日期:2009.8.17
//修改人:chujianjun@sunman.cn,tanchao@sunman.cn
//-------------------------------------------------------------------------------
void putsizeimage(unsigned char XSIZE,unsigned char YSIZE,unsigned char code *s)
{  unsigned char k,lx,ly,a1,a2,x,Lcd_Mask;
   k=XPOS&0x7;
   XSIZE+=k;
   x=XPOS;
   s--;
   for(ly=0;ly<YSIZE;ly++,YPOS++)
   {  for(XPOS=x,lx=k;lx<XSIZE;)
      {  unsigned char p;
         a1=*s;
         s++;
         a2=*s;
         if(CharImageReverse)
         {   a1=~a1;
             a2=~a2;
         }
         for(p=0;p<k;p++)
         {  a2>>=1;
            if((a1&0x1)==1)
               a2+=0x80;
            a1>>=1;
         }
         if((k==0) && (XSIZE<lx+8))
         {   lcdpos();
             lx+=8;
             XPOS+=8;
         }
         else
         {   lcdpos();
             a1=lcdrd();
             lcdpos();
             lx+=8;
             XPOS+=8;
             Lcd_Mask=0xff;
             p=XSIZE&0x7;
             while(p>0)
             {   Lcd_Mask>>=1;
                 XPOS--;
                 p--;
             }
             p=0xff;
             while(XSIZE<lx)
             {   lx--;
                 XPOS--;
                 p<<=1;
             }
             Lcd_Mask&=p;
             a2&=Lcd_Mask;
             a2|=a1&(~Lcd_Mask);
         }
         lcdwd(a2);
      }
      if((k!=0) && (XSIZE&0x7 != 0) && (k >= XSIZE&0x7)) s--;
   }
   if(XDOTS-x < XSIZE-k)
       XPOS=0;
   else
       YPOS-=YSIZE;
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
//以下point为绘图操作的基本子程序,line,lineto,rect为绘图操作的扩充子程序.
//-------------------------------------------------------------------------------
//子程序名称:void point(bit b).
//功能:按b的数据在(XPOS,YPOS)位置绘制点.
//修改日期:2009.8.18
//修改人:chujianjun@sunman.cn,tanchao@sunman.cn
//-------------------------------------------------------------------------------
void point(bit b)
{   unsigned char i,Lcd_Mask,j;
    Lcd_Mask=0x80;
    i=XPOS&0x7;
    while(i>0)
    {   Lcd_Mask>>=1;
        i--;
    }
    lcdpos();
    j=lcdrd();
    lcdpos();
    if(b)
        lcdwd(j|Lcd_Mask);
    else
        lcdwd(j&(~Lcd_Mask));
}
//-------------------------------------------------------------------------------
//子程序名称:void line(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,bit b)
//功能:按b的数据绘制(x0,y0)-(x1,y1)的直线
//修改日期:2009.8.18
//修改人:chujianjun@sunman.cn,tanchao@sunman.cn
//-------------------------------------------------------------------------------
void line(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,bit b)
{   unsigned char dx,dy;
    unsigned int dk;
    XPOS=x0;
    YPOS=y0;
    point(b);
    dx=(x1>=x0)?x1-x0:x0-x1;
    dy=(y1>=y0)?y1-y0:y0-y1;
    if(dx==dy)
    {   while(XPOS!=x1)
        {   if(x1>x0) XPOS++;else XPOS--;
            if(y1>y0) YPOS++;else YPOS--;
            point(b);
        }
    }
    else if (dx>dy)
    {   dk=dy;
        dy=0;
        while(XPOS!=x1)
        {   if(x1>x0) XPOS++;else XPOS--;
            dy++;
            if(y1>y0) YPOS=y0+(dk*dy+dx/2)/dx;else YPOS=y0-(dk*dy+dx/2)/dx;
            point(b);
        }
    }
    else
    {   dk=dx;
        dx=0;
        while(YPOS!=y1)
        {   if(y1>y0) YPOS++;else YPOS--;
            dx++;
            if(x1>x0) XPOS=x0+(dk*dx+dy/2)/dy;else XPOS=x0-(dk*dx+dy/2)/dy;
            point(b);
        }
    }
}
//-------------------------------------------------------------------------------
//子程序名称:void lineto(unsigned char x1,unsigned char y1,bit b)
//功能:按b的数据绘制(XPOS,YPOS)-(x1,y1)的直线
//修改日期:2009.8.18
//修改人:chujianjun@sunman.cn,tanchao@sunman.cn
//-------------------------------------------------------------------------------
void lineto(unsigned char x1,unsigned char y1,bit b)//绘制(XPOS,YPOS)-(X1,Y1)的直线
{   line(XPOS,YPOS,x1,y1,b);
}
//-------------------------------------------------------------------------------
//子程序名称:void rect(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,bit b)
//功能:按b的数据绘制(x0,y0)-(x1,y1)的矩形
//修改日期:2009.8.18
//修改人:chujianjun@sunman.cn,tanchao@sunman.cn
//-------------------------------------------------------------------------------
void rect(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,bit b)
{   line(x0,y0,x1,y0,b);
    line(x1,y0,x1,y1,b);
    line(x1,y1,x0,y1,b);
    line(x0,y1,x0,y0,b);
}
//-------------------------------------------------------------------------------
//以下lcdfill,lcdpos,lcdreset为T6963型硬件接口的240128液晶显示模块的基本子程序
//-------------------------------------------------------------------------------
//子程序名称:void lcdfill(unsigned char d).
//功能:整屏显示d表示的字节数据.
//修改日期:2009.08.07
//修改人:chujianjun@sunman.cn,tanchao@sunman.cn
//-------------------------------------------------------------------------------
void lcdfill(unsigned char d)           //整屏显示d代表的字节数据子程序
{   unsigned char i,j;
    lcdwc3(0x24,0,0);                   //设置内部RAM地址为0
    for(i=0;i<128;i++)    //128行
        for(j=0;j<30;j++)               //30个字节数据共240列
            lcdwd(d);                   //送图形数据
}
//-------------------------------------------------------------------------------
//子程序名称:void lcdpos(void).
//功能:设置坐标点(XPOS,YPOS)位置对应的内部RAM地址.
//修改日期:2009.08.07
//修改人:chujianjun@sunman.cn,tanchao@sunman.cn
//-------------------------------------------------------------------------------
void lcdpos(void)                       //设置坐标点(XPOS,YPOS)内部RAM地址的子程序
{  unsigned int CurrentAddress;
   CurrentAddress=YPOS*30;              //LCDRAM地址=30*YPOS+(XPOS/8)
   CurrentAddress+=XPOS/8;
   lcdwc3(0x24,CurrentAddress&0xff,CurrentAddress/256);
}
//-------------------------------------------------------------------------------
//子程序名称:void lcdreset(void)
//功能:液晶显示控制器初始化
//修改日期:2009.08.07
//修改人:chujianjun@sunman.cn,tanchao@sunman.cn
//-------------------------------------------------------------------------------
void lcdreset(void)                     //液晶显示控制器初始化子程序
{   lcdwc(0x80);                        //OR MODE
    lcdwc(0x98);                        //图形模式开
    lcdwc3(0x42,0,0);                   //设置图形显示区首地址为0
    lcdwc3(0x43,30,0);                  //设置图形显示区宽度为30字节共240点
}
//-------------------------------------------------------------------------------
//以下lcdwd,lcdrd,lcdwc,lcdwc2,lcdwc3为T6963C型硬件接口[MCS51模拟口线接口方式]的
//    基本子程序;lcdwdata,lcdrdata,lcdwaitidle为内部子程序.
//-------------------------------------------------------------------------------
//子程序名称:void lcdwd(unsigned char d).
//功能:向液晶显示控制器写图形数据.
//修改日期:2009.08.07
//修改人:chujianjun@sunman.cn,tanchao@sunman.cn
//-------------------------------------------------------------------------------
void lcdwd(unsigned char d)             //送图形数据子程序
{   lcdwc2(0xc0,d);
}
//-------------------------------------------------------------------------------
//子程序名称:unsigned char lcdrd(void).
//功能:从液晶显示控制器中读图形数据.
//修改日期:2009.08.07
//修改人:chujianjun@sunman.cn,tanchao@sunman.cn
//-------------------------------------------------------------------------------
unsigned char lcdrd(void)               //读图形数据子程序
{   lcdwc(0xc1);                        //送读图形数据指令
    return lcdrdata();                  //读数据
}
//-------------------------------------------------------------------------------
//子程序名称:lcdwc(unsigned char c).
//功能:向液晶显示控制器送指令.
//修改日期:2009.08.07
//修改人:chujianjun@sunman.cn,tanchao@sunman.cn
//-------------------------------------------------------------------------------
void lcdwc(unsigned char c)             //送指令子程序
{   lcdwaitidle();                      //检测液晶显示控制器是否空闲
    P1=c;
    CDPIN=1;                            //CD=1 CE=0 WR=0
    CEPIN=0;
    WRPIN=0;
    _nop_();
    WRPIN=1;
    CEPIN=1;
}
//-------------------------------------------------------------------------------
//子程序名称:lcdwc2(unsigned char c,unsigned char d).
//功能:向液晶显示控制器送2字节指令.
//修改日期:2009.08.07
//修改人:chujianjun@sunman.cn,tanchao@sunman.cn
//-------------------------------------------------------------------------------
void lcdwc2(unsigned char c,unsigned char d) //送2字节指令子程序
{   lcdwdata(d);                        //送指令参数
    lcdwc(c);                           //送指令码
}
//-------------------------------------------------------------------------------
//子程序名称:lcdwc3(unsigned char c,unsigned char d1,unsigned char d2).
//功能:向液晶显示控制器送3字节指令.
//修改日期:2009.08.07
//修改人:chujianjun@sunman.cn,tanchao@sunman.cn
//-------------------------------------------------------------------------------
void lcdwc3(unsigned char c,unsigned char d1,unsigned char d2) //送3字节指令子程序
{   lcdwdata(d1);                        //送指令参数1
    lcdwdata(d2);                        //送指令参数2
    lcdwc(c);                            //送指令码
}
//-------------------------------------------------------------------------------
//子程序名称:unsigned char lcdrdata(void).
//功能:从液晶显示控制器中读数据.
//修改日期:2009.08.07
//修改人:chujianjun@sunman.cn,tanchao@sunman.cn
//-------------------------------------------------------------------------------
unsigned char lcdrdata(void)            //读数据子程序
{   unsigned char d;
    lcdwaitidle();                      //检测液晶显示控制器是否空闲
    P1=0xff;
    CDPIN=0;                            //CD=0 CE=0 RD=0
    CEPIN=0;
    RDPIN=0;
    _nop_();
    d=P1;
    RDPIN=1;
    CEPIN=1;
    return d;
}
//-------------------------------------------------------------------------------
//子程序名称:void lcdwdata(unsigned char d).
//功能:向液晶显示控制器送数据.
//修改日期:2009.08.07
//修改人:chujianjun@sunman.cn,tanchao@sunman.cn
//-------------------------------------------------------------------------------
void lcdwdata(unsigned char d)          //送数据子程序
{   lcdwaitidle();                      //检测液晶显示控制器是否空闲
    P1=d;
    CDPIN=0;                            //CD=0 CE=0 WR=0
    CEPIN=0;
    WRPIN=0;
    _nop_();
    WRPIN=1;
    CEPIN=1;
}
//-------------------------------------------------------------------------------
//子程序名称:voidlcdwaitidle(void).
//功能:忙检测,在对液晶显示控制器操作的每一条指令之前,需检测液晶显示器是否空闲.
//修改日期:2009.08.07
//修改人:chujianjun@sunman.cn,tanchao@sunman.cn
//-------------------------------------------------------------------------------
void lcdwaitidle(void)                  //忙检测子程序
{   unsigned char i,d;
    P1=0xff;
    CDPIN=1;                            //CD=1 CE=0 RD=0
    CEPIN=0;
    for(i=0;i<20;i++)                   //
    {   RDPIN=0;
        _nop_();
        d=P1;
        RDPIN=1;
        if( (d&0x3) == 0x3 ) break;     //D0D1=11 空闲退出
    }
    CEPIN=1;
}

#endif 0