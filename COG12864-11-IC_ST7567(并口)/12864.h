#ifndef __12864_H__
#define __12864_H__

#define uchar unsigned char 
#define uint unsigned int 
#define data   P1 
    
//sbit cs  =  P3^0;    
//sbit rst =  P3^3;   
//sbit di  =  P3^2;   //0-命令模式 1-数据模式
//sbit wr  =  P3^5;   
//sbit rd  =  P3^4;


sbit cs  =  P3^3;    
sbit rst =  P3^2;   
sbit di  =  P3^5;   //0-命令模式 1-数据模式
sbit wr  =  P3^1;   
sbit rd  =  P3^0;

///按键接口定义(与测试板一致)
sbit Key1 =P2^5;//HOLD
sbit Key2 =P2^6;//UP
sbit Key3 =P2^7;//DN 

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
void lcd_kuang(void);
void disp_ascii0816(uchar *ptrAscii, uchar startP, uchar startC);
void disp_ascii0508(uchar *ptrAscii, uchar startP, uchar startC);
void display_hanzi(uchar *p,uchar startP, uchar startC);
void display_hanzi_fan(uchar *p,uchar startP, uchar startC);
void display_photo(uchar *p,uchar startP,uchar startC);
void Init_IC(); 
unsigned int DN();
unsigned int UP();
unsigned int HOLD();
void Yehui_UI();
void whatsapp();
void lcd_describe(void);
void trans_display_photo(uchar *p,uchar startP,uchar startC);
	
#endif