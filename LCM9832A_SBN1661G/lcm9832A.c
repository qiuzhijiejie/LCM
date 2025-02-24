#include <REGX52.H>
#include "lcm9832a.h"
#include <intrins.h>
#include "pic.h"
sbit int0=P2^2;		//external interrup input
bit  log=0;			//display stop mark

void Step(void) interrupt 0 using 0
{
	Delay1(3000);
	if(int0==0)	log=!log;
    while(!int0) Delay1(3000);
    return;
}


void Delay1(uint n)
{
	while(n--);  
}

void Delay2(uint x)
{
	while(x)
	{   
	  x--;	 
	
		for(;log==1;);
 	}
}

void Delay(uint MS)
{	unsigned char j;
	while(MS--)
	{
       for(j=0;j<5;j++)
	   Delay2(10);

	}
}

////////////////////////Timer interrupt   Produce 2 KHZ clock signal
void clock_timer(void) interrupt 1 using 1  //
{
 	TH0=0xff;   //(65536-250)/256;	Timer high byte   1s=1000000us  1000000/4000=250    250us H  250us L   A cycle for 500 us
 	TL0=0x83;   //(65536-250)%256;	Timer low  byte
	CL=~CL;
  return;
}
 

 /*
 ////////////////////////Timer interrupt   Produce 10 KHZ clock signal
void clock_timer(void) interrupt 1 using 1  //
{
 	TH0=0xff;   //(65536-50)/256;	Timer high byte   1s=1000000us  1000000/10000=100    50us H  50us L   A cycle for 100 us
 	TL0=0xCE;   //(65536-50)%256;	Timer low  byte
	CL=~CL;
  return;
}

 */

#if (TIMING==6800)
//6800 interface  //check left half screen busy  
void Busy_L() //7.9
{
    unsigned char flag=0xff;
    A0_RS=0;  //command
	CS1=0;    //Left 
	E_RD=0;   
	RW_WR=1;	
	while ((flag&0x80)==0x80)
	{
		E_RD=1;
		_nop_();
	    flag=DATA_BUS;
    	E_RD=0;
	}
	CS1=1;
}

#else
//8080 interface  //check left half screen busy  
void Busy_L()
{
    unsigned char flag=0xff;
    A0_RS=0;CS1=0;E_RD=1;RW_WR=1;
	while ((flag&0x80)==0x80)
	{
		E_RD=0;
		_nop_();
	    flag=DATA_BUS;
    	E_RD=1;
	}
	CS1=1;
}

#endif

#if (TIMING==6800)
//6800 interface  //check right half screen busy  
void Busy_R()
{
    unsigned char flag=0xff;
    A0_RS=0; //Command
	CS2=0;   //Right
	E_RD=0;
	RW_WR=1;
	while ((flag&0x80)==0x80)
	{
		E_RD=1;
		_nop_();
	    flag=DATA_BUS;
    	E_RD=0;
	}
	CS2=1;
}

#else
//8080 interface  //check right half screen busy 
void Busy_R()
{
    unsigned char flag=0xff;
    A0_RS=0;CS2=0;E_RD=1;RW_WR=1;
	while ((flag&0x80)==0x80)
	{
		E_RD=0;
		_nop_();
	    flag=DATA_BUS;
    	E_RD=1;
	}
	CS2=1;
}

#endif

#if (TIMING==6800)
//6800 interface  //write data to left half screen 
void Write_Data_L(unsigned char dat) //8.1
{
    Busy_L();
    A0_RS=1; //Data
	CS1=0;   //light
    RW_WR=0;
	DATA_BUS=dat;
    E_RD=1;
    _nop_();
    E_RD=0;
    RW_WR=1;
	CS1=1;
	return;
}

#else
//8080 interface  //write data to left half screen 
void Write_Data_L(unsigned char dat)
{
    Busy_L();
    A0_RS=1;
	CS1=0;
    E_RD=1;
    RW_WR=0;
	DATA_BUS=dat;
    _nop_();
    RW_WR=1;
	CS1=1;
	return;
}

#endif


#if (TIMING==6800)
//6800 interface  //write data to right half screen 
void Write_Data_R(unsigned char dat) 
{
   Busy_R();
    A0_RS=1;
	CS2=0;
    RW_WR=0;
	DATA_BUS=dat;
    E_RD=1;
    _nop_();
    E_RD=0;
    RW_WR=1;
	CS2=1;
	return;
}

#else
//8080 interface  //write data to right half screen
void Write_Data_R(unsigned char dat)
{
    Busy_L();
    A0_RS=1;
	CS2=0;
    E_RD=1;
    RW_WR=0;
	DATA_BUS=dat;
    _nop_();
    RW_WR=1;
	CS2=1;
	return;
}

#endif


#if (TIMING==6800)
//6800 interface  //write command to left half screen
void Write_Instruction_L(unsigned char cmd)
{
    Busy_L();
    A0_RS=0;
	CS1=0;
    RW_WR=0;
	DATA_BUS=cmd;
    E_RD=1;
	_nop_();
    E_RD=0;
    RW_WR=1;
	CS1=1;
	return;
}

#else
//8080 interface  //write command to left half screen
void Write_Instruction_L(unsigned char cmd)
{
    Busy_L();
    A0_RS=0;
	CS1=0;
    E_RD=1;
    RW_WR=0;
	DATA_BUS=cmd;
	_nop_();
    RW_WR=1;
	CS1=1;
	return;
}

#endif

#if (TIMING==6800)
//6800 interface  //write command to right half screen
void Write_Instruction_R(unsigned char cmd)
{
//    Busy_R();
    A0_RS=0;
	CS2=0;
    RW_WR=0;
	DATA_BUS=cmd;
    E_RD=1;
	_nop_();
    E_RD=0;
    RW_WR=1;
	CS2=1;
	return;
}

#else
//8080 interface  //write command to right half screen
void Write_Instruction_R(unsigned char cmd)
{
    Busy_R();
    A0_RS=0;
	CS2=0;
    E_RD=1;
    RW_WR=0;
	DATA_BUS=cmd;
	_nop_();
    RW_WR=1;
	CS2=1;
	return;
}

#endif




// Set page address 0~4
void Set_Page_Address(unsigned char add)
{
    add=0xb8|add;
    Write_Instruction_L(add);
    Write_Instruction_R(add);
	return;
}


//Set left half screen column address
void Set_Column_Address_L(unsigned char address)
{
    Write_Instruction_L(address&0x7F);
}

//Set right half screen column address
void Set_Column_Address_R(unsigned char address)
{
    Write_Instruction_R(address&0x7F);
}


// Initial LCM
void Initial(void)
{	//RESET///////////////
	RST=0;
	Delay(2);
	RST=1;
	Delay(10);
	//////////////////////
   /* RST rising edge edge 6800 interface  
	/* ///////////////////////////////////////////////
	The hardware RESET is edge-sensitive. It is not level-sensitive. That is, either a
	falling edge or a rising edge on this pin can reset the chip. The voltage level after
	the reset pulse selects the type of interface timing. If the voltage level after the reset
	pulse stays at HIGH, interface timing for the 68-type microcontroller is selected. If
	the voltage level after the reset pulse stays at LOW, then interface timing for the
	80-type microcontroller is selected.
	Therefore, a positive RESET pulse selects the 80-type microcontroller for interface
	and a negative RESET pulse selects the 68-type microcontroller for interface.
	*/////////////////////////////////////////////////

	Write_Instruction_L(0xE2);		//Software RESET left half screen
	Write_Instruction_R(0xE2);		//Software RESET right half screen

	Write_Instruction_L(0xA4);		//Static Drive OFF 
	Write_Instruction_R(0xA4);		//Static Drive OFF 

	Write_Instruction_L(0xA9);		//Select Duty Register 1/32
	Write_Instruction_R(0xA9);		//Select Duty Register 1/32

	Write_Instruction_L(0xA0);		//Memory/Segment Mapping Register  Normal Mapping
	Write_Instruction_R(0xA0);		//Memory/Segment Mapping Register  Normal Mapping

	Write_Instruction_L(0xC0);		//Start line
	Write_Instruction_R(0xC0);		//Start line


	Write_Instruction_L(0xAF);		//Turn on display
	Write_Instruction_R(0xAF);		//Turn on display

}

void Display_dot(unsigned char value,unsigned char value1)
{
    unsigned char i,j;
    for(i=0;i<0x04;i++)
	{
		Set_Page_Address(i);
        Set_Column_Address_L(0x00);
        Set_Column_Address_R(0x00);

		for(j=0;j<32;j++)
		{
		        Write_Data_L(value);
		        Write_Data_L(value1);
		        Write_Data_R(value1);
		        Write_Data_R(value);
		}
	}
    return;
}
//Show some Chinese characters
void Show_chinese(void) 
{
    unsigned char page, col, index;
	Display_Picture(pic1);
	
	//Display_One_Chinese_16x16(0,16,1,ft); 
    // 显示第一页的汉字
    page = 0;
    for (index = 0, col = 10; index < 5; index++, col += 16) {
        Display_One_Chinese_16x16(page, col, index, ft);
    }

    // 显示第二页的汉字
    page = 2;
    for (index = 5, col = 10; index < 10; index++, col += 16) {
        Display_One_Chinese_16x16(page, col, index, ft);
    }
}


//display one 16*16 dot character
void Display_One_Chinese_16x16(unsigned char page, unsigned char col, unsigned char Character_coding,  unsigned char ft[]) 
{
    unsigned char i,columnaddress=col;

	Set_Page_Address(page);
	if(col<61) 
	{		       
		for(i=0;i<16;i++)
	  	{
		 if(columnaddress<61)		 
			{Set_Column_Address_L(columnaddress);
			Write_Data_L(ft[Character_coding*32+i]);
			}
		else
			{Set_Column_Address_R(columnaddress-61);
			Write_Data_R(ft[Character_coding*32+i]);
			}
		 columnaddress+=1;
		}

		columnaddress=col;
		Set_Page_Address(page+1);	
		for(i=16;i<32;i++)
	  	{
		 if(columnaddress<61)		 
			{Set_Column_Address_L(columnaddress);
			Write_Data_L(ft[Character_coding*32+i]);
			}
		else
			{Set_Column_Address_R(columnaddress-61);
			Write_Data_R(ft[Character_coding*32+i]);
			}
		 columnaddress+=1;
		}
	}


	else  
	{		       
		for(i=0;i<16;i++)
	  	{
		 	if(columnaddress<122)		 
			{Set_Column_Address_R(columnaddress-61);
			Write_Data_R(ft[Character_coding*32+i]);
			}
			else
			{if(page<=1)
					{Set_Page_Address(page+2);
					Set_Column_Address_L(columnaddress-122);
					Write_Data_L(ft[Character_coding*32+i]);
					}
			}	
		 columnaddress+=1;
		}

		columnaddress=col;
		Set_Page_Address(page+1);	
		for(i=16;i<32;i++)
	  	{
			 if(columnaddress<122)		 
			{Set_Column_Address_R(columnaddress-61);
			Write_Data_R(ft[Character_coding*32+i]);
			}
			else
			{if(page<=1){Set_Page_Address(page+3);
						Set_Column_Address_L(columnaddress-122);
						Write_Data_L(ft[Character_coding*32+i]);
						}
			}
		 columnaddress+=1;
		}
	}

}



void Display_One_Chinese_8x16(unsigned char page, unsigned char col, unsigned char Character_coding, unsigned char ft[]) 
{
    unsigned char i, columnaddress = col;

    Set_Page_Address(page);
    if (col < 61) 
    {		       
        for (i = 0; i < 8; i++)  // 每次写入 8 列
        {
            if (columnaddress < 61)		 
            {
                Set_Column_Address_L(columnaddress);
                Write_Data_L(ft[Character_coding * 16 + i]);
            }
            else
            {
                Set_Column_Address_R(columnaddress - 61);
                Write_Data_R(ft[Character_coding * 16 + i]);
            }
            columnaddress += 1;
        }

        columnaddress = col;
        Set_Page_Address(page + 1);	
        for (i = 8; i < 16; i++)  // 第二部分数据（第 8 到 16 行）
        {
            if (columnaddress < 61)		 
            {
                Set_Column_Address_L(columnaddress);
                Write_Data_L(ft[Character_coding * 16 + i]);
            }
            else
            {
                Set_Column_Address_R(columnaddress - 61);
                Write_Data_R(ft[Character_coding * 16 + i]);
            }
            columnaddress += 1;
        }
    }
    else  
    {		       
        for (i = 0; i < 8; i++)  // 每次写入 8 列
        {
            if (columnaddress < 122)		 
            {
                Set_Column_Address_R(columnaddress - 61);
                Write_Data_R(ft[Character_coding * 16 + i]);
            }
            else
            {
                if (page <= 1)
                {
                    Set_Page_Address(page + 2);
                    Set_Column_Address_L(columnaddress - 122);
                    Write_Data_L(ft[Character_coding * 16 + i]);
                }
            }	
            columnaddress += 1;
        }

        columnaddress = col;
        Set_Page_Address(page + 1);	
        for (i = 8; i < 16; i++)  // 第二部分数据（第 8 到 16 行）
        {
            if (columnaddress < 122)		 
            {
                Set_Column_Address_R(columnaddress - 61);
                Write_Data_R(ft[Character_coding * 16 + i]);
            }
            else
            {
                if (page <= 1)
                {
                    Set_Page_Address(page + 3);
                    Set_Column_Address_L(columnaddress - 122);
                    Write_Data_L(ft[Character_coding * 16 + i]);
                }
            }
            columnaddress += 1;
        }
    }
}

//display Picture
void Display_Picture(unsigned char pic[])
{
    unsigned char i,j;

	for(i=0;i<0x04;i++)
	{
	Set_Page_Address(i);
    Set_Column_Address_L(0x00);
    Set_Column_Address_R(0x00);

        for(j=0;j<61;j++)				//display left half screen
		{
		    Write_Data_L(pic[i*98+j]);
		}
        for(j=61;j<98;j++)				//display right half screen
		{
		    Write_Data_R(pic[i*98+j]);
		}
	}
    return;
}


void Display_One_Emglish(void)
{	
    unsigned char col, index;

	Display_Picture(pic2);
	
    for (index = 0, col = 15; index < 8; index++, col += 8) {
        Display_One_Chinese_8x16(1, col, index, ft0);
    }

}
//unsigned char Key()
//{
//	if(KEY==0)
//	{
//		Delay(1000);
//		while(KEY==0);	
//		return 1;
//	}
//	return 1;
//}


void Display_pic(void)
{
		Display_dot(0xAA,0x55);
		Delay(400);
		Display_dot(0x55,0xAA);
		Delay(400);
		Display_dot(0x55,0x55);
		Delay(400);
		Display_dot(0xAA,0xAA);
		Delay(400);
		Display_dot(0xFF,0x00);
		Delay(400);
		Display_dot(0x00,0xFF);
		Delay(400);
}


void MAPPING(unsigned char bit_value) // bit_value: 1 or 0
{
    unsigned char flag = 0xff;
    A0_RS = 0;  // command
    CS1 = 0;    // Left
    E_RD = 0;
    RW_WR = 1;

    if (bit_value == 1) {
        // Set the 6th bit to 1
        while ((flag & 0x40) == 0x40) { // Check the 6th bit (0x40)
            E_RD = 1;
            _nop_();
            flag = DATA_BUS;
            E_RD = 0;
        }
    } else {
        // Set the 6th bit to 0
        while ((flag & 0x40) != 0x40) { // Check the 6th bit (0x40)
            E_RD = 1;
            _nop_();
            flag = DATA_BUS;
            E_RD = 0;
        }
    }

    CS1 = 1;
}
