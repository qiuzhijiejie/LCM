#ifndef __LCM823160_H__
#define __LCM823160_H__
#include <REGX52.H>

sbit LCDDATA = P1^1;
sbit LCDCS1  = P1^2;
sbit LCDWR   = P1^0;

void lcd_init();
void lcd_clear();
void lcd_all();
void write_com(unsigned char com);
void write_data(unsigned char add1,dat2);
void display();    
void delay(unsigned int z);  
void display_number(unsigned char num);
void display_digits_simple();
#endif