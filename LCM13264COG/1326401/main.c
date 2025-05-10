// ST7567,ST7565R¿´£¬UC1701×ÛºÏ³ÌÐò
// writer: 	laozhongmin
// version:01
// date:2013.01.11

//¶¨ÒåÍ ·ÎÄ¼þ
#include <reg52.h>
#include <intrins.h>
#include <STC89C52.c>					 //¶ÁÐ´´æ´¢Æ÷³ÌÐò
//#include "DATA.txt"						//Í¼Æ¬´æ´¢Êý¾Ý
#include <stdio.h>
#define uchar unsigned char
#define uint unsigned int 



//Ä£¿é²¢ÐÐ½Ó¿Ú¶¨Òå(½Ó¿Ú°´Êµ¼Ê²â¼Ü¸ü¸Ä)
#define dataport  P1			   //²¢¿ÚÍ¨Ñ¶Êý¾Ý½Å
sbit CS    = P3^0;		          //¶¨ÒåÆ¬Ñ¡½ÅÎªP3.0
sbit RST   = P3^1;		          //¶¨Òå¸´Î»½ÅÎªP3.1
sbit RS    = P3^2;		          //¶¨ÒåÊý¾Ý/Ö¸Áî¿ØÖÆ½ÅÎªP3.2
sbit RW_WR = P3^3;		          //¶¨Òå¶ÁÐ´¿ØÖÆ(68)»òÐ´(80)½ÅÎªP3.3
sbit E_RD  = P3^4;		          //¶¨Òå¶ÁÐ´Ê¹ÄÜ(68)»ò¶Á(80)½ÅÎªP3.4
//---------------------------------------------------------

/*
//Ä£¿é´®ÐÐ½Ó¿Ú¶¨Òå(½Ó¿Ú°´Êµ¼Ê²â¼Ü¸ü¸Ä)
sbit SDA = P1^4;  //SDA			  //¶¨Òå´®ÐÐÊý¾Ý½ÅÎªP1.4
sbit SCK = P1^3;				  //¶¨Òå´®ÐÐÊ±ÖÓ½ÅÎªP1.3
sbit RS  = P1^2;  //A0			 //¶¨ÒåÊý¾Ý/Ö¸Áî¿ØÖÆ½ÅÎªP1.2
sbit RST = P1^1;				 //¶¨Òå¸´Î»½ÅÎªP1.1
sbit CS  = P1^0;			   //¶¨ÒåÆ¬Ñ¡½ÅÎªP10
//---------------------------------------------------------	
*/
 
//°´¼ü½Ó¿Ú¶¨Òå(Óë²âÊÔ°åÒ»ÖÂ)
sbit SW	=P2^5;
sbit DN	=P2^6;
sbit UP	=P2^7;
sbit key=P2^4;
unsigned char ADJUST=21;	//¶Ô±È¶È¼Ä´æÆ÷³õÊ¼Öµ	£¬vopµ÷ÕûÉèÖÃ  µ±Ç°ÉèÖÃÊýÖµVDD=3.0V,VOP=23=9.5V

//0~9,ASCIIÂëÊý×é
unsigned char code ASC[10*16]=
{
0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00,0x00,0x07,0x08,0x10,0x10,0x08,0x07,0x00,/*"016"*/
0x00,0x10,0x10,0xF8,0x00,0x00,0x00,0x00,0x00,0x10,0x10,0x1F,0x10,0x10,0x00,0x00,/*"117"*/
0x00,0x30,0x08,0x08,0x08,0x88,0x70,0x00,0x00,0x18,0x14,0x12,0x11,0x10,0x18,0x00,/*"218"*/
0x00,0x30,0x08,0x88,0x88,0x48,0x30,0x00,0x00,0x0C,0x10,0x10,0x10,0x11,0x0E,0x00,/*"319"*/
0x00,0x80,0x40,0x20,0xF8,0x00,0x00,0x00,0x00,0x03,0x02,0x12,0x1F,0x12,0x12,0x00,/*"420"*/
0x00,0xF8,0x88,0x48,0x48,0x48,0x88,0x00,0x00,0x0C,0x10,0x10,0x10,0x10,0x0F,0x00,/*"521"*/
0x00,0xE0,0x10,0x88,0x88,0x98,0x00,0x00,0x00,0x0F,0x11,0x10,0x10,0x10,0x0F,0x00,/*"622"*/
0x00,0x38,0x08,0x08,0xC8,0x38,0x08,0x00,0x00,0x00,0x00,0x1F,0x00,0x00,0x00,0x00,/*"723"*/
0x00,0x70,0x88,0x88,0x88,0x88,0x70,0x00,0x00,0x0E,0x11,0x10,0x10,0x11,0x0E,0x00,/*"824"*/
0x00,0xF0,0x08,0x08,0x08,0x90,0xE0,0x00,0x00,0x00,0x19,0x11,0x11,0x08,0x07,0x00,/*"925"*/
};


//º¯ÊýÉêÃ÷ÈçÏÂ
void Delay_Xms(unsigned int Xms);
unsigned char Change(unsigned int Xms);
void InitST7567(void);
void ST7567_WriteComd(unsigned char comd);
void ST7567_WriteData(unsigned char dat);
void ST7567_WriteData1(unsigned char dat);
void DispPic(unsigned char *dispdata);
void DispPic1(unsigned char *dispdata);
void DispFram(unsigned char x,unsigned char y);
void DispDot(unsigned char x,unsigned char y);
void DispDots(unsigned char x,unsigned char y);


void push()
{
	 Delay_Xms(100);	
   
  	while(key==1)
   {
   	Delay_Xms(100);
   
   
   }
}

// ====================================//
// ÑÓÊ±º¯Êý
// ÊäÈë²ÎÊý:Xms
// Êä³ö²ÎÊý:ÎÞ
// ¹¦ÄÜËµÃ÷: Xms*1ms
// ====================================//
void Delay_Xms(unsigned int Xms)
{//ÐèÑÓÊ±µÄmsÊý
	unsigned int i,j;
	for(i=0;i<Xms;i++)
		for(j=0;j<150;j++);//ÑÓÊ±µÄ»ùÊýÔ¼1ms
}

void init_cpu(void)
{
    IE = 0x00;
    Delay_Xms(50);
}

// ====================================//
// ³õÊ¼»¯º¯Êý
// ÊäÈë²ÎÊý:ÎÞ
// Êä³ö²ÎÊý:ÎÞ
// ×¢:ÓëÆäËüST7567¼°NT75451/NT7534/NT7538ÊÇ×îºó±¶Ñ¹ÊýÑ¡ÔñÓÐ²îÒìÆäËüÏàÍ¬
// ====================================//


 // ST7567³õÊ¼»¯º¯Êý
void InitST7567(void)
{
 	Delay_Xms(100);	// ÒÔÏÂ¼¸ÐÐ´úÂëÊÇ²úÉúÓ²¼þ¸´Î»
	RST = 0;
 	Delay_Xms(50);	// 
	RST = 1;
 	Delay_Xms(100);	// 

	ST7567_WriteComd(0xe2);	//soft reset¿ªÊ¼½øÐÐÈí¸´Î»
 	Delay_Xms(10);
	ST7567_WriteComd(0xa2);	//bias selectÈ·¶¨Îª1/6BIAS,1/33duty
 	Delay_Xms(2);
	ST7567_WriteComd(0xa0);	// ADC selectÈ·¶¨SEG·´É¨seg0--->seg131
 	Delay_Xms(2);
	ST7567_WriteComd(0xc8);	//SHL selectÈ·¶¨COMÎªÕýÉ¨com63--->com0
 	Delay_Xms(2);
	ST7567_WriteComd(0x23);	//regulator resistor select V0µ÷Õûµç×èÉè¶¨(1 + Rb/Ra)=6.5		0X26+0X41		vdd=5.0v,vop=10.0v						 0x26Ô­À´µÄÊýÖµ
 	Delay_Xms(2);
	ST7567_WriteComd(0x81);	//The Electronic Volume (Double Byte Command)Êý×ÖµçÎ»Æ÷µ÷½ÚVOÊä³öµçÑ¹(Ò²¾ÍÊÇ¶Ô±È¶È)Á½×Ö½Ú				 0x81Ô­À´µÄÊýÖµ
	ST7567_WriteComd(ADJUST);	 //vop ÉèÖÃ
 	Delay_Xms(2);
	ST7567_WriteComd(0x2c);	//power control1 onÄÚ²¿±¶Ñ¹µçÂ·¿ªÆô,Ðè·ÖÒÔÏÂÈý²½´ò¿ªµçÂ·
 	Delay_Xms(2);
	ST7567_WriteComd(0x2e);	//power control2
 	Delay_Xms(2);
	ST7567_WriteComd(0x2f);	//power control3
 	Delay_Xms(2);
	ST7567_WriteComd(0xf1);	//The Booster Ratio (Double Byte Command)Éè¶¨±¶Ñ¹Êý,Á½×Ö½Ú
 	Delay_Xms(2);
	ST7567_WriteComd(0x00);	//"00"×î´ó4±¶Ñ¹,"01"×î´ó5±¶Ñ¹,"03"×î´ó6±¶Ñ¹
 	Delay_Xms(2);
	ST7567_WriteComd(0x40);	//Éè¶¨ÆðÊ¼ÐÐµÚ1ÐÐ
 	Delay_Xms(2);
	ST7567_WriteComd(0xa4);	//Display All Points ON/OFF to Normal display modeÕý³£ÏÔÊ¾Ä£Ê½Ñ¡Ôñ
 	Delay_Xms(2);
	ST7567_WriteComd(0xaf);	//Display ON/OFF¿ªÏÔÊ¾Éè¶¨
 	Delay_Xms(2);
}
 //===========================


/*
 // ST7565R³õÊ¼»¯º¯Êý

void InitST7567(void)
   	
{	Delay_Xms(100);	// ÒÔÏÂ¼¸ÐÐ´úÂëÊÇ²úÉúÓ²¼þ¸´Î»
	RST = 0;
 	Delay_Xms(50);	// 
	RST = 1;
 	Delay_Xms(100);	// 

	ST7567_WriteComd(0xe2);	//soft reset¿ªÊ¼½øÐÐÈí¸´Î»
 	Delay_Xms(10);
	ST7567_WriteComd(0xa2);	//bias selectÈ·¶¨Îª1/9BIAS,1/65duty
 	Delay_Xms(2);
	ST7567_WriteComd(0xa0);	// ADC selectÈ·¶¨SEG·´É¨seg0--->seg131
 	Delay_Xms(2);
	ST7567_WriteComd(0xc8);	//SHL selectÈ·¶¨COMÎªÕýÉ¨com63--->com0
 	Delay_Xms(2);
	ST7567_WriteComd(0x26);	//regulator resistor select V0µ÷Õûµç×èÉè¶¨(1 + Rb/Ra)=6.5		0X26+0X41		vdd=5.0v,vop=10.0v						 0x26Ô­À´µÄÊýÖµ
 	Delay_Xms(2);
	ST7567_WriteComd(0x81);	//The Electronic Volume (Double Byte Command)Êý×ÖµçÎ»Æ÷µ÷½ÚVOÊä³öµçÑ¹(Ò²¾ÍÊÇ¶Ô±È¶È)Á½×Ö½Ú				 0x81Ô­À´µÄÊýÖµ
	ST7567_WriteComd(ADJUST);	 //vop ÉèÖÃ
 	Delay_Xms(2);												
	ST7567_WriteComd(0x2f);	//power control3
 	Delay_Xms(2);
	ST7567_WriteComd(0xf8);	//The Booster Ratio (Double Byte Command)Éè¶¨±¶Ñ¹Êý,Á½×Ö½Ú
 	Delay_Xms(2);
	ST7567_WriteComd(0x01);	//"00"×î´ó4±¶Ñ¹,"01"×î´ó5±¶Ñ¹,"03"×î´ó6±¶Ñ¹
 	Delay_Xms(2);
	ST7567_WriteComd(0x40);	//Éè¶¨ÆðÊ¼ÐÐµÚ1ÐÐ
 	Delay_Xms(2);
	ST7567_WriteComd(0xa4);	//Display All Points ON/OFF to Normal display modeÕý³£ÏÔÊ¾Ä£Ê½Ñ¡Ôñ
 	Delay_Xms(2);
	ST7567_WriteComd(0xaf);	//Display ON/OFF¿ªÏÔÊ¾Éè¶¨
 	Delay_Xms(2);


 }

/*
void InitST7567(void)
   	
{	ST7567_WriteComd(0xa2);//   1/3bias
 		ST7567_WriteComd(0xa0);//ADC select:normal
  		ST7567_WriteComd(0xa4);//Display all point ON/OFF:normal display
  		ST7567_WriteComd(0xc0);//Common output mode select:normal direction
 		ST7567_WriteComd(0x22);//V0 boltage regulator ratio set:5.0(0X24)
        	Delay_Xms(10);
		ST7567_WriteComd(0x81);//Electronic volume register set
 		ST7567_WriteComd(17);//Electronic volume register set
 		Delay_Xms(10);
 		ST7567_WriteComd(0x2c);//Power control set:booster on
        	Delay_Xms(10);
  		ST7567_WriteComd(0x2e);//Power control set:booster on/regulator on
        	Delay_Xms(10);
 		ST7567_WriteComd(0x2f);//Power control set:booster on/regulator on/follower on
        	Delay_Xms(10);
 		ST7567_WriteComd(0xaf);//display on

   }

 */

//==============================================
unsigned char code    jin[128]= 
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xC0,0xF0,0xF8,0x7C,0x3C,0x7C,0xF8,0xE0,0xC0,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x20,0x60,0x70,0x78,0x38,0x3C,0x7E,0x6F,0x67,0x67,0x63,0x61,0xE0,0xE0,0xE0,0xE0,0x61,0x63,0x63,0x67,0x6F,0x7E,0x7E,0x1C,0x3C,0x38,0x78,0x78,0x10,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x18,0xD8,0xD8,0xD8,0x98,0x18,0x18,0xFF,0xFF,0xFF,0xFF,0x18,0x18,0x38,0xF8,0xD8,0xD8,0xD8,0x18,0x18,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x30,0x30,0x30,0x30,0x31,0x37,0x37,0x33,0x32,0x30,0x3F,0x3F,0x3F,0x3F,0x30,0x38,0x3F,0x3F,0x3F,0x33,0x30,0x30,0x30,0x30,0x30,0x00,0x00,0x00};/*"½ð",0*/
/* (32 X 32 , ºÚÌå, ¼Ó´Ö )*/
uchar code    se[128]= 
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xC0,0xF0,0xFC,0xFC,0xF8,0xC8,0xC8,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x10,0x30,0x38,0x3C,0xFE,0xFF,0xFF,0xF7,0x33,0x30,0x30,0x30,0x30,0xF0,0xF0,0xF8,0xFE,0x3F,0x3F,0x33,0x31,0x30,0xF0,0xF0,0xF0,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0x1C,0x1C,0x1C,0x1C,0x1C,0x1F,0x1F,0x1F,0x1F,0x1C,0x1C,0x1C,0x1C,0x1C,0x3F,0x3F,0x3F,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x1F,0x3F,0x3F,0x38,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x38,0x3E,0x1E,0x0C,0x00,0x00};/*"É«",1*/
/* (32 X 32 , ºÚÌå, ¼Ó´Ö )*/
uchar code    tong[128]= 
{0x00,0x00,0x00,0x00,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0xE0,0xE0,0x60,0x68,0x7C,0x7C,0x7C,0x60,0x60,0xE0,0xE0,0xE0,0x60,0x60,0x60,0x60,0x60,0x60,0x00,0x00,0x00,
0x00,0x00,0x00,0x04,0x04,0x04,0xE4,0xE4,0xE4,0x64,0x67,0x67,0x67,0x67,0x64,0xE4,0xE4,0xE4,0x64,0x67,0x67,0x67,0x65,0x64,0xE4,0xE4,0xE4,0x04,0x04,0x04,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x3F,0x3F,0x3F,0x32,0x32,0x32,0x32,0x32,0x32,0xFF,0xFF,0xFF,0x32,0x32,0x32,0x32,0x32,0x32,0x3F,0x3F,0x3F,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x30,0x30,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x3F,0x3F,0x3F,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x30,0x30,0x00,0x00};/*"Í¯",2*/
/* (32 X 32 , ºÚÌå, ¼Ó´Ö )*/
uchar code    nian[128]= 
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xE0,0xF8,0xFC,0xFC,0x98,0x88,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x00,
0x00,0x00,0x20,0x30,0x78,0x7C,0x3F,0xDF,0xCF,0xC3,0xC1,0xC1,0xC1,0xC1,0xC1,0xC1,0xFF,0xFF,0xFF,0xFF,0xC1,0xC1,0xC1,0xC1,0xC1,0xC1,0xC1,0xC1,0x01,0x00,0x00,0x00,
0x00,0x00,0xC0,0xC0,0xC0,0xC0,0xC0,0xFF,0xFF,0xFF,0xFF,0xC0,0xC0,0xC0,0xC0,0xC0,0xFF,0xFF,0xFF,0xFF,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0x7F,0x7F,0x7F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};/*"Äê",3*/
/* (32 X 32 , ºÚÌå, ¼Ó´Ö )*/



void hz_display16(unsigned char pag, unsigned char col, unsigned char cok,unsigned char code *hzk)	   //Ð´ÈëÒ»¸ö32*32µÄ×Ö·û
{
		 uchar a=0;	
         uchar j=0;
         uchar i=0;
		
         for(j=0;j<4;j++)                          //ÕâÀïÐ´ËÄÒ³
         {
             ST7567_WriteComd(0xb0+pag+j);          //pageÔÚÕâÀï¾ÍÊÇ´Ópage±íÊ¾µÄÕâÒ³¿ªÊ¼Ð´Æð¡£
				
			 ST7567_WriteComd(0x10+cok);		  //Ë«Ö¸Áî¿ØÖÆÁÐ£¬Ðè¼Ó±äÁ¿À´¿ØÖÆSEGÆðÊ¼¶Ë
             ST7567_WriteComd(0x00+col);          //´ÓÁÐÎªcol±íÊ¾µÄÁÐ¿ªÊ¼
				 
          for(i=0;i<32;i++)                           //ÕâÀïÐ´32ÁÐ
               ST7567_WriteData(hzk[32*j+i]);        //JÎªÒ³£¬I£¬ÎªµÚJÐÐµÚI¸öÊý
         }
}







// ====================================//
// ÏÔÊ¾Ò»¸öÍ¼Æ¬º¯Êý
// ÊäÈë²ÎÊý:ÏÔÊ¾Í¼Æ¬Êý×é
// Êä³ö²ÎÊý:ÎÞ
// ====================================//
void DispPic(unsigned char *dispdata)
{
	unsigned char i,page;	//¶¨Òå¾Ö²¿×Ö·û±äÁ¿
	unsigned int j=0;		//¶¨Òå¾Ö²¿ÕûÐÍ±äÁ¿

	// ÏÂÃæÕâ¶ÎÑ­»·ÎªÏÔÊ¾C0~C31
	page=0xb0;				//³õÊ¼¿ªÊ¼É¨ÃèÒ³
	while(page<0xb4)		//¹²4Ò³
	{
		ST7567_WriteComd(page);	//Éè¶¨É¨ÃèÒ³
	    ST7567_WriteComd(0x10);	//Éè¶¨ÆðÊ¼SEGÊý,Á½×Ö½Ú
	    ST7567_WriteComd(0x00);
	    for(i=0;i<132;i++)
    	{//Ð´128SEG
      		ST7567_WriteData(dispdata[j]);	//µ÷Êý×éÏÔÊ¾Êý¾Ý½øÐÐÐ´ÏÔÊ¾
      		j++;
    	}
    	page++;	//Ò³×Ô¶¯¼Ó1
	}
	// ÏÂÃæÕâ¶ÎÑ­»·ÎªÏÔÊ¾C32~C63
	page=0xb4;				//³õÊ¼¿ªÊ¼É¨ÃèÒ³
	while(page<0xb8)		//¹²4Ò³
	{
		ST7567_WriteComd(page);	//Éè¶¨É¨ÃèÒ³
	    ST7567_WriteComd(0x10);	//Éè¶¨ÆðÊ¼SEGÊý,Á½×Ö½Ú
	    ST7567_WriteComd(0x00);
	    for(i=0;i<132;i++)
    	{//Ð´128SEG
      		ST7567_WriteData(dispdata[j]);	//µ÷Êý×éÏÔÊ¾Êý¾Ý½øÐÐÐ´ÏÔÊ¾
      		j++;
    	}
    	page++;	//Ò³×Ô¶¯¼Ó1
	}
}

void DispPic1(unsigned char *dispdata)
{
	unsigned char i,page;	//¶¨Òå¾Ö²¿×Ö·û±äÁ¿
	unsigned int j=0;		//¶¨Òå¾Ö²¿ÕûÐÍ±äÁ¿

	// ÏÂÃæÕâ¶ÎÑ­»·ÎªÏÔÊ¾C0~C31
	page=0xb0;				//³õÊ¼¿ªÊ¼É¨ÃèÒ³
	while(page<0xb4)		//¹²4Ò³
	{
		ST7567_WriteComd(page);	//Éè¶¨É¨ÃèÒ³
	    ST7567_WriteComd(0x10);	//Éè¶¨ÆðÊ¼SEGÊý,Á½×Ö½Ú
	    ST7567_WriteComd(0x00);
	    for(i=0;i<132;i++)
    	{//Ð´128SEG
      		ST7567_WriteData1(dispdata[j]);	//µ÷Êý×éÏÔÊ¾Êý¾Ý½øÐÐÐ´ÏÔÊ¾
      		j++;
    	}
    	page++;	//Ò³×Ô¶¯¼Ó1
	}
	// ÏÂÃæÕâ¶ÎÑ­»·ÎªÏÔÊ¾C32~C63
	page=0xb4;				//³õÊ¼¿ªÊ¼É¨ÃèÒ³
	while(page<0xb8)		//¹²4Ò³
	{
		ST7567_WriteComd(page);	//Éè¶¨É¨ÃèÒ³
	    ST7567_WriteComd(0x10);	//Éè¶¨ÆðÊ¼SEGÊý,Á½×Ö½Ú
	    ST7567_WriteComd(0x00);
	    for(i=0;i<132;i++)
    	{//Ð´128SEG
      		ST7567_WriteData1(dispdata[j]);	//µ÷Êý×éÏÔÊ¾Êý¾Ý½øÐÐÐ´ÏÔÊ¾
      		j++;
    	}
    	page++;	//Ò³×Ô¶¯¼Ó1
	}
}



// ====================================//
// ÏÔÊ¾¸ôÐÐ¸ôÁÐ¼ä¸ôµãÆåÅÌµÄº¯Êý
// ÊäÈë²ÎÊý:Ë«×Ö½Ú
// Êä³ö²ÎÊý:ÎÞ
// ====================================//
void DispFram(unsigned char x,unsigned char y)
{
	unsigned char i,page;	//¶¨Òå¾Ö²¿×Ö·û±äÁ¿

	// ÏÂÃæÕâ¶ÎÑ­»·ÎªÏÔÊ¾C0~C31
	page=0xb0;				//³õÊ¼¿ªÊ¼É¨ÃèÒ³
	while(page<0xb4)		//¹²4Ò³
	{
		ST7567_WriteComd(page);	//Éè¶¨É¨ÃèÒ³
	    ST7567_WriteComd(0x10);	//Éè¶¨ÆðÊ¼SEGÊý,Á½×Ö½Ú
	    ST7567_WriteComd(0x00);
	    for(i=0;i<132;i++)
    	{//Ð´128SEG
      		ST7567_WriteData(x);	//ÏÔÊ¾Êý¾Ý½øÐÐÐ´ÏÔÊ¾
      		ST7567_WriteData(y);	//ÏÔÊ¾Êý¾Ý½øÐÐÐ´ÏÔÊ¾
    	}
    	page++;	//Ò³×Ô¶¯¼Ó1
	}
	// ÏÂÃæÕâ¶ÎÑ­»·ÎªÏÔÊ¾C32~C63
	page=0xb4;				//³õÊ¼¿ªÊ¼É¨ÃèÒ³
	while(page<0xb8)		//¹²4Ò³
	{
		ST7567_WriteComd(page);	//Éè¶¨É¨ÃèÒ³
	    ST7567_WriteComd(0x10);	//Éè¶¨ÆðÊ¼SEGÊý,Á½×Ö½Ú
	    ST7567_WriteComd(0x00);
	    for(i=0;i<132;i++)
    	{//Ð´128SEG
      		ST7567_WriteData(x);	//ÏÔÊ¾Êý¾Ý½øÐÐÐ´ÏÔÊ¾
      		ST7567_WriteData(y);	//ÏÔÊ¾Êý¾Ý½øÐÐÐ´ÏÔÊ¾
    	}
    	page++;	//Ò³×Ô¶¯¼Ó1
	}
}
// ====================================//
// ÏÔÊ¾¸ôÐÐ¸ôÁÐ¼ä¸ôµãÆåÅÌµÄº¯Êý
// ÊäÈë²ÎÊý:Ë«×Ö½Ú
// Êä³ö²ÎÊý:ÎÞ
// ====================================//
void DispDot(unsigned char x,unsigned char y)
{
	unsigned char i,j,page;	//¶¨Òå¾Ö²¿×Ö·û±äÁ¿
	// ÏÂÃæÕâ¶ÎÑ­»·ÎªÏÔÊ¾C0~C31
	page=0xb0;				//³õÊ¼¿ªÊ¼É¨ÃèÒ³
	while(page<0xb4)		//¹²4Ò³
	{
	    ST7567_WriteComd(page);	//Éè¶¨É¨ÃèÒ³
	    ST7567_WriteComd(0x10);	//Éè¶¨ÆðÊ¼SEGÊý,Á½×Ö½Ú
	    ST7567_WriteComd(0x00);
	    for(i=0;i<17;i++)
    	    {
           for(j=0;j<4;j++)
            { 
      		ST7567_WriteData(x);	//ÏÔÊ¾Êý¾Ý½øÐÐÐ´ÏÔÊ¾
              }
            for(j=0;j<4;j++)
            { 
      		ST7567_WriteData(y);	//ÏÔÊ¾Êý¾Ý½øÐÐÐ´ÏÔÊ¾
            }
    	}
    	page++;	//Ò³×Ô¶¯¼Ó1
	}
	// ÏÂÃæÕâ¶ÎÑ­»·ÎªÏÔÊ¾C32~C63
	page=0xb4;				//³õÊ¼¿ªÊ¼É¨ÃèÒ³
	while(page<0xb8)		//¹²4Ò³
	{
	    ST7567_WriteComd(page);	//Éè¶¨É¨ÃèÒ³
	    ST7567_WriteComd(0x10);	//Éè¶¨ÆðÊ¼SEGÊý,Á½×Ö½Ú
	    ST7567_WriteComd(0x00);
	    for(i=0;i<17;i++)
    	    {
           for(j=0;j<4;j++)
            { 
      		ST7567_WriteData(x);	//ÏÔÊ¾Êý¾Ý½øÐÐÐ´ÏÔÊ¾
              }
            for(j=0;j<4;j++)
            { 
      		ST7567_WriteData(y);	//ÏÔÊ¾Êý¾Ý½øÐÐÐ´ÏÔÊ¾
            }
    	}
    	page++;	//Ò³×Ô¶¯¼Ó1
	}
}
// ====================================//
// ÏÔÊ¾¸ôÐÐ¸ôÁÐ¼ä¸ôµãÆåÅÌµÄº¯Êý
// ÊäÈë²ÎÊý:Ë«×Ö½Ú
// Êä³ö²ÎÊý:ÎÞ
// ====================================//
void DispDots(unsigned char x,unsigned char y)
{
	unsigned char i,j,page;	//¶¨Òå¾Ö²¿×Ö·û±äÁ¿


	// ÏÂÃæÕâ¶ÎÑ­»·ÎªÏÔÊ¾C0~C31
	page=0xb0;				//³õÊ¼¿ªÊ¼É¨ÃèÒ³
	while(page<0xb4)		//¹²4Ò³
	{
	    ST7567_WriteComd(page);	//Éè¶¨É¨ÃèÒ³
	    ST7567_WriteComd(0x10);	//Éè¶¨ÆðÊ¼SEGÊý,Á½×Ö½Ú
	    ST7567_WriteComd(0x00);
	    for(i=0;i<33;i++)
    	    {
           for(j=0;j<2;j++)
            { 
      		ST7567_WriteData(x);	//ÏÔÊ¾Êý¾Ý½øÐÐÐ´ÏÔÊ¾
              }
            for(j=0;j<2;j++)
            { 
      		ST7567_WriteData(y);	//ÏÔÊ¾Êý¾Ý½øÐÐÐ´ÏÔÊ¾
            }
    	}
    	page++;	//Ò³×Ô¶¯¼Ó1
	}
	// ÏÂÃæÕâ¶ÎÑ­»·ÎªÏÔÊ¾C32~C63
	page=0xb4;				//³õÊ¼¿ªÊ¼É¨ÃèÒ³
	while(page<0xb8)		//¹²4Ò³
	{
	    ST7567_WriteComd(page);	//Éè¶¨É¨ÃèÒ³
	    ST7567_WriteComd(0x10);	//Éè¶¨ÆðÊ¼SEGÊý,Á½×Ö½Ú
	    ST7567_WriteComd(0x00);
	    for(i=0;i<33;i++)
    	    {
           for(j=0;j<2;j++)
            { 
      		ST7567_WriteData(x);	//ÏÔÊ¾Êý¾Ý½øÐÐÐ´ÏÔÊ¾
              }
            for(j=0;j<2;j++)
            { 
      		ST7567_WriteData(y);	//ÏÔÊ¾Êý¾Ý½øÐÐÐ´ÏÔÊ¾
            }
    	}
    	page++;	//Ò³×Ô¶¯¼Ó1
	}
}

// ====================================//
// Ö÷º¯Êý,Ö»¾ÙÀý,ÏêÏ¸µÄÏÔÊ¾ÄÚÈÝ¸ù¾Ý¹æ·¶
// ====================================//
void main(void)
{
	init_cpu();
	
	Delay_Xms(100);
	while(1)  
	{
	InitST7567();	//Ò»¿ªÊ¼³õÊ¼»¯¿ØÖÆÆ÷
  Start:	//Ìø×ª±êºÅ
      			   	    
//-------------------------------------------------------------------------		
	
//		 hz_display16(0,0,0,jin); 
//	  		Delay_Xms(1000);
//			 
//			 
//	    hz_display16(0,0,18,se); 
//	  	Delay_Xms(1000);
	
	
		DispFram( 0xff,0xff );				 //È«ÏÔ
		 Delay_Xms(500);
		if( Change(70)==1 ) goto Start;	//ÔÝÍ£¼üÓÐ¶¯×÷Ìø×ªµ½³ÌÐòµÄ×îÍ·ÉÏ
//		 push();


		DispFram( 0x00,0x00 );			   //ÎÞÏÔ
		 Delay_Xms(500);
		if( Change(70)==1 ) goto Start;	//ÔÝÍ£¼üÓÐ¶¯×÷Ìø×ªµ½³ÌÐòµÄ×îÍ·ÉÏ
	 	 //push();
//------------------------------------------------------------------	
		DispDot( 0xf0,0x0f );			 //´óÆåÅÌ
		 Delay_Xms(500);
	  	if( Change(70)==1 ) goto Start;	//ÔÝÍ£¼üÓÐ¶¯×÷Ìø×ªµ½³ÌÐòµÄ×îÍ·ÉÏ
		 //push();
//------------------------------------------------------------------	
		DispDots( 0x33,0xcc );			  //Ð¡ÆåÅÌ
		 Delay_Xms(500);
		if( Change(70)==1 ) goto Start;	//ÔÝÍ£¼üÓÐ¶¯×÷Ìø×ªµ½³ÌÐòµÄ×îÍ·ÉÏ
		// push();
//------------------------------------------------------------------	
		DispFram( 0x55,0xaa );				  //»¨µã		
		 Delay_Xms(500);
		if( Change(70)==1 ) goto Start;	//ÔÝÍ£¼üÓÐ¶¯×÷Ìø×ªµ½³ÌÐòµÄ×îÍ·ÉÏ
	// push();
	
		DispFram( 0x55,0x55 );					 //	 ºáÏß
		 Delay_Xms(500);
		if( Change(70)==1 ) goto Start;	//ÔÝÍ£¼üÓÐ¶¯×÷Ìø×ªµ½³ÌÐòµÄ×îÍ·ÉÏ
	 //push();

	//	DispFram( 0xaa,0xaa );				     //	ºáÏß
	//	  Delay_Xms(1000);
	 	//	if( Change(300)==1 ) goto Start;	//ÔÝÍ£¼üÓÐ¶¯×÷Ìø×ªµ½³ÌÐòµÄ×îÍ·ÉÏ
		 //push();
//-------------------------------------------------------------------------		
		DispFram( 0xff,0x00 );					 //ÊúÏß 
		  Delay_Xms(500);
		if( Change(70)==1 ) goto Start;	//ÔÝÍ£¼üÓÐ¶¯×÷Ìø×ªµ½³ÌÐòµÄ×îÍ·ÉÏ
	// push();

	//	DispFram( 0x00,0xff );
	//	 Delay_Xms(1000);
	//	if( Change(300)==1 ) goto Start;	//ÔÝÍ£¼üÓÐ¶¯×÷Ìø×ªµ½³ÌÐòµÄ×îÍ·ÉÏ
	 //push();	 
	 


	}
}
//-------------------------------------
/*
// ====================================//
// 4Ïß·½Ê½µÄÐ´Ö¸Áî×Ó³ÌÐò
// ÊäÈë²ÎÊý:comd
// Êä³ö²ÎÊý:ÎÞ
// ====================================//
void ST7567_WriteComd(unsigned char comd)
{
	unsigned char i;
	CS = 0;								//ÖÃµÍCS±íÊ¾Ñ¡ÖÐÐ¾Æ¬¿ÉÒÔÓÐÐ§µÄ¶ÔÐ¾Æ¬½øÐÐ¿ØÖÆ
	RS = 0;								//ÖÃµÍRS±íÊ¾Ö´ÐÐÖ¸ÁîµÄ´«ËÍ
	SCK = 0;							//ÖÃµÍSCKÎª²úÉúÉÏÉýÑØ×÷×¼±¸
	for(i=0;i<8;i++)
	{//Ñ­»·´«ËÍ¹²8Î»´Ó¸ßÎ»¿ªÊ¼
		if( (comd&0x80)==0x80 ) SDA = 1;//ÅÐ¶ÏcomdµÄ×î¸ßÎ»,ÊÇ"1"ÔòÖÃSDA=1
		else SDA = 0;					//comd×î¸ßÎ»Îª"0",ÔòÖÃSDA=0
		_nop_();_nop_();_nop_();_nop_();	//Êý¾ÝÎÈ¶¨Ò»¶¨Ê±¼ä
		_nop_();_nop_();_nop_();_nop_();	//Êý¾ÝÎÈ¶¨Ò»¶¨Ê±¼ä
		SCK = 1;						//ÖÃµÍSCKÎª²úÉúÉÏÉýÑØ×÷×¼±¸
		_nop_();_nop_();_nop_();_nop_();	//Êý¾ÝÎÈ¶¨Ò»¶¨Ê±¼ä
		_nop_();_nop_();_nop_();_nop_();	//Êý¾ÝÎÈ¶¨Ò»¶¨Ê±¼ä
		SCK=0;							//ÖÃ¸ßSCK²úÉúÉÏÉýÑØ
		_nop_();_nop_();_nop_();_nop_();	//Êý¾ÝÎÈ¶¨Ò»¶¨Ê±¼ä
		_nop_();_nop_();_nop_();_nop_();	//Êý¾ÝÎÈ¶¨Ò»¶¨Ê±¼ä
        comd = comd<<1;					//Íù¸ßÎ»ÒÆÒ»Î»¼ÌÐø

	}
	CS = 1;								//ÖÃ¸ßCS±íÊ¾ÈÎºÎ¶¯×÷¶ÔÐ¾Æ¬ÎÞÐ§
}

// ====================================//
// 4Ïß·½Ê½µÄÐ´Êý¾Ý×Ó³ÌÐò
// ÊäÈë²ÎÊý:dat
// Êä³ö²ÎÊý:ÎÞ
// ====================================//
void ST7567_WriteData(unsigned char dat)
{
	unsigned char i;
	CS = 0;								//ÖÃµÍCS±íÊ¾Ñ¡ÖÐÐ¾Æ¬¿ÉÒÔÓÐÐ§µÄ¶ÔÐ¾Æ¬½øÐÐ¿ØÖÆ
	RS = 1;								//ÖÃ¸ßRS±íÊ¾Ö´ÐÐÊý¾ÝµÄ´«ËÍ
	SCK = 0;							//ÖÃµÍSCKÎª²úÉúÉÏÉýÑØ×÷×¼±¸
	for(i=0; i<8; i++)
	{//Ñ­»·´«ËÍ¹²8Î»´Ó¸ßÎ»¿ªÊ¼
		if( (dat&0x80)==0x80 ) SDA = 1;//ÅÐ¶ÏcomdµÄ×î¸ßÎ»,ÊÇ"1"ÔòÖÃSDA=1
		else SDA = 0;					//comd×î¸ßÎ»Îª"0",ÔòÖÃSDA=0
		_nop_();_nop_();_nop_();_nop_();	//Êý¾ÝÎÈ¶¨Ò»¶¨Ê±¼ä
		_nop_();_nop_();_nop_();_nop_();	//Êý¾ÝÎÈ¶¨Ò»¶¨Ê±¼ä
		SCK = 1;						//ÖÃµÍSCKÎª²úÉúÉÏÉýÑØ×÷×¼±¸
		_nop_();_nop_();_nop_();_nop_();	//Êý¾ÝÎÈ¶¨Ò»¶¨Ê±¼ä
		_nop_();_nop_();_nop_();_nop_();	//Êý¾ÝÎÈ¶¨Ò»¶¨Ê±¼ä
		SCK=0;							//ÖÃ¸ßSCK²úÉúÉÏÉýÑØ
		_nop_();_nop_();_nop_();_nop_();	//Êý¾ÝÎÈ¶¨Ò»¶¨Ê±¼ä
		_nop_();_nop_();_nop_();_nop_();	//Êý¾ÝÎÈ¶¨Ò»¶¨Ê±¼ä
        dat = dat<<1;					//Íù¸ßÎ»ÒÆÒ»Î»¼ÌÐø
	}
	CS = 1;								//ÖÃ¸ßCS±íÊ¾ÈÎºÎ¶¯×÷¶ÔÐ¾Æ¬ÎÞÐ§
}

void ST7567_WriteData1(unsigned char dat)
{
	unsigned char i;
	CS = 1;								//ÖÃµÍCS±íÊ¾Ñ¡ÖÐÐ¾Æ¬¿ÉÒÔÓÐÐ§µÄ¶ÔÐ¾Æ¬½øÐÐ¿ØÖÆ
	RS = 1;								//ÖÃ¸ßRS±íÊ¾Ö´ÐÐÊý¾ÝµÄ´«ËÍ
	SCK = 0;							//ÖÃµÍSCKÎª²úÉúÉÏÉýÑØ×÷×¼±¸
	for(i=0; i<8; i++)
	{//Ñ­»·´«ËÍ¹²8Î»´Ó¸ßÎ»¿ªÊ¼
		if( (dat&0x80)==0x80 ) SDA = 1;//ÅÐ¶ÏcomdµÄ×î¸ßÎ»,ÊÇ"1"ÔòÖÃSDA=1
		else SDA = 0;					//comd×î¸ßÎ»Îª"0",ÔòÖÃSDA=0
		_nop_();_nop_();_nop_();_nop_();	//Êý¾ÝÎÈ¶¨Ò»¶¨Ê±¼ä
		_nop_();_nop_();_nop_();_nop_();	//Êý¾ÝÎÈ¶¨Ò»¶¨Ê±¼ä
		SCK = 1;						//ÖÃµÍSCKÎª²úÉúÉÏÉýÑØ×÷×¼±¸
		_nop_();_nop_();_nop_();_nop_();	//Êý¾ÝÎÈ¶¨Ò»¶¨Ê±¼ä
		_nop_();_nop_();_nop_();_nop_();	//Êý¾ÝÎÈ¶¨Ò»¶¨Ê±¼ä
		SCK=0;							//ÖÃ¸ßSCK²úÉúÉÏÉýÑØ
		_nop_();_nop_();_nop_();_nop_();	//Êý¾ÝÎÈ¶¨Ò»¶¨Ê±¼ä
		_nop_();_nop_();_nop_();_nop_();	//Êý¾ÝÎÈ¶¨Ò»¶¨Ê±¼ä
        dat = dat<<1;					//Íù¸ßÎ»ÒÆÒ»Î»¼ÌÐø
	}
	CS = 1;								//ÖÃ¸ßCS±íÊ¾ÈÎºÎ¶¯×÷¶ÔÐ¾Æ¬ÎÞÐ§
}

*/

// ====================================//
// 8Î»²¢ÐÐÊý¾Ý×ÜÏßµÄÐ´Ö¸Áî×Ó³ÌÐò
// ÊäÈë²ÎÊý:comd
// Êä³ö²ÎÊý:ÎÞ
// ====================================//


void ST7567_WriteComd(unsigned char comd)
{
	//8080Ê±Ðò
  CS = 0;								//ÖÃµÍCS±íÊ¾Ñ¡ÖÐÐ¾Æ¬¿ÉÒÔÓÐÐ§µÄ¶ÔÐ¾Æ¬½øÐÐ¿ØÖÆ
	RS = 0;								//ÖÃµÍRS±íÊ¾Ö´ÐÐÖ¸Áî´«Êä
	E_RD = 1;							//ÖÃ¸ßE_RDÐ´¶¯×÷×¼±¸
	dataport = comd;					//°Ñcomd´«µÝ¹ýÀ´µÄ²ÎÊýÐ´Èëµ½LCDÊý¾ÝÏßÉÏ
	RW_WR = 0;							//ÖÃµÍRW_WR½øÐÐÉÏÉýÑØµÄ²úÉú×÷×¼±¸
	_nop_();_nop_();_nop_();_nop_();	//Êý¾Ý½¨Á¢Ò»¶¨Ê±¼ä
	RW_WR = 1;							//ÖÃ¸ßRW_WR²úÉúÉÏÉýÑØÊý¾Ý´ËÊ±Ëø´æ½øÐ¾Æ¬ÄÚ²¿µçÂ·
	_nop_();_nop_();_nop_();_nop_();	//Êý¾Ý±£³ÖÒ»¶¨Ê±¼ä
	CS = 1;								//ÖÃ¸ßCS±íÊ¾ÈÎºÎ¶¯×÷¶ÔÐ¾Æ¬ÎÞÐ§


// //	6800Ê±Ðò
//	CS = 0;								//ÖÃµÍCS±íÊ¾Ñ¡ÖÐÐ¾Æ¬¿ÉÒÔÓÐÐ§µÄ¶ÔÐ¾Æ¬½øÐÐ¿ØÖÆ,Ë³Ðò1
//	RS = 0;								//ÖÃµÍRS±íÊ¾Ö´ÐÐÖ¸Áî´«Êä,Ë³Ðò2
//	RW_WR = 0;							//ÖÃµÍRW_WR±íÊ¾Ð´¶¯×÷×¼±¸,Ë³Ðò3
//	E_RD = 1;							//ÖÃ¸ßE_RD½øÐÐÏÂ½µÑØµÄ²úÉú×÷×¼±¸,Ë³Ðò4
//	dataport = comd;					//°Ñcomd´«µÝ¹ýÀ´µÄ²ÎÊýÐ´Èëµ½LCDÊý¾ÝÏßÉÏ,Ë³Ðò5
//	_nop_();_nop_();_nop_();_nop_();	//Êý¾Ý±£³ÖÒ»¶¨Ê±¼ä
//	E_RD = 0;							//ÖÃµÍE_RD²úÉúÏÂ½µÑØÊý¾Ý´ËÊ±Ëø´æ½øÐ¾Æ¬ÄÚ²¿µçÂ·,Ë³Ðò6
//	_nop_();_nop_();_nop_();_nop_();	//Êý¾Ý½¨Á¢Ò»¶¨Ê±¼ä
//	CS = 1;								//ÖÃ¸ßCS±íÊ¾ÈÎºÎ¶¯×÷¶ÔÐ¾Æ¬ÎÞÐ§,Ë³Ðò7
}

// ====================================//
// 8Î»²¢ÐÐÊý¾Ý×ÜÏßµÄÐ´Êý¾Ý×Ó³ÌÐò
// ÊäÈë²ÎÊý:dat
// Êä³ö²ÎÊý:ÎÞ
// ====================================//
void ST7567_WriteData(unsigned char dat)
{
	//8080Ê±Ðò
	CS = 0;								//ÖÃµÍCS±íÊ¾Ñ¡ÖÐÐ¾Æ¬¿ÉÒÔÓÐÐ§µÄ¶ÔÐ¾Æ¬½øÐÐ¿ØÖÆ
	RS = 1;								//ÖÃ¸ßRS±íÊ¾Ö´ÐÐÊý¾Ý´«Êä
	E_RD = 1;							//ÖÃ¸ßE_RDÐ´¶¯×÷×¼±¸
	dataport = dat;						//°Ñdat´«µÝ¹ýÀ´µÄ²ÎÊýÐ´Èëµ½LCDÊý¾ÝÏßÉÏ
	RW_WR = 0;							//ÖÃµÍRW_WR½øÐÐÉÏÉýÑØµÄ²úÉú×÷×¼±¸
	_nop_();_nop_();_nop_();_nop_();	//Êý¾Ý½¨Á¢Ò»¶¨Ê±¼ä
	RW_WR = 1;							//ÖÃ¸ßRW_WR²úÉúÉÏÉýÑØÊý¾Ý´ËÊ±Ëø´æ½øÐ¾Æ¬ÄÚ²¿µçÂ·
	_nop_();_nop_();_nop_();_nop_();	//Êý¾Ý±£³ÖÒ»¶¨Ê±¼ä
	CS = 1;								//ÖÃ¸ßCS±íÊ¾ÈÎºÎ¶¯×÷¶ÔÐ¾Æ¬ÎÞÐ§

//	//6800Ê±Ðò
//	CS = 0;								//ÖÃµÍCS±íÊ¾Ñ¡ÖÐÐ¾Æ¬¿ÉÒÔÓÐÐ§µÄ¶ÔÐ¾Æ¬½øÐÐ¿ØÖÆ,Ë³Ðò1
//	RS = 1;								//ÖÃ¸ßRS±íÊ¾Ö´ÐÐÊý¾Ý´«Êä
//	RW_WR = 0;							//ÖÃµÍRW_WR±íÊ¾Ð´¶¯×÷×¼±¸,Ë³Ðò3
//	E_RD = 1;							//ÖÃ¸ßE_RD½øÐÐÏÂ½µÑØµÄ²úÉú×÷×¼±¸,Ë³Ðò4
//	dataport = dat;						//°Ñdat´«µÝ¹ýÀ´µÄ²ÎÊýÐ´Èëµ½LCDÊý¾ÝÏßÉÏ,Ë³Ðò5
//	_nop_();_nop_();_nop_();_nop_();	//Êý¾Ý±£³ÖÒ»¶¨Ê±¼ä
//	E_RD = 0;							//ÖÃµÍE_RD²úÉúÏÂ½µÑØÊý¾Ý´ËÊ±Ëø´æ½øÐ¾Æ¬ÄÚ²¿µçÂ·,Ë³Ðò6
//	_nop_();_nop_();_nop_();_nop_();	//Êý¾Ý½¨Á¢Ò»¶¨Ê±¼ä
//	CS = 1;								//ÖÃ¸ßCS±íÊ¾ÈÎºÎ¶¯×÷¶ÔÐ¾Æ¬ÎÞÐ§,Ë³Ðò7
}



 void ST7567_WriteData1(unsigned char dat)
{
	//8080Ê±Ðò
	CS = 1;								//ÖÃµÍCS±íÊ¾Ñ¡ÖÐÐ¾Æ¬¿ÉÒÔÓÐÐ§µÄ¶ÔÐ¾Æ¬½øÐÐ¿ØÖÆ
	RS = 1;								//ÖÃ¸ßRS±íÊ¾Ö´ÐÐÊý¾Ý´«Êä
	E_RD = 1;							//ÖÃ¸ßE_RDÐ´¶¯×÷×¼±¸
	dataport = dat;						//°Ñdat´«µÝ¹ýÀ´µÄ²ÎÊýÐ´Èëµ½LCDÊý¾ÝÏßÉÏ
	RW_WR = 0;							//ÖÃµÍRW_WR½øÐÐÉÏÉýÑØµÄ²úÉú×÷×¼±¸
	_nop_();_nop_();_nop_();_nop_();	//Êý¾Ý½¨Á¢Ò»¶¨Ê±¼ä
	RW_WR = 1;							//ÖÃ¸ßRW_WR²úÉúÉÏÉýÑØÊý¾Ý´ËÊ±Ëø´æ½øÐ¾Æ¬ÄÚ²¿µçÂ·
	_nop_();_nop_();_nop_();_nop_();	//Êý¾Ý±£³ÖÒ»¶¨Ê±¼ä
	CS = 1;								//ÖÃ¸ßCS±íÊ¾ÈÎºÎ¶¯×÷¶ÔÐ¾Æ¬ÎÞÐ§

//	//6800Ê±Ðò
//	CS = 1;								//ÖÃµÍCS±íÊ¾Ñ¡ÖÐÐ¾Æ¬¿ÉÒÔÓÐÐ§µÄ¶ÔÐ¾Æ¬½øÐÐ¿ØÖÆ,Ë³Ðò1
//	RS = 1;								//ÖÃ¸ßRS±íÊ¾Ö´ÐÐÊý¾Ý´«Êä
//	RW_WR = 0;							//ÖÃµÍRW_WR±íÊ¾Ð´¶¯×÷×¼±¸,Ë³Ðò3
//	E_RD = 1;							//ÖÃ¸ßE_RD½øÐÐÏÂ½µÑØµÄ²úÉú×÷×¼±¸,Ë³Ðò4
//	dataport = dat;						//°Ñdat´«µÝ¹ýÀ´µÄ²ÎÊýÐ´Èëµ½LCDÊý¾ÝÏßÉÏ,Ë³Ðò5
//	_nop_();_nop_();_nop_();_nop_();	//Êý¾Ý±£³ÖÒ»¶¨Ê±¼ä
//	E_RD = 0;							//ÖÃµÍE_RD²úÉúÏÂ½µÑØÊý¾Ý´ËÊ±Ëø´æ½øÐ¾Æ¬ÄÚ²¿µçÂ·,Ë³Ðò6
//	_nop_();_nop_();_nop_();_nop_();	//Êý¾Ý½¨Á¢Ò»¶¨Ê±¼ä
//	CS = 1;								//ÖÃ¸ßCS±íÊ¾ÈÎºÎ¶¯×÷¶ÔÐ¾Æ¬ÎÞÐ§,Ë³Ðò7
}




// ====================================//
// ¶Ô±È¶ÈÉè¶¨¼°ÏÔÊ¾¶Ô±È¶È²ÎÊýº¯Êý
// ÊäÈë²ÎÊý:ADJUST
// Êä³ö²ÎÊý:ÎÞ
// ÊµÏÖ¶Ô±È¶ÈµÄ¸üÐÂ,¼°ÏÔÊ¾µ±Ç°µ÷ÕûµÄÊýÖµ
// ====================================//
void SetContrast(void)
{
	unsigned char i,j,page;	//¶¨Òå¾Ö²¿±äÁ¿

	ST7567_WriteComd(0x81);	//¶Ô±È¶Èµ÷½Ú¼Ä´æÆ÷
	ST7567_WriteComd(ADJUST);	//Ð´Èë¶Ô±È¶ÈÊýÖµ

	i=ADJUST;
	i=i/10;		//È¡³ö¶Ô±È¶ÈÊýÖµµÄÊ®Î»
	page=0xb2;	//ÔÚ0xb2~0xb3Ò³ÉÏÏÔÊ¾16*8DOTS
	ST7567_WriteComd(page);	//Éè¶¨Ò³µØÖ·
	ST7567_WriteComd(0x10);	//Ë«×Ö½ÚÉè¶¨ÆðÊ¼SEG
	ST7567_WriteComd(4);		//´ÓSEG4¿ªÊ¼ÏÔÊ¾
	for(j=0;j<8;j++)
	{
		ST7567_WriteData( ASC[(i)*16+j] );	//ÏÔÊ¾×Ö·ûµÄÉÏ°ë½Ú8*8
	}
	page++;	//Ò³µØÖ·¼Ó1
	ST7567_WriteComd(page);	//Éè¶¨Ò³µØÖ·
	ST7567_WriteComd(0x10);	//Ë«×Ö½ÚÉè¶¨ÆðÊ¼SEG
	ST7567_WriteComd(4);		//´ÓSEG4¿ªÊ¼ÏÔÊ¾
	for(j=8;j<16;j++)
	{
		ST7567_WriteData( ASC[(i)*16+j] );	//ÏÔÊ¾×Ö·ûµÄÏÂ°ë½Ú8*8,ÕâÑùÍê³ÉÒ»¸öÕû×ÖµÄÏÔÊ¾
	}

	i=ADJUST;
	i=i%10;		//È¡³ö¶Ô±È¶ÈÊýÖµµÄ¸öÎ»
	page=0xb2;	//ÔÚ0xb2~0xb3Ò³ÉÏÏÔÊ¾16*8DOTS
	ST7567_WriteComd(page);	//Éè¶¨Ò³µØÖ·
	ST7567_WriteComd(0x10);	//Ë«×Ö½ÚÉè¶¨ÆðÊ¼SEG
	ST7567_WriteComd(12);		//´ÓSEG12¿ªÊ¼ÏÔÊ¾
	for(j=0;j<8;j++)
	{
		ST7567_WriteData( ASC[(i)*16+j] );	//ÏÔÊ¾×Ö·ûµÄÉÏ°ë½Ú8*8
	}
	page++;	//Ò³µØÖ·¼Ó1
	ST7567_WriteComd(page);	//Éè¶¨Ò³µØÖ·
	ST7567_WriteComd(0x10);	//Ë«×Ö½ÚÉè¶¨ÆðÊ¼SEG
	ST7567_WriteComd(12);		//´ÓSEG12¿ªÊ¼ÏÔÊ¾
	for(j=8;j<16;j++)
	{
		ST7567_WriteData( ASC[(i)*16+j] );	//ÏÔÊ¾×Ö·ûµÄÏÂ°ë½Ú8*8,ÕâÑùÍê³ÉÒ»¸öÕû×ÖµÄÏÔÊ¾
	}
}




// ====================================//
// °´¼ü´¦Àíº¯Êý(´ø¶¨Ê±¹¦ÄÜ×ÜÌå¶¨Ê±Ô¼1ms*Xms)
// ÊäÈë²ÎÊý:Xms
// Êä³ö²ÎÊý:ÔÝÍ£¼ü°´ÏÂ±êÖ¾
// ¹¦ÄÜËµÃ÷: 
// 1, ÔÚ¶¨Ê±¹ý³ÌÖÐÊ×ÏÈÅÐ¶ÏÊÇ·ñÓÐÔÝÍ£¼ü°´ÏÂ,ÈçÓÐÔò¶¨Ê±Ê§Ð§,´ËÊ±Í¬Ê±ÅÐ¶Ï¶Ô±È¶ÈÉÏÏÂµ÷½Ú¼ü¼°ÔÝÍ£¼ü,
//    Èç¹û¶Ô±È¶Èµ÷½Ú¼üÓÐ°´ÏÂÔò½øÐÐ¶Ô±È¶Èµ÷Õû,Èç¹ûÔÝÍ£¼üÓÐ°´ÏÂÔò¼ÇÂ¼ÏÂÔÝÍ£¼üÓÐ°´ÏÂµÄ×´Ì¬²¢·µ»ØÒ»¸ö"1"²ÎÊý²¢ÍË³ö³ÌÐò.
// 2, ÔÚ¶¨Ê±¹ý³ÌÖÐÔÝÍ£¼üÒ»Ö±Ã»ÓÐ¶¯×÷,ÔòÖ±½Ó·µ»Ø"0"²ÎÊý²¢ÍË³ö³ÌÐò.
// ====================================//
unsigned char Change(unsigned int Xms)
{
	unsigned int i;
	unsigned char j,h;
	j=h=0;
	for(i=0;i<Xms;i++)
	{//Ðè¶¨Ê±µÄmsÊý
		for(j=0;j<150;j++)
		{//¶¨Ê±µÄ»ùÊýÔ¼1ms
			if( SW==0 )
			{//Èç¹ûÔÝÍ£¼ü°´ÏÂÓÐÐ§
				Delay_Xms(10);	//Ïû¶¶¶¯
				if ( SW==0 )
				{//ÔÝÍ£¼ü°´ÏÂÓÐÐ§
					while( SW==0 );	//µÈ´ýÔÝÍ£¼üµ¯Æð
					Delay_Xms(10);	//Ïû¶¶¶¯
					while(1)
					{
						if( SW==0 )
						{//ÔÝÍ£¼üÔÙ´Î°´ÏÂ
							Delay_Xms(10);	//Ïû¶¶¶¯
							if ( SW==0 )
							{//ÔÝÍ£¼ü°´ÏÂÓÐÐ§
								while( SW==0 );	//µÈ´ýÔÝÍ£¼üµ¯Æð
								Delay_Xms(10);	//Ïû¶¶¶¯
								h = 1;	//°ÑÔÝÍ£¼ü±êÖ¾ÖÃ1
								goto Eixt;	//Ìøµ½ÍË³ö³ÌÐò±êºÅ
							}
						}
						else if( UP==0 )
						{//¶Ô±È¶È+¼ü°´ÏÂ
							Delay_Xms(10);	//Ïû¶¶¶¯
							if ( UP==0 )
							{//¶Ô±È¶È+¼ü°´ÏÂÓÐÐ§
								while( UP==0 )	//µÈ´ý´Ë¼üµ¯Æð
								{
									if( DN==0 )
									{
										Delay_Xms(5);	//Ïû¶¶¶¯
										if ( DN==0 )
										{//¶Ô±È¶È-¼ü°´ÏÂÓÐÐ§
											while( (DN==1)&&(UP==1) );	//µÈ´ý´Ë¼üµ¯Æð
											Delay_Xms(5);	//Ïû¶¶¶¯
											h = 1;	//°ÑÔÝÍ£¼ü±êÖ¾ÖÃ1
											goto Eixt;	//Ìøµ½ÍË³ö³ÌÐò±êºÅ
										}
									}
								}
								Delay_Xms(5);	//Ïû¶¶¶¯
								ADJUST++;	//¶Ô±È¶È¼Ä´æÆ÷¼ÓÒ»µµ
								//======adjust contrast=====¶Ô±È¶ÈÉè¶¨
								SetContrast();	
								//==========================
							}				
						}
						else if( (DN==0)&&(UP==1) )
						{//¶Ô±È¶È-¼ü°´ÏÂ
							Delay_Xms(10);	//Ïû¶¶¶¯
							if ( DN==0 )
							{//¶Ô±È¶È-¼ü°´ÏÂÓÐÐ§
								while( DN==0 );	//µÈ´ý´Ë¼üµ¯Æð
								Delay_Xms(5);	//Ïû¶¶¶¯
								ADJUST--;	//¶Ô±È¶È¼Ä´æÆ÷¼õÒ»µµ
								//======adjust contrast=====¶Ô±È¶ÈÉè¶¨
								SetContrast();	
								//==========================
							}				
						}
					}
				}
			}
		}
	}
	Eixt: Delay_Xms(1);	//ÍË³ö³ÌÐò±êºÅ
	return (h);	//·µ»Ø²ÎÊý
}

//------------------------------------------------------------Õý·½ÐÎ												   
//---------------------------------------------------------------

