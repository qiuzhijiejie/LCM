#ifndef __242oled_H__
#define __242oled_H__


//unsigned char code OLED_init_cmd[25];
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
void display4(void);
void display5(void);
void OLED_dotted_pattern(void);
void OLED_dotted_Reversepattern(void);


#endif