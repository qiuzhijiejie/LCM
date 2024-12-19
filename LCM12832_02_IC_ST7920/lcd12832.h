#ifndef __lcd12832_H__
#define __lcd12832_H__



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
unsigned char lcdrd(void);              //从液晶显示控制器读数据
void lcdwd(unsigned char d);            //送图形数据子程序
void lcdwc(unsigned char c);            //送指令子程序
void lcdwaitidle(void);                 //控制器忙检测子程序
void glcdfill_with_params(unsigned char oddData, unsigned char evenData);
unsigned char key();
void HandleKey();
void DisplayPatterns();
#endif