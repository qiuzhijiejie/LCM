#ifndef __picture_H__
#define __picture_H__

unsigned char code Img_SUNMAN_128x64[];

#endif



////----------------------------------------------------------------------
///*EASTRISING TECHNOLOGY CO,.LTD.*/
//// Module    : ERM12864-7 Series 
//// Lanuage   : C51 Code
//// Create    : JAVEN
//// Date      : Aug 08 2018
//// Drive IC  : ST7920
//// INTERFACE : Serial Mode(R9=0R)
//// MCU 	: STC89C52RC           
////***************************************************************************   
//#include <INTRINS.H>   
//#include <stdio.h>
//#include <math.h>
//#include <reg52.h>
//#include <string.h>

//unsigned char code IC_DAT[];
//unsigned char code pic1[];
//unsigned char code pic2[];
//unsigned char code pic3[];


//sbit    CS =P2^0;  //RS
//sbit    SID=P2^1;  //RW
//sbit    SCLK=  P2^2;	//E
//sbit    RES=P2^4;

//void 	delay(unsigned int ms);
//void    TransferData(char data1,bit DI);
//void    display(void);
//void    display_grapic(void);
//void    delayms(unsigned int n);
//void    DisplayLine(unsigned char line1,unsigned char line2);
//void    DisplayGraphic(unsigned char code *adder);
//void    delay(unsigned int m);
//void    lcd_mesg(unsigned char code *adder1);


//unsigned char code IC_DAT[]={
//"??????????????"  
//"????????????"
//"?????????????"
//"??????????????"
//};

//void     initinal(void)           //LCD initinal  Fonts Mode
//          {

//           TransferData(0x30,0);  //Extended Function Set :8BIT,RE=0: basic instruction set, G=0 :graphic display OFF
//           delay(100);            
//           TransferData(0x30,0);  //Function Set
//           delay(37);             
//           TransferData(0x08,0);  //Display on Control
//           delay(100);            
//           TransferData(0x10,0);  //Cursor Display Control
//           delay(100);            
//           TransferData(0x0C,0);  //Display Control,D=1,display on
//           delay(100);            
//           TransferData(0x01,0);  //Display Clear
//           delayms(10);             
//           TransferData(0x06,0);  //Enry Mode Set
//           delay(100);            
//          }


//void     initina2(void)           //Graphic display Mode
//          {

//           TransferData(0x30,0);  //Extended Function Set RE=1: extended instruction
//           delay(100);       
//           TransferData(0x01,0);  //Display Clear
//           delayms(10);     
//           TransferData(0x36,0);  //Extended Function Set:RE=1: extended instruction set
//           delay(37);            
//           TransferData(0x3E,0);  //EXFUNCTION(DL=8BITS,RE=1,G=1)
//           delay(100);                      
//          }


//void    main(void)
//     {
// 	 	   delayms(40);             //Delay 40ms
//           RES=0;                 //Reset
//           delay(1);             
//           RES=1;                 //Reset set H
//           delay(10);

//         while(1)
//	    {
//		  initina2();            //Graphic display Mode    

//          DisplayGraphic(pic1);  //show Graphic 
//          delayms(2000);
//  
//          DisplayGraphic(pic2);  //show Graphic 
//          delayms(2000);

//          DisplayGraphic(pic3);  //show Graphic 
//          delayms(2000);

//          initinal();   		 // Fonts Mode
//          delay(100);           
//          lcd_mesg(IC_DAT);      //Show chanese fonts
//          delayms(2000);

// 	  
//        }
//      }



//void   lcd_mesg(unsigned char code *adder1)
//{
//	unsigned char i;
//        TransferData(0x80,0);  //Set RAM Address
//        delay(100);
//	for(i=0;i<32;i++)
//		{
//	  		TransferData(*adder1,1);
//	   		adder1++;
//		}

//        TransferData(0x90,0);  //Set RAM Address
//        delay(100);
//	for(i=32;i<64;i++)
//		{
//	  		TransferData(*adder1,1);
//	   		adder1++;
//		}
//}

//void SendByteLCD(unsigned char WLCDData)
//{unsigned char i;
//for(i=0;i<8;i++)
//	{
//		if((WLCDData<<i)&0x80)SID=1;
//	
//		else SID=0;
//		
//		SCLK=0; ;
//		
//		SCLK=1 ;
//		
//	}
//	
//}

//void  SPIWR(unsigned char Wdata,unsigned char RS)
//{
//SendByteLCD(0xf8+(RS<<1));
//SendByteLCD(Wdata&0xf0);
//SendByteLCD((Wdata<<4)&0xf0);
//}



//void    TransferData(unsigned char data1,bit DI)  //if DI=0  Transfer command DI=1  Transfer data
//          {
//            if(DI==1)SPIWR(data1,1);  //DATA
//			else SPIWR(data1,0);	  //CMD
//                   
//           }

//void    delayms(unsigned int n)            //delay n ms
//                   {
//                      unsigned int i,j;
//                      for(i=0;i<n;i++)
//                         for(j=0;j<125;j++);
//                   }


//void    delay(unsigned int m)           
//                   {
//                     m--;
//                   }

//void DisplayGraphic(unsigned char code *adder)
//{

//   int i,j;
////*******display top half screen
//   for(i=0;i<32;i++)              //
//    { 
//      TransferData((0x80 + i),0); //SET   VERTICAL ADDRESS
//      TransferData(0x80,0);       //SET   HORIZONTAL ADDRESS
//      for(j=0;j<16;j++)
//       {
//         TransferData(*adder,1);
//         adder++;
//       }
//    }
////*******display bottom half screen
//   for(i=0;i<32;i++)              //
//    {
//      TransferData((0x80 + i),0); //SET VERTICAL ADDRESS
//      TransferData(0x88,0);       //SET HORIZONTAL ADDRESS
//      for(j=0;j<16;j++)
//       {
//         TransferData(*adder,1);
//         adder++;
//       }
//    }
//}

//unsigned char code pic1[]={};


//unsigned char code pic2[]={};
//unsigned char code pic3[]={};