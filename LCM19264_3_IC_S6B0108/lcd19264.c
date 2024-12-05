#include <INTRINS.H>    
#include <STDIO.H>     
#include <MATH.H>    
#include <ABSACC.H>    
#include <REG52.H>  
#include "lcd19264.h"
#include "picture.h"

sbit RS  =P3^0;    
sbit RW  =P3^1;     
sbit E   =P3^2;     
sbit CSB =P3^3;    
sbit CSA =P3^4;    
sbit RES =P3^5; 
bit  Log =0;  
sbit int0=P2^1;
sbit KEY=P2^0;

unsigned char Page;     
unsigned char Col;  
 


void BusyL(void)   
{
    unsigned char flag = 0xFF; 
    

    CSB = 0;  
    CSA = 0;   
    RS = 0;    
    RW = 1;   
    P1 = 0xFF; 

    while ((flag & 0x80) == 0x80)
    {
        E = 1;    
        _nop_();     
        flag = P1;   
        E = 0;       
    }
}


void BusyM(void)   
{
    unsigned char flag = 0xFF; 

    
    CSB = 0;   
    CSA = 1;   
    RS = 0;   
    RW = 1;    

    P1 = 0xFF;




    while ((flag & 0x80) == 0x80)
    {
        E = 1;      

        _nop_();    

        flag = P1;   

        E = 0;       
	}
}
   

  
void BusyR(void)   
{
	unsigned char  flag=0xff; 
	CSB= 1;   
	CSA= 0;     
	RS = 0;    
	RW = 1;   
	P1 = 0xFF;   
	while ((flag&0x80)==0x80)
	{
		E=1;
		_nop_();
		flag=P1;
		E=0;
	}  
}   
   
   

void Locatexy(void)   
{   
	 unsigned char x,y;   
	 switch (Col&0xc0) /*  col.and.0xC0 */   
	 {    
		  case 0: {BusyL();break;}  
		  case 0x40: {BusyM();break;}  
		  case 0x80: {BusyR();break;} 
	 }   
	 x = Col&0x3F|0x40; /* col.and.0x3f.or.Set X Address*/   
	 y = Page&0x07|0xB8; /* row.and.0x07.or.set Page */   
	   
	 RS = 0;      
	 RW = 0;      
	 P1 = y;  
	 E  = 1;   
	 _nop_();   
	 E  = 0;   
	 _nop_();   
	 
	 RS = 0;   
	 RW = 0;   
	 P1 = x;      
	 E  = 1;   
	 _nop_();   
	 E  = 0;   
	 _nop_(); 
 

}   

//1-64    
void WriteCommandL( unsigned char CommandByte )   
{   
	 BusyL();   
	 P1 = CommandByte;   
	 RS = 0;   
	 RW = 0;   
	 E  = 1;   
	 _nop_();   
	 E  = 0;   
	 _nop_();   
}   

// 65-128      
void WriteCommandM( unsigned char CommandByte )   
{   
	 BusyM();   
	 P1 = CommandByte;   
	 RS = 0;    
	 RW = 0;   
	 E  = 1;   
	 _nop_();   
	 E  = 0;   
	 _nop_();   
}   

//129-192    
void WriteCommandR( unsigned char CommandByte )   
{   
	 BusyR();   
	 P1 = CommandByte;   
	 RS = 0;   
	 RW = 0;   
	 E  = 1;   
	 _nop_();   
	 E  = 0;   
	 _nop_();   
}   
   

/*Read data  */   
unsigned char ReadData( void )   
{   
	 unsigned char DataByte;   
	 Locatexy();   
	 RS = 1;    
	 RW = 1;  
	 P1 = 0xFF;  
	  _nop_();   
	 
	 E  = 1;    
	 _nop_();   

	 DataByte = P1;  
	 E  = 0;   
	 _nop_();  
	 CSB=1;
	 CSA=1;
	 return DataByte;   
}   
   



 
/*Write data   */   
void WriteData( unsigned char DataByte )   
{   
	 Locatexy();   
	 RS = 1;    
	 RW = 0;     
	 P1 = DataByte; 
	 E  = 1;    
	 _nop_();   
	 E  = 0;   
	 _nop_(); 
	 CSB=1;
	 CSA=1;
}   


   
void LcmClear( void )   
{   
	 Page = 0;   
	 Col  = 0;   
	 for(Page=0;Page<8;Page++)   
	  for(Col=0;Col<192;Col++)   
	   WriteData(0);   
}   

  
void LcmInit( void )   
{  	
	RES=0; 
 	Delay(10);
        RES=1;

	 WriteCommandL(0x3f); //DISPALY ON
	 WriteCommandM(0x3f);   
	 WriteCommandR(0x3f);   
	   
	 WriteCommandL(0xc0); //SET START ADDRESS  
	 WriteCommandM(0xc0);   
	 WriteCommandR(0xc0);   
	   
	 WriteCommandL(0x3f); //DISPLAY ON  
	 WriteCommandM(0x3f);   
	 WriteCommandR(0x3f);   
		 
}   
   
void LcmPutDots( unsigned char DotByte,unsigned char DotByte1 )   
{   
	 Page = 0;   
	 Col  = 0;   
	 for(Page=0;Page<8;Page++)   
	 {   
		  for(Col=0;Col<192;Col++)   
		  {   
			   WriteData( DotByte );
			   Col++;
			   WriteData( DotByte1 );  
		  } 
		
	 }   
}   
   
void LcmPutBMP( unsigned char *puts )   
{   
	 unsigned int X=0;         
	 Page = 0;   
	 Col  = 0;   
	 for(Page=0;Page<8;Page++)   
	 {   
		  for(Col=0;Col<192;Col++)   
		  {   
			   WriteData( puts[X] );   
			   X++;   
		  }   
	 }   
}   
   
void LcmReverseBMP( void )   
{   
 unsigned char temp;   
	 Page = 0;   
	 Col  = 0;   
	 for(Page=0;Page<8;Page++)   
	 {   
		  for(Col=0;Col<192;Col++)   
		  {   
			   temp = ReadData();   

			   temp = ReadData();   
			   temp = ~temp;   
			   WriteData(temp);   
		  }   
	 }   
}   

void Display_8_8(unsigned char C[],unsigned char i)
{ 
	 unsigned char k,num=0;
	 num=i*0x08;
	 for(k=0;k<0x08;k++)
	 {
		  WriteData(C[num+k]);
		  Col+=0x01;
	 } 

}


void Display_16_16(unsigned char F[],unsigned char i)
{
	 unsigned char k=0;
	 unsigned int num=0;
	 num=i*0x20;

	 for(k=0;k<0x10;k++)
	 {
		  WriteData(F[num+k]);
		  Col+=0x01;
	 } 
	  Col-=0x10;
	  Page+=0x01;   
	  for(k=16;k<0x20;k++)
	 {
		  WriteData(F[num+k]);
		  Col+=0x01;
	 } 
	  Page-=0x01; 
	  Delay(10); 
}

void Display_character()
{ 
	unsigned char k,c, a=0;
	 Col  = 0;
	 for(c=0;c<8;c++)
	 { 
		 Page = c; 
		for(k=0;k<24;k++)
		{ 
			Display_8_8(character,0);
		}
	  Col  = 0;
	 }
	  Delay(100);

	 for(c=0;c<8;c++)
	 { Page = c; 
		for(k=0;k<24;k++)
		{ 
			Display_8_8(character,1);
		}
	  Col  = 0;
	 }
	  Delay(100);

	 for(c=0;c<8;c++)
	 { Page = c; 
		for(k=0;k<24;k++)
		{ 
			Display_8_8(character,2);
		}
	  Col  = 0;
	 }
	  Delay(100);


}



void Display_Chinese()
{
	unsigned char a=0;
//	 Page = 0;   
//	 Col  = 0;
//	 for(a=0;a<12;a++)
//	 {
//	   Display_16_16(font,a);
//	 }
//	 Page = 2;   
//	 Col  = 0;
//	 for(a=12;a<24;a++)
//	 {
//		Display_16_16(font,a);
//	 }

//	 Page = 4;   
//	 Col  = 0;
//	 for(a=24;a<36;a++)
//	 {
//		Display_16_16(font,a);
//	 }

//	 Page = 6;   
//	 Col  = 0;
//	 for(a=36;a<48;a++)
//	{
//		Display_16_16(font,a);
//	 }


 Page = 1;   
 Col  = 16;
 for(a=0;a<10;a++)
 {
   Display_16_16(font,a);
 }
 
 Page = 4;   
 Col  = 16;
 for(a=10;a<20;a++)
 {
   Display_16_16(font,a);
 }

}
 
void Delay(unsigned int MS)   
{   
	 unsigned char us,usn;   
	 while(MS!=0)   
	 {   
	  usn = 2; //for 12M    
	  while(usn!=0)   
	  {   
		   us=0xf6;   
		   while (us!=0){us--;};   
		   usn--;   
	  }   
		MS--;   
	 }  
	 while(Log);
}   



void Step(void) interrupt 0 using 0
{ _nop_(); 
  _nop_(); 
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_(); 
  _nop_(); 
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
	if(int0==0)
	Log=!Log;
    while(!int0);
  return;

}


unsigned char key()
{
    if (KEY == 0) 
    {
        Delay(1);  
        if (KEY == 0) 
        {
            while (KEY == 0);
            return 1; 
        }
    }
    return 0; 
}
