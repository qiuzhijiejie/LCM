#ifndef __12864_H__
#define __12864_H__

#include <REGX52.H>


sbit sclk = P3^6;   /*接口定义:lcd_sclk就是LCD的D6 时钟线*/
sbit sid  = P3^7;   /*接口定义:lcd_sid就是LCD的D7 数据输入*/
sbit di   = P1^2;   /*接口定义:lcd_rs就是LCD的D/I H为数据 L为指令*/
sbit rst  = P1^0;   /*接口定义:lcd_reset就是LCD的/RST*/
sbit cs   = P1^1;   /*接口定义:lcd_cs1就是LCD的cs*/


////HCG13264-70
//sbit sclk = P1^7;   /*接口定义:lcd_sclk就是LCD的D6 时钟线*/
//sbit sid  = P3^1;   /*接口定义:lcd_sid就是LCD的D7 数据输入*/
//sbit di   = P1^6;   /*接口定义:lcd_rs就是LCD的D/I H为数据 L为指令*/
//sbit rst  = P1^5;   /*接口定义:lcd_reset就是LCD的/RST*/
//sbit cs   = P1^4;   /*接口定义:lcd_cs1就是LCD的cs*/

sbit LED = P0^6;  

#define uchar unsigned char 
#define uint unsigned int 


void delayUs(uint Tus);
void delayMs(uint Tms);
void LCD_Reset();
void write_cmd_dat(uchar cmda,uchar com);
void SetCA(uchar Caddr);
void SetPA(uchar Paddr);
void write_data(uint com,uchar page,uchar clum);
void GotoXY(uchar Paddr, uchar Caddr);
void display_test(uchar fst,uchar snd);
void Clear(void);
void wait_and_clear(void);
void lcd_test();
void lcd_kuang(void) ;
void disp_ascii0816(uchar *ptrAscii, uchar startP, uchar startC);
void disp_ascii0508(uchar *ptrAscii, uchar startP, uchar startC);
void display_hanzi(uchar *p,uchar startP, uchar startC);
void display_hanzi_fan(uchar *p,uchar startP, uchar startC);
void display_photo(uchar *p,uchar startP,uchar startC);
void Init_IC() ; 
void Yehui_UI();
void WhatsApp();

#endif




