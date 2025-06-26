#ifndef __HT1622_H__
#define __HT1622_H__
#include <REGX52.H>

sbit LCDDATA = P1^1;
sbit LCDCS1  = P1^2;
sbit LCDWR   = P1^0;
sbit LCDRD   = P1^3;

void lcd_init();
void lcd_clear();
void lcd_all();
void write_com(unsigned char com);
void write_data(unsigned char add1,dat2);   
void delay(unsigned int z);  
void display_sequence();	
void display_wave_sequence();
#endif