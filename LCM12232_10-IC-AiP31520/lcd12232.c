#include <REGX52.H>
#include <AT89X52.H>
#include <intrins.h>
#include "picture.h"

#define uint  unsigned int
#define uchar unsigned char
#define A0                    P3_0  	//Selection of command or data.  1 :data  0 : command
#define E1                    P3_2		//Enable signal (E1) Right 
#define E2                    P3_3		//Enable signal (E2) Left
#define R_W                   P3_1		//Read/Write (R/W) signal for the 68-type microcontroller, or WRITE(WR) signal for the 80-type microcontroller.
#define RST                   P3_5		//RESET PIN;
#define DATA_BUS              P1		//Data bus	

sbit LCD_KEY=P2^0;
sbit int0 = P2^0;						//external interrup input
bit  log  = 0;							//display stop mark
unsigned char i;
void Delay(unsigned char ms)	//@12.000MHz
{
	while(ms--)
	{
		unsigned char data i, j;
		i = 12;
		j = 169;
		do
		{
			while (--j);
		} while (--i);
	}
}

uint ReadKey1() // ������⺯��
{
    if (LCD_KEY == 1)           
	{  // ����������
       Delay(1); // �򵥵�ȥ��������
        if (LCD_KEY == 1) 
		{  // ��鰴���Ƿ���Ȼ����
            while (LCD_KEY == 1);  // �ȴ������ͷ�
            return 1;  // ���ذ���������
        }
    }
    return 0;  // ����û�б�����
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
	




//write Data to left half screen 
void Write_Data_L(unsigned char Data) 
{  	Delay1(5);
    A0=1;
    R_W=0;
	DATA_BUS=Data;
    E1=1;
    _nop_();
    E1=0;
	return;
}

//write data to right half screen 
void Write_Data_R(unsigned char Data)
{
	Delay1(5);
	A0=1;//1:D7-D0��Ϊ��ʾ����
	R_W=0;//R_W=0 ʱ��ʾд����
	DATA_BUS=Data;
	E2=1;
	_nop_();
	E2=0;
	return;    
}

//write command to left half screen
void write_command_L(unsigned char cmd)
{	
	Delay1(20);
    A0=0;//0:D7-D0��Ϊ��ʾ��������
    R_W=0;
	DATA_BUS=cmd;
    E1=1;
	_nop_();
    E1=0;
	return;
}


//write command to right half screen
void write_command_R(unsigned char cmd)
{	
	Delay1(20);
    A0=0;
    R_W=0;
	DATA_BUS=cmd;
    E2=1;
	_nop_();
    E2=0;
	return;
}


// Set page address 0~4
void Set_Page_Address(unsigned char add)
{
    add=0xb8|add;// (B8 Hex)-Table 15
    write_command_L(add);
    write_command_R(add);
	return;
}


//Set left half screen column address
void Set_Column_Address_L(unsigned char address)
{
    write_command_L(address&0x7F);//0x4F
}

//Set right half screen column address
void Set_Column_Address_R(unsigned char address)
{
    write_command_R(address&0x7F);
}


// Initial 
void Initial(void)
{	
	RST=1;
	Delay(10);
	RST=0;
	Delay(2);
	RST=1;
	Delay(10);

	write_command_L(0xE2);		//Software RESET left half screen
	write_command_R(0xE2);		//Table 27

	write_command_L(0xA4);		//Static Drive OFF 
	write_command_R(0xA4);//A5  //Table 21 A5                    
	
	write_command_L(0xA9);		//Select Duty Register 1/32/ (1/16-A8)
	write_command_R(0xA9);		//Table 23

	write_command_L(0xA0);		//Memory/Segment Mapping Register  Normal Mapping
	write_command_R(0xA0);//A1	//Table 19

	write_command_L(0xC0);		//Start line
	write_command_R(0xC0);		//Table 13


	write_command_L(0xAF);		//Turn on display
	write_command_R(0xAF);//AE	//Table 11

}


//����Ļ����ʾͼ�������
void Display_dot(unsigned char value,unsigned char value1)
{
    unsigned char i,j;
    for(i=0;i<0x04;i++)//i��0��3,4ҳ
	{
		Set_Page_Address(i);
        Set_Column_Address_L(0x00);
        Set_Column_Address_R(0x00);

		for(j=0;j<32;j++)//ÿһҳ��д��32�е�����
		{
		        Write_Data_L(value);
		        Write_Data_L(value1);
		        Write_Data_R(value1);
		        Write_Data_R(value);
		}
	}
    return;
}


/*
display one 16*16 dot character
page��ָ��Ҫ���ĸ�ҳ��ַ�Ͽ�ʼ��ʾ���֡�ҳ����Ļ�Ĵ�ֱ�����ϵ�һ���߼�����

col��ָ������һ�п�ʼ��ʾ���֣�����ˮƽ�����λ�á�

Character_coding�����ֵı��룬�������ֿ���ѡ���ĸ��֡�

ft[]����ģ�������飬�洢���к��ֵ���ģ��Ϣ��ÿ��������32�ֽڣ�16x16���󣩱�ʾ��

*/
//display one 16*16 dot character

void Display16_16Chinese(unsigned char page, unsigned char col, unsigned char Character_coding,  unsigned char ft[]) 
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

// display one 8��16 dot character
void Display8_16English(unsigned char page, unsigned char col, unsigned char Character_coding, unsigned char ft[]) 
{
    unsigned char i, columnaddress = col;

    // ���õ�һ��ҳ�棨��ʾǰ8�е����ݣ�
    Set_Page_Address(page);
    if (col < 61) 
    {		       
        // ����ǰ8�����ݣ��ϰ벿�֣�
        for (i = 0; i < 8; i++)
        {
            if (columnaddress < 61) // ����Ļ
            {
                Set_Column_Address_L(columnaddress);      // ���������е�ַ
                Write_Data_L(ft[Character_coding * 16 + i]);  // д������
            }
            else  // ����Ļ
            {
                Set_Column_Address_R(columnaddress - 61); // ���������е�ַ
                Write_Data_R(ft[Character_coding * 16 + i]);  // д������
            }
            columnaddress += 1;  // �е�ַ����
        }

        // �����°벿��
        columnaddress = col;
        Set_Page_Address(page + 1);  // �л�����һҳ����ʾ�°벿�ֵ�8��
        for (i = 8; i < 16; i++)
        {
            if (columnaddress < 61)  // ����Ļ
            {
                Set_Column_Address_L(columnaddress);      // ���������е�ַ
                Write_Data_L(ft[Character_coding * 16 + i]);  // д������
            }
            else  // ����Ļ
            {
                Set_Column_Address_R(columnaddress - 61); // ���������е�ַ
                Write_Data_R(ft[Character_coding * 16 + i]);  // д������
            }
            columnaddress += 1;  // �е�ַ����
        }
    }
    else  
    {		       
        // ����������ķ�Χ��
        for (i = 0; i < 8; i++)
        {
            if (columnaddress < 122)  // ����Ļ
            {
                Set_Column_Address_R(columnaddress - 61);  // ���������е�ַ
                Write_Data_R(ft[Character_coding * 16 + i]);  // д������
            }
            else if (page <= 1)  // �絽��һ��ҳ���������ʾ
            {
                Set_Page_Address(page + 2);                // ������һ��ҳ��
                Set_Column_Address_L(columnaddress - 122); // ���������е�ַ
                Write_Data_L(ft[Character_coding * 16 + i]);  // д������
            }
            columnaddress += 1;  // �е�ַ����
        }

        // ��ʾ�°벿��
        columnaddress = col;
        Set_Page_Address(page + 1);	
        for (i = 8; i < 16; i++)
        {
            if (columnaddress < 122)  // ����Ļ
            {
                Set_Column_Address_R(columnaddress - 61);  // ���������е�ַ
                Write_Data_R(ft[Character_coding * 16 + i]);  // д������
            }
            else if (page <= 1)  // ��ҳ��ʾ
            {
                Set_Page_Address(page + 3);                // ������һ��ҳ��
                Set_Column_Address_L(columnaddress - 122); // ���������е�ַ
                Write_Data_L(ft[Character_coding * 16 + i]);  // д������
            }
            columnaddress += 1;  // �е�ַ����
        }
    }
	
}


//Show some Chinese characters
void show_chinese(void)
{	
	Display_dot(0x00,0x00);
    for (i = 0; i < 7; i++) 
    {
        Display16_16Chinese(2, i * 16, i, ft);
    }

    // �� page 0 ��ʾ�ַ� 7 �� 9
    for (i = 0; i < 3; i++) 
    {
        Display16_16Chinese(0, i * 16, i + 7, ft);
    }
}

void show_english(void)
{	
	Display_dot(0x00,0x00);
    for (i = 0; i < 12; i++) 
    {
        Display8_16English(2, i * 8, i, English_character);
    }

    for (i = 0; i < 12; i++) 
    {
        Display8_16English(0, i * 8, i + 16,English_character);
    }
}



//display Picture
void Display_Picture(unsigned char pic[])
{
    unsigned char i,j,num=0;

	for(i=0;i<0x04;i++)
	{
	Set_Page_Address(i);
    Set_Column_Address_L(0x00);
    Set_Column_Address_R(0x00);

        for(j=0;j<61;j++)				//display left half screen
		{
		    Write_Data_L(pic[i*122+j]);
		}
        for(j=61;j<122;j++)				//display right half screen
		{
		    Write_Data_R(pic[i*122+j]);
		}
	}
    return;
}