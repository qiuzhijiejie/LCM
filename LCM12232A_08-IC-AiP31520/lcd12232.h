#ifndef __lcd12232_H__
#define __lcd12232_H__
#define uint  unsigned int
#define uchar unsigned char


void Delay(unsigned char ms);
uint ReadKey1() ;
void Delay1(uint n);
void Delay2(uint x);
void Write_Data_L(unsigned char Data);
void Write_Data_R(unsigned char Data);
void write_command_L(unsigned char cmd);
void write_command_R(unsigned char cmd);
void Set_Page_Address(unsigned char add);
void Set_Column_Address_L(unsigned char address);
void Set_Column_Address_R(unsigned char address);
void Initial(void);
void Display_dot(unsigned char value,unsigned char value1);
void show_chinese(void);
void show_english(void);
void Display_Picture(unsigned char pic[]);
void Display16_16Chinese(unsigned char page, unsigned char col, unsigned char Character_coding,  unsigned char ft[]) ;
void Display8_16English(unsigned char page, unsigned char col, unsigned char Character_coding,  unsigned char ft[]) ;
void DisplayPatterns();
void HandleKey() ;



#endif