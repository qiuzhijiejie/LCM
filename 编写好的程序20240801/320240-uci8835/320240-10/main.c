/********* GY3224170203  IC/SED1335F & IST3225&IST3028 ********/
/*********  1/240Duty  1/16Bias  Vdd=5.0V  Vop=23.2V  ********/


#include <reg52.h>  
#include <intrins.h>

//#include <character.txt>
//#include <character1.txt>
//#include <character2.txt>
//#include <character3.txt>

#define Uchar unsigned char
#define Uint  unsigned int
#define NOP _nop_()

sbit RES=P1^2;
sbit CS =P1^3;
sbit RD_E=P1^4;
sbit WR_RW=P1^5;
sbit A0 =P1^6;

//sbit RES=P3^2;
//sbit CS =P3^5;
//sbit RD_E=P3^0;
//sbit WR_RW=P3^1;
//sbit A0 =P3^3;


void ComWrite(Uchar com);
void DatWrite(Uchar dat);
void Initial(void);
//void DispBmp11(Uchar bmp[],Uchar dot1,Uchar dot2);  //if dot1 =1 then display bmp[],
 void DispBmp(Uchar bmp[]);                                                  // else display dot1 and dot2 
 
void press(void);

//sbit J1=P2^0;
//sbit J2=P2^1;
//sbit J3=P2^2;
//sbit J4=P2^3;		

//Uchar PAGE=0;	
		 
void delay(unsigned int time)
{
  Uint i,j;
	for(i=0;i<100;i++)
	for(j=0;j<time;j++);
}


void checkboard(unsigned char data1,unsigned char data2)
     { 
       Uint m,s;
       ComWrite(0x46);     //CSRW
       DatWrite(0x00);        //p1
       DatWrite(0x00);        //p
       ComWrite(0x42);     //mwrite DISPL    
       for(s=0;s<160;s++)
			 {
				 for(m=0;m<20;m++) 
						{ if(s%2==0) DatWrite(data1);
							else DatWrite(data2);
						}
			 }
     }
/*		 
void character122(void)
     { 
       long m,k=0;
      // init();
       ComWrite(0x59);   //DISP ON
       DatWrite(0x06); 
       ComWrite(0x4c);   //CSR DIR
       ComWrite(0x42);   //mwrite DISPLAY DATA OUTPUT
       for(m=0;m<1200;m++)
       DatWrite(0x20);      //fill blank 
       ComWrite(0x46);   //CSRW
       DatWrite(0x00);      //p1
       DatWrite(0x00);      //p
     // ComWrite(0x4c);  //CSR DIR      
       ComWrite(0x42);   //mwrite DISPLAY DATA OUTPUT
       for(m=0;m<1200;m++)
         { DatWrite(0x20+k); k++;if(k>95)k=0; }
     }

void et(void)
{
long m;//k=0;
      // init();
       ComWrite(0x59);   //DISP ON
       DatWrite(0x06); 
       ComWrite(0x4c);   //CSR DIR
       ComWrite(0x42);   //mwrite DISPLAY DATA OUTPUT
       for(m=0;m<1200;m++)
       DatWrite(0x20);      //fill blank 
       ComWrite(0x46);   //CSRW
       DatWrite(0x00);      //p1
       DatWrite(0x00);      //p
     // ComWrite(0x4c);  //CSR DIR      
       ComWrite(0x42);   //mwrite DISPLAY DATA OUTPUT
       for(m=0;m<1200;m++)
         { DatWrite(0x20+37);
           DatWrite(0x20+52); 
         }
}
*/
void main(void)
 {
   
   Initial();
	 delay(300);
	 
//    ComWrite(0x5b);          //陪ボΘ㏑
//    DatWrite(0x1c);          //跋瓜   .把计1
   while(1)
   {
//			DispBmp(character);
//			delay(1500);
	//		DispBmp(character1);	
	//		delay(1000);
//			DispBmp(character3);	
//			delay(1500);
      
			checkboard(0xff,0xff);
			delay(8500);	
    
			checkboard(0x00,0x00);	
			delay(8500);

			checkboard(0x55,0x55);	
			delay(8500);		 
//			checkboard(0xaa,0xaa);
//			delay(8500);
//      checkboard(0x00,0xff);
//      delay(8500);
//      checkboard(0xff,0x00);
//      delay(8500);

		 
	
  }

 }
 /*
void DispBmp11(Uchar bmp[],Uchar dot1,Uchar dot2)
 {
   Uint cnt,cnt2;
         
   ComWrite(0x46);            //砞竚夹皐㏑
   DatWrite(0x00);                  //               .把计 1 
   DatWrite(0x00);                  //跋       .把计 2
                 
   ComWrite(0x42);            //硈尿糶秨㏑
  if(dot1==1)            for(cnt=0;cnt<9600;cnt++)  DatWrite(bmp[cnt]);            // 320*240/8=9600       						     								  							  							  
    else              
                            for(cnt2=0;cnt2<240;cnt2++)      //240 Com
                             {
                             for(cnt=0;cnt<40;cnt++) DatWrite(dot1);    //320/8=40
                             for(cnt=0;cnt<40;cnt++) DatWrite(dot2);    //320/8=40
                             }     
                        
                           
   delay(10000);
 }
 
 void DispBmp(Uchar bmp[])
 {
   Uint cnt;
         
   ComWrite(0x46);            //砞竚夹皐㏑
   DatWrite(0x00);                  //               .把计 1 
   DatWrite(0x00);                  //跋       .把计 2
                 
   ComWrite(0x42);            //硈尿糶秨㏑
    for(cnt=0;cnt<9600;cnt++)  
	      DatWrite(bmp[cnt]);            // 320*240/8=9600   
	
	 
 }
*/  
void Initial()
{
 
  Uchar System[]={0x32,0x87,0x07,19,0x42,0xa0,20,0x00};             // ╰参砞竚㏑把计 p1~p8
  Uchar Scroll[]={0x00,0x00,0x9f,0x00,0x60,0x9f,0x00,0x70,0x00,0x80}; // 陪ボ办砞竚㏑把计 p1~p10  Uchar cnt;
  Uchar cnt;

  RES  =0;
  delay(500);
  RES  =1;
  delay(500);

  ComWrite(0x40);		    // system set command begin
  for(cnt=0;cnt<=7;cnt++) DatWrite(System[cnt]);  //╰参㏑把计
  ComWrite(0x44);		    // scroll set command begin
  for(cnt=0;cnt<=9;cnt++) DatWrite(Scroll[cnt]);  //陪ボ㏑把计

//  ComWrite(0x5a);     // 闽陪ボ
//  DatWrite(0x00);
//  ComWrite(0x5b);     // 闽陪ボ
//  DatWrite(0x00);
//	
  ComWrite(0x4c);     //夹+1 簿笆よ
  ComWrite(0x59);     // 闽陪ボ
  DatWrite(0x04);     // 秨材跋陪ボ,闽夹 . ㏑把计   //0x04
 }
 

  
void DatWrite(Uchar dat)
{
  CS =0;
  A0 =0;
  RD_E =1;
  P0 =dat;  delay(1);
  WR_RW =0; delay(1);
  WR_RW =1; delay(1);
  CS =1;
	/*
CS=0;
A0=0;
WR=0;
E=0;
P1=dat;
WR=1;
E=1;
CS=1;	
	*/
}
void ComWrite(Uchar com)
{
  CS =0;
  A0 =1;
  RD_E =1; 
  P0 =com; delay(1);
  WR_RW =0;delay(1);
  WR_RW =1;	delay(1);
  CS=1;
/*
CS=0;
A0=1;
WR=0;
E=0;
P1=com;
WR=1;
E=1;
CS=1;	
*/	
}

#if 0
void press(void)
{
	unsigned int i=0;
	P2=0xff;
	
	for (i=0;i<20000;i++)
	{
	  delay(5);
		if (J4==0) //J4
		{
		 delay(10);
				if (J4==0) //J4
			{
				i=20005;
				while(!J4);
		  }
		}
		
		if(J3==0) //J3
	 {
			delay(10);
			if (J3==0) //J3
			{
		  i=20005;
			while(!J3);
     }
		}

		if (J1==0) //J1
		{
		 	delay(10);
				if (J1==0) //J1
			{			
			i=20005;
		 if(PAGE<7)
		 PAGE++;
		 else PAGE=0;
		 while(!J1); 
	   }
		 
		}
	  if(J2==0) //J2
	    {
		 	delay(10);
			if(J2==0)
			{
				i=20005;
		 if(PAGE>0)	 
	     PAGE--;
		 else PAGE=7;
	   while(!J2); 
     } 
		}
	  
	}  
}	

#endif

