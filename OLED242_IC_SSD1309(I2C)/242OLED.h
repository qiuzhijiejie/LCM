#ifndef __242oled_H__
#define __242oled_H__



#define set_0   0x01
#define set_1   0x02
#define clr_0   0xFE
#define clr_1   0xFD

#define IIC_SCK_0  P3 &=clr_0            
#define IIC_SCK_1  P3|=set_0       
#define IIC_SDA_0  P3&=clr_1            
#define IIC_SDA_1  P3|=set_1

#define OLED_COLUMN_NUMBER 128
#define OLED_LINE_NUMBER 64
#define OLED_PAGE_NUMBER (OLED_LINE_NUMBER/8)
#define OLED_COLUMN_OFFSET 0
#define OLED_LINE_OFFSET 0

void delay_us(unsigned int _us_time);
void delay_ms(unsigned int _ms_time);
void IIC_write(unsigned char date);
void IIC_start();
void IIC_stop();
void OLED_send_cmd(unsigned char o_command);
void OLED_send_data(unsigned char o_data);
void Column_set(unsigned char column);
void Page_set(unsigned char page);
void OLED_clear(void);
void OLED_full(void);
void OLED_init(void);
void Picture_display(const unsigned char *ptr_pic);
void Picture_ReverseDisplay(const unsigned char *ptr_pic);
void IO_init(void);
unsigned int ReadKey1();
void display1(void);
void display2(void);
void display3(void);
void OLED_fullROW(unsigned char Data);
void OLED_fullCOL(unsigned char oddData, unsigned char evenData);

#endif