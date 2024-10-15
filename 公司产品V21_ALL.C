/*
晶振11.0592hz  模块通用显示程序
PCB编号V21版本；
高生于2018/12/20 19:33:13
使用多个品牌-MCU;0xFFFF=64K存储空间；
于深圳公明,深圳市众博达科技有限公司；
;          		        -------------------------
;  DB0=          	1		|P1.0=T2       	     VCC_|	40	      		VDD
;  DB1=          	2		|P2.1=T2EX    	 AD0=P0.0|	39	      		=A0//74LS164-DOUT
;  DB2=         	3		|P1.2	           AD1=P0.1|	38            =A1//74LS164-CLK
;  DB3=         	4		|P1.3	       		 AD2=P0.2|	37            =UP
;  DB4=         	5		|P1.4	       		 AD3=P0.3|	36            =KOR
;  DB5=         	6		|P1.5=MOSI			 AD4=P0.4|	35            =SCL
;  DB6=  scl     	7		|P1.6=MISO		   AD5=P0.5|	34            =SDA
;  DB7=  sda     	8		|P1.7=SCK				 AD6=P0.6|	33            =KOG
;  REH=          	9		|RST             AD7=P0.7|	32            =KOB
;  CS1=         	10	|P3.0=RXD 	       EA/VPP|	31            ====
;  CS2=          	11	|P3.1=TXD       ALE/PROG_|	30            ====
;  KEY=          	12	|P3.2=INT0	        /PSEN|	29            ====
;  SET=          	13	|P3.3=INT1	     A15=P2.7|	28            DB15
;  D/I=         	14	|P3.4=T0         A14=P2.6|	27            DB14
;  R/W=         	15	|P3.5=T1         A13=P2.5|	26            DB13
;  CS3          	16	|P3.6=wrr	       A12=P2.4|	25            DB12
;  EN/DATA=      	17	|P3.7=RD	       A11=P2.3|	24            DB11
;  ====          	18	|OSC0            A10=P2.2|	23            DB10
;  ====          	19	|OSC1	            A9=P2.1|	22            DB9
;  ====         	20	|GND	            A8=P2.0|	21            DB8
;                     -------------------------
;
*/



//更换MCU，需要更改的地方:******************************************************
//1-头文件
//2-延时时间参数
//3-IO口设置
//4-Eeprom子程序
//5-数码管的驱动程序




//******************************************************************************
//#include <STC90CXXRD.h>
#include <STC12C5A60S2.h>
//******************************************************************************


#include <absacc.h>
#include <intrins.h>
#include <math.h>



#define uchar 	unsigned char
#define uint 		unsigned int


#define u8 			unsigned char
#define u16 		unsigned int

#include <Data_Segment.h>

#include <Data_matrix.h>




//常用参数设置



///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
uchar module_num;							//模块大分类，整体系列
uchar code time_max=15;			//模块大分类数量
uchar code time_min=1;
uchar program_num;	 					//模块小分类，系列下属
uchar program_max;	 					//模块小分类最大下属数
uchar PCF8576DUTY;
uchar PCF8576DUTY;						//PCF8576的duty参数值
uchar BITNUM;									//SPLC063静态的段数
uchar Charater_n_Line;				//S6A0069的显示扫面1/8或1/16;
uchar HT1621_num;  						//需要测试的HT1621的个数
uchar DUTYBIAS;   						//HT1621的Duty&Bias设置参数
uchar LCDNUM;									//HT1621系列SEG段数=LCDNUM*2
uchar RCXT;										//HT1621的震荡方式
uchar bdata 		spp;
sbit mm    		=	spp^0;  			//用于单步停止用
sbit BIT_TMP	=	spp^1;
sbit BL_G  		=	spp^2;
sbit BL_B  		=	spp^3;



sbit bflag=ACC^7;  		 				//用于忙信号检测用
uchar delayTime;          		//延时时间选择
uchar xdata htdata[32]; 			//HT1621暂存数据
uchar IIC_data[16];
uint code xunhuan=999999;			//测试循环次数






//******************************************************************************
#include "IO_V21_STC12C5A60S2.h"
//******************************************************************************




#define 	ce8  				rs						//HT1621_NUM=片选0

#define 	CSB  				cs1						//显示IC的片选
#define 	PSB  				cs1						//显示IC的并口串口选择
#define 	en2  				cs1						//显示IC的片选2
#define 	RST  				cs2						//显示IC的复位端口
#define 	en3  				cs3						//显示IC的片选2

#define 	HT_CS0			ce8
#define 	HT_CS1 			ce7
#define 	HT_CS2 			ce6
#define 	HT_CS3 			ce5
#define 	HT_CS4 			ce4
#define 	HT_CS5 			ce3
#define 	HT_CS6 			ce2
#define 	HT_CS7 			ce1
#define 	HT_CS8 			ce0

#define 	HT_CLK 			wrr
#define 	HT_SDA 			en
#define 	LOOK_063		rs

#define 	RW1067_SID 	ce0						//SPI接口方式的数据端口
#define 	RW1067_SCK 	ce1						//SPI接口方式的时钟端口
#define 	RW1067_CSB 	ce2						//SPI接口方式的片选端口


#define 	IIC2SCL			wrr						//PCF8576时钟
#define 	IIC2SDA			rs						//PCF8576数据


#define 	HT1616SetDat()  SER_DAT=1
#define 	HT1616ClrDat()  SER_DAT=0

#define 	TM1617ClrClk() 	SER_CLK=0
#define 	TM1617SetClk() 	SER_CLK=1

#define 	TM1617ClrCS()  	SER_CS1=0
#define 	TM1617SetCS()  	SER_CS1=1

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#define SomeNOP(); _nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();



///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


//******************************************************************************
uchar code Count=18;		//延时参数由  Count 决定多少的数值,
//Crystal oscillator
//------------------------------------------------
//  12MHz /	12T /	 	 1	us 	/count 1   =AT89C55W
//------------------------------------------------
//  12MHz /	 6T /	 0.5	us 	/count 2   =N78E517A
//------------------------------------------------
//  12MHz /	 1T /	 0.083us 	/count 12  =8A8K64S4
//------------------------------------------------
//  18MHz /	 1T /	 0.055us 	/count 18  =N76E003A
//------------------------------------------------
#include "Delay.h"
//******************************************************************************







//******************************************************************************
//#include "Eeprom_STC90C58RD.h"
#include "Eeprom_STC12C5A60S2.h"
//#include "Eeprom_24C16.h"
//#include "Eeprom_24C512.h"
//******************************************************************************





//******************************************************************************
//#include "DisLED_TM1617.h"
//#include "DisLED_TM1616.h"
#include "DisLED_74LS164.h"
//******************************************************************************




//*****以下可以直接替换********************************************************
//*****************************************************************************
//*****************************************************************************
//*****************************************************************************
//*****************************************************************************

//写指令=S6B0108
void wi_6800a(uchar x){
  //busy6800a();
  						dly();
  rs=0; 			dly();//_nop_();
  wrr=0;			dly();//_nop_();
	P1=x; 			dly();//_nop_();
  en=1;
   						dly();//_nop_();
	en=0;				_nop_();
  }
//写数据=S6B0108
void wd_6800a(uchar x){
  //busy6800a();
  						dly();
  rs=1; 			dly();//_nop_();
  wrr=0;			dly();//_nop_();
	P1=x; 			dly();//_nop_();
  en=1;
   						dly();//_nop_();
	en=0;				_nop_();
  }

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//写指令=第二个6800指令的IC，以cs1作为E2信号
void wi_6800b(uchar x){
  //busy6800b();
  						dly();
  rs=0; 			dly();//_nop_();
  wrr=0;			dly();//_nop_();
	P1=x; 			dly();//_nop_();
  en2=1;
   						dly();//_nop_();
	en2=0;				_nop_();
  }
//写数据=第二个6800指令的IC，以cs1作为E2信号
void wd_6800b(uchar x){
  //busy6800b();
  						dly();
  rs=1;				dly();//_nop_();
  wrr=0;			dly();//_nop_();
	P1=x; 			dly();//_nop_();
  en2=1;
   						dly();//_nop_();
	en2=0;				_nop_();
  }

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//写指令=st7920并口
void wi_6800c(uchar x) {
  						dly();
  rs=0; 			dly();//_nop_();
	wrr=0;	 		dly();//_nop_();
	P1=x; 			dly();//_nop_();
	en=1;
  						dly();//_nop_();
	en=0;				_nop_();
  	}

//写数据=st7920并口
void wd_6800c(uchar x) {
  						dly();
  rs=1; 			dly();//_nop_();
	wrr=0; 			dly();//_nop_();
	P1=x; 			dly();//_nop_();
	en=1;
  						dly();//_nop_();
	en=0;				_nop_();
	}


//写指令=st7920并口
void wi_6800d(uchar x) {
   						dly();
  rs=0; 			dly();//_nop_();
	wrr=0; 			dly();//_nop_();
	P1=x; 			dly();//_nop_();
	en3=1;
  						dly();//_nop_();
	en3=0;			_nop_();
  	}

//写数据=st7920并口
void wd_6800d(uchar x) {
   						dly();
  rs=1; 			dly();//_nop_();
	wrr=0; 			dly();//_nop_();
	P1=x; 			dly();//_nop_();
	en3=1;
  						dly();//_nop_();
	en3=0;			_nop_();
	}


//写指令4bit=S6A0069
void wi_6800a_4bit(uchar x){
  									dly();
  rs=0;							dly();//_nop_();
  wrr=0;						dly();//_nop_();
	P1=x&0xf0;				dly();//_nop_();
  en=1;
										dly();//_nop_();
	en=0;

   									dly();
  rs=0;							dly();//_nop_();
  wrr=0;						dly();//_nop_();
	P1=(x<<4)&0xf0;		dly();//_nop_();
  en=1;
         						dly();//_nop_();
	en=0;
  }

 //写数据4bit=S6A0069
void wd_6800a_4bit(uchar x){
   									dly();
  rs=1;							dly();//_nop_();
  wrr=0;						dly();//_nop_();
	P1=x&0xf0;				dly();//_nop_();
  en=1;
                    dly();//_nop_();
	en=0;

  dly();            dly();
  rs=1;							dly();//_nop_();
  wrr=0;						dly();//_nop_();
	P1=(x<<4)&0xf0;		dly();//_nop_();
  en=1;
  	                dly();//_nop_();
	en=0;
 }


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// RW1067_SID ce0	=P1.7
// RW1067_SCK ce1	=P1.6
// RW1067_CSB ce2	=P1.5

//SPI_Serial模式
void high_serialRW1067(void){
	RW1067_SCK=0;   dly();
	RW1067_SID=1;   dly();
	RW1067_SCK=1;   dly();
	}

void low_serialRW1067(void){
	RW1067_SCK=0;   dly();
	RW1067_SID=0;   dly();
	RW1067_SCK=1;   dly();
  }

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/*
sbit 		ce0	=P1^7;  					//HT1621_NUM=片选8

sbit 		ce1	=P1^6;  					//HT1621_NUM=片选1
sbit 		ce2	=P1^5;  					//HT1621_NUM=片选2
sbit 		ce3	=P1^4;  					//HT1621_NUM=片选3
sbit 		ce4	=P1^3;  					//HT1621_NUM=片选4
sbit 		ce5	=P1^2;  					//HT1621_NUM=片选5
sbit 		ce6	=P1^1;  					//HT1621_NUM=片选6
sbit 		ce7	=P1^0;  					//HT1621_NUM=片选7


*/


//写指令SPI=4LINE
//写指令SPI
void wi_serial_RW1067(uchar x) {
 uchar i;
  RW1067_CSB=0;
	 ce7=0;ce6=0;ce5=0;ce4=0;ce3=0;wrr=0;en=0;
	 rs=0;
	 //dly();

	 for (i=0;i<8;i++) {
    if (x>=0x80) high_serialRW1067();
     else low_serialRW1067();
    x=x<<1;
   }
  RW1067_CSB=1;
}

//写数据=4LINE
//写数据
void wd_serial_RW1067(uchar x) {
 uchar i;
  RW1067_CSB=0;
	 ce7=0;ce6=0;ce5=0;ce4=0;ce3=0;wrr=0;en=0;
   rs=1;
   dly();

	 for (i=0;i<8;i++) {
    if (x>=0x80) high_serialRW1067();
     else low_serialRW1067();
    x=x<<1;
   }
   RW1067_CSB=1;
}



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//写CGRAM ICON
void wcgram_0069a(void) {
 uchar n,i;
  wi_6800a(0x40);		//写CGRAM ICON
  for(i=0;i<7;i++) {
   for(n=0;n<8;n++) {wd_6800a(cha[i][n]); }
  }
 }

 void wcgram_0069b(void) {
 uchar n,i;
  wi_6800b(0x40);		//写CGRAM ICON
  for(i=0;i<7;i++) {
   for(n=0;n<8;n++) {wd_6800b(cha[i][n]); }
  }
 }

void wcgram4bit_0069(void) {
 uchar n,i;
 wi_6800a_4bit(0x40);  //写CGRAM ICON
 for(i=0;i<7;i++) {
  for(n=0;n<8;n++) {wd_6800a_4bit(cha[i][n]); }
  }
 }

void wcgram_seriall_RW1067(void) {

  uchar n,i;
  wi_serial_RW1067(0x40);
  for(i=0;i<7;i++) {
   for(n=0;n<8;n++) {wd_serial_RW1067(cha[i][n]); }
  }
 }


void wcgram_serial_7070s3(void) {
  uchar n,i;
  wi_serial_RW1067(0x40);
  wi_serial_RW1067(0x3c);
  wi_serial_RW1067(0xbf);
  for(i=0;i<7;i++) {
   for(n=0;n<8;n++) {wi_serial_RW1067(cha[i][n]);}
  }
 }


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void reset_s6a0069a(void){						//初始化屏幕,清屏=S6A0069a

 if(Charater_n_Line==0){							//2-line
  	wi_6800a(0x38);  		delay_ms(44);
  	wi_6800a(0x38);  		delay_ms(44);
  	wi_6800a(0x38);  		//dely();
	}
  	else{																//1-line
   	wi_6800a(0x30);   	delay_ms(44);
   	wi_6800a(0x30);   	delay_ms(44);
   	wi_6800a(0x30);   	//dely();
   }


  	wi_6800a(0x06);   	dely();			//输入方式
  	wi_6800a(0x01);   	delay_ms(22);		//清屏
  	wi_6800a(0x0c);   	dely();			//开示
  }

void reset_s6a0069b(void){						//初始化屏幕,清屏=S6A0069b

 if(Charater_n_Line==0){							//2-line
  	wi_6800b(0x38);  		delay_ms(44);
 		wi_6800b(0x38);  		delay_ms(44);
  	wi_6800b(0x38);  		//dely();
	}

  else{																//1-line
  	wi_6800b(0x30);  		delay_ms(44);
  	wi_6800b(0x30);  		delay_ms(44);
  	wi_6800b(0x30);  		//dely();
  }

 		wi_6800b(0x06);  		dely();			//输入方式
 		wi_6800b(0x01);  		delay_ms(22);			//清屏
 		wi_6800b(0x0c);  		dely();			//开示
 }

////////////////////////////////////////////////////////////////////////////////

void reset_4bit_s6a0069(void){						//初始化屏幕,清屏=S6A0069
 if(Charater_n_Line==0){
   wi_6800a_4bit(0x28); 	delay_ms(44);
   wi_6800a_4bit(0x28); 	delay_ms(44);
	 wi_6800a_4bit(0x28);  	//dely();
	}
  else{
 	 wi_6800a_4bit(0x20);  	delay_ms(44);
 	 wi_6800a_4bit(0x20);  	delay_ms(44);
	 wi_6800a_4bit(0x20);  	//dely();
  	}

 wi_6800a_4bit(0x06);  		dely(); 			//输入方式
 wi_6800a_4bit(0x01);  		delay_ms(22);		//清屏
 wi_6800a_4bit(0x0c);  		dely();   		//开示
 }



void reset_RW1067(void){									//初始化屏幕,清屏=SPI
	 RST=0;
	 delay_ms(33);
	 RST=1;
	 PSB=1; 																//并口选择=1;

  wi_6800a(0x30);						delay_ms(44);
  wi_6800a(0x30);						delay_ms(44);
  wi_6800a(0x30);						dely();
  wi_6800a(0x0c);           dely();
  wi_6800a(0x01);           delay_ms(22);
  wi_6800a(0x06);           dely();
  wi_6800a(0x02);           dely();
  wi_6800a(0x34);           dely();
  wi_6800a(0x09);           dely();
  wi_6800a(0x30);           dely();
 }


void reset_serial_RW1067(void) {					//复位成字符模式SPI
	 RST=0;
	 delay_ms(99);													//复位reset
	 RST=1;
	 PSB=0; 																//串口选择=0;
   dely();

   wi_serial_RW1067(0x30); delay_ms(66);

   wi_serial_RW1067(0x0c); delay_ms(11);
   wi_serial_RW1067(0x01); delay_ms(33);
   wi_serial_RW1067(0x06); delay_ms(11);

   wi_serial_RW1067(0x34); delay_ms(11);
   wi_serial_RW1067(0x09); delay_ms(11);

   wi_serial_RW1067(0x30); delay_ms(11);
}


//复位成字符模式st7070
void reset_serial_ST7070s4(void) {
	 RST=0;
	 delay_ms(33);
	 RST=1;
 	 CSB=0; 																	//串口选择=0;

   ce8=1;ce7=1;ce6=1;ce5=1;ce4=1;en=1;			//DB0-DB5=1;en=1;
   wrr=0;																		//rw=0;

   wi_serial_RW1067(0x38); delay_ms(44);
   wi_serial_RW1067(0x38); dely();

   wi_serial_RW1067(0x0c); dely();
   wi_serial_RW1067(0x01); delay_ms(22);
   wi_serial_RW1067(0x06); dely();

   wi_serial_RW1067(0x3c); dely();
   wi_serial_RW1067(0x40); dely();

   wi_serial_RW1067(0x38); dely();
}

//复位成字符模式st7070
void reset_serialST7070s3(void){
	 RST=0;
	 dely();
	 RST=1;
 	 CSB=0;  																	//串口选择=0;

   ce8=1;ce7=1;ce6=1;ce5=1;ce4=1;en=1;			//DB0-DB5=1;en=1;
   wrr=0;rs=0;															//rw=0;	rs=0;

   wi_serial_RW1067(0x38); 	delay_ms(44);
   wi_serial_RW1067(0x38);  dely();

   wi_serial_RW1067(0x0c);	dely(); 				//display on
   wi_serial_RW1067(0x01);	delay_ms(22);		//clear display
   wi_serial_RW1067(0x06);	dely(); 				//mode
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


void disall_ST7070character_s3(uchar datt) {
  uchar n;
  wi_serial_RW1067(0x38);delay_ms(44);
  wi_serial_RW1067(0x80);dely();
  wi_serial_RW1067(0x3c);dely();
  wi_serial_RW1067(0x8f);dely();
   for(n=0;n<16;n++) {
   wi_serial_RW1067(n+datt);dely();
  }

  wi_serial_RW1067(0x38);delay_ms(44);
  wi_serial_RW1067(0xc0);dely();
  wi_serial_RW1067(0x3c);dely();
  wi_serial_RW1067(0x8f);dely();
   for(n=0;n<16;n++) {
   wi_serial_RW1067(n+datt);dely();
  }

}


  //全显示=SPI
void disall_7070s3_serial(uchar dat){

  uchar n;
  wi_serial_RW1067(0x38);delay_ms(44);
  wi_serial_RW1067(0x80);dely();
  wi_serial_RW1067(0x3c);dely();
  wi_serial_RW1067(0x8f);dely();

   for(n=0;n<16;n++) {
   wi_serial_RW1067(dat);dely();
   }

  wi_serial_RW1067(0x38);delay_ms(44);
  wi_serial_RW1067(0xc0);dely();
  wi_serial_RW1067(0x3c);dely();
  wi_serial_RW1067(0x8f);dely();

  for(n=0;n<16;n++) {
   wi_serial_RW1067(dat);dely();
   }

 }

 //全显示=char-a
void dischar_0069a_4bit(uchar dat1,dat2) {
 uchar n;
  wi_6800a_4bit(0x80);dely();
  for(n=0;n<40;n++) {
   wd_6800a_4bit(dat1+n);dely();
   }
  wi_6800a_4bit(0xc0);dely();
  for(n=0;n<40;n++) {
   wd_6800a_4bit(dat2+n);dely();
   }
}


////////////////////////////////////////////////////////////////////////////////

void dison4bit_all(uchar dat) {									//全显示=S6A0069
 uchar n;

 wi_6800a_4bit(0x80); dely();
  for(n=0;n<40;n++) {wd_6800a_4bit(dat);dely(); }
 wi_6800a_4bit(0xc0); dely();
  for(n=0;n<40;n++) {wd_6800a_4bit(dat);dely(); }
 }




//全显示=S6A0069a
void disall_0069a(uchar dat) {
 uchar n;
 wi_6800a(0x80);dely();
 for(n=0;n<40;n++) {
 wd_6800a(dat);dely();
  }
 wi_6800a(0xc0);dely();
 for(n=0;n<40;n++) {
 wd_6800a(dat);dely();
  }
 }

 //全显示=S6A0069b
void disall_0069b(uchar dat) {
 uchar n;
 wi_6800b(0x80);dely();
 for(n=0;n<40;n++) {
 wd_6800b(dat);dely();
  }
 wi_6800b(0xc0);dely();
 for(n=0;n<40;n++) {
 wd_6800b(dat);dely();
  }
 }




void dis_0069_string1(unsigned char *str) {
 uchar i;

  wi_6800a(0x80);dly();
  for(i=0;i<16;i++) {
  wd_6800a(*str);dely();
  str++;
  }
}
void dis_0069_string2(unsigned char *str) {
 uchar i;

  wi_6800a(0xc0);dly();
  for(i=0;i<16;i++) {
  wd_6800a(*str);dely();
  str++;
  }
}

void dis_char_string(unsigned char *str) {
 uchar i;

  wi_6800a(0x80);
  for(i=0;i<40;i++) {
  wd_6800a(*str+0x30);dly();
  str++;
  }
  wi_6800a(0xc0);
  for(i=0;i<24;i++) {
  wd_6800a(*str+0x30);dly();
  str++;
  }


}





///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void dis_prog_time(unsigned char ddat) {

 if(ddat==0){
		dis_0069_string1("A Program Num   \n");
		dis_0069_string2("  Time data     \n");}
 if(ddat==1){
		dis_0069_string1("  Program Num   \n");
		dis_0069_string2("A Time data     \n");}
}

void dis_time(unsigned char ddat) {
 uchar d2,d3;
		dis_0069_string1(" Time data      \n");
		dis_0069_string2("                \n");

 		d2=ddat%100/10;
		d3=ddat%100%10;
    wi_6800a(0xc3);

    wd_6800a(d2+0x30);
    wd_6800a(d3+0x30);
}


void dis_program(unsigned char ddat) {

if(ddat==0){
		dis_0069_string1("A 0=Character   \n");
		dis_0069_string2("   1=Dot matrix \n");}
if(ddat==1){
		dis_0069_string1("A 1=Dot matrix  \n");
		dis_0069_string2("   2=Segment    \n");}

if(ddat==2){
		dis_0069_string1("A 2=Segment     \n");
		dis_0069_string2("   3=Dot Segment\n");}

if(ddat==3){
		dis_0069_string1("A 3=Dot Segment \n");
		dis_0069_string2("   4=Tobacco    \n");}

if(ddat==4){
		dis_0069_string1("A 4=Tobacco     \n");
		dis_0069_string2("   5=Dot-SED1335\n");}

if(ddat==5){
		dis_0069_string1("A 5=Dot-SED1335 \n");
		dis_0069_string2("   6=Dot-T6963C \n");}

if(ddat==6){
		dis_0069_string1("A 6=Dot-T6963C  \n");
		dis_0069_string2("   7=Dot-PCF8576\n");}

if(ddat==7){
		dis_0069_string1("A 7=Dot-PCF8576 \n");
		dis_0069_string2("   8=Dot-ST7920 \n");}

if(ddat==8){
		dis_0069_string1("A 8=Dot-ST7920  \n");
		dis_0069_string2("   9=Dot-SBN1661\n");}

if(ddat==9){
		dis_0069_string1("A 9=Dot-SBN1661 \n");
		dis_0069_string2("======END=======\n");}

}

void dis_module(unsigned char ddat,mode) {

////////////////////////////////////////////////////////////
	if(ddat==0){
		if(mode==0) {	dis_0069_string1("0.0=AIP31066*2  \n");
									dis_0069_string2(" 1/16D 8Bit Bus \n");}

		if(mode==1) {	dis_0069_string1("0.1=AIP31066*2  \n");
									dis_0069_string2(" 1/8D  8Bit Bus \n");}

		if(mode==2) {	dis_0069_string1("0.2=AIP31066*1  \n");
									dis_0069_string2(" 1/16D 4Bit Bus \n");}

		if(mode==3) {	dis_0069_string1("0.3=AIP31066*1  \n");
									dis_0069_string2(" 1/8D  4Bit Bus \n");}

		if(mode==4) {	dis_0069_string1("0.4=ST7070*1    \n");
									dis_0069_string2(" 4Line Serial   \n");}

		if(mode==5) {	dis_0069_string1("0.5=ST7070*1    \n");
									dis_0069_string2(" 3Line Serial   \n");}

		if(mode==6) {	dis_0069_string1("0.6=RW1067*1    \n");
									dis_0069_string2(" 1/16D 8Bit Bus \n");}

		if(mode==7) {	dis_0069_string1("0.7=RW1067*1    \n");
									dis_0069_string2(" 4Line Serial   \n");}
    }
////////////////////////////////////////////////////////////
	if(ddat==1){
		if(mode==0) {	dis_0069_string1("1.0=AIP31108*2  \n");
									dis_0069_string2(" 12864 Dots CS*2\n");}

		if(mode==1) {	dis_0069_string1("1.1=AIP31108*3  \n");
									dis_0069_string2(" 19264 Dots CS*2\n");}

		if(mode==2) {	dis_0069_string1("1.2=AIP31108*3  \n");
									dis_0069_string2(" 19264 Dots CS*3\n");}

		if(mode==3) {	dis_0069_string1("1.3=AIP31108*4  \n");
									dis_0069_string2(" 25664 Dots CS*3\n");}
    }
////////////////////////////////////////////////////////////
	if(ddat==2){
		if(mode==0) {	dis_0069_string1("2.0=HT1621*1    \n");
									dis_0069_string2(" 1/4Duty 1/3Bias\n");}

		if(mode==1) {	dis_0069_string1("2.1=HT1621*2    \n");
									dis_0069_string2(" 1/4Duty 1/3Bias\n");}

		if(mode==2) {	dis_0069_string1("2.2=HT1621*3    \n");
									dis_0069_string2(" 1/4Duty 1/3Bias\n");}

		if(mode==3) {	dis_0069_string1("2.3=HT1621*4    \n");
									dis_0069_string2(" 1/4Duty 1/3Bias\n");}

		if(mode==4) {	dis_0069_string1("2.4=HT1621*5    \n");
									dis_0069_string2(" 1/4Duty 1/3Bias\n");}

		if(mode==5) {	dis_0069_string1("2.5=HT1621*6    \n");
									dis_0069_string2(" 1/4Duty 1/3Bias\n");}

		if(mode==6) {	dis_0069_string1("2.6=HT1621*7    \n");
									dis_0069_string2(" 1/4Duty 1/3Bias\n");}

		if(mode==7) {	dis_0069_string1("2.7=HT1621*8    \n");
									dis_0069_string2(" 1/4Duty 1/3Bias\n");}

		if(mode==8) {	dis_0069_string1("2.8=HT1621*9    \n");
									dis_0069_string2(" 1/4Duty 1/3Bias\n");}
/////////////////
		if(mode==9) {	dis_0069_string1("2.9=HT1622*1    \n");
									dis_0069_string2(" Data=32*8 Dots \n");}

		if(mode==10){	dis_0069_string1("2.A=RW1026*1    \n");
									dis_0069_string2(" Data=48*4 Dots \n");}

		if(mode==11){	dis_0069_string1("2.B=HT1623*1    \n");
									dis_0069_string2(" Data=48*8 Dots \n");}
////////////////
		if(mode==12){	dis_0069_string1("2.C=HT1621*1    \n");
									dis_0069_string2(" 1/4Duty 1/2Bias\n");}

		if(mode==13){	dis_0069_string1("2.D=HT1621*1    \n");
									dis_0069_string2(" 1/3Duty 1/3Bias\n");}

		if(mode==14){	dis_0069_string1("2.E=HT1621*1    \n");
									dis_0069_string2(" 1/3Duty 1/2Bias\n");}

		if(mode==15){	dis_0069_string1("2.F=HT1621*1    \n");
									dis_0069_string2(" 1/2Duty 1/2Bias\n");}
 ////////////////
		if(mode==16){	dis_0069_string1("2.G=HT1621*1    \n");
									dis_0069_string2(" Ext Clock      \n");}

		if(mode==17){	dis_0069_string1("2.H=HT1621*1    \n");
									dis_0069_string2(" Ext Crystal    \n");}
     }
////////////////////////////////////////////////////////////
	if(ddat==3){
		if(mode==0) {	dis_0069_string1("3.0=AIP31065*1  \n");
									dis_0069_string2(" 40 Sengment    \n");}

		if(mode==1) {	dis_0069_string1("3.1=AIP31063*1  \n");
									dis_0069_string2(" 80 Sengment    \n");}

		if(mode==2) {	dis_0069_string1("3.2=AIP31063*2  \n");
									dis_0069_string2(" 160 Sengment   \n");}
    }
////////////////////////////////////////////////////////////
	if(ddat==4){
		if(mode==0) {	dis_0069_string1("4.0=HT1621*7    \n");
									dis_0069_string2(" CRB1029-2009   \n");}

		if(mode==1) {	dis_0069_string1("4.1=HT1621*7    \n");
									dis_0069_string2(" ZBD5056-2015   \n");}

		if(mode==2) {	dis_0069_string1("4.2=HT1621*7    \n");
									dis_0069_string2(" ZBD6025-2016   \n");}

		if(mode==3) {	dis_0069_string1("4.3=HT1621*7    \n");
									dis_0069_string2(" ZBD7104-2017   \n");}

		if(mode==4) {	dis_0069_string1("4.4=HT1621*7    \n");
									dis_0069_string2(" ZBD8106-2018   \n");}


    }
////////////////////////////////////////////////////////////

	if(ddat==5){
		if(mode==0) {	dis_0069_string1("5.0=SED1335     \n");
									dis_0069_string2(" 240*64 Dots    \n");}

		if(mode==1) {	dis_0069_string1("5.1=SED1335     \n");
									dis_0069_string2(" 320*80 Dots    \n");}

		if(mode==2) {	dis_0069_string1("5.2=SED1335     \n");
									dis_0069_string2(" 320*160 Dots   \n");}

		if(mode==3) {	dis_0069_string1("5.3=SED1335     \n");
									dis_0069_string2(" 320*240 Dots   \n");}

		if(mode==4) {	dis_0069_string1("5.4=SED1335     \n");
									dis_0069_string2(" 320*240+TOUCH  \n");}

    }
////////////////////////////////////////////////////////////


	if(ddat==6){
		if(mode==0) {	dis_0069_string1("6.0=RA6963      \n");
									dis_0069_string2(" 128*64 Dots    \n");}

		if(mode==1) {	dis_0069_string1("6.1=RA6963      \n");
									dis_0069_string2(" 240*64 Dots    \n");}

		if(mode==2) {	dis_0069_string1("6.2=RA6963      \n");
									dis_0069_string2(" 160*128 Dots   \n");}

		if(mode==3) {	dis_0069_string1("6.3=RA6963      \n");
									dis_0069_string2(" 240*128 Dots   \n");}

		if(mode==4) {	dis_0069_string1("6.4=RA6963      \n");
									dis_0069_string2(" 160*80 Dots    \n");}

		if(mode==5) {	dis_0069_string1("6.5=RA6963      \n");
									dis_0069_string2(" 240*80 Dots    \n");}

    }
////////////////////////////////////////////////////////////



	if(ddat==7){
		if(mode==0) {	dis_0069_string1("7.0=PCF8576*2   \n");
									dis_0069_string2(" 1/4Duty 1/3Bias\n");}

		if(mode==1) {	dis_0069_string1("7.1=PCF8576*2   \n");
									dis_0069_string2(" 1/4Duty 1/2Bias\n");}

		if(mode==2) {	dis_0069_string1("7.2=PCF8576*2   \n");
									dis_0069_string2(" 1/3Duty 1/3Bias\n");}

		if(mode==3) {	dis_0069_string1("7.3=PCF8576*2   \n");
									dis_0069_string2(" 1/3Duty 1/2Bias\n");}

		if(mode==4) {	dis_0069_string1("7.4=PCF8576*2   \n");
									dis_0069_string2(" 1/2Duty 1/2Bias\n");}

		if(mode==5) {	dis_0069_string1("7.5=PCF8576*2   \n");
									dis_0069_string2(" 1/1Duty 1/1Bias\n");}

    }
////////////////////////////////////////////////////////////

	if(ddat==8){
		if(mode==0) {	dis_0069_string1("8.0=ST7920*1 Par\n");
									dis_0069_string2("  256*32 Dots   \n");}

		if(mode==1) {	dis_0069_string1("8.1=ST7920*1 Ser\n");
									dis_0069_string2("  256*32 Dots   \n");}

		if(mode==2) {	dis_0069_string1("8.2=ST7920*2 Par\n");
									dis_0069_string2("  128*64 Dots   \n");}

		if(mode==3) {	dis_0069_string1("8.3=ST7920*1 Ser\n");
									dis_0069_string2("  128*64 Dots   \n");}

		if(mode==4) {	dis_0069_string1("8.4=ST7920*1 Par\n");
									dis_0069_string2("  192*64 Dots   \n");}
    }


	if(ddat==9){
		if(mode==0) {	dis_0069_string1("9.0=SBN1661*2   \n");
									dis_0069_string2(" 122*32 Dots    \n");}

		if(mode==1) {	dis_0069_string1("9.1=SBN1661*1   \n");
									dis_0069_string2("  61*16 Dots    \n");}
    }

}

 //全显示=SPI
void disall_RW1067(uchar dat) {
  uchar n;
  wi_6800a(0x02);dely();

  wi_6800a(0x80);dely();
  for(n=0;n<16;n++) {
  wd_6800a(dat);dely();
  }
  wi_6800a(0xa0);dely();
  for(n=0;n<16;n++) {
  wd_6800a(dat);dely();
  }
  wi_6800a(0xc0);dely();
  for(n=0;n<16;n++) {
  wd_6800a(dat);dely();
  }
  wi_6800a(0xe0);dely();
  for(n=0;n<16;n++) {
  wd_6800a(dat);dely();
  }
 }

 //全显示=SPI
void disall_RW1067character(uchar datt) {
  uchar n;
  wi_6800a(0x02);dely();

  wi_6800a(0x80);dely();
  for(n=0;n<16;n++) {
  wd_6800a(0x20+n+datt);dely();
  }
  wi_6800a(0xa0);dely();
  for(n=0;n<16;n++) {
  wd_6800a(0x30+n+datt);dely();
  }
  wi_6800a(0xc0);dely();
  for(n=0;n<16;n++) {
  wd_6800a(0x40+n+datt);dely();
  }
  wi_6800a(0xe0);dely();
  for(n=0;n<16;n++) {
  wd_6800a(0x60+n+datt);dely();
  }
 }


 //全显示=SPI
void disall_RW1067_serial(uchar dat){

  uchar n;
  wi_serial_RW1067(0x02);//dely();

  wi_serial_RW1067(0x80);//dely();
  for(n=0;n<16;n++) {
   wd_serial_RW1067(dat);//dely();
  }

  wi_serial_RW1067(0xa0);//dely();
  for(n=0;n<16;n++) {
   wd_serial_RW1067(dat);//dely();
  }

  wi_serial_RW1067(0xc0);//dely();
  for(n=0;n<16;n++) {
   wd_serial_RW1067(dat);//dely();
  }

  wi_serial_RW1067(0xe0);//dely();
  for(n=0;n<16;n++) {
   wd_serial_RW1067(dat);//dely();
  }
 }






void disall_RW1067character_serial(uchar datt) {
  uchar n;
  wi_serial_RW1067(0x02);dely();

  wi_serial_RW1067(0x80);dely();
  for(n=0;n<16;n++) {
  wd_serial_RW1067(0x20+n+datt);dely();
  }
  wi_serial_RW1067(0xa0);dely();
  for(n=0;n<16;n++) {
  wd_serial_RW1067(0x30+n+datt);dely();
  }
  wi_serial_RW1067(0xc0);dely();
  for(n=0;n<16;n++) {
  wd_serial_RW1067(0x40+n+datt);dely();
  }
  wi_serial_RW1067(0xe0);dely();
  for(n=0;n<16;n++) {
  wd_serial_RW1067(0x60+n+datt);dely();
  }
 }
void disall_ST7070character_serial(uchar datt) {
  uchar n;
  wi_serial_RW1067(0x02);dely();
  wi_serial_RW1067(0x80);dely();
  for(n=0;n<40;n++) {
  wd_serial_RW1067(0x30+n+datt);dely();
  }

  wi_serial_RW1067(0xc0);dely();
  for(n=0;n<40;n++) {
  wd_serial_RW1067(0x40+n+datt);dely();
  }
}










/////////////////////////////////////////////////////////////////////////////////
//全显示=char-a
void dischar_0069a(uchar dat1,dat2) {
 uchar n;
  wi_6800a(0x80);dely();
  for(n=0;n<40;n++) {
   wd_6800a(dat1+n);dely();
   }
  wi_6800a(0xc0);dely();
  for(n=0;n<40;n++) {
   wd_6800a(dat2+n);dely();
   }
}

//全显示=char-b
void dischar_0069b(uchar dat1,dat2) {
 uchar n;
  wi_6800b(0x80);dely();
  for(n=0;n<40;n++) {
   wd_6800b(dat1+n);dely();
   }
  wi_6800b(0xc0);dely();
  for(n=0;n<40;n++) {
   wd_6800b(dat2+n);dely();
   }
}







void test_0069_8bit_x_line(void){
  uchar p;

  	reset_s6a0069a(); disall_0069a(0xff);           						//1-全显示
  	reset_s6a0069b(); disall_0069b(0xff); //背光控制              						//1-全显示
  for(p=0;p<xunhuan;p++){

   	reset_s6a0069a(); disall_0069a(0xff);           						//1-全显示
  	reset_s6a0069b(); disall_0069b(0xff);  //背光控制              						//1-全显示

  	wcgram_0069a();
  	wcgram_0069b();






  	disall_0069a(0x06);                   						//1-全显示
  	disall_0069b(0x06);        delay(666+222*delayTime);

  	disall_0069a(0x00);
  	disall_0069b(0x00);        delay(666+222*delayTime);

  	disall_0069a(0x01);
  	disall_0069b(0x01);        delay(666+222*delayTime);

  	disall_0069a(0x02);
   	disall_0069b(0x02);        delay(666+222*delayTime);

  	disall_0069a(0x03);
  	disall_0069b(0x03);        delay(666+222*delayTime);

  	disall_0069a(0x04);
  	disall_0069b(0x04);        delay(666+222*delayTime);

  	disall_0069a(0x05);
  	disall_0069b(0x05);        delay(666+222*delayTime);

   	disall_0069a('G');
   	disall_0069b('S');         delay(666+222*delayTime);

  	dischar_0069a(0x30,0x40);
  	dischar_0069b(0x30,0x40);  delay(666+222*delayTime);

  	dischar_0069a(0xb1,0xc1);
  	dischar_0069b(0xb1,0xc1);  delay(666+222*delayTime);


 		}
 }



 void test_0069_4bit_x_line(void){
  uchar p;

	 	reset_4bit_s6a0069();

	 for(p=0;p<xunhuan;p++){




	 	reset_4bit_s6a0069();
	 	wcgram4bit_0069();

		dison4bit_all(0xff); delay(666+222*delayTime);
	 	dison4bit_all(0x06); delay(666+222*delayTime);
	 	dison4bit_all(0x00); delay(666+222*delayTime);
	 	dison4bit_all(0x01); delay(666+222*delayTime);
	 	dison4bit_all(0x02); delay(666+222*delayTime);
	 	dison4bit_all(0x03); delay(666+222*delayTime);
	 	dison4bit_all(0x04); delay(666+222*delayTime);
	 	dison4bit_all(0x05); delay(666+222*delayTime);

   dison4bit_all('G');  delay(666+222*delayTime);
 	 dison4bit_all('S');  delay(666+222*delayTime);

	 	dischar_0069a_4bit(0x30,0x40); delay(666+222*delayTime);

  	dischar_0069a_4bit(0xb1,0xc1); delay(666+222*delayTime);

  	dison4bit_all(0x20); delay(666+222*delayTime);

 	 }
	}


//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////


 void test_0070_4line_x_line(void){
	uchar p;

 reset_serial_ST7070s4();


 for(p=0;p<xunhuan;p++){


  reset_serial_ST7070s4();
  wcgram_seriall_RW1067();

    disall_RW1067_serial(6);        delay(222+222*delayTime);
    disall_RW1067_serial(0);        delay(222+222*delayTime);
    disall_RW1067_serial(1);        delay(222+222*delayTime);
    disall_RW1067_serial(2);        delay(222+222*delayTime);
    disall_RW1067_serial(3);        delay(222+222*delayTime);
    disall_RW1067_serial(4);        delay(222+222*delayTime);
    disall_RW1067_serial(5);        delay(222+222*delayTime);
    disall_RW1067_serial('G');        delay(222+222*delayTime);
    disall_RW1067_serial('S');        delay(222+222*delayTime);
    disall_ST7070character_serial(0x00); delay(222+222*delayTime);
    disall_ST7070character_serial(0x80); delay(222+222*delayTime);

  }
}











//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////




 void test_0070_3line_x_line(void){
	uchar p;
     reset_serialST7070s3();

 for(p=0;p<xunhuan;p++){

   reset_serialST7070s3();
    wcgram_serial_7070s3();

    disall_7070s3_serial(6);        delay(222+222*delayTime);
    disall_7070s3_serial(0);        delay(222+222*delayTime);
    disall_7070s3_serial(1);        delay(222+222*delayTime);
    disall_7070s3_serial(2);        delay(222+222*delayTime);
    disall_7070s3_serial(3);        delay(222+222*delayTime);
    disall_7070s3_serial(4);        delay(222+222*delayTime);
    disall_7070s3_serial(5);        delay(222+222*delayTime);
    disall_7070s3_serial('G');        delay(222+222*delayTime);
    disall_7070s3_serial('S');        delay(222+222*delayTime);
    disall_ST7070character_s3(0x30); delay(222+222*delayTime);
    disall_ST7070character_s3(0xb1); delay(222+222*delayTime);

  }
}








void test_RW1067_Parallel(void){
 	uchar p;
    reset_RW1067();

 for(p=0;p<xunhuan;p++){
   reset_RW1067();
  wcgram_0069a();

    disall_RW1067(6);        delay(222+222*delayTime);
    disall_RW1067(0);        delay(222+222*delayTime);
    disall_RW1067(1);        delay(222+222*delayTime);
    disall_RW1067(2);        delay(222+222*delayTime);
    disall_RW1067(3);        delay(222+222*delayTime);
    disall_RW1067(4);        delay(222+222*delayTime);
    disall_RW1067(5);        delay(222+222*delayTime);
    disall_RW1067('G');        delay(222+222*delayTime);
    disall_RW1067('S');        delay(222+222*delayTime);
    disall_RW1067character(0x00); delay(222+222*delayTime);
    disall_RW1067character(0x80); delay(222+222*delayTime);
  }
 }

void test_RW1067_Serial(void){
	uchar p;
  reset_serial_RW1067();
 	for(p=0;p<xunhuan;p++){
  	reset_serial_RW1067();
  	wcgram_seriall_RW1067();

    disall_RW1067_serial(6);        			delay(222+222*delayTime);
    disall_RW1067_serial(0);        			delay(222+222*delayTime);
    disall_RW1067_serial(1);        			delay(222+222*delayTime);
    disall_RW1067_serial(2);        			delay(222+222*delayTime);
    disall_RW1067_serial(3);        			delay(222+222*delayTime);
    disall_RW1067_serial(4);        			delay(222+222*delayTime);
    disall_RW1067_serial(5);        			delay(222+222*delayTime);
    disall_RW1067_serial('G');        		delay(222+222*delayTime);
    disall_RW1067_serial('S');        		delay(222+222*delayTime);
    disall_RW1067character_serial(0x11); 	delay(222+222*delayTime);
    disall_RW1067character_serial(0x21); 	delay(222+222*delayTime);
  }
 }

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////


void panduan25664(uchar num){		//低电平选择;

 if(num==0){
  if(program_num==0){cs1=0;cs2=1;cs3=1; }
  if(program_num==1){cs1=0;cs2=0;cs3=1; }
  if(program_num==2){cs1=0;cs2=1;cs3=1; }
  if(program_num==3){cs1=0;cs2=0;cs3=1; }
  }

 if(num==1){
  if(program_num==0){cs1=1;cs2=0;cs3=1; }
  if(program_num==1){cs1=1;cs2=0;cs3=1; }
  if(program_num==2){cs1=1;cs2=0;cs3=1; }
  if(program_num==3){cs1=1;cs2=0;cs3=1; }
  }

 if(num==2){
  if(program_num==0){cs1=0;cs2=1;cs3=1; }
  if(program_num==1){cs1=0;cs2=1;cs3=1; }
  if(program_num==2){cs1=1;cs2=1;cs3=0; }
  if(program_num==3){cs1=0;cs2=1;cs3=1; }
  }

 if(num==3){
  if(program_num==0){cs1=1;cs2=0;cs3=1; }
  if(program_num==1){cs1=0;cs2=1;cs3=1; }
  if(program_num==2){cs1=1;cs2=1;cs3=0; }
  if(program_num==3){cs1=1;cs2=1;cs3=1; }
  }
}



//初始化屏幕1,清屏128*64=S6B0108;电平选择00,01,10;
void retdis_s6b0108a_25664(void) {
 uchar m;
 for(m=0;m<4;m++) {
  panduan25664(m);dely();
  wi_6800a(0xc0); dely();dely();
  wi_6800a(0x3f); dely();dely();

 }
}



//全显示数据192*64=S6B0108;
void disall_0108_25664(uchar da1,da2) {
 uchar i,j;
 uchar m,h;

if(program_num==0){h=2;}
if(program_num==1|program_num==2){h=3;}
if(program_num==3){h=4;}





  for(i=0;i<8;i++) {

   for(m=0;m<h;m++) {

  panduan25664(m);

   wi_6800a(0x40);
   wi_6800a(0xb8+i);
   for(j=0;j<32;j++) {
    wd_6800a(da1);
    wd_6800a(da2);
   }
  }
 }




}



//写汉字128*64=S6B0108;x=列,y=行,z=哪个数字
void shuzi_0108(uchar x) {
 uchar i;

 wi_6800a(0x50);
 wi_6800a(0xb9);
  for(i=0;i<8;i++) {wd_6800a(hz0816h[x][i]);}

 wi_6800a(0x50);
 wi_6800a(0xba);
  for(i=0;i<8;i++) {wd_6800a(hz0816h[x][i+8]);}

}


////////////////////////////////////////////////////////////////////////////////
//画方框64*64=S6B0108;
void window_0108(void) {
 uchar i;
 uchar m,h;

 if(program_num==0){h=2;}
 if(program_num==1|program_num==2){h=3;}
 if(program_num==3){h=4;}

 for(m=0;m<h;m++) {

  panduan25664(m);

  shuzi_0108(m);

  wi_6800a(0x40);wi_6800a(0xb8);
   for(i=0;i<64;i++) {wd_6800a(0x01);}

	wi_6800a(0x40);wi_6800a(0xbf);
   for(i=0;i<64;i++) {wd_6800a(0x80);}

  for(i=0;i<8;i++) {
   wi_6800a(0x40); wi_6800a(0xb8+i);
   wd_6800a(0xff);
  }

  for(i=0;i<8;i++) {
   wi_6800a(0x7f); wi_6800a(0xb8+i);
   wd_6800a(0xff);
  }
 }
}



//写任意一幅图象0108
void distu_0108(uchar *mintu) {
 uchar i,k,m,h;

 if(program_num==0){h=2;}
 if(program_num==1|program_num==2){h=3;}
 if(program_num==3){h=4;}




 for(i=0;i<8;i++) {
  for(m=0;m<h;m++) {
   panduan25664(m);

   for(k=0;k<64;k++) {
     wi_6800a(0x40+k);
    wi_6800a(0xb8+i);

    wd_6800a(*(mintu++));
   }
  }
 }
}





////////////////////////////////////////////////////////////////////////////////


void test_0108_25664(void){
	uchar p;
	retdis_s6b0108a_25664();
  for(p=0;p<xunhuan;p++){
   retdis_s6b0108a_25664();
   disall_0108_25664(0x00,0x00);

 	 window_0108();delay(222+222*delayTime);


//	 distu_0108(tu256064H1);delay(222+222*delayTime);
 if(program_num==0){  	 distu_0108(tu128064H1);}
 if(program_num==1|program_num==2){  	 distu_0108(tu192064H1);}
 if(program_num==3){  	 distu_0108(tu256064H1);}


 delay(222+222*delayTime);




   disall_0108_25664(0x00,0x00);    delay(222+222*delayTime);              //全显示全关闭两次
   disall_0108_25664(0xff,0xff);    delay(222+222*delayTime);              //全显示全关闭两次
   disall_0108_25664(0x00,0x00);    delay(222+222*delayTime);              //全显示全关闭两次
   disall_0108_25664(0x55,0x55);   	delay(222+222*delayTime);
   disall_0108_25664(0xaa,0xaa);   	delay(222+222*delayTime);
   disall_0108_25664(0xff,0x00);   	delay(222+222*delayTime);
   disall_0108_25664(0x00,0xff);   	delay(222+222*delayTime);
   disall_0108_25664(0x55,0xaa);   	delay(222+222*delayTime);
   disall_0108_25664(0xaa,0x55);   	delay(222+222*delayTime);
   }
  }

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

 void sethtdata(uchar dat1,dat2){			//送双数据32byte
	uchar i;
for(i=0x00;i<16;i++) { htdata[i*2]=dat1;htdata[i*2+1]=dat2;}
}

//CRB1029C=HT1621LCD时序，数据右对齐=pdat:数据  n:位数
 void sendHT162xByte(uchar pdat,uchar n){
  for(;n>0;n--){
    HT_CLK=0;		_nop_();_nop_();_nop_();_nop_();  						//延时待数据稳定
    if(pdat&(1<<(n-1))){HT_SDA=1;}		//准备数据(高位在前)
    else {HT_SDA=0;}
     _nop_();_nop_();_nop_();_nop_();  	 								//延时待数据稳定
    HT_CLK=1;
     _nop_();_nop_();_nop_();_nop_();
     _nop_();_nop_();_nop_();_nop_(); 										//延时使占空比为50%(可选)
  }
}

//CRB1029C=判断IC
void panduan1621cs(uchar num){
 if(num==0) HT_CS0=0;
 if(num==1) HT_CS1=0;
 if(num==2) HT_CS2=0;
 if(num==3) HT_CS3=0;
 if(num==4) HT_CS4=0;
 if(num==5) HT_CS5=0;
 if(num==6) HT_CS6=0;
 if(num==7) HT_CS7=0;
 if(num==8) HT_CS8=0;
}

void reset1621num(void){
  uchar i,j,*p;
  for(j=0;j<HT1621_num;j++){

  HT_CLK=1;	HT_SDA=1;	HT_CS0=1;	P1=0xff;

    panduan1621cs(j);
    sendHT162xByte(0x04,0x03);	//100
    sendHT162xByte(0x01,0x08);	//8位指令;打开系统振荡器
    sendHT162xByte(0x00,0x01);	//发第9位指令

  HT_CLK=1;	HT_SDA=1;	HT_CS0=1;	P1=0xff;
    panduan1621cs(j);
    sendHT162xByte(0x04,0x03);	//100
    sendHT162xByte(0x03,0x08);	//8位指令;打开LCD偏压发生器
    sendHT162xByte(0x00,0x01);	//发第9位指令

  HT_CLK=1;	HT_SDA=1;	HT_CS0=1;	P1=0xff;
    panduan1621cs(j);
    sendHT162xByte(0x04,0x03);	//100
    sendHT162xByte(DUTYBIAS,0x08);	//8位指令;4com3bias
    sendHT162xByte(0x00,0x01);	//发第9位指令

  HT_CLK=1;	HT_SDA=1;	HT_CS0=1;	P1=0xff;
    panduan1621cs(j);
    sendHT162xByte(0x04,0x03);	//100
    sendHT162xByte(RCXT,0x08);	//8位指令;4com3bias
    sendHT162xByte(0x00,0x01);	//发第9位指令

  p=htdata;
  HT_CLK=1;	HT_SDA=1;	HT_CS0=1;	P1=0xff;
   panduan1621cs(j);
   sendHT162xByte(0x05,0x03);  //101
   sendHT162xByte(0x00,0x06);  //从0地址开始
   for(i=0x00;i<LCDNUM;i++){
    sendHT162xByte(*(p++),0x08);
    }
 }
  HT_CS0=1;	P1=0xff;
}


//HT162*NUM=SEG点显示
void disSEG_1621num(void){
 uchar i,j,k,n=0;
 for(j=0;j<HT1621_num;j=j+1){
  for(i=0;i<LCDNUM*2;i=i+1){
   for(k=0;k<4;k=k+1){
   HT_CLK=1;	HT_SDA=1;	HT_CS0=1;	P1=0xff;
   panduan1621cs(j);
   sendHT162xByte(0x05,0x03);	//
   sendHT162xByte(i,0x06);	//
   sendHT162xByte(ddat1621[k],0x04);
   delay(11+11*delayTime);
   displayLED(n/100,(n%100)/10,(n%100)%10);n++;
   }
  }
 }
 HT_CS0=1;	P1=0xff;
 displayLED(0,module_num,program_num);
}

//HT162*NUM=COM列显示
void disCOM_1621num(void){
 uchar i,j,k,n=0;
 for(j=0;j<HT1621_num;j=j+1){
  for(i=0;i<LCDNUM*2;i=i+1){
    for(k=0;k<4;k=k+1){
   HT_CLK=1;	HT_SDA=1;	HT_CS0=1;	P1=0xff;
   panduan1621cs(j);
   sendHT162xByte(0x05,0x03);	//
   sendHT162xByte(i,0x06);	//
   sendHT162xByte(0xff,0x04);
   }
   delay(222+222*delayTime);
   displayLED(n/100,(n%100)/10,(n%100)%10);n++;
  }
 }
 HT_CS0=1;	P1=0xff;
 displayLED(0,module_num,program_num);
}


void test_Holtek_segment(void){
	uchar p;
  sethtdata(0x00,0x00);  reset1621num(); 	delay(999+222*delayTime); //背光控制
  sethtdata(0xff,0xff);  reset1621num(); 	delay(999+222*delayTime); //背光控制
  sethtdata(0x00,0x00);  reset1621num(); 	delay(999+222*delayTime); //背光控制
  sethtdata(0xff,0xff);  reset1621num(); 	delay(999+222*delayTime); //背光控制
  sethtdata(0x00,0x00);  reset1621num(); 	delay(999+222*delayTime); //背光控制
   for(p=0;p<xunhuan;p++){
//   	sethtdata(0x00,0x00); reset1621num();
//		for(i=0;i<10;i++){
//    	disnum1621x3a(i);				delay(222+222*delayTime);
//     }
   sethtdata(0xff,0x00); reset1621num(); 	delay(999+222*delayTime);
   sethtdata(0x00,0xff); reset1621num(); 	delay(999+222*delayTime);

   sethtdata(0x11,0x11); reset1621num(); 	delay(999+222*delayTime);
   sethtdata(0x22,0x22); reset1621num(); 	delay(999+222*delayTime);
   sethtdata(0x44,0x44); reset1621num(); 	delay(999+222*delayTime);
   sethtdata(0x88,0x88); reset1621num(); 	delay(999+222*delayTime);

   sethtdata(0x55,0x55); reset1621num(); 	delay(999+222*delayTime);
   sethtdata(0xaa,0xaa); reset1621num(); 	delay(999+222*delayTime);
   sethtdata(0x55,0xaa); reset1621num(); 	delay(999+222*delayTime);
   sethtdata(0xaa,0x55); reset1621num(); 	delay(999+222*delayTime);

   sethtdata(0x00,0x00);
   reset1621num();
   disCOM_1621num();			delay(999+222*delayTime);

   sethtdata(0x00,0x00);
   reset1621num();
   disSEG_1621num();			delay(999+222*delayTime);

 }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

//写S6A0063数据
//rs =串行数据锁存
//wrr=串行数据时钟
//en =串行数据

//#define 	HT_CLK 			wrr
//#define 	HT_SDA 			en
//#define 	LOOK_063  		rs



 void wi80bit_wcgram(uchar dat,n){
 uchar i;
 for(i=0;i<n;i++){htdata[i]=0xff;}
 	htdata[n]=dat;
 for(i=n+1;i<21;i++){htdata[i]=0x00;}
}

void wi80bit_all(uchar dat){
 uchar i;

 LOOK_063=0;
 for(i=0;i<BITNUM;i++){ sendHT162xByte(dat,8);}		//wi8bit(DOT[dat][i]);}
 LOOK_063=1;dely();dely();dely();dely();dely();
 LOOK_063=0;
}

void wi80bit_lie(void){
 uchar i,j,k,n;
 n=0;
 for(i=0;i<BITNUM;i++){
 	for(j=0;j<8;j++){
   wi80bit_wcgram(ddat1621[j],i);
   LOOK_063=0;
   for(k=0;k<BITNUM;k++){
   		sendHT162xByte(htdata[k],8);
   		}		//wi8bit(DOT[dat][i]);}
   	LOOK_063=1;
   		dely();dely();dely();dely();dely();
   	LOOK_063=0;
    	delay(33+33*delayTime);
    displayLED(n/100,(n%100)/10,(n%100)%10);n++;
  }

 }
 displayLED(0,module_num,program_num);
}

 void test_063_static(void){
	uchar p;


	wi80bit_all(0xff);
 	for(p=0;p<xunhuan;p++){
  	wi80bit_all(0xff);    delay(222+222*delayTime);
  	wi80bit_all(0x00);    delay(222+222*delayTime);
  	wi80bit_all(0xff);    delay(222+222*delayTime);
  	wi80bit_all(0x00);    delay(222+222*delayTime);

  	wi80bit_lie();

   	displayLED(27,module_num,program_num);       //
  	wi80bit_all(0xff);    delay(222+222*delayTime);
  	wi80bit_all(0x00);    delay(222+222*delayTime);
  	wi80bit_all(ddat1621[0]);    delay(222+222*delayTime);
  	wi80bit_all(ddat1621[1]);    delay(222+222*delayTime);
  	wi80bit_all(ddat1621[2]);    delay(222+222*delayTime);
  	wi80bit_all(ddat1621[3]);    delay(222+222*delayTime);
  	wi80bit_all(ddat1621[4]);    delay(222+222*delayTime);
  	wi80bit_all(ddat1621[5]);    delay(222+222*delayTime);
  	wi80bit_all(ddat1621[6]);    delay(222+222*delayTime);
  	wi80bit_all(ddat1621[7]);    delay(222+222*delayTime);
  	wi80bit_all(0x00);           delay(222+222*delayTime);
  }
 }
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


//CRB1029C=判断IC
void panduan1621(uchar num){

 if(num==0) ce1=0;
 if(num==1) ce2=0;
 if(num==2) ce3=0;
 if(num==3) ce4=0;
 if(num==4) ce5=0;
 if(num==5) ce6=0;
 if(num==6) ce7=0;
 if(num==7) ce0=0;
}


void reset1621x8(void){
  uchar i,j,*p;
  for(j=0;j<8;j++){
   wrr=1;en =1;
      ce0=1;ce1=1;ce2=1;ce3=1;ce4=1;ce5=1;ce6=1;ce7=1;
   panduan1621(j);
    sendHT162xByte(0x04,0x03);	//100
    sendHT162xByte(0x01,0x08);	//8位指令;打开系统振荡器
    sendHT162xByte(0x00,0x01);	//发第9位指令

  wrr=1;en =1;
     ce0=1;ce1=1;ce2=1;ce3=1;ce4=1;ce5=1;ce6=1;ce7=1;
    panduan1621(j);
    sendHT162xByte(0x04,0x03);	//100
    sendHT162xByte(0x03,0x08);	//8位指令;打开LCD偏压发生器
    sendHT162xByte(0x00,0x01);	//发第9位指令

   wrr=1;en =1;
      ce0=1;ce1=1;ce2=1;ce3=1;ce4=1;ce5=1;ce6=1;ce7=1;
    panduan1621(j);
    sendHT162xByte(0x04,0x03);	//100
    sendHT162xByte(0x29,0x08);	//8位指令;4com3bias
    sendHT162xByte(0x00,0x01);	//发第9位指令

   p=htdata;
   wrr=1;en =1;
      ce0=1;ce1=1;ce2=1;ce3=1;ce4=1;ce5=1;ce6=1;ce7=1;
   panduan1621(j);
   sendHT162xByte(0x05,0x03);  //101
   sendHT162xByte(0x00,0x06);  //从0地址开始
   for(i=0x00;i<16;i++){
    sendHT162xByte(*(p++),0x08);
    }
      ce0=1;ce1=1;ce2=1;ce3=1;ce4=1;ce5=1;ce6=1;ce7=1;
 }
}

///////////////////////////////////////////////////////////////
//CRB1029C=依次显示
void dislie1621x8(void){
 uchar i,j,k,n=0;
 for(j=0;j<8;j=j+1){
  for(i=0;i<LCDNUM*2;i=i+1){
   for(k=0;k<4;k=k+1){
   wrr=1;en =1;
      ce0=1;ce1=1;ce2=1;ce3=1;ce4=1;ce5=1;ce6=1;ce7=1;
   panduan1621(j);
   sendHT162xByte(0x05,0x03);	//
   sendHT162xByte(i,0x06);	//
   sendHT162xByte(ddat1621[k],0x04);
   delay(11+11*delayTime);
   displayLED(n/100,(n%100)/10,(n%100)%10);n++;
   }
  }
 }
 displayLED(0,module_num,program_num);
}


void disnum1621x8a(uchar y){
 uchar i,j,tep;
 for(j=0;j<7;j=j+1){
 for(i=0;i<16;i=i+1){
   if(j<3){tep=shuzi_2[y];}
   if(j==3&i<8){tep=shuzi_3[y];}
   if(j==3&i>7){tep=shuzi_2[y];}
   if(j>3){tep=shuzi_1[y];}
   wrr=1;en =1;
      ce0=1;ce1=1;ce2=1;ce3=1;ce4=1;ce5=1;ce6=1;ce7=1;
   panduan1621(j);
   sendHT162xByte(0x05,0x03);	//
   sendHT162xByte(i*2,0x06);	//
   sendHT162xByte(tep,0x04);	//
   wrr=1;en =1;
      ce0=1;ce1=1;ce2=1;ce3=1;ce4=1;ce5=1;ce6=1;ce7=1;
   panduan1621(j);
   sendHT162xByte(0x05,0x03);	//
   sendHT162xByte(i*2+1,0x06);	//
   sendHT162xByte(tep>>4,0x04);	//
      ce0=1;ce1=1;ce2=1;ce3=1;ce4=1;ce5=1;ce6=1;ce7=1;
   }
  }
}



void disnum1621x8b(uchar y){
 uchar i,j,tep;
 for(j=0;j<7;j=j+1){
 for(i=0;i<16;i=i+1){
   if(j==6){
      if(i<4)     {tep=shuzi_6[y];}
      if(i>3&i<10){tep=shuzi_4[y];}
      if(i>9)     {tep=shuzi_6[y];}
     }
   if(j==5){
      if(i<4)     {tep=shuzi_1[y];}
      if(i>3)     {tep=shuzi_8[y];}
     }
   if(j==4){
      if(i<2)     {tep=shuzi_9[y];}
      if(i>1)     {tep=shuzi_6[y];}
      }
   if(j==3){
      if(i<10)    {tep=shuzi_4[y];}
      if(i>9)     {tep=shuzi_6[y];}
     }
   if(j==2){
      if(i<4)    {tep=shuzi_6[y];}   //dgdgfdfgdfg
      if(i>3&i<8){tep=shuzi_a[y];}
      if(i>7)     {tep=shuzi_5[y];}
     }
   if(j==1){tep=shuzi_5[y];}
   if(j==0){tep=shuzi_4[y];}
   wrr=1;en =1;
   ce0=1;ce1=1;ce2=1;ce3=1;ce4=1;ce5=1;ce6=1;ce7=1;
   panduan1621(j);
   sendHT162xByte(0x05,0x03);	//
   sendHT162xByte(i*2,0x06);	//
   sendHT162xByte(tep,0x04);	//
   wrr=1;en =1;
   ce0=1;ce1=1;ce2=1;ce3=1;ce4=1;ce5=1;ce6=1;ce7=1;
   panduan1621(j);
   sendHT162xByte(0x05,0x03);	//
   sendHT162xByte(i*2+1,0x06);	//
   sendHT162xByte(tep>>4,0x04);	//
   ce0=1;ce1=1;ce2=1;ce3=1;ce4=1;ce5=1;ce6=1;ce7=1;
   }
  }
}

void disnum1621x8c(uchar y){
 uchar i,j,tep;
 for(j=0;j<7;j=j+1){
 for(i=0;i<16;i=i+1){
   if(j==6){
        if(i<8)     {tep=shuzi_2[y];}	//
        if(i>7)     {tep=shuzi_b[y];}  //
     }
   if(j==5){
       if(i<13)     {tep=shuzi_2[y];}	//
        if(i>12)     {tep=shuzi_b[y];}  //
  	}
   if(j==4){
   	 tep=shuzi_2[y];
   	}
   if(j==3){
        if(i<4)     {tep=shuzi_b[y];}	//
         if(i>3)     {tep=shuzi_2[y];}  //
  	}
   if(j==2){
         if(i<6)     {tep=shuzi_b[y];}   //dgdgfdfgdfg
        if(i>5)     {tep=shuzi_2[y];}  //
     }
   if(j==1){ //tep=shuziE_a[y];}         //
      if(i<3)      {tep=shuzi_b[y];}
      if(i>2&i<9)  {tep=shuzi_2[y];}
      if(i>8&i<12) {tep=shuzi_9[y];}//?????????????
      if(i>11)     {tep=shuzi_7[y];}  //
	}
   if(j==0){
        if(i<10)    {tep=shuzi_3[y];}   //dgdgfdfgdfg
        if(i>9)     {tep=shuzi_9[y];}  //
	 	}
   wrr=1;en =1;
      ce0=1;ce1=1;ce2=1;ce3=1;ce4=1;ce5=1;ce6=1;ce7=1;
   panduan1621(j);
   sendHT162xByte(0x05,0x03);	//
   sendHT162xByte(i*2,0x06);	//
   sendHT162xByte(tep,0x04);	//
   wrr=1;en =1;
      ce0=1;ce1=1;ce2=1;ce3=1;ce4=1;ce5=1;ce6=1;ce7=1;
   panduan1621(j);
   sendHT162xByte(0x05,0x03);	//
   sendHT162xByte(i*2+1,0x06);	//
   sendHT162xByte(tep>>4,0x04);	//
      ce0=1;ce1=1;ce2=1;ce3=1;ce4=1;ce5=1;ce6=1;ce7=1;
   }
  }
}


void disnum1621x8d(uchar y){
  uchar i,j,tep;

 for(j=0;j<7;j=j+1){
 for(i=0;i<16;i=i+1){
   if(j==6){     //=ic3
        if(i<10)     {tep=shuzi_a[y];}	//
        if(i>9)     {tep=shuzi_5[y];}  //
     }
   if(j==5){      //=ic1
       if(i<12)     {tep=shuzi_9[y];}	//
        if(i>11)     {tep=shuzi_3[y];}  //
  	}
   if(j==4){      //=ic2
         if(i<2)     {tep=shuzi_9[y];}	//
        if(i>1)     {tep=shuzi_a[y];}  //
	}

   if(j==3){     //=ic4
           if(i<16)     {tep=shuzi_5[y];}  //
	}
   if(j==2){      //=ic5
            if(i<16)     {tep=shuzi_5[y];}  //
	}
   if(j==1){  //=ic6
          if(i<6)     {tep=shuzi_5[y];}	//
        if(i>5)     {tep=shuzi_a[y];}  //
	}
   if(j==0){   //=ic7
        if(i<10)    {tep=shuzi_5[y];}   //b
        if(i>9)     {tep=shuzi_a[y];}  //a
	}
   wrr=1;en =1;
   ce0=1;ce1=1;ce2=1;ce3=1;ce4=1;ce5=1;ce6=1;ce7=1;
   panduan1621(j);
   sendHT162xByte(0x05,0x03);	//
   sendHT162xByte(i*2,0x06);	//
   sendHT162xByte(tep,0x04);	//
   wrr=1;en =1;
   ce0=1;ce1=1;ce2=1;ce3=1;ce4=1;ce5=1;ce6=1;ce7=1;
   panduan1621(j);
   sendHT162xByte(0x05,0x03);	//
   sendHT162xByte(i*2+1,0x06);	//
   sendHT162xByte(tep>>4,0x04);	//
   ce0=1;ce1=1;ce2=1;ce3=1;ce4=1;ce5=1;ce6=1;ce7=1;
   }
  }
}

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

void disnum1621x8e(uchar y){
 uchar i,j,tep;
 for(j=0;j<8;j=j+1){
 for(i=0;i<16;i=i+1){
   if(j==7){     //=ic3
        if(i<4)     {tep=shuzi_a[y];}	//
        if(i>3)     {tep=shuzi_5[y];}  //
     }
   if(j==6){     //=ic3
        if(i<10)     {tep=shuzi_9[y];}	//
        if(i>9)     {tep=shuzi_3[y];}  //
     }
   if(j==5){      //=ic1
       if(i<6)     {tep=shuzi_8[y];}	//
        if(i>5)     {tep=shuzi_8[y];}  //
  	}
   if(j==4){      //=ic2
        if(i<4)    {tep=shuzi_a[y];}
       if(i>3&i<6) {tep=shuzi_a[y];}
       if(i>5&i<10){tep=shuzi_5[y];}
      if(i>9)      {tep=shuzi_a[y];}
	}
   if(j==3){     //=ic4
       if(i<4)     {tep=shuzi_5[y];}
       if(i>3)     {tep=shuzi_a[y];}
	}
   if(j==2){      //=ic5
            if(i<6)     {tep=shuzi_a[y];}  //
            if(i>5)     {tep=shuzi_5[y];}  //
	}
   if(j==1){  //=ic6
          if(i<16)     {tep=shuzi_a[y];}	//
	}
   if(j==0){   //=ic7
       if(i<12)     {tep=shuzi_a[y];}
      if(i>11)     {tep=shuzi_5[y];}
	}
   wrr=1;en =1;
   ce0=1;ce1=1;ce2=1;ce3=1;ce4=1;ce5=1;ce6=1;ce7=1;
   panduan1621(j);
   sendHT162xByte(0x05,0x03);	//
   sendHT162xByte(i*2,0x06);	//
   sendHT162xByte(tep,0x04);	//
   wrr=1;en =1;
   ce0=1;ce1=1;ce2=1;ce3=1;ce4=1;ce5=1;ce6=1;ce7=1;
   panduan1621(j);
   sendHT162xByte(0x05,0x03);	//
   sendHT162xByte(i*2+1,0x06);	//
   sendHT162xByte(tep>>4,0x04);	//
   ce0=1;ce1=1;ce2=1;ce3=1;ce4=1;ce5=1;ce6=1;ce7=1;
   }
  }
}


 void test_1621x8_kaoyanji(void){
  uchar p,i,j;

   DUTYBIAS=0x29; LCDNUM=16;
   j=0;

   sethtdata(0x00,0x00);
   reset1621x8();

   for(p=0;p<xunhuan;p++){
   	sethtdata(0xff,0xff); reset1621x8(); delay(222+222*delayTime);
   	sethtdata(0x00,0x00); reset1621x8(); delay(222+222*delayTime);
    sethtdata(0x55,0x55); reset1621x8(); delay(222+222*delayTime);
    sethtdata(0xaa,0xaa); reset1621x8(); delay(222+222*delayTime);


   	for(i=0;i<10;i++){
    	if(program_num==0){disnum1621x8a(i);delay(222+222*delayTime);}
    	if(program_num==1){disnum1621x8b(i);delay(222+222*delayTime);}
    	if(program_num==2){disnum1621x8c(i);delay(222+222*delayTime);}
    	if(program_num==3){disnum1621x8d(i);delay(222+222*delayTime);}
    	if(program_num==4){disnum1621x8e(i);delay(222+222*delayTime);}
     }

   sethtdata(0x00,0x00); reset1621x8();

   sethtdata(0x11,0x11); reset1621x8(); delay(222+222*delayTime);
   sethtdata(0x22,0x22); reset1621x8(); delay(222+222*delayTime);
   sethtdata(0x44,0x44); reset1621x8(); delay(222+222*delayTime);
   sethtdata(0x88,0x88); reset1621x8(); delay(222+222*delayTime);

   sethtdata(0x00,0x00); reset1621x8();

   dislie1621x8();
 }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

//***********************************************************
//   程序名称: retdis                                       *
//   程序用途: 将DMF50081初始化成3层图形方式                *
//***********************************************************

/*设定三个显示区域为:第一显示区   0000H----27FFH  (10K容量)
                     第二显示区   2800H----4FFFH  (10K容量)
                     第三显示区   5000H----77FFH  (10K容量)*/

uchar code LCDSYSINIT[8]			={0x30,0x87,0x07,0x27,0x30,0xf0,0x28,0x00};
uchar code LCDSCRINIT[10]			={0x00,0x00,0xf0,0x00,0x40,0xf0,0x00,0x80,0x00,0x00};      //P1-P10参数






uchar code SET1335_320240A[8]	={0x30,0x87,0x07,0x27,0x30,0xf0,0x28,0x00};                //P1-P8参数
//                                             40*8=320seg
uchar code SET1335_320240B[10]={0x00,0x00,0xf0,0x00,0x40,0xf0,0x00,0x80,0x00,0x00};      //P1-P10参数0xf0=
//                                        240duty        240duty


uchar code SET1335_320160A[8]	={0x30,0x87,0x07,0x27,0x45,0xa0,0x28,0x00};                //P1-P8参数
//                                             40*8=320seg
uchar code SET1335_320160B[10]={0x00,0x00,0xa0,0x00,0x40,0xa0,0x00,0x80,0x00,0x00};      //P1-P10参数
//                                        160duty        160duty


uchar code SET1335_32080A[8]	={0x30,0x87,0x07,0x27,0x8B,0x50,0x28,0x00};                //P1-P8参数
//                                             40*8=320seg
uchar code SET1335_32080B[10]	={0x00,0x00,0x50,0x00,0x40,0x50,0x00,0x80,0x00,0x00};      //P1-P10参数
//                                        80duty         80duty



//P1-P8参数            		 	  	P1   P2   P3   P4   P5   P6   P7   P8
uchar code SET1335_24064A[8]	={0x30,0x87,0x07,0x1d,0x8B,0x50,0x28,0x00};                //P1-P8参数
//                                             30*8=240seg
uchar code SET1335_24064B[10]	={0x00,0x00,0x40,0x00,0x40,0x40,0x00,0x80,0x00,0x00};      //P1-P10参数
//                                        64duty         64duty


//Q1-Q10参数           		 	   	Q1   Q2   Q3   Q4   Q5   Q6   Q7   Q8   Q9   Q10




//P4= CR        0x1d          /*定义显示窗口的长度,以字符为单位39*/
//P5= TCR       49            /*定义LCD的时间常数45.这个时间常数应该尽量小(场频就大)*/
//P6= LF        0x3f          /*定义LCD的行数239*/
//P7= APL       0x20          /*定义LCD一行所占缓冲区的字节数,低8位40*/
//P8= APH       0             /*定义LCD一行所占缓冲区的字节数,高8位00*/
//

// SAD1      0x0000        /*定义显示一区首址*/
// SAD1L     0x00          /*定义显示一区首址低8位*/
// SAD1H     0x00          /*定义显示一区首址高8位*/
// SL1       64           /*定义显示一区所占显示屏幕的行数*/
//
// SAD2      0x0200        /*定义显示二区首址*/
// SAD2L     0x00          /*定义显示二区首址低8位*/
// SAD2H     0x02          /*定义显示二区首址高8位*/
// SL2       127           /*定义显示二区所占显示屏幕的行数*/
//
// SAD3      0x1c00        /*定义显示三区首址*/
// SAD3L     0x00          /*定义显示三区首址低8位*/
// SAD3H     0x1c          /*定义显示三区首址高8位*/
//
// SAD4      0x2600        /*定义显示四区首址*/
// SAD4L     0x00          /*定义显示四区首址低8位*/
// SAD4H     0x26          /*定义显示四区首址高8位*/




/////////////////////////////////////////////////////////////////////////////////
//   程序名称: clear
//   程序用途: 清屏幕
/////////////////////////////////////////////////////////////////////////////////
void lcd_cls_1335(uchar part){
 uint i = 10240;   		//10240 = 0x2800  10KB RAM
 cs1=0;
 wd_6800c(0x46);      //Write cursor address
 wi_6800c(0x00);      //First write the low eight
      if(part == 1) wi_6800c(0x00);   //1area
 else if(part == 2) wi_6800c(0x28);		//2area
 else if(part == 3) wi_6800c(0x50);		//3area
 else if(part == 4) wi_6800c(0x00);		//4area
 else {i = 32768;   wi_6800c(0x00);}	//All dispay area 32KB RAM
 wd_6800c(0x4c);   					//Cursor right direction
 wd_6800c(0x42);    				//write data
 while(i--) wi_6800c(0x00);	//Write 0
}

/////////////////////////////////////////////////////////////////////////////////
void retdis_1335(void)     {
 uchar i;

 	RST=0;
	delay_ms(33);
	RST=1;

	CSB=0;			//CSB=0;



   wd_6800c(0x40);                          //送入SYS命令
   	for(i=0;i<8;i++) {

     	if(program_num==0){wi_6800c(SET1335_24064A[i]) ;}
     	if(program_num==1){wi_6800c(SET1335_32080A[i]) ;}
     	if(program_num==2){wi_6800c(SET1335_320160A[i]);}
     	if(program_num==3){wi_6800c(SET1335_320240A[i]);}
     	if(program_num==4){wi_6800c(SET1335_320240A[i]);}
			_nop_();
       }                                		//送入8个SYSTERM数据
   wd_6800c(0x44);                          //送入SCROLL命令
    for(i=0;i<10;i++){
     	if(program_num==0){wi_6800c(SET1335_24064B[i]) ;}
     	if(program_num==1){wi_6800c(SET1335_32080B[i]) ;}
      if(program_num==2){wi_6800c(SET1335_320160B[i]);}
      if(program_num==3){wi_6800c(SET1335_320240B[i]);}
    	if(program_num==4){wi_6800c(SET1335_320240B[i]);}
       _nop_();
       }                              			//送入十个SCROLL数据

   wd_6800c(0x5a);  	//送入HDOTSCR命令
   wi_6800c(0x00);  	//送入HDOTSCR数据

   wd_6800c(0x5b);  	//送入OVLAY命令
   wi_6800c(0x00);  	//送入OVLAY数据,显示方式==(L1*L2)+L3 10图形


   wd_6800c(0x59);  	//开关显示
   wi_6800c(0x54);   	//开启一.二四.三, 关闭光标old 0x55

 }

/////////////////////////////////////////////////////////////////////////////////
void dis1335_char_0(void)  {
 uchar i,j,k;
  cs1=0;
  k=0x20;
  wd_6800c(0x4c);             	//CSRDIR 代码
  wd_6800c(0x46);             	//CSRW 代码
  wi_6800c(0x00);
  wi_6800c(0x00);
  wd_6800c(0x42);              	//连续写
   for(i=0;i<30;i++) {	        //清RAM区
   for(j=0;j<40;j++) {	        //清RAM区
    wi_6800c(k); k++;
  }
 }
}


/////////////////////////////////////////////////////////////////////////////////
//写两个字符到dot区域
void dis1335_dot_1(uchar dat1,dat2)  {
 uchar i,j;
  cs1=0;
  wd_6800c(0x59);  			//开关显示
  wi_6800c(0x10);  			//开启一.二四.三, 关闭光标old 0x55

  wd_6800c(0x5B); 			//送入OVLAY命令
  wi_6800c(0x01);  			//送入OVLAY数据,显示方式==(L1*L2)+L3 10图形

  wd_6800c(0x4c);				//CSRDIR 代码
  wd_6800c(0x46);
  wi_6800c(0x00);
  wi_6800c(0x40);

  wd_6800c(0x42);				//连续写

  for(i=0;i<120;i++) {	//清RAM区
   for(j=0;j<40;j++) { wi_6800c(dat1); }
   for(j=0;j<40;j++) { wi_6800c(dat2); }
  }
 }



//设定图形x,y值
void ag_1335(uchar xxx,yyy) {
 uint temp,tt;
 uchar d1,d2;
  temp=40*yyy;
  tt=xxx;
  temp=temp+tt;
  temp=temp+0x4000;
  d2=temp/256;
  d1=temp%256;

  wd_6800c(0x46);      	//Write cursor address
  wi_6800c(d1);      		//First write the low eight
  wi_6800c(d2);      		//First write the low eight

  wd_6800c(0x4c);   		//Cursor right direction
  wd_6800c(0x42);    		//write data  连续
}



///////////////////////////////////////////////////////////////
//写任意16*8字符x=列;y=行;z=字符;m是否反显;以16*8为坐标
 void shuzi_1335(uchar x,y,z) {
	uchar i,k=0;
 	for (i=0;i<16;i++) {
  	ag_1335(x,y+i);
   	wi_6800c(hz0816v[z][k]);
    k++;
	 }
 }

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
//画方框128*64=S6B0108;
void window_1335(void) {
 uchar  k;
 uint  xx,yy,p1,p2,p3,p4,p5,p6;

/*
    	if(program_num==0|program_num==4){wi_6800c(SET1335_32080A[i]); }
     	if(program_num==1){wi_6800c(SET1335_320160A[i]);}
     	if(program_num==2){wi_6800c(SET1335_320240A[i]);}
     	if(program_num==3){wi_6800c(SET1335_320240A[i]);}
*/

    	if(program_num==0){yy=60,xx=64;} 	  //24064
    	if(program_num==1){yy=80,xx=80;}		//32080
     	if(program_num==2){yy=80,xx=160;}		//320160
     	if(program_num==3){yy=80,xx=240;} 	//320240
     	if(program_num==4){yy=80,xx=240;} 	//320240


 for(k=0;k<xx;k++) {
  wd_6800c(0x46);      					//Write cursor address
  wi_6800c((0x4000+40*k)%256);	//First write the low eight
  wi_6800c((0x4000+40*k)/256);
  wd_6800c(0x4c);   						//Cursor right direction
  wd_6800c(0x42);    						//write data  连续
  wi_6800c(0xc0);
  }


 for(k=0;k<xx;k++) {
  wd_6800c(0x46);      					//Write cursor address
  wi_6800c((0x4027+40*k)%256); 	//First write the low eight
  wi_6800c((0x4027+40*k)/256);
  wd_6800c(0x4c);   						//Cursor right direction
  wd_6800c(0x42);    						//write data  连续
  wi_6800c(0x03);
  }


 	wd_6800c(0x46);      					//Write cursor address
 	wi_6800c(0x00);      					//First write the low eight
 	wi_6800c(0x40);
 	wd_6800c(0x4c);   							//Cursor right direction
 	wd_6800c(0x42);    						//write data  连续
 	for(k=0;k<yy;k++) {wi_6800c(0xff);}

 	wd_6800c(0x46);      					//Write cursor address
 	wi_6800c(0x30);      					//First write the low eight
 	wi_6800c(0x65);
 	wd_6800c(0x4c);   							//Cursor right direction
 	wd_6800c(0x42);    						//write data  连续
 	for(k=0;k<yy;k++) {wi_6800c(0xff);}

 	p1=yy*4/100;
 	p2=yy*4%100/10;
 	p3=yy*4%100%10;

 	p4=xx/100;
 	p5=xx%100/10;
 	p6=xx%100%10;



 	shuzi_1335(4,20,p1);
 	shuzi_1335(5,20,p2);
 	shuzi_1335(6,20,p3);
 	shuzi_1335(7,20,p4);
 	shuzi_1335(8,20,p5);
 	shuzi_1335(9,20,p6);


}


 void test_sed1335(void){
 uchar p;
 retdis_1335();

 for(p=0;p<xunhuan;p++){

  retdis_1335();

  lcd_cls_1335(0);
  dis1335_char_0();	delay(999+999*delayTime);

//  lcd_cls_1335(0);
 	dis1335_dot_1(0x00,0x00);

  window_1335();  	delay(999+999*delayTime);

 	dis1335_dot_1(0xff,0x00); delay(222+222*delayTime);
 	dis1335_dot_1(0x00,0xff); delay(222+222*delayTime);
 	dis1335_dot_1(0xaa,0xaa); delay(222+222*delayTime);
 	dis1335_dot_1(0x55,0x55); delay(222+222*delayTime);
 	dis1335_dot_1(0xaa,0x55); delay(222+222*delayTime);
 	dis1335_dot_1(0x55,0xaa); delay(222+222*delayTime);

 }
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////



//=====================================================
//240*128=T6963=TOSHIBA==忙标志-读取
//=====================================================
uchar busy6963(void) {	 /*数据,指令读写判别,忙标志*/
        uchar a;
	rs=1;			_nop_();
	en=1;			_nop_();
	wrr=1;		_nop_();
	P1=0xff;	_nop_();
 	en=0; 		dly();
	a=P1;			_nop_();
	en=1;			_nop_();
	return(a);
}
///////////////////////////////////////////////////////
//数据,指令读写判别
void pan1_6963(void) {
 	uchar k,m;
 	k=80;
 	do {m=busy6963()&3;
 	    k--;}
	while (m!=3&&k==0);
}
///////////////////////////////////////////////////////
//数据自动读判别
void pan2_6963(void) {
	uchar k,m;
 	k=80;
 	do {m=busy6963()&4;
 	    k--;}
	while (m!=4&&k==0);
}
///////////////////////////////////////////////////////
//数据自动写判别
void pan3_6963(void) {
	uchar k,m;
 	k=80;
 	do {m=busy6963()&8;
 	    k--;}
	while (m!=8&&k==0);
}
///////////////////////////////////////////////////////
//控制指令
void ctrl_6963(uchar dat) {
	pan1_6963();
	rs=1;			_nop_();
	P1=dat;		_nop_();
	wrr=0;		dly();
	wrr=1;		_nop_();
}
///////////////////////////////////////////////////////
//写数据
  void wi_6963(uchar dat) {
	pan1_6963();
	rs=0;			_nop_();
	P1=dat;		_nop_();
	wrr=0; 		dly();
	wrr=1;		_nop_();
}
///////////////////////////////////////////////////////
//自动写
void autowi_6963(uchar dat){
	pan3_6963();
	rs=0;			_nop_();
	P1=dat;		_nop_();
	wrr=0;		dly();
	wrr=1;		_nop_();
}
///////////////////////////////////////////////////////
//设定图形x,y值
void ag_6963(uchar x,y) {
	unsigned int xy;
	xy=y;
	xy=xy*32+x+512;
	wi_6963(xy&0xff);
	wi_6963(xy/256);
	ctrl_6963(0x24);
}

///////////////////////////////////////////////////////
//设定文本x,y值
void at_6963(uchar x,y) {
	wi_6963(y*32+x);
	wi_6963(0);
	ctrl_6963(0x24);
}
///////////////////////////////////////////////////////
//写图形文本数据
void wdata_6963(uchar z) {
 	wi_6963(z);
 	ctrl_6963(0xc0);
}
///////////////////////////////////////////////////////
//LCD清屏t6963
void cls_6963(void) {
 	uint i;
	wi_6963(0);
	wi_6963(0);
	ctrl_6963(0x24); ;			/*设定文本x,y值*/
	ctrl_6963(0xb0); 				/*自动写方式*/
	for (i=0;i<0x1234;i++) {
		autowi_6963(0x00);
	}
	ctrl_6963(0xb2); 				/*结束自动写方式*/
  ctrl_6963(0x9c);
}

/*;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;*/
/*写任意8*16字符x=列;y=行;z=字符;m是否反显;以8*8为坐标*/
/*;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;*/

void display6963_0816(uchar x,y,z) {
	unsigned char i,j,k;
	k=0;
	for (i=0;i<16;i++) {
		ag_6963(j+x,y*8+i);
		wdata_6963(hz0816v[z][k]);
	 	k++;
	}
 }

///////////////////////////////////////////////////////
//写240128屏幕数据,双数据
void wdata_6963two(uchar da1,uchar da2) {
  uchar i,j;
   ag_6963(0,0);
  for(i=0;i<64;i++){
   for(j=0;j<32;j++){
    wdata_6963(da1);
    }
   for(j=0;j<32;j++){
    wdata_6963(da2);
    }
   }
  }




//写任意一幅图象240*128; T6963
void window_6963(void) {

 uchar xx,yy,k,i;
 uint p1,p2,p3,p4,p5,p6;
	if(program_num==0){xx=128;yy=64; k=15;}		////12864
	if(program_num==1){xx=240;yy=64; k=29;}		////24064
	if(program_num==2){xx=160;yy=128;k=19;}		////160128
	if(program_num==3){xx=240;yy=128;k=29;}		////240128
	if(program_num==4){xx=160;yy=80; k=19;}		////16080
	if(program_num==5){xx=240;yy=80; k=29;}		////24080

 	cls_6963();

  for(i=0;i<yy;i++){
	ag_6963(0,i);
  wi_6963(0x80);
  ctrl_6963(0xc0);
	}

  for(i=0;i<yy;i++){
	ag_6963(k,i);
  wi_6963(0x01);
  ctrl_6963(0xc0);
	}

	ag_6963(0,0);
  for(i=0;i<30;i++){
	 wi_6963(0xff);
   ctrl_6963(0xc0);
   }

	ag_6963(0,yy-1);
  for(i=0;i<30;i++){
	 wi_6963(0xff);
   ctrl_6963(0xc0);
   }


 	p1=xx/100;
 	p2=xx%100/10;
 	p3=xx%100%10;

 	p4=yy/100;
 	p5=yy%100/10;
 	p6=yy%100%10;

 	ag_6963(0,0);
  display6963_0816(1,1,p1);
  display6963_0816(2,1,p2);
  display6963_0816(3,1,p3);
  display6963_0816(4,1,p4);
  display6963_0816(5,1,p5);
  display6963_0816(6,1,p6);

}

//写任意一幅图象240*128; T6963
void distu_6963(uchar *datt) {
  uchar xx,yy,i,j;
 	if(program_num==0){xx=128;yy=64; }		////12864
	if(program_num==1){xx=240;yy=64; }		////24064
	if(program_num==2){xx=160;yy=128;}		////160128
	if(program_num==3){xx=240;yy=128;}		////240128
	if(program_num==4){xx=160;yy=80; }		////16080
	if(program_num==5){xx=240;yy=80; }		////24080

  xx=xx/8;

  for(i=0;i<yy;i++){
   ag_6963(0,i);
   for(j=0;j<xx;j++){
     wi_6963(*(datt++));
     ctrl_6963(0xc0);
   }
  }
 }


///////////////////////////////////////////////////////
//LCD初始化t6963
void reset_6963(void) {


  RST=0;
	delay_ms(66);
	RST=1;

	CSB=0;


 	wi_6963(0x00);	dely();
 	wi_6963(0x00);	dely();
 	ctrl_6963(0x40);dely();				/*文本首址0000h*/

	wi_6963(32);		dely();
	wi_6963(0x00);	dely();
	ctrl_6963(0x41);dely();				/*文本区域,每行30字节32*/

	wi_6963(0x00);  dely();
	wi_6963(0x02);  dely();
	ctrl_6963(0x42);dely();				/*图形首址0200h*/

  wi_6963(32);		dely();
  wi_6963(0x00);	dely();
  ctrl_6963(0x43);dely();				/*图形区域,每行30字节32*/

	ctrl_6963(0x81);dely();				/*显示方式:文本图形异或*/
	ctrl_6963(0x90);dely();				/*显示开关:文本图形开显示*/
	ctrl_6963(0xa0);dely();				/*光标形状*/

}




 void test_T6963(void){
  uchar p,k,i,j;

  reset_6963();

  for(p=0;p<xunhuan;p++){

  reset_6963();
  cls_6963();

	window_6963();					delay(222+222*delayTime); delay(222+222*delayTime);





 	if(program_num==0){distu_6963(tu128064V1);}		////12864
	if(program_num==1){distu_6963(tu240064V1);}		////24064
	if(program_num==2){distu_6963(tu160128V1);}		////160128x
	if(program_num==3){distu_6963(tu240128V1);}		////240128
  if(program_num==4){distu_6963(tu160080V1);}		////16080x
 	if(program_num==5){distu_6963(tu240080V1);}		////24080x


  delay(222+222*delayTime);
  delay(222+222*delayTime);



	cls_6963();
	at_6963(0,0);
      k=0;
      for(i=0;i<4;i++){
       for(j=0;j<32;j++){wdata_6963(k);k++;dely();}
       }
      k=0;
      for(i=0;i<4;i++){
       for(j=0;j<32;j++){wdata_6963(k);k++;dely();}

       }
      k=0;
      for(i=0;i<4;i++){
       for(j=0;j<32;j++){wdata_6963(k);k++;dely();}
       }
      k=0;
      for(i=0;i<4;i++){
       for(j=0;j<32;j++){wdata_6963(k);k++;dely();}
       }
   delay(555+555*delayTime*2);

   cls_6963();
   wdata_6963two(0xff,0x00);       delay(222+222*delayTime);
   wdata_6963two(0x00,0xff);       delay(222+222*delayTime);
   wdata_6963two(0x55,0x55);       delay(222+222*delayTime);
   wdata_6963two(0xaa,0xaa);       delay(222+222*delayTime);
   wdata_6963two(0x55,0xaa);       delay(222+222*delayTime);
   wdata_6963two(0xaa,0x55);       delay(222+222*delayTime);


 }
}




////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////




///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
//**************iic起使信号*******************
void IIC2_start(void){
  IIC2SCL=1; 	dly();   	//SDA
  IIC2SDA=1;	dly();   	//SCL
  IIC2SCL=0;	dly();
  IIC2SDA=0;	dly();
}
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
//**************iic结束信号*******************
void IIC2_end(void){
  IIC2SCL=0;	dly();
  IIC2SDA=1; 	dly();
  IIC2SCL=1;	dly();
}
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
//**************对IIC总线产生应答*************
//void IIC2ACK(void){
//	IIC2SCL=0;	dly();
//	IIC2SDA=1; 	dly();
//	IIC2SDA=0; 	dly();
//	IIC2SCL=1;	dly();
//}
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
//*****************不对IIC总线产生应答********
//void IIC2NoAck(void){
//	IIC2SCL=1;	dly();
//	IIC2SDA=1; 	dly();
//	IIC2SDA=0;	dly();
//	IIC2SCL=0;	dly();
//}
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
//*****************iic应答信号****************
void read2ack(void){
  uchar k;
  k=50;
  IIC2SCL=1;	dly();
  IIC2SDA=1; 	dly();
  do{ dly();  dly();
    	k--;
   	}while(IIC2SCL==1&&k!=0);
  IIC2SDA=0;	dly();
 }
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
//*****************iic-H****************
void high2(void){
  IIC2SCL=1;	dly();
  IIC2SDA=1;		dly();dly();
  IIC2SDA=0;		dly();
}
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
//*****************iic-L****************
void low2(void){
  IIC2SCL=0;	dly();
  IIC2SDA=1;		dly();dly();
  IIC2SDA=0;		dly();
}

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
//*****************iic写入八位数据*********
void IIC2_wrbyte(uchar x){
  uchar i;
  for (i=0;i<8;i++){
   if (x>=0x80) high2();
     else low2();
    x=x<<1;
   }
  read2ack();
}


///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
//*****************iic读出八位数据*********
//uchar IIC2_rdbyte(void) {
//   uchar i,da2=0xff;
//   for(i=0;i<8;i++) {
//   IIC2SDA=1;	_nop_();
//   da2=(da2<<1)|IIC2SCL;
//   dly();
//   IIC2SDA=0;
//  }
//  return da2;
//}
//
//
void wr8576(void){
 uchar i,nuu,*p;

 if(program_num==0){nuu=20;}		//1/4Duty,1/3Bias;
 if(program_num==1){nuu=20;}		//1/4Duty,1/2Bias;
 if(program_num==2){nuu=14;}		//1/3Duty,1/3Bias;
 if(program_num==3){nuu=14;}		//1/3Duty,1/2Bias;
 if(program_num==4){nuu=10;}		//1/2Duty,1/2Bias;
 if(program_num==5){nuu=5;}			//1/1Duty,1/1Bias;



  p=htdata;
  IIC2_start();
  IIC2_wrbyte(0x70);  					//地址000
  IIC2_wrbyte(PCF8576DUTY); 		//功能
  IIC2_wrbyte(0xe0);  					//地址000
  IIC2_wrbyte(0x80); 						//RAM地址00开始
  IIC2_wrbyte(0x70);  					//地址000
  for(i=0;i<nuu;i++) {
   IIC2_wrbyte(*(p++));
  }
  IIC2_end();

  p=htdata;
  IIC2_start();
  IIC2_wrbyte(0x70);  					//地址000
  IIC2_wrbyte(PCF8576DUTY); 		//功能
  IIC2_wrbyte(0xe1);  					//地址000
  IIC2_wrbyte(0x80); 						//RAM地址00开始
  IIC2_wrbyte(0x70);  					//地址000
  for(i=0;i<nuu;i++) {
   IIC2_wrbyte(*(p++));
  }
  IIC2_end();

}


void wr8576_lie(void){
 uchar i,j,k,n;
 uchar nuu;
 if(program_num==0){nuu=20;}		//1/4Duty,1/3Bias;
 if(program_num==1){nuu=20;}		//1/4Duty,1/2Bias;
 if(program_num==2){nuu=14;}		//1/3Duty,1/3Bias;
 if(program_num==3){nuu=14;}		//1/3Duty,1/2Bias;
 if(program_num==4){nuu=10;}		//1/2Duty,1/2Bias;
 if(program_num==5){nuu=5;}			//1/1Duty,1/1Bias;

 n=0;

 for(i=0;i<nuu;i++){
 	for(j=0;j<8;j++){
   wi80bit_wcgram(ddat1621[j],i);

 IIC2_start();
 IIC2_wrbyte(0x70);  					//地址000
 IIC2_wrbyte(PCF8576DUTY); 		//功能
 IIC2_wrbyte(0xe0);  					//地址000
 IIC2_wrbyte(0x80); 					//RAM地址00开始
 IIC2_wrbyte(0x70);  					//地址000
 for(k=0;k<nuu;k++) {
  IIC2_wrbyte(htdata[k]);
 }
 IIC2_end();

  IIC2_start();
 IIC2_wrbyte(0x70);  					//地址000
 IIC2_wrbyte(PCF8576DUTY); 		//功能
 IIC2_wrbyte(0xe1);  					//地址000
 IIC2_wrbyte(0x80); 					//RAM地址00开始
 IIC2_wrbyte(0x70);  					//地址000
 for(k=0;k<nuu;k++) {
  IIC2_wrbyte(htdata[k]);
  }
  IIC2_end();
  delay(33+33*delayTime);

  }
 }
}

 void test_PCF8576_segment(void){
  uchar p;

  sethtdata(0xff,0xff); wr8576(); delay(222+222*delayTime);
  sethtdata(0x00,0x00); wr8576(); delay(222+222*delayTime);

 for(p=0;p<xunhuan;p++){
  sethtdata(0xff,0xff); wr8576(); delay(222+222*delayTime);
  sethtdata(0x00,0x00); wr8576(); delay(222+222*delayTime);

  sethtdata(0x11,0x11); wr8576(); delay(222+222*delayTime);
  sethtdata(0x22,0x22); wr8576(); delay(222+222*delayTime);
  sethtdata(0x44,0x44); wr8576(); delay(222+222*delayTime);
  sethtdata(0x88,0x88); wr8576(); delay(222+222*delayTime);

  sethtdata(0x55,0x55); wr8576(); delay(222+222*delayTime);
  sethtdata(0xaa,0xaa); wr8576(); delay(222+222*delayTime);
  sethtdata(0x55,0xaa); wr8576(); delay(222+222*delayTime);
  sethtdata(0xaa,0x55); wr8576(); delay(222+222*delayTime);

  sethtdata(0x00,0x00); wr8576(); wr8576_lie();
  _nop_();
  }
}









////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

//==============================================================================
//ST7920P68模式
//==============================================================================
////////////////////////////////////////////////////////////////////////////////
//ST7920P68模式==复位成字符模式
void reset_st7920p1_character(void) {
 wi_6800c(0x30); delay_ms(44);
 wi_6800c(0x30); delay_ms(44);
 wi_6800c(0x30); delay_ms(22);

 wi_6800c(0x0c); delay_ms(11);  	//开示
 wi_6800c(0x01); delay_ms(22);  	//清屏
 wi_6800c(0x06); delay_ms(11);  	//输入方式
 }
////////////////////////////////////////////////////////////////////////////////
//复位成图形模式
 void reset_st7920p1_matrix(void) {
 wi_6800c(0x36); delay_ms(44);
 wi_6800c(0x36); delay_ms(44);
 wi_6800c(0x36); delay_ms(22);
 }

////////////////////////////////////////////////////////////////////////////////
//ST7920P68模式==复位成字符模式
void reset_st7920p2_character(void) {
 wi_6800d(0x30); delay_ms(44);
 wi_6800d(0x30); delay_ms(44);
 wi_6800d(0x30); delay_ms(22);

 wi_6800d(0x0c); delay_ms(11);  	//开示
 wi_6800d(0x01); delay_ms(22);  	//清屏
 wi_6800d(0x06); delay_ms(11);  	//输入方式
 }
////////////////////////////////////////////////////////////////////////////////
//复位成图形模式
void reset_st7920p2_matrix(void) {
 wi_6800d(0x36); delay_ms(44);
 wi_6800d(0x36); delay_ms(44);
 wi_6800d(0x36); delay_ms(22);
 }



void wcgram_st7920p1(void) {      //写CGRAM;datt
 uchar i;
 wi_6800c(0x40); dely();
 for (i=0;i<64;i++){
 	wd_6800c(datt[i]);
 }
}

void wcgram_st7920p2(void) {      //写CGRAM;datt
 uchar i;
 wi_6800d(0x40); dely();
 for (i=0;i<64;i++){
 	wd_6800d(datt[i]);
 }
}

void disicon_st7920p1(uchar dat1,dat2) {
 uchar i;
  wi_6800c(0x80);
  for(i=0;i<33;i++) {
  wd_6800c(dat1);
  wd_6800c(dat2); ;
  }
}
void disicon_st7920p2(uchar dat1,dat2) {
 uchar i;
  wi_6800d(0x80);
  for(i=0;i<33;i++) {
  wd_6800d(dat1);
  wd_6800d(dat2); ;
  }
}



/////////////////////////////////////////////////////////////////////////////////
//写字符串到st7920
void wr_st7920p1_character_string(unsigned char *str) {
 uchar i;
  for(i=0;i<16;i++) {
  wd_6800c(*str);
  wd_6800c(*(str+1));
  str++;str++;
  }
}
/////////////////////////////////////////////////////////////////////////////////
//写字符串到st7920
void wr_st7920p2_character_string(unsigned char *str) {
 uchar i;
  for(i=0;i<16;i++) {
  wd_6800d(*str);
  wd_6800d(*(str+1));
  str++;str++;
  }
}


/////////////////////////////////////////////////////////////////////////////////
//写2个字符到st7920
void all_st7920p1(uchar dat1,dat2) {
 uchar i,j;

 reset_st7920p1_matrix();
 for(i=0;i<16;i++){
  wi_6800c(0x80+i*2);
  wi_6800c(0x80);
  for(j=0;j<32;j++){wd_6800c(dat1);  }    //*tu
  wi_6800c(0x81+i*2);
  wi_6800c(0x80);
  for(j=0;j<32;j++){wd_6800c(dat2);  }    //*tu
 }
}
/////////////////////////////////////////////////////////////////////////////////
//写2个字符到st7920
void all_st7920p2(uchar dat1,dat2) {
 uchar i,j;
 reset_st7920p2_matrix();
 for(i=0;i<16;i++){
  wi_6800d(0x80+i*2);
  wi_6800d(0x80);
  for(j=0;j<32;j++){wd_6800d(dat1);  }    //*tu
  wi_6800d(0x81+i*2);
  wi_6800d(0x80);
  for(j=0;j<32;j++){wd_6800d(dat2);  }    //*tu
 }
}



/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////


void distu_7920p_25632(uchar *mintu) {
 uchar i,j;

  reset_st7920p1_matrix();

  for(i=0;i<32;i++){
   wi_6800c(0x80+i);
   wi_6800c(0x80);
   for(j=0;j<32;j++){
    wd_6800c(*(mintu++));
   }
  }
}

void distu_7920p_12864(uchar *mintu) {
 uchar i,j;

  reset_st7920p1_matrix();


  for(i=0;i<32;i++){
   wi_6800c(0x80+i);
   wi_6800c(0x80);
   for(j=0;j<16;j++){
    wd_6800c(*(mintu++));
   }
  }

  for(i=0;i<32;i++){
   wi_6800c(0x80+i);
   wi_6800c(0x80+8);
   for(j=0;j<16;j++){
    wd_6800c(*(mintu++));
   }
  }
}

void distu_7920p_19264(uchar *mintu) {
 uchar i,j;

  reset_st7920p1_matrix();

  for(i=0;i<32;i++){
   wi_6800c(0x80+i);
   wi_6800c(0x80);
   for(j=0;j<24;j++){
    wd_6800c(*(mintu++));
   }
  }
  for(i=0;i<32;i++){
   wi_6800d(0x80+i);
   wi_6800d(0x80);
   for(j=0;j<24;j++){
    wd_6800d(*(mintu++));
   }
  }
}





 void test_ST7920_6800(void){
  uchar p;


 	for(p=0;p<xunhuan;p++){

  RST=0;
	delay_ms(44);
	RST=1;

	PSB=1;

  reset_st7920p1_character();
	reset_st7920p2_character();

  wcgram_st7920p1();
  wcgram_st7920p2();




	all_st7920p1(0x00,0x00);
 	all_st7920p2(0x00,0x00); 	  delay(222+222*delayTime);

	all_st7920p1(0x00,0xff);
	all_st7920p2(0x00,0xff);		delay(222+222*delayTime);

	all_st7920p1(0xff,0x00);
	all_st7920p2(0xff,0x00);		delay(222+222*delayTime);

	all_st7920p1(0x55,0x55);
	all_st7920p2(0x55,0x55);		delay(222+222*delayTime);

	all_st7920p1(0xaa,0xaa);
	all_st7920p2(0xaa,0xaa);		delay(222+222*delayTime);

	all_st7920p1(0xaa,0x55);
	all_st7920p2(0xaa,0x55);		delay(222+222*delayTime);

	all_st7920p1(0x55,0xaa);
	all_st7920p2(0x55,0xaa); 		delay(222+222*delayTime);





if(program_num==0) distu_7920p_25632(tu256032V1);
if(program_num==2) distu_7920p_12864(tu128064V1);
if(program_num==4) distu_7920p_19264(tu192064V1);


delay(222+222*delayTime);
delay(222+222*delayTime);


  reset_st7920p1_character();
	reset_st7920p2_character();



  disicon_st7920p1(0x00,0x00);
  disicon_st7920p2(0x00,0x00); delay(222+222*delayTime);

		wi_6800c(0x80);dely();
		wr_st7920p1_character_string("深圳市众博达科技有限公司Shenzhen\n");

 		wi_6800c(0x90);dely();
  	wr_st7920p1_character_string(" Zhongboda Technology Limited   \n");

		wi_6800d(0x80);dely();
   	wr_st7920p2_character_string("0123456789ABCDEFGHIJKLMNOPQRSTUV\n");

 		wi_6800d(0x90);dely();
  	wr_st7920p2_character_string("WXYZ_abcdefghijklmnopqrstuvwxyz_\n");

  		delay(222+222*delayTime);


		wi_6800c(0x80);dely();
		wr_st7920p1_character_string("0123456789ABCDEFGHIJKLMNOPQRSTUV\n");

 		wi_6800c(0x90);dely();
  	wr_st7920p1_character_string("WXYZ_abcdefghijklmnopqrstuvwxyz_\n");

		wi_6800d(0x80);dely();
   	wr_st7920p2_character_string("深圳市众博达科技有限公司Shenzhen\n");

 		wi_6800d(0x90);dely();
  	wr_st7920p2_character_string(" Zhongboda Technology Limited   \n");

  		delay(222+222*delayTime);

  }
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////





//===============================================================
//ST7920_Serial指令模式
//===============================================================
/////////////////////////////////////////////////////////////////
//ST7920_Serial指令
void high_serialST7920(void)        {
	en=0;
	wrr=1;		_nop_();_nop_();_nop_();_nop_();
	en=1;
	}
void low_serialST7920(void){
	en=0;
	wrr=0;		_nop_();_nop_();_nop_();_nop_();
	en=1;
   	}
/////////////////////////////////////////////////////////////////
//写指令
void wi_serial7920(uchar x){
   uchar i;

   rs=1;


   high_serialST7920();	//同步
   high_serialST7920();
   high_serialST7920();
   high_serialST7920();
   high_serialST7920();
   low_serialST7920();	//R/W=0
   low_serialST7920();	//D/I=0
   low_serialST7920();	//结束位

   for (i=0;i<4;i++)
   {
    if (x>=0x80) high_serialST7920();
     else low_serialST7920();
    x=x<<1;
   }
   low_serialST7920();	//空4位
   low_serialST7920();
   low_serialST7920();
   low_serialST7920();

   for (i=0;i<4;i++)
   {
    if (x>=0x80) high_serialST7920();
     else low_serialST7920();
    x=x<<1;
   }
   low_serialST7920();	//空4位
   low_serialST7920();
   low_serialST7920();
   low_serialST7920();
   rs=0;

 }
///////////////////////////////////////////////////////////////////
//写数据
void wd_serial7920(uchar x){
   uchar i;
   rs=1;


   high_serialST7920();	//同步
   high_serialST7920();
   high_serialST7920();
   high_serialST7920();
   high_serialST7920();
   low_serialST7920();	//R/W=0
   high_serialST7920();	//D/I=1
   low_serialST7920();	//结束位

   for (i=0;i<4;i++) {
    if (x>=0x80) high_serialST7920();
     else low_serialST7920();
    x=x<<1;
   }

   low_serialST7920();	//空4位
   low_serialST7920();
   low_serialST7920();
   low_serialST7920();

   for (i=0;i<4;i++)
   {
    if (x>=0x80) high_serialST7920();
     else low_serialST7920();
    x=x<<1;
   }
   low_serialST7920();	//空4位
   low_serialST7920();
   low_serialST7920();
   low_serialST7920();
   rs=0;

}
///////////////////////////////////////////////////////////////////////
//复位成字符模式
void reset_st7920serial_character(void) {
 wi_serial7920(0x30); delay_ms(44);
 wi_serial7920(0x0c);	delay_ms(11);//输入方式
 wi_serial7920(0x01);	delay_ms(22);//清屏
 wi_serial7920(0x06);	delay_ms(11);//开示
 }
///////////////////////////////////////////////////////////////////////
//复位成图形模式
void reset_st7920serial_matrix(void) {
// wi_serial7920(0x36);
// wi_serial7920(0x36);
 wi_serial7920(0x36);delay_ms(44);
 }




void wcgram_st7920serial(void) {      //写CGRAM;datt
 uchar i;
 wi_serial7920(0x40);
 for (i=0;i<64;i++){
 	wd_serial7920(datt[i]);
 }
}


void disicon_st7920serial(uchar dat1,dat2) {
 uchar i;
  wi_serial7920(0x80);
  for(i=0;i<33;i++) {
  wd_serial7920(dat1);
  wd_serial7920(dat2); ;
  }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
void disall7920a_serial(uchar dat1,dat2) {
 uchar  i,j;
 reset_st7920serial_matrix();
 for(i=0;i<16;i++){
  wi_serial7920(0x80+i*2);
  wi_serial7920(0x80);
  for(j=0;j<32;j++){wd_serial7920(dat1);}    //*tu
  wi_serial7920(0x80+i*2+1);
  wi_serial7920(0x80);
  for(j=0;j<32;j++){wd_serial7920(dat2);}    //*tu
 }
}


void wr_st7920serial_character_string(unsigned char *str) {
 uchar i;
  for(i=0;i<16;i++) {
  wd_serial7920(*str);
  wd_serial7920(*(str+1));
  str++;str++;
  }
}



void distu_7920s_25632(uchar *mintu) {
 uchar i,j;

  reset_st7920serial_matrix();

  for(i=0;i<32;i++){
   wi_serial7920(0x80+i);
   wi_serial7920(0x80);
   for(j=0;j<32;j++){
    wd_serial7920(*(mintu++));
   }
  }
}

void distu_7920s_12864(uchar *mintu) {
 uchar i,j;

  reset_st7920serial_matrix();

  for(i=0;i<32;i++){
   wi_serial7920(0x80+i);
   wi_serial7920(0x80);
   for(j=0;j<16;j++){
    wd_serial7920(*(mintu++));
   }
  }
  for(i=0;i<32;i++){
   wi_serial7920(0x80+i);
   wi_serial7920(0x80+8);
   for(j=0;j<16;j++){
    wd_serial7920(*(mintu++));
   }
  }
}


 void test_ST7920_serial(void){
  uchar p;

  RST=0;
	delay_ms(22);
	RST=1;
	PSB=0;									//PSB=0;

   reset_st7920serial_character();

  for(p=0;p<xunhuan;p++){
   reset_st7920serial_character();
   wcgram_st7920serial();










		reset_st7920serial_matrix();

 	  disall7920a_serial(0xff,0xff);  delay(222+222*delayTime);
 	  disall7920a_serial(0x00,0x00);  delay(222+222*delayTime);
	  disall7920a_serial(0xff,0x00);  delay(222+222*delayTime);
	  disall7920a_serial(0x00,0xff);  delay(222+222*delayTime);
	  disall7920a_serial(0x55,0x55);  delay(222+222*delayTime);
	  disall7920a_serial(0xaa,0xaa);  delay(222+222*delayTime);
	  disall7920a_serial(0xaa,0x55);  delay(222+222*delayTime);
	  disall7920a_serial(0x55,0xaa);  delay(222+222*delayTime);

		if(program_num==1) distu_7920s_25632(tu256032V1);
		if(program_num==3) distu_7920s_12864(tu128064V1);
    delay(222+222*delayTime);
    delay(222+222*delayTime);


  	reset_st7920serial_character();

    disicon_st7920serial(0x00,0x00);
  		delay(222+222*delayTime);

 		wi_serial7920(0x80);
  	wr_st7920serial_character_string("深圳市众博达科技有限公司Shenzhen\n");
 	  wi_serial7920(0x90);
    wr_st7920serial_character_string(" Zhongboda Technology limited   \n");
  		delay(222+222*delayTime);

    wi_serial7920(0x80);
   	wr_st7920serial_character_string("0123456789ABCDEFGHIJKLMNOPQRSTUV\n");
  	wi_serial7920(0x90);
   	wr_st7920serial_character_string("WXYZ_abcdefghijklmnopqrstuvwxyz_\n");
 		 delay(222+222*delayTime);





  }
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////



//*;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//*全显示数据da  122*32
//*;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
void disall_12232(uchar da1,da2) {
 uchar i,k;
 for(i=0;i<4;i++) {

  wi_6800a(0x00);  wi_6800a(0xb8+i); for(k=0;k<30;k++) {wd_6800a(da1);wd_6800a(da2);}
  wi_6800a(0x3c);  wi_6800a(0xb8+i); wd_6800a(da1);
  wi_6800b(0x00);  wi_6800b(0xb8+i); for(k=0;k<30;k++) {wd_6800b(da2);wd_6800b(da1);}
  wi_6800b(0x3c);  wi_6800b(0xb8+i); wd_6800b(da2);

  }
 }


//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;//
//写任意一幅图象122*32
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;//
void dismintu(uchar *mintu) {
 uchar i,k;
 for(i=0;i<4;i++)
  {
  for(k=0;k<122;k++)
    {
     if (k<61) {
     		wi_6800a(0x00+k);
     		wi_6800a(0xb8+i);
       		wd_6800a(*(mintu++));
                }
   else {
     	wi_6800b(0x00+k-61);
     	wi_6800b(0xb8+i);
       	wd_6800b(*(mintu++));
	 }
     }
  }
 }

/*;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;*/
/*初始化屏幕,清屏
/*;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;*/
void retdis_SBN1661(void) {


  wi_6800a(0xe2);   wi_6800b(0xe2);
  wi_6800a(0xae);   wi_6800b(0xae);
  wi_6800a(0xa4);   wi_6800b(0xa4);
  wi_6800a(0xa9);   wi_6800b(0xa9);
  wi_6800a(0xa0);   wi_6800b(0xa0);
  wi_6800a(0xee);   wi_6800b(0xee);
  wi_6800a(0x00);   wi_6800b(0x00);
  wi_6800a(0xd0);   wi_6800b(0xd0);
  wi_6800a(0xaf);   wi_6800b(0xaf);
  disall_12232(0x00,0x00);
 }


 void test_SBN1661_6800(void){
  uchar p;


 	for(p=0;p<xunhuan;p++){

  RST=0;
	delay_ms(44);
	RST=1;

  retdis_SBN1661();                    //复位程序

  dismintu(tu122032_h);		//显示图形
  delay(222+222*delayTime);

  disall_12232(0xff,0x00);           //全显示全关闭两次
  delay(222+222*delayTime);

  disall_12232(0x00,0xff);           //全显示全关闭两次
  delay(222+222*delayTime);

  disall_12232(0xaa,0xaa);           //全显示全关闭两次
  delay(222+222*delayTime);

  disall_12232(0x55,0x55);           //全显示全关闭两次
  delay(222+222*delayTime);


  disall_12232(0x55,0xaa);           //全显示全关闭两次
  delay(222+222*delayTime);


  disall_12232(0xaa,0x55);           //全显示全关闭两次
  delay(222+222*delayTime);


  }
}











////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

 void  module_mmax(void){
    if(module_num>9){module_num=0;}

   if(module_num==0){program_max=8;	}		//字符类S6A0069类程序最多	8
   if(module_num==1){program_max=4;	}		//点阵S6B0108类程序最多		4
   if(module_num==2){program_max=18;	}		//段码HT1621*3类程序最多	8
   if(module_num==3){program_max=3;	}		//点字S6B0063类程序最多		3
   if(module_num==4){program_max=4;	}		//段码HT1621*8类程序最多	5
   if(module_num==5){program_max=4;	}		//点阵SED13305类程序最多	4
   if(module_num==6){program_max=6;	}		//点阵T6963C类程序最多		6
   if(module_num==7){program_max=6;	}		//段码PCF8576类程序最多		6
   if(module_num==8){program_max=5;	}		//点阵ST7920类程序最多		2
   if(module_num==9){program_max=2;	}		//点阵SBN1661M18类程序最多		2


	}




///////////////////////////////////////////////////////////////////////
//////////////////////////////////////主程序///////////////////////////
///////////////////////////////////////////////////////////////////////
void main(void)	 {		//主程序//

 uchar i,j,t,k,*p;
 uchar bdata 		xx;
 uchar setup_num;
 uchar datatemp;
 uint poo;
 uchar h6,h5,h4,h3,h2,h1;
 uchar drt[4];




 delay_ms(33);


//******************************************************************************
//STC12C5A60S2-特别使用
//4SW指令为
// P4SW=0x70;
//******************************************************************************

	P0M1=0x00;
	P0M0=0xff;

	P1M1=0x00;
	P1M0=0xff;

	P2M1=0x00;
 	P2M0=0xff;

	P3M1=0x00;
 	P3M0=0xf3;
  

 P0=0xff;
 P1=0xff;
 P2=0xff;
 P3=0xff; 

poo=0;k=0;t=0;
 program_num=0;				//初始值
 setup_num=0;
 mm=1;
 delayTime=22;
k=0;
///////////////////////////////////////////////////////////////
  reset_s6a0069a();
  wcgram_0069a();


///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

  read_eeprom();				//选择EEPROM存贮位置

//判断读出的数据是否超标,如果超出标准值,则赋值为0,
/*



for(i=0;i<8;i++) {
dis_module(0,i);		delay(999+999*delayTime);
}
for(i=0;i<4;i++) {
dis_module(1,i);		delay(999+999*delayTime);
}
for(i=0;i<18;i++) {
dis_module(2,i);		delay(999+999*delayTime);
}
for(i=0;i<3;i++) {
dis_module(3,i);		delay(999+999*delayTime);
}
for(i=0;i<6;i++) {
dis_module(4,i);		delay(999+999*delayTime);
}
for(i=0;i<4;i++) {
dis_module(5,i);		delay(999+999*delayTime);
}
for(i=0;i<6;i++) {
dis_module(6,i);		delay(999+999*delayTime);
}
for(i=0;i<6;i++) {
dis_module(7,i);		delay(999+999*delayTime);
}

for(i=0;i<2;i++) {
dis_module(8,i);		delay(999+999*delayTime);
}

for(i=0;i<18;i++) {
dis_module(9,i);		delay(999+999*delayTime);
}

	for(i=0;i<10;i++) {
dis_program(i);		delay(999+999*delayTime);
}



                       
*/
  module_num=IIC_data[0];

  module_mmax();      


  program_num=IIC_data[1];
  	if(program_num>program_max){program_num=0;}

  delayTime=IIC_data[2];
   if(delayTime>time_max|delayTime<time_min){delayTime=time_min;}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

 // dis_module(5,2);


dis_module(module_num,program_num);



 for(i=0;i<33;i++){


  if(set==0){ dely();
   while(set==0) {;}   dely();



   if(setup_num==0){
   		dis_prog_time(setup_num);
   		displayLED(0,25,27);

   	  dely();}								//显示PRG,设置程序入口
   if(setup_num==1){
   		dis_prog_time(setup_num);
   		displayLED(0,13,14);

   	  dely();} 							//显示DEL,设置延时时间

   while(1){
    
   	
   	
 	  if(set==0) {dely();
 	  	while(set==0) {dely();}
 	  	goto setup;										//设置setup_num入口数据
 	  	}
    if(key==0){dely();
     while(key==0) {dely();}
     setup_num++;
     if(setup_num>1){setup_num=0;}	//循环菜单
     if(setup_num==0){
     	dis_prog_time(setup_num);
     	displayLED(0,25,27);

      dely();}								//显示PR,
     if(setup_num==1){
     	dis_prog_time(setup_num);
     	displayLED(0,13,14);

      dely();}  							//显示DE,
     }
    }
 goto start;
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

   	setup:

     if(setup_num==0){
     		dis_program(module_num);
        displayLED(0,25,module_num);
    		dely();}	//显示P&module_num,
     if(setup_num==1){
     		dis_time(delayTime);
     		displayLED(0,13,delayTime);
     		dely();} 	//显示D&delayTime,

   		while(1){
        delay_ms(11);
    			
					
     	  if(set==0){dely();
      	 while(set==0) {dely();}
    		 if(setup_num==1){
					IIC_data[0] =module_num;												//设置程序入口
   				IIC_data[1] =program_num;                          //设置延时时间
          IIC_data[2] =delayTime;
    			white_eeprom();						//选择EEPROM存贮位置
    		 goto start;
    			 }
      			else{goto setprogram;}
      		}

     	 if(key==0){
     	 	while(key==0) {;}

        if(setup_num==0){
        	module_num++;											//设置程序入口
        	if(module_num>9){module_num=0;}	//////需要更改最大种类数量
        	dis_program(module_num);
       	displayLED(0,25,module_num);
         	}
        if(setup_num==1){
        	delayTime++;											  //设置延时时间
        	if(delayTime>time_max|delayTime<time_min){delayTime=time_min;}
        	dis_time(delayTime);
        	displayLED(0,13,delayTime);
        	}
     	  }
       }
      }
    }
 goto start;
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


 setprogram:
  //   	BL_G  =1;
 //		BL_B  =1;

   while(1){
        k++;
				if(k%2==0){BL_B=~BL_B;}  

   	module_mmax();
   	if(program_num>program_max){program_num=0;}
   	dis_module(module_num,program_num);
	  displayLED(25,module_num,program_num);
	  delay_ms(123);


     	 if(set==0){dely();  dely();  dely();  dely();  dely();  
        while(set==0) {dely();  dely();  }

    	//  if(setup_num==1){
					IIC_data[0] =module_num;												//设置程序入口
   				IIC_data[1] =program_num;
          IIC_data[2] =delayTime;						//设置延时时间
    			white_eeprom();						//选择EEPROM存贮位置
    			// wr24c16_16byte();
    		 goto start;
      	// }
//      	else{goto setprogram;}
        }


     	 if(key==0){dely();  dely();  dely();  dely();  dely();  
     	 	while(key==0) {dely();  dely();  }
        program_num++;
        if(program_num>program_max){program_num=0;}

     	  }
   }

 start:


BL_G=1;BL_B=0;
 switch(module_num) {	                        //分配程序的入口


///////////////////////////////////////////////////////////////
//执行程序
///////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
//0=s6a0069;  =8bit并口68mode  =40*2    --1/16  Duty
//



case 0: {displayLED(27,module_num,program_num);  //字符测试程序=S6A0069=SAMSUNG




  if(program_num==0){ Charater_n_Line=0; test_0069_8bit_x_line(); }
  if(program_num==1){ Charater_n_Line=1; test_0069_8bit_x_line(); }
  if(program_num==2){ Charater_n_Line=0; test_0069_4bit_x_line(); }
  if(program_num==3){ Charater_n_Line=1; test_0069_4bit_x_line(); }
  if(program_num==4){ test_0070_4line_x_line(); }
  if(program_num==5){ test_0070_3line_x_line(); }
  if(program_num==6){ test_RW1067_Parallel(); }
  if(program_num==7){ test_RW1067_Serial(); }

break;
}

 case 1: {displayLED(27,module_num,program_num);  //字符测试程序=S6A0069=SAMSUNG



 test_0108_25664();

break;
}


 case 2: {displayLED(27,module_num,program_num);  //字符测试程序=S6A0069=SAMSUNG



 if(program_num==0) { DUTYBIAS=0x29;LCDNUM=16;RCXT=0x18;HT1621_num=1;test_Holtek_segment();}
 if(program_num==1) { DUTYBIAS=0x29;LCDNUM=16;RCXT=0x18;HT1621_num=2;test_Holtek_segment();}
 if(program_num==2) { DUTYBIAS=0x29;LCDNUM=16;RCXT=0x18;HT1621_num=3;test_Holtek_segment();}
 if(program_num==3) { DUTYBIAS=0x29;LCDNUM=16;RCXT=0x18;HT1621_num=4;test_Holtek_segment();}
 if(program_num==4) { DUTYBIAS=0x29;LCDNUM=16;RCXT=0x18;HT1621_num=5;test_Holtek_segment();}
 if(program_num==5) { DUTYBIAS=0x29;LCDNUM=16;RCXT=0x18;HT1621_num=6;test_Holtek_segment();}
 if(program_num==6) { DUTYBIAS=0x29;LCDNUM=16;RCXT=0x18;HT1621_num=7;test_Holtek_segment();}
 if(program_num==7) { DUTYBIAS=0x29;LCDNUM=16;RCXT=0x18;HT1621_num=8;test_Holtek_segment();}
 if(program_num==8) { DUTYBIAS=0x29;LCDNUM=16;RCXT=0x18;HT1621_num=9;test_Holtek_segment();}

 if(program_num==9) { DUTYBIAS=0x29;LCDNUM=32;RCXT=0x18;HT1621_num=1;test_Holtek_segment();}
 if(program_num==10){ DUTYBIAS=0x29;LCDNUM=24;RCXT=0x18;HT1621_num=1;test_Holtek_segment();}
 if(program_num==11){ DUTYBIAS=0x29;LCDNUM=48;RCXT=0x18;HT1621_num=1;test_Holtek_segment();}

 if(program_num==12){ DUTYBIAS=0x28;LCDNUM=16;RCXT=0x18;HT1621_num=1;test_Holtek_segment();} //data=0x28；1/4Duty，1/2Bias  内部震荡
 if(program_num==13){ DUTYBIAS=0x25;LCDNUM=16;RCXT=0x18;HT1621_num=1;test_Holtek_segment();} //data=0x25；1/3Duty，1/3Bias  内部震荡
 if(program_num==14){ DUTYBIAS=0x24;LCDNUM=16;RCXT=0x18;HT1621_num=1;test_Holtek_segment();} //data=0x24；1/3Duty，1/2Bias 内部震荡
 if(program_num==15){ DUTYBIAS=0x20;LCDNUM=16;RCXT=0x18;HT1621_num=1;test_Holtek_segment();} //data=0x20；1/2Duty，1/2Bias 内部震荡
 if(program_num==16){ DUTYBIAS=0x29;LCDNUM=16;RCXT=0x1c;HT1621_num=1;test_Holtek_segment();} //data=0x29；1/4Duty，1/3Bias  外部震荡输入256k
 if(program_num==17){ DUTYBIAS=0x29;LCDNUM=16;RCXT=0x14;HT1621_num=1;test_Holtek_segment();} //data=0x29；1/4Duty，1/3Bias  外部晶振震荡

break;
}

 case 3: {displayLED(27,module_num,program_num);  //字符测试程序=S6A0069=SAMSUNG



 if(program_num==0){BITNUM=5; }
 if(program_num==1){BITNUM=10;}
 if(program_num==2){BITNUM=20;}
 test_063_static();

break;
}
   

 case 4: {displayLED(27,module_num,program_num);  //字符测试程序=S6A0069=SAMSUNG



 test_1621x8_kaoyanji();

break;
}



 case 5: {displayLED(27,module_num,program_num);  //字符测试程序=S6A0069=SAMSUNG



 test_sed1335();

break;
}

 case 6: {displayLED(27,module_num,program_num);  //字符测试程序=S6A0069=SAMSUNG



	test_T6963();

break;
}



 case 7: {displayLED(27,module_num,program_num);  //字符测试程序=S6A0069=SAMSUNG



  if(program_num==0){PCF8576DUTY=0xcc;test_PCF8576_segment();} //1/4Duty,1/3Bias;
  if(program_num==1){PCF8576DUTY=0xc8;test_PCF8576_segment();} //1/4Duty,1/2Bias;
  if(program_num==2){PCF8576DUTY=0xcf;test_PCF8576_segment();} //1/3Duty,1/3Bias;
  if(program_num==3){PCF8576DUTY=0xcb;test_PCF8576_segment();} //1/3Duty,1/2Bias;
  if(program_num==4){PCF8576DUTY=0xca;test_PCF8576_segment();} //1/2Duty,1/2Bias;
  if(program_num==5){PCF8576DUTY=0xc9;test_PCF8576_segment();} //1/1Duty,1/1Bias;


break;
}

 case 8: {displayLED(27,module_num,program_num);  //字符测试程序=S6A0069=SAMSUNG



  if(program_num==0){test_ST7920_6800();}					//并口25632
  if(program_num==1){test_ST7920_serial();}				//串口12864
  if(program_num==2){test_ST7920_6800();}					//并口25632
  if(program_num==3){test_ST7920_serial();}				//串口12864
  if(program_num==4){test_ST7920_6800();} 				//并口19264

break;
}

 case 9: { displayLED(27,module_num,program_num);  //字符测试程序=S6A0069=SAMSUNG



  if(program_num==0){test_SBN1661_6800();}
  if(program_num==1){test_SBN1661_6800();}


break;
}


 }
}
