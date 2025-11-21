
//CONTROL IC UC1671C
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
#include <reg52.h>
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
#define uchar unsigned char
#define uint  unsigned int
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
sbit SDA=P3^5;
sbit SCL=P3^6;
sbit RST=P3^4;

//uchar code ch2[]={0x1d,0xbd,0xbd,0xbd,0xbd,0xb6,0x09,0xb6,0x09,0xb6,0x09,0xb6,0x09,0xb6,0x09,0xbd,0x90,0x3d,0x90,0x3d,0x90,0x3d,0x90,0x3d,0x90,0x30,0x10,0x1d,0x90,0x3d,0x90,0x3d,0x90,0x3d,0x90,0x3d,0x90,0x36,0x09,0xb6,0x09,0xb6,0x09,0xb6,0x09,0xb6,0x09,0xb1,0x01,0x10,0x01,0x60,0x9b,0x60,0x9b,0x60,0x9b,0x60,0x9b,};
uchar code ch2[]={0xbd,0xbd,0xbd,0xbd,0xb6,0x09,0xb6,0x09,0xb6,0x09,0xb6,0x09,0xb6,0x09,0xbd,0x90,0x3d,0x90,0x3d,0x90,0x3d,0x90,0x3d,0x90,0x30,0x10,0x1d,0x90,0x3d,0x90,0x3d,0x90,0x3d,0x90,0x3d,0x90,0x36,0x09,0xb6,0x09,0xb6,0x09,0xb6,0x09,0xb6,0x09,0xb1,0x01,0x10,0x01,0x60,0x9b,0x60,0x9b,0x60,0x9b,0x60,0x9b,};
uchar code ch4[]={0x06,0xd6,0xd6,0xd6,0xd6,0xd3,0x09,0x63,0x09,0x63,0x09,0x63,0x09,0x63,0x09,0x66,0x90,0x66,0x90,0x66,0x90,0x66,0x90,0x66,0x90,0x61,0x01,0x06,0x90,0x66,0x90,0x66,0x90,0x66,0x90,0x66,0x90,0x63,0x09,0x63,0x09,0x63,0x09,0x63,0x09,0x63,0x09,0x60,0x10,0x10,0x11,0x30,0x96,0x30,0x96,0x30,0x96,0x30,0x96,};
uchar code ch5[]={0x1f,0x4f,0x4f,0x4f,0x4f,0x42,0xf6,0xa2,0xf6,0xa2,0xf6,0xa2,0xf6,0xa2,0xf6,0xa5,0x6f,0x25,0x6f,0x25,0x6f,0x25,0x6f,0x25,0x6f,0x21,0x00,0x15,0x6f,0x25,0x6f,0x25,0x6f,0x25,0x6f,0x25,0x6f,0x22,0xf6,0xa2,0xf6,0xa2,0xf6,0xa2,0xf6,0xa2,0xf6,0xa1,0x01,0x01,0x11,0x2f,0x6a,0x2f,0x6a,0x6f,0x2a,0x2f,0x6a,};
uchar code  on[]={0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,};
uchar code off[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,};
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
void i2c_start(void);
void i2c_write(uchar m);
void i2c_read(void);
void i2c_stop(void);

#define AUTO 0x80		//P2^7: Input auto_run_id
#define ADDU 0x40		//P2^6: Display picture UP to end
#define SUBD 0x20		//P2^5: Display picture Down to begin
#define ADUP 0x10		//P2^4: For COG module:adjust Register's value to add
#define ADDN 0x08		//P2^3: For COG moduel:adjust Register's value to sub
#define KEY_IN	P2
#define v0		52 //  30VLCD 8.83

uchar constr;		// ???   
#define BmpMax 5
uchar autoid;	//Indicate that LCD display automatically
uchar bmp_id;	//The picture's id to display
uchar dispid;	//To reflash the display picture

void nop(void);

void delay_n_ms(uint n);
void delay100us(void);
void delay10us(void);
void lcd_inital(void);
void display_all(void);
void display_off(void);
void display_icon(void);

void display_all_on(void);
#define dl0 1000
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
void nop()
{
}
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
void delay_n_ms(uint n)
{
      uint i=0,j;
      j=n;
      for(j=n;j>0;j--)
      {
            for(i=0;i<125;i++)
			{
			}
      }
         
}
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
void delay10us()
{
      char i;
	  for(i=1;i>0;i--)
	  {
	  }
}
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$


void IC_Start(void)
{ 
      delay_n_ms(1);
      SDA=1;
	  SCL=1;
	  delay10us();
	  SDA=0;
	  delay10us();
	  SCL=0;				
}

void IC_Stop(void)
{
    SDA=0;
      delay10us();
      SCL=1;
      delay10us();
      SDA=1; 
			
}

////-------------------
void F_Serial_WR_I2C(char R_Data_CMD)
{	
	char i;
	for(i=0;i<8;i++)
	{		
	    SCL=0;
		
		if(R_Data_CMD&0x80)		
			SDA=1;
		else
			SDA=0;
		R_Data_CMD<<=1;

         nop();
           nop();
           SCL=1;
		   nop();
 		   nop();
           SCL=0;		
	}	
          nop();
      nop();
      SCL=0;
      delay10us();
      SCL=1;
      nop();
      nop();   
      nop();
      while(SDA==1);
	  SCL=0;
}	

void F_WriteInst(char R_CMD)
{		
	
		IC_Start();
		F_Serial_WR_I2C(0xa8);
									
		F_Serial_WR_I2C(R_CMD);
					
}
										
void F_WriteData(char R_Data)	//--stop
{				

		F_Serial_WR_I2C(R_Data);
		IC_Stop();		

}

void F_WriteData_R(char R_Data)	//no stop
{				

		F_Serial_WR_I2C(R_Data);

}

//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//void lcd_inital(void)
//{
//constr=v0;
//       RST=0;
//     delay_n_ms(30);
//      RST=1;
//     delay_n_ms(20);
// //---------------------------------	
//        F_WriteInst(0x50);		//soft reset 
//		 delay_n_ms(5);				//5ms

//		F_WriteInst(0x51);		//device selcet
//		F_WriteData(0x00);	
//	
//		F_WriteInst(0x52);		//
//		F_WriteData(0x01);		//0x01 80HZ /0x03 120HZ //0X04 160HZ

//		F_WriteInst(0x53);		//Pump control
//		F_WriteData(0x01);		//PUMP enabled, 0X01 internal VLCD generation
//		F_WriteInst(0x54);		//PUMP VOLTAGE SET
//		F_WriteData(52);	    //VLCD = 2.5 + VSET * 0.05 5.0V
//		F_WriteInst(0x55);		//Display mode
//		F_WriteData(0x01);		///1/3 bias
//		
//		
//		
//		    // 开启 COM/SEG 输出
//    F_WriteInst(0x56);		
//    F_WriteData(0x00);	    // 起始地址归零

//    // 打开显示输出
//    F_WriteInst(0x59);		
//    F_WriteData(0x01);	    // Display ON（1=打开显示输出）

//    // 或者部分版本是：
//    // F_WriteInst(0x5A);
//    // F_WriteData(0x01);

//    // 设置全显测试（可选）
//    F_WriteInst(0x58);
//    F_WriteData(0x01);      // 全显开启测试用
//    delay_n_ms(500);
//    F_WriteData(0x00);      // 再关闭全显，恢复正常显示

//} 


void display_all_on(void)
{
    uchar i;

    F_WriteInst(0x51);      // Device select
    F_WriteData(0x00);

    F_WriteInst(0x56);      // Address pointer
    F_WriteData_R(0x00);    // 从 SEG0 开始

    for(i = 0; i < 120; i++)   // 全屏段码写满
    {
        F_WriteData_R(0xFF);
    }
    IC_Stop();
}

////$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
void display_d(uchar *ptr)
{
		uchar i;
		F_WriteInst(0x51);	// device
		F_WriteData(0x00);
	
		F_WriteInst(0x56);	// ADDRESS
		F_WriteData_R(0x00);// seg 0	
			
		for(i=0;i<59;i++)
		{
		 F_WriteData_R(*ptr);  
		  ptr++;	
		} 
      IC_Stop();
}


void lcd_inital(void)
{
    constr = 20;   // 对应 VLCD 电压调整
    RST = 0;
    delay_n_ms(30);
    RST = 1;
    delay_n_ms(20);

    // 软件复位
    F_WriteInst(0x50);
    delay_n_ms(10);

    // 设备选择
    F_WriteInst(0x51);
    F_WriteData(0x00);

    // 帧率设置（80Hz）
    F_WriteInst(0x52);
    F_WriteData(0x01);

    // Pump 控制
    F_WriteInst(0x53);
    F_WriteData(0x01);  // 内部VLCD启用

    // Pump电压设置
    F_WriteInst(0x54);
    F_WriteData(constr); // 可调亮度（2.5 + VSET*0.05）

    // 显示模式设置（1/4 Duty, 1/3 Bias 通常更亮）
    F_WriteInst(0x55);
    F_WriteData(0x03);  // 尝试改成0x03看看亮不亮

    delay_n_ms(50);

    // 地址初始化
    F_WriteInst(0x56);
    F_WriteData(0x00);

    // 打开显示输出
    F_WriteInst(0x59);
    F_WriteData(0x01);  // Display ON

    delay_n_ms(100);

    // 全显测试（可选，测试所有段是否通）
    F_WriteInst(0x58);
    F_WriteData(0x01);
    delay_n_ms(300);
    F_WriteData(0x00);  // 关闭全显
}

///////////////////////////////////////////
void display_on()
{
		uchar i;
		F_WriteInst(0x51);	// device
		F_WriteData(0x00);
	
		F_WriteInst(0x56);	// ADDRESS
		F_WriteData_R(0x00);// seg 0	
		
		for(i=0;i<117;i++)
		{
			 F_WriteData_R(0xff);  
		} 
		IC_Stop();
}
//-----------------------------------------------------------------------------
void display_off()
{
		uchar i;
		F_WriteInst(0x51);	// device
		F_WriteData(0x00);
	
		F_WriteInst(0x56);	// ADDRESS
		F_WriteData_R(0x00);// seg 0	
		
		for(i=0;i<59;i++)
		{
			 F_WriteData_R(0x00);  
		} 
		IC_Stop();
}
//-----------------------------------------------------------------------------
void do_key()
{
	uchar tkeyin;

	KEY_IN=0xff;
	delay_n_ms(15);
	tkeyin=KEY_IN;

	// ж  ? ? ± ?
	if((tkeyin&AUTO)==0x00)	autoid=1;
	else autoid=0;

	//       · bmp_id++
	if((tkeyin&ADDU)==0x00)
	{
	 
		dispid=1;		//? ± ?  "1"		
		bmp_id++;
		if(bmp_id>=BmpMax) 
			bmp_id=0;
	}
		
	//       ? bmp_id--
	if((tkeyin&SUBD)==0x00)
	{
		dispid=1;		//? ± ?  "1"
		if(bmp_id>0) 
		{
			bmp_id--;
		}
		else
 	    {
		bmp_id=BmpMax-1;
		}
 
	}

			
	// ???   constr++
	if((tkeyin&ADUP)==0x00)
	{
		if(constr<63) constr++;
		F_WriteInst(0x54);		//PUMP VOLTAGE SET
		F_WriteData(constr);	////VLCD = 2.5 + VSET * 0.05 5.0V
	}
	// ???   constr--
	if((tkeyin&ADDN)==0x00)
	{
		if(constr>0) constr--;
		F_WriteInst(0x54);		//PUMP VOLTAGE SET
		F_WriteData(constr);	////VLCD = 2.5 + VSET * 0.05 5.0V
	}
			
	delay_n_ms(150);
}
////////////////////////////////////////
//void main( void )   
//{
//      P1=0xff;
//      P3=0xff;
//      lcd_inital();
//      delay10us();
//      do
//      {
//        display_on();
//		delay_n_ms(5000);;
//			 display_d(on);
//	         delay_n_ms(500);		
//			 display_d(off);
//			 delay_n_ms(500);       
//			 display_d(on);
//			 delay_n_ms(500);

//      }while(1); 
//}


void main (void)  
{
    autoid=1;
    bmp_id=0;
	dispid=1;
      P1=0xff;
      P3=0xff;
      lcd_inital();
      delay10us();

//	  display_d(off);

//while(1);
display_d(ch2);
delay_n_ms(99999999999999999);
//display_d(ch4);
//delay_n_ms(10000);
//display_d(ch5);
//delay_n_ms(10000);
//display_d(on);
//delay_n_ms(10000);	
//	while(1)
//	{
//		do_key();
//		if(dispid==1)
//		{
//			switch(bmp_id)
//			{
//				case 0:			//? ?   
//					display_on();
//					break;
//			    case 1:			//   ?   
//					display_off();
//			 		break;
//				case 2:			//   ?   1
//					display_d(ch2);
//			 		break;
//				case 3:			//   ?   2
//				    display_d(ch4);
//				    break;
//				case 4:			//   ?   2
//				    display_d(ch5);
//				    break;		
//				default:
//					break;
//			}
//			dispid=0;			//   ? ± ?
//		}

//		//For auto display switch
//		if(autoid==1) 
//		{
//			dispid=1;
//			bmp_id++;
//			if(bmp_id>=BmpMax) bmp_id=0;

//				 delay_n_ms(dl0);	// ?   ??  ?dl0

//		}
//	}
}
