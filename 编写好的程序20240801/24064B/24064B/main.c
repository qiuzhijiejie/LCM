//SMG24064B??????????????????[C51????][MCS51??????]
//????: SMG24064B?????????????
//  ????????SMG24064B???????MCS51???????MCS51?????
//??????,????SMG240128A?????????,?????????240128A???.
//  ???????T6963C?????MCS51??????????,T6963C?????24064
//????????????,24064????????,24064????????,24064?
//????????????,??SMG24064?????????????????????.
//  ???????,?240?X64????????????,????,???????,??
//?????,???????,????,??,????,????.
//-------------------------------------------------------------------------------
//????: 2009.09.28
//????: VW For Windows
//????: AT89C52 MCU,MCS-51?? MCU,etc.
//???: ???(chujianjun@sunman.cn),??(tanchao@sunman.cn).
//????: ??(0731-84167806,84167807),??(0731-84167807),??(www.sunman.cn)
//????: ???????????????,???????.
//????: http:/www.sunman.cn/lcm/product/SMG24064B.html
//-------------------------------------------------------------------------------
#include <reg51.h>
#include <intrins.h>
//????????????
void exsample(void);                    //????
void exdelay(void);                     //???????
void putchar(unsigned int c);           //????????
void putstr(unsigned char code *s);     //????????,????0???
unsigned char code *getchinesecodepos(unsigned int ac);
unsigned char code *getenglishcodepos(unsigned char ac);
void putimage(unsigned char code *s);   //???????
void putsizeimage(unsigned char XSIZE,unsigned char YSIZE,unsigned char code *s);
                                        //????????????
void point(bit b);
void line(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,bit b);
void lineto(unsigned char x1,unsigned char y1,bit b);//??(XPOS,YPOS)-(X1,Y1)???
void rect(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,bit b);
void lcdfill(unsigned char d);          //????d??????????
void lcdpos(void);                      //?????(XPOS,YPOS)??RAM??????
void lcdreset(void);                    //?????????????
void lcdwd(unsigned char d);            //????????
unsigned char lcdrd(void);              //????????
void lcdwc(unsigned char c);            //??????
void lcdwc2(unsigned char c,unsigned char d);//?2???????
void lcdwc3(unsigned char c,unsigned char d1,unsigned char d2);//?3???????
unsigned char lcdrdata(void);           //??????
void lcdwdata(unsigned char d);         //??????
void lcdwaitidle(void);                 //??????
//-------------------------------------------------------------------------------
//SMG24064B???????????
//PIN1: FG   [???]-------------------VSS
//PIN2: VSS  [???]-------------------VSS
//PIN3: VDD  [????]-----------------VDD(+5V)
//PIN4: Vo   [LCD ????]-------------?10K?????2K???VSS?VEE
//                                      Vo=VEE??????
//PIN5: WR   [??? ???]------------P3.1
//PIN6: RD   [??? ???]------------P3.2
//PIN7: CE   [???? ???]----------P3.3
//PIN8: CD   [??/?????(H/L)]---P3.0
//PIN9: NC   [??]---------------------???
//PIN10:RST  [??? ???]------------VDD
//PIN11:D0   [Data I/O]-----------------P1.0
//PIN12:D1   [Data I/O]-----------------P1.1
//PIN13:D2   [Data I/O]-----------------P1.2
//PIN14:D3   [Data I/O]-----------------P1.3
//PIN15:D4   [Data I/O]-----------------P1.4
//PIN16:D5   [Data I/O]-----------------P1.5
//PIN17:D6   [Data I/O]-----------------P1.6
//PIN18:D7   [Data I/O]-----------------P1.7
//PIN19:FS   [????]-----------------VSS
//PIN20:VEE  [LCD????]--------------???????.
//PIN21:BLA  [?????]---------------??????.
//PIN22:BLK  [?????]---------------VSS
//?:8031??????12MHz.
//???http://download.sunman.cn/lcm/product/1/SMG24064B.pdf
//-------------------------------------------------------------------------------
//???????????????????
//??????????,????????????????
sbit CDPIN=P3^0;                        // C/D???????
sbit CEPIN=P3^3;                        // CE???????
sbit WRPIN=P3^1;                        // WR???????
sbit RDPIN=P3^2;                        // RD???????
//-------------------------------------------------------------------------------
//??XPOS,YPOS??????????????????
//??????????,??????????????????
unsigned char XPOS;                     //???LCD??????
unsigned char YPOS;                     //???LCD??????
bit CharImageReverse;                   //??????????,0(????),1(??)

#define XDOTS   240                     //????X????
#define YDOTS   64                      //????X????
//-------------------------------------------------------------------------------
//???????????
//?????????Sunman??????????????.exe?????charlib.c?
//????????
//???????????
//????Sunman??????????????.exe????
//????:[??] ????:[????]
//????:[??] ????:[??] ????:[16]

#define ENGLISHCHARNUMBER  34      //?????????????????
#define CHINESECHARNUMBER  1       //?????????????????
#define ENGLISHCHARSIZE    8       //????X????????
#define CHINESECHARSIZE    16      //????X????????Y????????
#define ENGLISHCHARDOTSIZE 16      //????????????
#define CHINESECHARDOTSIZE 32      //????????????

unsigned int code EnglishCode[]={
0x20,    //??001:[ ]
0x2d,    //??002:[-]
0x2e,    //??003:[.]
0x30,    //??004:[0]
0x32,    //??005:[2]
0x34,    //??006:[4]
0x36,    //??007:[6]
0x41,    //??008:[A]
0x42,    //??009:[B]
0x43,    //??010:[C]
0x44,    //??011:[D]
0x45,    //??012:[E]
0x49,    //??013:[I]
0x4c,    //??014:[L]
0x4d,    //??015:[M]
0x4e,    //??016:[N]
0x4f,    //??017:[O]
0x52,    //??018:[R]
0x53,    //??019:[S]
0x54,    //??020:[T]
0x57,    //??021:[W]
0x5a,    //??022:[Z]
0x61,    //??023:[a]
0x63,    //??024:[c]
0x65,    //??025:[e]
0x67,    //??026:[g]
0x68,    //??027:[h]
0x69,    //??028:[i]
0x6c,    //??029:[l]
0x6e,    //??030:[n]
0x6f,    //??031:[o]
0x72,    //??032:[r]
0x74,    //??033:[t]
0x79     //??034:[y]
};

unsigned int code ChineseCode[]={
0xa1a1   //??001:[ ]
};

unsigned char code EnglishCharDot[]={
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,   //??001:[ ]
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,   //??002:[-]
0x7e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,   //??003:[.]
0x00,0x00,0x00,0x00,0x60,0x60,0x00,0x00,
0x00,0x00,0x00,0x18,0x24,0x42,0x42,0x42,   //??004:[0]
0x42,0x42,0x42,0x42,0x24,0x18,0x00,0x00,
0x00,0x00,0x00,0x3c,0x42,0x42,0x42,0x02,   //??005:[2]
0x04,0x08,0x10,0x20,0x42,0x7e,0x00,0x00,
0x00,0x00,0x00,0x04,0x0c,0x0c,0x14,0x24,   //??006:[4]
0x24,0x44,0x7f,0x04,0x04,0x1f,0x00,0x00,
0x00,0x00,0x00,0x18,0x24,0x40,0x40,0x5c,   //??007:[6]
0x62,0x42,0x42,0x42,0x22,0x1c,0x00,0x00,
0x00,0x00,0x00,0x10,0x10,0x18,0x28,0x28,   //??008:[A]
0x24,0x3c,0x44,0x42,0x42,0xe7,0x00,0x00,
0x00,0x00,0x00,0xf8,0x44,0x44,0x44,0x78,   //??009:[B]
0x44,0x42,0x42,0x42,0x44,0xf8,0x00,0x00,
0x00,0x00,0x00,0x3e,0x42,0x42,0x80,0x80,   //??010:[C]
0x80,0x80,0x80,0x42,0x44,0x38,0x00,0x00,
0x00,0x00,0x00,0xf8,0x44,0x42,0x42,0x42,   //??011:[D]
0x42,0x42,0x42,0x42,0x44,0xf8,0x00,0x00,
0x00,0x00,0x00,0xfc,0x42,0x48,0x48,0x78,   //??012:[E]
0x48,0x48,0x40,0x42,0x42,0xfc,0x00,0x00,
0x00,0x00,0x00,0x7c,0x10,0x10,0x10,0x10,   //??013:[I]
0x10,0x10,0x10,0x10,0x10,0x7c,0x00,0x00,
0x00,0x00,0x00,0xe0,0x40,0x40,0x40,0x40,   //??014:[L]
0x40,0x40,0x40,0x40,0x42,0xfe,0x00,0x00,
0x00,0x00,0x00,0xee,0x6c,0x6c,0x6c,0x6c,   //??015:[M]
0x6c,0x54,0x54,0x54,0x54,0xd6,0x00,0x00,
0x00,0x00,0x00,0xc7,0x62,0x62,0x52,0x52,   //??016:[N]
0x4a,0x4a,0x4a,0x46,0x46,0xe2,0x00,0x00,
0x00,0x00,0x00,0x38,0x44,0x82,0x82,0x82,   //??017:[O]
0x82,0x82,0x82,0x82,0x44,0x38,0x00,0x00,
0x00,0x00,0x00,0xfc,0x42,0x42,0x42,0x7c,   //??018:[R]
0x48,0x48,0x44,0x44,0x42,0xe3,0x00,0x00,
0x00,0x00,0x00,0x3e,0x42,0x42,0x40,0x20,   //??019:[S]
0x18,0x04,0x02,0x42,0x42,0x7c,0x00,0x00,
0x00,0x00,0x00,0xfe,0x92,0x10,0x10,0x10,   //??020:[T]
0x10,0x10,0x10,0x10,0x10,0x38,0x00,0x00,
0x00,0x00,0x00,0xd6,0x54,0x54,0x54,0x54,   //??021:[W]
0x54,0x6c,0x28,0x28,0x28,0x28,0x00,0x00,
0x00,0x00,0x00,0x7e,0x84,0x04,0x08,0x08,   //??022:[Z]
0x10,0x20,0x20,0x42,0x42,0xfc,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x38,   //??023:[a]
0x44,0x0c,0x34,0x44,0x4c,0x36,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,   //??024:[c]
0x22,0x40,0x40,0x40,0x22,0x1c,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3c,   //??025:[e]
0x42,0x42,0x7e,0x40,0x42,0x3c,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3e,   //??026:[g]
0x44,0x44,0x38,0x40,0x3c,0x42,0x42,0x3c,
0x00,0x00,0x00,0x00,0xc0,0x40,0x40,0x5c,   //??027:[h]
0x62,0x42,0x42,0x42,0x42,0xe7,0x00,0x00,
0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x70,   //??028:[i]
0x10,0x10,0x10,0x10,0x10,0x7c,0x00,0x00,
0x00,0x00,0x00,0x10,0x70,0x10,0x10,0x10,   //??029:[l]
0x10,0x10,0x10,0x10,0x10,0x7c,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xdc,   //??030:[n]
0x62,0x42,0x42,0x42,0x42,0xe7,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3c,   //??031:[o]
0x42,0x42,0x42,0x42,0x42,0x3c,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xee,   //??032:[r]
0x32,0x20,0x20,0x20,0x20,0xf8,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x10,0x10,0x7c,   //??033:[t]
0x10,0x10,0x10,0x10,0x12,0x0c,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xe7,   //??034:[y]
0x42,0x24,0x24,0x18,0x18,0x10,0x10,0x60
};

unsigned char code ChineseCharDot[]={
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,   //??001:[ ]
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};
//-------------------------------------------------------------------------------
//??????????
//?????????SUNMAN?????????.EXE?????imagelib.c??????
//??
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
    {   lcdreset();                     //??????????
        exsample();                     //????
    }
}
void exsample(void)                     //????
{
//0.??????,?LCD????????
    CharImageReverse=0;                 //????
    lcdfill(0);                         //??

//1.????: ?(1,19)????32??SUNMAN??
    XPOS=1;
    YPOS=19;
    putimage(Img_sunman_32);            //??????LCDRAM???
    exdelay();                          //???600mS

//2.?????: ?(6,3)????"LCM24064B Oriental Technology"???.
    XPOS=6;
    YPOS=3;
    putstr("LCM24064B Oriental Technology");
    exdelay();                          //???600mS

//3.?????: ?(40,32)????"WWW.ORIENTAL-LCD.COM"???.
    XPOS=40;
    YPOS=32;
    putstr("WWW.ORIENTAL-LCD.COM");
    exdelay();                          //???600mS

//4.????: ?(40,32)????"WWW.ORIENTAL-LCD.COM"???.
    XPOS=40;
    YPOS=32;
    CharImageReverse=1;
    putstr("WWW.ORIENTAL-LCD.COM");
    CharImageReverse=0;
    exdelay();                          //???600mS

//5.????: RECT(0,0)-(239,63),???.
    rect(0,0,239,63,1);
    exdelay();                          //???600mS

//6.????: point(14,52),??.
    XPOS=14;
    YPOS=52;
    point(1);
    exdelay();                          //???600mS

//7.????: line(14,85)-(119,63),??.
    line(14,52,119,63,1);
    exdelay();                          //???600mS

//8.????: lineto(225,52),??.
    lineto(225,52,1);
    exdelay();                          //???600mS

//9.????: lineto(14,52),??.
    lineto(14,52,1);
    exdelay();                          //???600mS

//10.????: ?????????.
    lcdfill(0);                        //??
    exdelay();                          //???600mS
}
void exdelay(void)                      //???????
{ unsigned char i,j,k;                  //???600mS
  for(i=0;i<60;i++)
    for(j=0;j<64;j++)
      for(k=0;k<51;k++);
}
//-------------------------------------------------------------------------------
//??putchar???????????,putstr???????????,getchinesecodepos,
//getenglishcodepos???????????.
//-------------------------------------------------------------------------------
//?????:void putchar(unsigned int c).
//??:?(XPOS,YPOS)?????????,?c>128 ???????,???????
//????:2009.8.18
//???:chujianjun@sunman.cn,tanchao@sunman.cn
//-------------------------------------------------------------------------------
void putchar(unsigned int c)            //????????
{   if(c>128)
        putsizeimage(CHINESECHARSIZE,CHINESECHARSIZE,getchinesecodepos(c));
    else
        putsizeimage(ENGLISHCHARSIZE,CHINESECHARSIZE,getenglishcodepos(c));
}
//-------------------------------------------------------------------------------
//?????:void putstr(unsigned char code *s).
//??:??????,?*s=0 ???????.
//????:2009.8.18
//???:chujianjun@sunman.cn,tanchao@sunman.cn
//-------------------------------------------------------------------------------
void putstr(unsigned char code *s)      //????????,????0???
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
//?????:unsigned char code *getchinesecodepos(unsigned char ac).
//??:??????????????????????????????.
//????:2009.8.18
//???:chujianjun@sunman.cn,tanchao@sunman.cn
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
//?????:unsigned char code *getenglishcodepos(unsigned char ac).
//??:????ASCII????????????ASCII??????????.
//????:2009.8.18
//???:chujianjun@sunman.cn,tanchao@sunman.cn
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
//??putsizeimage???????????,putimage???????????
//-------------------------------------------------------------------------------
//?????:void putsizeimage(unsigned char XSIZE,unsigned YSIZE,
//                             unsigned char code *s)
//??:?(XPOS,YPOS)????XSIZE??YISZE??????*S.
//????:2009.8.17
//???:chujianjun@sunman.cn,tanchao@sunman.cn
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
//?????:void putimage(unsigned char code *s).
//??:?(XPOS,YPOS)????XSIZE[*s]??YISZE[*(s+1)]??????[*(s+2)].
//????:2009.8.18
//???:chujianjun@sunman.cn,tanchao@sunman.cn
//-------------------------------------------------------------------------------
void putimage(unsigned char code *s)    //???????
{   unsigned char XSIZE,YSIZE;
    XSIZE=*s;
    s++;
    YSIZE=*s;
    s++;
    putsizeimage(XSIZE,YSIZE,s);
}
//-------------------------------------------------------------------------------
//??point???????????,line,lineto,rect???????????.
//-------------------------------------------------------------------------------
//?????:void point(bit b).
//??:?b????(XPOS,YPOS)?????.
//????:2009.8.18
//???:chujianjun@sunman.cn,tanchao@sunman.cn
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
//?????:void line(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,bit b)
//??:?b?????(x0,y0)-(x1,y1)???
//????:2009.8.18
//???:chujianjun@sunman.cn,tanchao@sunman.cn
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
//?????:void lineto(unsigned char x1,unsigned char y1,bit b)
//??:?b?????(XPOS,YPOS)-(x1,y1)???
//????:2009.8.18
//???:chujianjun@sunman.cn,tanchao@sunman.cn
//-------------------------------------------------------------------------------
void lineto(unsigned char x1,unsigned char y1,bit b)//??(XPOS,YPOS)-(X1,Y1)???
{   line(XPOS,YPOS,x1,y1,b);
}
//-------------------------------------------------------------------------------
//?????:void rect(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,bit b)
//??:?b?????(x0,y0)-(x1,y1)???
//????:2009.8.18
//???:chujianjun@sunman.cn,tanchao@sunman.cn
//-------------------------------------------------------------------------------
void rect(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,bit b)
{   line(x0,y0,x1,y0,b);
    line(x1,y0,x1,y1,b);
    line(x1,y1,x0,y1,b);
    line(x0,y1,x0,y0,b);
}
//-------------------------------------------------------------------------------
//??lcdfill,lcdpos,lcdreset?T6963??????24064????????????
//-------------------------------------------------------------------------------
//?????:void lcdfill(unsigned char d).
//??:????d???????.
//????:2009.09.28
//???:chujianjun@sunman.cn,tanchao@sunman.cn
//-------------------------------------------------------------------------------
void lcdfill(unsigned char d)           //????d??????????
{   unsigned char i,j;
    lcdwc3(0x24,0,0);                   //????RAM???0
    for(i=0;i<YDOTS;i++)                //??
        for(j=0;j<30;j++)               //30??????240?
            lcdwd(d);                   //?????
}
//-------------------------------------------------------------------------------
//?????:void lcdpos(void).
//??:?????(XPOS,YPOS)???????RAM??.
//????:2009.08.07
//???:chujianjun@sunman.cn,tanchao@sunman.cn
//-------------------------------------------------------------------------------
void lcdpos(void)                       //?????(XPOS,YPOS)??RAM??????
{  unsigned int CurrentAddress;
   CurrentAddress=YPOS*30;              //LCDRAM??=30*YPOS+(XPOS/8)
   CurrentAddress+=XPOS/8;
   lcdwc3(0x24,CurrentAddress&0xff,CurrentAddress/256);
}
//-------------------------------------------------------------------------------
//?????:void lcdreset(void)
//??:??????????
//????:2009.08.07
//???:chujianjun@sunman.cn,tanchao@sunman.cn
//-------------------------------------------------------------------------------
void lcdreset(void)                     //?????????????
{   lcdwc(0x80);                        //OR MODE
    lcdwc(0x98);                        //?????
    lcdwc3(0x42,0,0);                   //???????????0
    lcdwc3(0x43,30,0);                  //??????????30???240?
}
//-------------------------------------------------------------------------------
//??lcdwd,lcdrd,lcdwc,lcdwc2,lcdwc3?T6963C?????[MCS51????????]?
//    ?????;lcdwdata,lcdrdata,lcdwaitidle??????.
//-------------------------------------------------------------------------------
//?????:void lcdwd(unsigned char d).
//??:?????????????.
//????:2009.08.07
//???:chujianjun@sunman.cn,tanchao@sunman.cn
//-------------------------------------------------------------------------------
void lcdwd(unsigned char d)             //????????
{   lcdwc2(0xc0,d);
}
//-------------------------------------------------------------------------------
//?????:unsigned char lcdrd(void).
//??:??????????????.
//????:2009.08.07
//???:chujianjun@sunman.cn,tanchao@sunman.cn
//-------------------------------------------------------------------------------
unsigned char lcdrd(void)               //????????
{   lcdwc(0xc1);                        //????????
    return lcdrdata();                  //???
}
//-------------------------------------------------------------------------------
//?????:lcdwc(unsigned char c).
//??:???????????.
//????:2009.08.07
//???:chujianjun@sunman.cn,tanchao@sunman.cn
//-------------------------------------------------------------------------------
void lcdwc(unsigned char c)             //??????
{   lcdwaitidle();                      //?????????????
    P1=c;
    CDPIN=1;                            //CD=1 CE=0 WR=0
    CEPIN=0;
    WRPIN=0;
    _nop_();
    WRPIN=1;
    CEPIN=1;
}
//-------------------------------------------------------------------------------
//?????:lcdwc2(unsigned char c,unsigned char d).
//??:?????????2????.
//????:2009.08.07
//???:chujianjun@sunman.cn,tanchao@sunman.cn
//-------------------------------------------------------------------------------
void lcdwc2(unsigned char c,unsigned char d) //?2???????
{   lcdwdata(d);                        //?????
    lcdwc(c);                           //????
}
//-------------------------------------------------------------------------------
//?????:lcdwc3(unsigned char c,unsigned char d1,unsigned char d2).
//??:?????????3????.
//????:2009.08.07
//???:chujianjun@sunman.cn,tanchao@sunman.cn
//-------------------------------------------------------------------------------
void lcdwc3(unsigned char c,unsigned char d1,unsigned char d2) //?3???????
{   lcdwdata(d1);                        //?????1
    lcdwdata(d2);                        //?????2
    lcdwc(c);                            //????
}
//-------------------------------------------------------------------------------
//?????:unsigned char lcdrdata(void).
//??:????????????.
//????:2009.08.07
//???:chujianjun@sunman.cn,tanchao@sunman.cn
//-------------------------------------------------------------------------------
unsigned char lcdrdata(void)            //??????
{   unsigned char d;
    lcdwaitidle();                      //?????????????
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
//?????:void lcdwdata(unsigned char d).
//??:???????????.
//????:2009.08.07
//???:chujianjun@sunman.cn,tanchao@sunman.cn
//-------------------------------------------------------------------------------
void lcdwdata(unsigned char d)          //??????
{   lcdwaitidle();                      //?????????????
    P1=d;
    CDPIN=0;                            //CD=0 CE=0 WR=0
    CEPIN=0;
    WRPIN=0;
    _nop_();
    WRPIN=1;
    CEPIN=1;
}
//-------------------------------------------------------------------------------
//?????:voidlcdwaitidle(void).
//??:???,???????????????????,????????????.
//????:2009.08.07
//???:chujianjun@sunman.cn,tanchao@sunman.cn
//-------------------------------------------------------------------------------
void lcdwaitidle(void)                  //??????
{   unsigned char i,d;
    P1=0xff;
    CDPIN=1;                            //CD=1 CE=0 RD=0
    CEPIN=0;
    for(i=0;i<20;i++)                   //
    {   RDPIN=0;
        _nop_();
        d=P1;
        RDPIN=1;
        if( (d&0x3) == 0x3 ) break;     //D0D1=11 ????
    }
    CEPIN=1;
}