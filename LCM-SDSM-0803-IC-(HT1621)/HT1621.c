#include "ht1621.h"
#include <intrins.h>
unsigned int i,j;

void lcd_init()
{
	write_com(0x01);   //SYS EN   open the oscillator
	write_com(0x03);   //LCD ON
	write_com(0x29);   //1621: 0x29:1/3bais,1/4duty   0x30  0x18
	write_com(0x40);   //0x40:Tone frequency, 4kHz ; 0x60: Tone frequency, 2kHz
	write_com(0x09);   //Turn on tone outputs
//  write_com(0x08);   //Turn off tone outputs
//	write_com(0xe3);   //NORMAL MODE
//	write_com(0x18);   //RC OSCILLATOR

}

void lcd_clear()
{
	unsigned char add;
	add=0x40;
	for(i=0;i<44;i++)
	{		
		write_data(add,0x00);
		add++;	
	}
}

void lcd_all()
{
	unsigned char add;
	add=0x40;
	for(i=0;i<44;i++) 
	{
		write_data(add,0xff);	
		add++;
	}
}

//命令模式
void  write_com(unsigned char com)
{
	unsigned char x,y;
	LCDCS1=0;
	LCDDATA=1;
	LCDWR=1;
	_nop_();
    _nop_();
	LCDWR=0;
	for(y=0;y<2;y++)//（命令代码：100）
	{
		LCDDATA=0;
		LCDWR=1;
		_nop_();
   	 	_nop_();		
		LCDWR=0;
	}
	
	for(x=0; x<8; x++)
	{
		if(com&0x80)
			LCDDATA = 1;
		else
			LCDDATA = 0;

		LCDWR=1;	
		_nop_();
     	_nop_();	
		LCDWR=0;
		com <<= 1; //从高位到低位依次发送 8 位命令内容
	}
	LCDDATA=1;
	LCDWR=1;
	_nop_();
    _nop_();
	LCDWR=0;
	LCDCS1=1;
}


void  write_data(unsigned char add1,date)
{
	unsigned char x, y;
	LCDCS1=0;
	LCDDATA=0;
	LCDWR=1;
	_nop_();
    _nop_();
	LCDWR=0;

	for(x=0; x<8; x++)
	{
		if(add1&0x80)
			LCDDATA = 1;
		else
			LCDDATA = 0;

		LCDWR=1;
		_nop_();
     	_nop_();
		LCDWR=0;

		add1 <<= 1; 
	}

	for(y=0;y<4;y++)
	{
		if(date&0x01)
			LCDDATA = 1;
		else
			LCDDATA = 0;
		LCDWR=1;
		_nop_();
     	_nop_();
		LCDWR=0;

		date >>= 1;   
	}
	LCDCS1=1;
}


/*延时*/
void delay(unsigned int z)             
{
 int j,k;
 for(j=0;j<z;j++)
 for(k=0;k<110;k++);
}



// 段码：高4位 + 低4位
typedef struct {
    unsigned char left;
    unsigned char right;
} SegmentData;

// 数字段码表
SegmentData digits[] = {
    {0x0D, 0x0F}, // 0
    {0x00, 0x0E}, // 1
    {0x0E, 0x0B}, // 2
    {0x0A, 0x0F}, // 3
    {0x03, 0x0E}, // 4
    {0x0B, 0x0D}, // 5
    {0x0F, 0x0D}, // 6
    {0x00, 0x0F}, // 7
    {0x0F, 0x0F}, // 8
    {0x03, 0x0F}  // 9
};

void display_digit(unsigned char digit, unsigned char mode)
{
    unsigned char addr;
    unsigned char count;
    unsigned char i;
    SegmentData seg;

    if (digit > 9 || (mode != 1 && mode != 2)) {
        return;
    }

    seg = digits[digit];

    if (mode == 1) {
        addr = 0x40;
        count = 9;
        for (i = 0; i < count; i++) {
            write_data(addr,     seg.left);
            write_data(addr + 1, seg.right);
            addr += 2;
        }
    } else {
        addr = 0x52;
        count = 7;
        for (i = 0; i < count; i++) {
            write_data(addr,     seg.right);
            write_data(addr + 1, seg.left);
            addr += 2;
        }
    }
}