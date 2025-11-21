#include "LCM823160.h"
#include <REGX52.H>
#include <intrins.h>

unsigned  int count;
unsigned int i,j;

void lcd_init()
{
	write_com(0x01);    //SYS EN   open the oscillator
	write_com(0x03);    //LCD ON

	write_com(0x29);  // 1621: 0x29:1/3bais,1/4duty   0x30  0x18

	write_com(0x40);  //  0x40:Tone frequency, 4kHz ; 0x60: Tone frequency, 2kHz
	write_com(0x09);  //  Turn on tone outputs
	//write_com(0x08);  //  Turn off tone outputs

//	write_com(0xe3); //NORMAL MODE


//	write_com(0x18); //RC OSCILLATOR

}
/***************************************************************/
void lcd_clear()
{
	unsigned char add;
	add=0x40;
	for(i=0;i<50;i++)//32
	{		
		write_data(add,0x00);
		add++;	
	}
}
/***************************************************************/
void lcd_all()
{
	unsigned char add;
	add=0x40;
	for(i=0;i<50;i++) //32
	{
		write_data(add,0xff);	
		add++;
	}
}
/***************************************************************/
void display()
{
	unsigned char add;
	add=0x40;
	for(i=0;i<50;i++)//32
	{
//		write_data(0x42,0x0a);/// add 0x01 ba
//write_data(0x43,0x0b);
    	write_data(add,0x01);		//1
		delay(200);
		write_data(add,0x03);		//2
		delay(200);

		write_data(add,0x07);		 //3
		delay(200);

		write_data(add,0x0f);		//4
		delay(200);
  /*
		write_data(add,0x1f);		 //5	ht1622
		delay(200);

		write_data(add,0x3f);		//6
		delay(200);

		write_data(add,0x7f);		//7
		delay(200);

		write_data(add,0xff);	 	 //8
		delay(200);	
	*/	 	
		add++;
	}
} 
/***************************************************************/
void  write_com(unsigned char com)
{
	unsigned char x,y;
	LCDCS1=0;
	LCDDATA=1;
	LCDWR=0;
	_nop_();
    _nop_();
	LCDWR=1;
	for(y=0;y<2;y++)
	{
		LCDDATA=0;
		LCDWR=0;
		_nop_();
   	 	_nop_();		
		LCDWR=1;
	}
	for(x=0; x<8; x++)
	{
		if(com&0x80)
			LCDDATA = 1;
		else
			LCDDATA = 0;

		LCDWR=0;	
		_nop_();
     	_nop_();	
		LCDWR=1;
		com <<= 1; 
	}
	LCDDATA=1;
	LCDWR=0;
	_nop_();
    _nop_();
	LCDWR=1;
	LCDCS1=1;
}
/***************************************************************/
void  write_data(unsigned char add1,date)
{
	unsigned char x, y;
	LCDCS1=0;
	LCDDATA=1;
	LCDWR=0;
	_nop_();
    _nop_();
	LCDWR=1;

	for(x=0; x<8; x++)
	{
		if(add1&0x80)
			LCDDATA = 1;
		else
			LCDDATA = 0;

		LCDWR=0;
		_nop_();
     	_nop_();
		LCDWR=1;

		add1 <<= 1; 
	}
	for(y=0;y<4;y++)
	{
		if(date&0x01)
			LCDDATA = 1;
		else
			LCDDATA = 0;
		LCDWR=0;
		_nop_();
     	_nop_();
		LCDWR=1;

		date >>= 1;   
	}
	LCDCS1=1;
}
/***************************************************************/

/*延时*/
void delay(unsigned int z)             
{
 int j,k;
 for(j=0;j<z;j++)
 for(k=0;k<110;k++);
}



unsigned char digits[][20] = {
	
    // 数字 0：从0x40到0x5a，写入左0x0A，右0x0F
    {0x40, 0x5a, 0x0A, 0x0F, 0xFF},

    // 数字 1
    {0x40, 0x5a, 0x00, 0x06, 0xFF},

    // 数字 2：分两段
    {0x40, 0x4a, 0x0C, 0x0B, 0x4f, 0x5d, 0x0D, 0x06, 0xFF},

    // 数字 3
    {0x40, 0x4a, 0x04, 0x0F, 0x4f, 0x5d, 0x0F, 0x04, 0xFF},

    // 数字 4
    {0x40, 0x4a, 0x06, 0x06, 0x4f, 0x5d, 0x06, 0x0C, 0xFF},

    // 数字 5
    {0x40, 0x4a, 0x06, 0x0D, 0x4f, 0x5d, 0x0B, 0x0C, 0xFF},

    // 数字 6
    {0x40, 0x4a, 0x0E, 0x0D, 0x4f, 0x5d, 0x0B, 0x0E, 0xFF},

    // 数字 7
    {0x40, 0x4a, 0x00, 0x07, 0x4f, 0x5d, 0x0E, 0x00, 0xFF},

    // 数字 8
    {0x40, 0x4a, 0x0E, 0x0F, 0x4f, 0x5d, 0x0F, 0x0E, 0xFF},

    // 数字 9
    {0x40, 0x4a, 0x06, 0x07, 0x4f, 0x5d, 0x0E, 0x0C, 0xFF}
};


void display_digits_simple()
{
    unsigned char addr;
    int i, j;

    for (i = 0; i < 10; i++)  // 显示0~9
    {
        j = 0;
        while (digits[i][j] != 0xFF)  // 以 0xFF 结尾
        {
            unsigned char start = digits[i][j++];
            unsigned char end   = digits[i][j++];
            unsigned char lbyte = digits[i][j++];
            unsigned char rbyte = digits[i][j++];

            for (addr = start; addr <= end; addr += 2)
            {
                write_data(addr,     lbyte);
                write_data(addr + 1, rbyte);
            }
        }
        delay(2000);
    }
}
