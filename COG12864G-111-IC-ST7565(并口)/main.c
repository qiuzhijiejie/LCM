//#include <reg51.H> 
//#include <intrins.h> 

//sbit lcd_rs=P3^2;    
//sbit lcd_reset=P3^3; 
//sbit lcd_cs1=P3^0;    
//sbit lcd_sid=P3^4;  
//sbit lcd_sclk=P3^1;  

//sbit Rom_IN=P1^0;  
//sbit Rom_OUT=P1^1;
//sbit Rom_SCK=P1^5;  
//sbit Rom_CS=P1^2;   

//#define uchar unsigned char  
//#define uint unsigned int 
//#define ulong unsigned long 
//uchar code jiong1[]={/*--  文字:  囧  --*/ 
///*--  宋体 12;  此字体下对应的点阵为：宽x高=16x16   --*/ 
//0x00,0xFE,0x82,0x42,0xA2,0x9E,0x8A,0x82,0x86,0x8A,0xB2,0x62,0x02,0xFE,0x00,0x00, 
//0x00,0x7F,0x40,0x40,0x7F,0x40,0x40,0x40,0x40,0x40,0x7F,0x40,0x40,0x7F,0x00,0x00}; 
//uchar code lei1[]={/*--  文字:  畾  --*/ 
///*--  宋体 12;  此字体下对应的点阵为：宽x高=16x16   --*/ 
//0x80,0x80,0x80,0xBF,0xA5,0xA5,0xA5,0x3F,0xA5,0xA5,0xA5,0xBF,0x80,0x80,0x80,0x00, 
//0x7F,0x24,0x24,0x3F,0x24,0x24,0x7F,0x00,0x7F,0x24,0x24,0x3F,0x24,0x24,0x7F,0x00}; 




////=============transfer command to LCM=============== 
///*写指令到LCD模块*/ 
//void transfer_command_lcd(int data1)    
//{ 
//	char i; 
//	lcd_rs=0; 
//	for(i=0;i<8;i++) 
//	  { 
//		lcd_sclk=0; 
//		if(data1&0x80) 
//			lcd_sid=1; 
//		else 
//			lcd_sid=0; 
//		lcd_sclk=1; 
//		data1=data1<<=1; 
//	  } 
//} 


///*写数据到LCD模块*/ 
//void transfer_data_lcd(int data1) 
//{ 
//	char i; 
//	lcd_rs=1; 
//	for(i=0;i<8;i++) 
//	  { 
//		lcd_sclk=0; 
//		if(data1&0x80) 
//			lcd_sid=1; 
//		else 
//			lcd_sid=0; 
//		lcd_sclk=1; 
//		data1=data1<<=1; 
//	 } 
//} 
// 
//void delay(int i)                
//{ 
// int j,k; 
// for(j=0;j<i;j++) 
// for(k=0;k<500;k++); 
//}


///*等待一个按键，我的主板是用P2.0与GND之间接一个按键*/ 
//void waitkey() 
//{ 
//	 repeat: 
//	if (P2&0x01) 
//		goto repeat; 
//	else 
//		delay(6); 
//	if 
//		(P2&0x01) goto 
//			repeat; 
//	else 
//	delay(40);; 
//} 
// 
// //============initial 
//void initial_lcd() 
//{ 
//	lcd_cs1=0; 
//	Rom_CS = 1; 
//	lcd_reset=0;     
//	delay(20); 
//	lcd_reset=1;  
//	delay(20);         
//	transfer_command_lcd(0xe2); 
//	delay(5); 
//	transfer_command_lcd(0x2c);  
//	delay(5);  
//	transfer_command_lcd(0x2e);  
//	delay(5); 
//	transfer_command_lcd(0x2f); 
//	delay(5); 
//	transfer_command_lcd(0x23);  /*粗调对比度，可设置范围0x20～0x27*/ 
//	transfer_command_lcd(0x81);  /*微调对比度*/ 
//	transfer_command_lcd(0x25);  /*0x1a,微调对比度的值，可设置范围0x00～0x3f*/ 
//	transfer_command_lcd(0xa2);  /*1/9 偏压比（bias）*/ 
//	transfer_command_lcd(0xc8);  /*行扫描顺序：从上到下*/ 
//	transfer_command_lcd(0xa0);  /*列扫描顺序：从左到右*/ 
//	transfer_command_lcd(0x60);  /*起始行：第一行开始*/ 
//	transfer_command_lcd(0xaf);   
//	lcd_cs1=1; 
//}

///*全屏清屏*/ 
//void clear_screen() 
//{ 
//	unsigned char i,j; 
//	lcd_cs1=0; 
//	Rom_CS = 1;  
//	for(i=0;i<9;i++) 
//	{ 
//		transfer_command_lcd(0xb0+i); 
//		transfer_command_lcd(0x10); 
//		transfer_command_lcd(0x00); 
//		for(j=0;j<132;j++) 
//		{ 
//			transfer_data_lcd(0x00); 
//		} 
//	 } 
//	lcd_cs1=1; 
//} 

///*显示16x16点阵图像、汉字、生僻字或16x16点阵的其他图标*/ 
//void display_graphic_16x16(uint page,uint column,uchar *dp) 
//{ 
//	uint i,j; 
//	uchar page_address; 
//	uchar column_address_L,column_address_H; 
//	
//	page_address = 0xb0+page-1; 
//	column_address_L =column&0x0f; 
//	column_address_H =((column>>4)&0x0f)+0x10; 
//		lcd_cs1=0; 
//	Rom_CS = 1;   
//	
//	for(j=0;j<2;j++) 
//	 { 
//		transfer_command_lcd(page_address+j);  /*设置页地址*/ 
//		transfer_command_lcd(column_address_H); /*设置列地址的高 4 位*/ 
//		transfer_command_lcd(column_address_L); /*设置列地址的低 4 位*/ 
//		for (i=0;i<16;i++) 
//		{  
//			transfer_data_lcd(*dp); /*写数据到 LCD,每写完一个8位的数据后列地址自动加1*/ 
//			dp++; 
//		} 

//	 } 
//	lcd_cs1=1; 
//}


///*显示8x16点阵图像、ASCII, 或8x16点阵的自造字符、其他图标*/ 
//void display_graphic_8x16(uint page,uchar column,uchar *dp) 
//{ 
//	uint i,j; 
//	uchar page_address; 
//	uchar column_address_L,column_address_H; 
//	lcd_cs1=0;  
//	page_address = 0xb0+page-1; 
//	column_address_L =column&0x0f; 
//	column_address_H =((column>>4)&0x0f)+0x10; 
//	
//	for(j=0;j<2;j++) 
//	 { 
//		transfer_command_lcd(page_address+j);  /*设置页地址*/ 
//		transfer_command_lcd(column_address_H); /*设置列地址的高 4 位*/ 
//		transfer_command_lcd(column_address_L); /*设置列地址的低 4 位*/ 
//		for (i=0;i<8;i++) 
//		{  
//			transfer_data_lcd(*dp); 
//			dp++; 
//		} 
//	 } 
//	lcd_cs1=1; 
//}



///*显示5*7点阵图像、ASCII, 或5x7点阵的自造字符、其他图标*/ 
//void display_graphic_5x7(uint page,uchar column,uchar *dp) 
//{ 
//	uint col_cnt; 
//	uchar page_address; 
//	uchar column_address_L,column_address_H; 
//	page_address = 0xb0+page-1; 
//	
//	lcd_cs1=0;  
//	
//	column_address_L =column&0x0f; 
//	column_address_H =((column>>4)&0x0f)+0x10; 
//	transfer_command_lcd(page_address);   /*Set Page Address*/ 
//	transfer_command_lcd(column_address_H); /*Set MSB of column Address*/ 
//	transfer_command_lcd(column_address_L); /*Set LSB of column Address*/
//	
//	for (col_cnt=0;col_cnt<8;col_cnt++) 
//	 {  
//		transfer_data_lcd(*dp); 
//		dp++; 
//	 } 
//	lcd_cs1=1; 
//}

///****送指令到晶联讯字库IC***/ 
//void send_command_to_ROM( uchar datu ) 
//{ 
//	uchar i; 
//	for(i=0;i<8;i++ ) 
//	 { 
//		if(datu&0x80) 
//			Rom_IN = 1; 
//		else 
//			Rom_IN = 0; 
//			datu = datu<<1; 
//			Rom_SCK=0; 
//			Rom_SCK=1; 
//	 } 
//} 


///*字库IC中取汉字或字符数据（1个字节）***/ 
//static uchar get_data_from_ROM( ) 
//{ 
//	uchar i; 
//	uchar ret_data=0; 
//	Rom_SCK=1; 
//	for(i=0;i<8;i++) 
//	 { 
//		Rom_OUT=1; 
//		Rom_SCK=0; 
//		ret_data=ret_data<<1; 
//		if( Rom_OUT ) 
//			ret_data=ret_data+1; 
//		else 
//			ret_data=ret_data+0; 
//		Rom_SCK=1; 
//	 } 
//	return(ret_data); 
//} 


///*从相关地址（addrHigh：地址高8位,addrMid：地址中8位,addrLow：地址8位，共24位）中连续读出DataLen个字节的数据到 pBuff
//的地址*/ 
///*连续读取*/ 
//void get_n_bytes_data_from_ROM(uchar addrHigh,uchar addrMid,uchar addrLow,uchar *pBuff,uchar DataLen ) 
//{ 
//	 uchar i; 
//	 Rom_CS = 0; 
//	 lcd_cs1=1;  
//	 Rom_SCK=0; 
//	 send_command_to_ROM(0x03); 
//	 send_command_to_ROM(addrHigh); 
//	 send_command_to_ROM(addrMid); 
//	 send_command_to_ROM(addrLow); 
//	 for(i = 0; i < DataLen; i++ ) 
//		  *(pBuff+i) =get_data_from_ROM(); 
//	 Rom_CS = 1; 
//}



//ulong  fontaddr=0; 
//void display_GB2312_string(uchar y,uchar x,uchar *text) 
//{ 
// uchar i= 0; 
// uchar addrHigh,addrMid,addrLow ; 
// uchar fontbuf[32];    
//	
// while((text[i]>0x00)) 
// { 
//	 
//  if(((text[i]>=0xb0) &&(text[i]<0xf7))&&(text[i+1]>=0xa1)) 
//  {    
//	  
//   /*国标简体（GB2312）汉字在JLX字库IC中的地址由以下公式来计算：*/ 
//   /*Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1)+ 846)*32+ BaseAdd;BaseAdd=0*/ 
//   /*由于担心8位单片机有乘法溢出问题，所以分三部取地址*/ 
//   fontaddr = (text[i]- 0xb0)*94;  
//   fontaddr += (text[i+1]-0xa1)+846; 
//   fontaddr = (ulong)(fontaddr*32); 
//    
//   addrHigh = (fontaddr&0xff0000)>>16;  /*地址的高8位,共24位*/ 
//   addrMid = (fontaddr&0xff00)>>8;      /*地址的中8位,共24位*/ 
//   addrLow = fontaddr&0xff;      /*地址的低8位,共24位*/ 
//   get_n_bytes_data_from_ROM(addrHigh,addrMid,addrLow,fontbuf,32 );/*取32个字节的数据，存到"fontbuf[32]"*/ 
// 
//	display_graphic_16x16(y,x,fontbuf);/*显示汉字到LCD上，y为页地址，x为列地址，fontbuf[]为数据*/ 
//   i+=2; 
//   x+=16; 
//  } 
//  else if((text[i]>=0x20) &&(text[i]<=0x7e))  
//  {       
//	   unsigned char fontbuf[16];    
//	   fontaddr = (text[i]- 0x20); 
//	   fontaddr = (unsigned long)(fontaddr*16); 
//	   fontaddr = (unsigned long)(fontaddr+0x3b7c0);    
//	   addrHigh = (fontaddr&0xff0000)>>16; 
//	   addrMid = (fontaddr&0xff00)>>8; 
//	   addrLow = fontaddr&0xff; 

//	   get_n_bytes_data_from_ROM(addrHigh,addrMid,addrLow,fontbuf,16 );/*取16个字节 的数据，存到"fontbuf[32]"*/ 
//		
//	   display_graphic_8x16(y,x,fontbuf);/*显示8x16的ASCII字到LCD上，y为页地址， x为列地址，fontbuf[]为数据*/ 
//	   i+=1; 
//	   x+=8; 
//  } 
//  else 
//   i++;  
// } 
//  
//} 

//void display_string_5x7(uchar y,uchar x,uchar *text) 
//{ 
//	 unsigned char i= 0; 
//	 unsigned char addrHigh,addrMid,addrLow ; 
//	 while((text[i]>0x00)) 
//	 { 
//	   
//	  if((text[i]>=0x20) &&(text[i]<=0x7e))  
//	  {       
//		   unsigned char fontbuf[8];    
//		   fontaddr = (text[i]- 0x20); 
//		   fontaddr = (unsigned long)(fontaddr*8); 
//		   fontaddr = (unsigned long)(fontaddr+0x3bfc0);    
//		   addrHigh = (fontaddr&0xff0000)>>16; 
//		   addrMid = (fontaddr&0xff00)>>8; 
//		   addrLow = fontaddr&0xff;  
//		   get_n_bytes_data_from_ROM(addrHigh,addrMid,addrLow,fontbuf,8);//取8个字节的数 
//		   display_graphic_5x7(y,x,fontbuf);/*显示5x7的ASCII字到LCD上，y为页地址，x为 列地址，fontbuf[]为数据*/ 
//		   i+=1; 
//		   x+=8; 
//	  } 
//	  else 
//	  i++;  
//	 } 
//  
//}


//void clear_screen_pattern(int x,int y) 
//{ 
//    unsigned char i, j; 
//    for(i = 0; i < 9; i++) 
//    { 
//        Rom_CS = 0; 
//        transfer_command_lcd(0xb0 + i); 
//        transfer_command_lcd(0x10); 
//        transfer_command_lcd(0x00); 

//        for(j = 0; j < 132; j++) 
//        {
//            if(j % 2 == 1)               // 奇数列亮
//                transfer_data_lcd(x); 
//            else                         // 偶数列灭
//                transfer_data_lcd(y); 
//        }
//    } 
//}

//void main(void) 
// {      
//	   Rom_CS=1; 
//      initial_lcd(); 
//      while(1) 
//      { 

//	 clear_screen_pattern(0xff,0x00) ;
//	delay(2000); 
//	clear_screen_pattern(0x00,0xff) ;
//	delay(2000); 
//	clear_screen_pattern(0x55,0xaa) ;
//	delay(2000); 
//	clear_screen_pattern(0xaa,0x55) ;
//	delay(2000);
//	clear_screen_pattern(0xaa,0xaa) ;
//	delay(2000);
//	clear_screen_pattern(0x55,0x55) ;
//	delay(2000);
////  clear_screen();    //clear all dots 
////       Rom_SCK=0;   
////  display_GB2312_string(1,1,"12864,带中文字库"); //在第1页，第1列，显示一串16x16
////		   Rom_SCK=0; 
////  display_GB2312_string(3,1,"16X16简体汉字库,"); 
////		   Rom_SCK=0; 
////  display_GB2312_string(5,1,"或8X16点阵ASCII,"); 
////     Rom_SCK=0; 
////  display_GB2312_string(7,1,"或5X7点阵ASCII码");           
//////  waitkey();  
////  clear_screen();   
////       Rom_SCK=0; 
////  display_GB2312_string(1,1,"GB2312简体字库及");  
////  display_GB2312_string(3,1,"有图型功能,可自");  
////  display_GB2312_string(5,1,"编大字或图像或生");  
////  display_GB2312_string(7,1,"僻字,例如:囧"); 
////  display_graphic_16x16(7,81,jiong1);     /*在第7页，第81列显示单个自 编生僻汉字“囧”*/ 
////  display_graphic_16x16(7,97,lei1);     /*在第7页，第97列显示单个自编生 僻汉字"畾“*/ 
//////  waitkey();  
////  clear_screen();    
////  display_GB2312_string(1,1,"<!@#$%^&*()_-+\/"); //在第1页，第1列，显示一串16x16 点阵汉字或8*16的ASCII字*/ 
////  display_string_5x7(3,1,"<!@#$%^&*()_-+\/");    //在第3页，第1列，显示一串5x7 
////display_string_5x7(4,1,"YHK ELECTRONICS "); 
////display_string_5x7(5,1,"CO.,LTD. estab- ");  
////display_string_5x7(6,1,"lished at year  "); 
////display_string_5x7(7,1,"2025.focus LCM  "); 
////display_string_5x7(8,1,"888-8888-809"); 
////// waitkey();     
// } 
//} 




#include <reg51.H> 
sbit rs=P3^5;   
sbit rd=P3^0;   
sbit wr=P3^1;   
sbit reset=P3^2; 
sbit cs1=P3^3;  

 
 
void transfer_data(int data1); 
void transfer_command(int data1); 
char code graphic1[]; 
char code graphic2[]; 
char code graphic3[]; 
char code graphic4[]; 
char code graphic5[]; 
char code graphic6[]; 
void delay(int i); 
void Delay1(int i); 
void disp_grap(char *dp); 
void initial_lcd(); 
void clear_screen(); 
void waitkey(); 
void clear_screen_pattern(int x,int y) ;


 //===============main program=================== 
void main(void) 
 {      
//	int i,j,k; 
	initial_lcd(); 
    while(1) 
{ 
	clear_screen_pattern(0xff,0x00) ;
	delay(2000); 
	clear_screen_pattern(0x00,0xff) ;
	delay(2000); 
	clear_screen_pattern(0x55,0xaa) ;
	delay(2000); 
	clear_screen_pattern(0xaa,0x55) ;
	delay(2000);
	clear_screen_pattern(0xaa,0xaa) ;
	delay(2000);
	clear_screen_pattern(0x55,0x55) ;
	delay(2000);
//	clear_screen_row_pattern() ;
//	delay(2000); 
//	clear_screen();   //clear all dots 
//	disp_grap(graphic1);  //display a picture of 128*64 dots 
//	delay(2000);  
  
 }
 } 
 
 
 //============initial 
void initial_lcd() 
{ 
reset=0;                 
delay(20); 
reset=1;   
/*低电平复位*/ 
/*复位完毕*/ 
delay(20);         
transfer_command(0xe2);  /*软复位*/
	delay(5); 
transfer_command(0x2c);  /*升压步聚 1*/ 
delay(5);  
transfer_command(0x2e);  /*升压步聚 2*/ 
delay(5); 
transfer_command(0x2f);  /*升压步聚 3*/ 
delay(5); 
transfer_command(0x23);  /*粗调对比度，可设置范围0x20～0x27*/ 
transfer_command(0x81);  /*微调对比度*/ 
transfer_command(0x2f);  /*微调对比度的值，可设置范围0x00～0x3f*/ 
transfer_command(0xa2);  /*1/9 偏压比（bias）*/ 
transfer_command(0xc8);  /*行扫描顺序：从上到下*/ 
transfer_command(0xa0);  /*列扫描顺序：从左到右*/ 
transfer_command(0x60);  /*起始行：第一行开始*/ 
transfer_command(0xaf);  /*开显示*/ 
}


//===============clear all dot martrics============= 
void clear_screen() 
{ 
	unsigned char i,j; 
	for(i=0;i<9;i++) 
	{ 
		cs1=0; 
		transfer_command(0xb0+i); 
		transfer_command(0x10); 
		transfer_command(0x00); 
		for(j=0;j<132;j++) 
		  {
			transfer_data(0x00); 
		  }
	 } 
}




void clear_screen_pattern(int x,int y) 
{ 
    unsigned char i, j; 
    for(i = 0; i < 9; i++) 
    { 
        cs1 = 0; 
        transfer_command(0xb0 + i); 
        transfer_command(0x10); 
        transfer_command(0x00); 

        for(j = 0; j < 132; j++) 
        {
            if(j % 2 == 1)               // 奇数列亮
                transfer_data(x); 
            else                         // 偶数列灭
                transfer_data(y); 
        }
    } 
}



//==================display a piture of 128*64 dots================ 
void disp_grap(char *dp) 
{ 
			int i,j; 
	for(i=0;i<8;i++) 
	{ 
	cs1=0; 
	transfer_command(0xb0+i);   //set page address, 
	transfer_command(0x10);
	transfer_command(0x00+1); 
	for(j=0;j<128;j++) 
	  {
		 transfer_data(*dp);     
		   dp++;    
	  }
	  }           
}


//=============transfer command to LCM=============== 
void transfer_command(int data1)    
{ 
	cs1=0; 
	rs=0; 
	rd=0; 
	wr=0; 
	P1=data1; 
	rd=1; 
	cs1=1; 
	rd=0; 
} 
//-----------transfer data to LCM--------------- 
void transfer_data(int data1) 
{ 
	cs1=0; 
	rs=1; 
	rd=0; 
	wr=0; 
	P1=data1; 
	rd=1; 
	cs1=1; 
	rd=0; 
} 


//=============delay time===================== 
void delay(int i)  	              
{ 
	 int j,k; 
	 for(j=0;j<i;j++) 
	 for(k=0;k<990;k++); 
} 
//=============delay time===================== 
void Delay1(int i) 
{ 
	 int j,k; 
	 for(j=0;j<i;j++) 
	 for(k=0;k<10;k++); 
} 
//--------------wait a switch,jump out if P2.0 get a signal"0"------------------ 
void waitkey() 
{ 
	 repeat: 
	if (P2&0x01) goto repeat; 
	else delay(6); 
	if (P2&0x01) goto repeat; 
	else 
	delay(40);; 
} 
