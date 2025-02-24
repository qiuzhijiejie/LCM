#ifndef __lcm9832a_H__
#define __lcm9832a_H__

#define uint  unsigned int
#define uchar unsigned char
	
#define TIMING                 6800			
#define A0_RS                  P3_0  	//Selection of Command or Data.  1 :Data  0 : Command
#define CS1                    P3_1		//CS pin. Left 
#define CS2                    P3_2	    //CS pin. Right
#define CL                 	   P3_3		//External Clock Input
#define E_RD                   P3_4		//Enable Signal (E) For the 68-type Microcontroller, or READ (RD) signal for the 80-type microcontroller.
#define RW_WR                  P3_5		//Read/Write (R/W) Signal for the 68-type Microcontroller, or WRITE(WR) signal for the 80-type microcontroller.
#define RST                    P3_6		//RESET PIN;	
#define KEY                    P2_0	    //KEY PIN;
#define DATA_BUS               P1		//Data Bus


void MAPPING(unsigned char bit_value);
void Display_One_Emglish(void);
void Display_One_Chinese_8x16(unsigned char page, unsigned char col, unsigned char Character_coding, unsigned char ft[]) ;
void Display_One_Chinese_16x16(unsigned char page, unsigned char col, unsigned char Character_coding, unsigned char ft[]); 
void Display_Picture(unsigned char pic[]);
void Show_chinese(void);
void Display_dot(unsigned char value,unsigned char value1);
void Initial(void);
void Set_Column_Address_R(unsigned char address);
void Set_Column_Address_L(unsigned char address);
void Set_Page_Address(unsigned char add);
void Write_Instruction_R(unsigned char cmd);
void Write_Instruction_L(unsigned char cmd);
void Write_Data_R(unsigned char dat); 
void Write_Data_L(unsigned char dat);
void Busy_R();
void Busy_L();
void Delay2(uint n);
void Delay1(uint x);
void Delay(uint MS);
unsigned char Key();
void Display_pic(void);


#endif