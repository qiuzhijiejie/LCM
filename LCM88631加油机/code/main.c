/*------------------------------------------------------------------*/
/* --- STC MCU Limited ---------------------------------------------*/
/* --- STC89-90xx Series 16-bit Timer Demo -------------------------*/
/* --- Mobile: (86)13922805190 -------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ---------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966----------------------*/
/* --- Web: www.STCMCU.com -----------------------------------------*/
/* --- Web: www.GXWMCU.com -----------------------------------------*/
/* If you want to use the program or the program referenced in the  */
/* article, please specify in which data and procedures from STC    */
/*------------------------------------------------------------------*/


#include "reg51.h"
#include "main.h"
#include "lcd.h"

//-----------------------------------------------
#define NONE_PARITY     0   //None parity
#define ODD_PARITY      1   //Odd parity
#define EVEN_PARITY     2   //Even parity
#define MARK_PARITY     3   //Mark parity
#define SPACE_PARITY    4   //Space parity
//-----------------------------------------------

#define FOSC 11059200L			//晶振频率定义
#define BAUD 9600              //串口波特率设定
#define T1MS 0xFC66   //定时1ms中断一次
#define T2MS 0xF8CD   //定时2ms中断一次
#define T5MS 0xEE00   //定时5ms中断一次
#define T10MS 0xDC00  //定时10ms中断一次


#define KEY_UP 		1		//按键定义
#define KEY_DOWN	2
#define KEY_RESET   3		//按键定义
#define KEY_FULL	4

sbit KEY0 = P0^4;			//按键0
sbit KEY1 = P0^5;			//按键1
sbit KEY2 = P0^6;			//按键2
sbit KEY3 = P0^7;			//按键3

//-----------------------------------------------
static u8 Freq=50,Cycle=5,Count_time=5;	//控制刷新频率
u16 time_dely;
u8 i,j;
u8 rev_mod=0;
//-----------------------------------------------
void timer0_isr() interrupt 1	//定时器0中断函数
{
	
    TL0 = T1MS;                     //reload timer0 low byte
    TH0 = T1MS >> 8;                //reload timer0 high byte

    if(time_dely)time_dely--;

    if(Count_time)
    {
	    Count_time--;
	    if(Count_time==0)
	    {
	    	Count_time=Cycle;
	    	
				if(LCD_POLARITY==0)	//极性反转
					LCD_POLARITY=1;
				else
					LCD_POLARITY=0;
				
				if(LCD_POLARITY==0)	//极性判断
				{
					LCD_Update_CGRAM(1);	//正输出
					LCD_Delay(1);
					ST_CP=1;		//com口输出
				}
				else
				{
					LCD_Update_CGRAM(0);	//反向输出
					LCD_Delay(1);
					ST_CP=0;		//com口输出
				}
			}
		}
		
}
//-----------------------------------------------

void Uart_Int_Isr() interrupt 4
{
	if (RI)
	{
		RI = 0;             //Clear receive interrupt flag
		rev_mod = SBUF;          //P0 show UART data
		//LCD_Set_All(0,LCD_Max,rev_mod);
	}
}
//-----------------------------------------------
void delay_us(u16 dely)
{
	while(dely--);	
}
//-----------------------------------------------
void delay_ms(u16 dely)
{
	while(dely--)
	{
		delay_us(66);
	}
}
//-----------------------------------------------
code u8 DBHEX[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};


void Uart_Init(void)	//串口初始化
{
    SCON = 0x50;            //8-bit variable UART
    TMOD |= 0x20;            //Set Timer1 as 8-bit auto reload mode
    TH1 = TL1 = -(FOSC/12/32/BAUD); //Set auto-reload vaule
    TR1 = 1;                //Timer1 start run
    ES = 1;                 //Enable UART interrupt
}

//-----------------------------------------------
void Uart_Send_Byte(u8 dat)
{
	SBUF = dat;
	while(TI==0);
	TI=0;	
}

void Uart_Send_Hex(u8 Byte_HEX)
{
	Uart_Send_Byte(DBHEX[Byte_HEX/16]);
	Uart_Send_Byte(DBHEX[Byte_HEX%16]);
}


void Uart_Send_IntHex(u16 Byte_HEX)
{
	Uart_Send_Byte(DBHEX[(Byte_HEX>>8)/16]);
	Uart_Send_Byte(DBHEX[(Byte_HEX>>8)%16]);
	Uart_Send_Byte(DBHEX[(Byte_HEX&0xff)/16]);
	Uart_Send_Byte(DBHEX[(Byte_HEX&0xff)%16]);
}


void Uart_Send_Dec(u8 Byte_Dec)
{
	Uart_Send_Byte(DBHEX[(Byte_Dec/100)%10]);
	Uart_Send_Byte(DBHEX[(Byte_Dec/10)%10]);
	Uart_Send_Byte(DBHEX[(Byte_Dec/1)%10]);
}


void Uart_Send_IntDec(u16 Int_dec)
{
	Uart_Send_Byte(DBHEX[(Int_dec/10000)%10]);
	Uart_Send_Byte(DBHEX[(Int_dec/1000)%10]);
	Uart_Send_Byte(DBHEX[(Int_dec/100)%10]);
	Uart_Send_Byte(DBHEX[(Int_dec/10)%10]);
	Uart_Send_Byte(DBHEX[(Int_dec/1)%10]);
}



void Uart_dbg_print(char *str)
{
	while(*str)
	{
		Uart_Send_Byte(*str++);
	}
}

//-----------------------------------------------
void Timer_Init(void)	//定时器初始化
{
	  TMOD |= 0x01;                    //set timer0 as mode1 (16-bit)
    TL0 = T1MS;                     //initial timer0 low byte
    TH0 = T1MS >> 8;                //initial timer0 high byte
    TR0 = 1;                        //timer0 start running
    ET0 = 1;                        //enable timer0 interrupt
}
//-----------------------------------------------
void KEY_Init(void)		//按键初始化
{ 
 	KEY0 = 1;
	KEY1 = 1;
 	KEY2 = 1;
	KEY3 = 1;

}
//-----------------------------------------------

u8 KEY_Scan(u8 mode)		//按键扫描函数
{	 
	static u8 key_up=1;
	if(mode)key_up=1;  
	if(key_up&&(KEY0==0||KEY1==0||KEY2==0||KEY3==0))
	{
		delay_ms(10);
		key_up=0;
		if(KEY0==0)return KEY_UP;
		else if(KEY1==0)return KEY_DOWN;
		else if(KEY2==0)return KEY_RESET;
		else if(KEY3==0)return KEY_FULL;
	}else if(KEY0==1&&KEY1==1&&KEY2==1&&KEY3==1)key_up=1; 	    
 	return 0;
}
//-------------------------------------------------
void Soft_Init(void)
{
	Freq=60;
	Cycle = 500/Freq;
	Count_time=Cycle;	//控制刷新频率
}
//-----------------------------------------------

void Hard_Init(void)
{
	KEY_Init();
	LCD_Port_Init();
	
}

//-----------------------------------------------
void main(void)	//主函数
{
	static u8 key,index=0;
	EA = 0;				//上电关闭中断
	delay_ms(10);
	Hard_Init();	//IO初始化
	Uart_Init();
	Timer_Init();	//定时器初始化
	Soft_Init();	//变量初始化
	Uart_dbg_print("system is init!\r\n");	//向串口打印信息
    EA = 1;				//打开中断

  while (1)
	{
		//---按键扫描和处理------------------------------------------
		key = KEY_Scan(0);
		if(key==KEY_UP)
		{
			if(Freq<200) Freq++;
			Cycle = 500/Freq;
			Uart_dbg_print("[KEY_UP] Freq= ");Uart_Send_Dec(Freq);
			Uart_dbg_print(" Cycle= ");Uart_Send_Dec(Cycle);
			Uart_dbg_print("\r\n");
		}
		else if(key==KEY_DOWN)
		{
			if(Freq>20) Freq--;
			Cycle = 500/Freq;
			Uart_dbg_print("[KEY_DN] Freq= ");Uart_Send_Dec(Freq);
			Uart_dbg_print(" Cycle= ");Uart_Send_Dec(Cycle);
			Uart_dbg_print("\r\n");
		}
		else if(key==KEY_RESET)
		{
			Freq=50;
			Cycle = 500/Freq;
			Uart_dbg_print("[KEY_RESET] Freq= ");Uart_Send_Dec(Freq);
			Uart_dbg_print(" Cycle= ");Uart_Send_Dec(Cycle);
			Uart_dbg_print("\r\n");
		}
		else if(key==KEY_FULL)
		{
			Freq=150;
			Cycle = 500/Freq;
			Uart_dbg_print("[KEY_FULL] Freq= ");Uart_Send_Dec(Freq);
			Uart_dbg_print(" Cycle= ");Uart_Send_Dec(Cycle);
			Uart_dbg_print("\r\n");
		}		

		if(time_dely==0)	//定时变化数据
		{
			time_dely=1000;
			
			index++;
			if(index>22)
			{
				LCD_Clear_All();//显示一轮后要全显
				index = 0;				
			} 
			else
			{
				//留三个位来显示频率
				CGRAM[2]=leddata10[(Freq/100)%10];
				CGRAM[1]=leddata10[(Freq/10)%10];
				CGRAM[0]=leddata10[Freq%10];
				//其余均按顺序显示
				LCD_Set_All(1,LCD_Max,leddata10[index]);
			}
		}
	}
}