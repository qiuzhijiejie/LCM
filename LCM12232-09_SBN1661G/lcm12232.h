#ifndef __lcm12232_H__
#define __lcm12232_H__

#define uint unsigned int
#define uchar unsigned char
#define TIMING                 6800			
#define A0                     P3_0  	//Selection of command or data.  1 :data  0 : command
#define CS1                    P3_1		//CS pin. left 
#define CS2                    P3_2	    //CS pin. right
#define CL                 	   P3_3		// external clock input
#define E_RD                   P3_4		// Enable signal (E) for the 68-type microcontroller, or READ (RD) signal for the 80-type microcontroller.
#define RW_WR                  P3_5		//Read/Write (R/W) signal for the 68-type microcontroller, or WRITE(WR) signal for the 80-type microcontroller.
#define RST                    P3_6		// RESET PIN;	

#define DATA_BUS               P1		//Data bus



void Display_One_Chinese_8x16(unsigned char page, unsigned char col, unsigned char Character_coding, unsigned char ft[]);
void Display_One_Chinese_16x16(unsigned char page, unsigned char col, unsigned char Character_coding,  unsigned char ft[]) ;
void Display_One_Number(void);
void Display_One_Emglish(void);
void Display_Picture(unsigned char pic[]);
void show_chinese(void);
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


#endif