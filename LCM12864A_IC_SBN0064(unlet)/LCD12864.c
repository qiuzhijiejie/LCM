#include <INTRINS.H>    
#include <STDIO.H>     
#include <MATH.H>    
#include <ABSACC.H>    
#include <REG52.H>       
#include "lcd12864.h"   
#include "picture.h"    

sbit RS  =P3^0;    
sbit RW  =P3^1;     
sbit E   =P3^2;    
   
sbit CS1 =P3^3;    //active "H"
sbit CS2 =P3^4;    //active "H"   
sbit int0=P2^1; 
sbit KEY=P2^0; 
bit loge=0;  

unsigned char Page;     
unsigned char Col; 

///***************************/   
/*Check Busy                 */   
/***************************/   
void BusyL(void)   
{unsigned char flag=0xff;  
 CS1= 1;       //active "H"
 CS2= 0;   
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
   
void BusyR(void)   
{ unsigned char flag=0xff;  
 CS1= 0;   
 CS2= 1;        //active "H"
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
  case 0x40: {BusyR();break;}  
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
/***************************/   
/*Write command 1-64               */   
/***************************/      
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
/***************************/   
/*Write command 65-128                */   
/***************************/     
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

/***************************/   
/*Read data                 */   
/***************************/   
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
 CS1=0;
 CS2=0;
 return DataByte;   
}   
   
 /***************************/   
/*Write data                 */   
/***************************/
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
 CS1=0;
 CS2=0;
}   
   
void LcmClear( void )   
{   
 Page = 0;   
 Col  = 0;   
 for(Page=0;Page<8;Page++)   
  for(Col=0;Col<128;Col++)   
   WriteData(0);   
}   
   
void LcmInit( void )   
{   
 WriteCommandL(0x3f); //DISPALY ON
 WriteCommandR(0x3f);   
   
 WriteCommandL(0xc0); //SET START ADDRESS  
 WriteCommandR(0xc0);   
   
 WriteCommandL(0x3f); //DISPLAY ON  
 WriteCommandR(0x3f);   
     
}   
   
void LcmPutDots( unsigned char DotByte,unsigned char DotByte1 )   
{   
 Page = 0;   
 Col  = 0;   
 for(Page=0;Page<8;Page++)   
 {   
  for(Col=0;Col<128;Col++)   
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
  for(Col=0;Col<128;Col++)   
  {   
   WriteData( puts[X] );   
   X++;   
  }   
 }   
}   

void LcmReverseBMPP( unsigned char *puts )   
{   
 unsigned int X=0;   
 Page = 0;   
 Col  = 0;   
 for(Page=0;Page<8;Page++)   
 {   
  for(Col=0;Col<128;Col++)   
  {   
   WriteData(~puts[X] );   
   X++;   
  }   
 }   
}     

/*void LcmReverseBMP( void )   
{   
 unsigned char temp;   
 Page = 0;   
 Col  = 0;   
 for(Page=0;Page<8;Page++)   
 {   
  for(Col=0;Col<128;Col++)   
  {   
   temp = ReadData();   

   temp = ReadData();   
   temp = ~temp;   
   WriteData(temp);   
  }   
 }   
}*/   

void Display_8_8(unsigned char C[],unsigned char i)
{ unsigned char k,num=0;
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
 unsigned int	num=0;
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
  Delay(80); 
}

void Display_character()
{ unsigned char k,c, a=0;
 
 Col  = 0;
 for(c=0;c<8;c++)
 { Page = c; 
	for(k=0;k<16;k++)
 	{ Display_8_8(character,0);
 	}
 Col  = 0;
 }
  Delay(500);

 for(c=0;c<8;c++)
 { Page = c; 
	for(k=0;k<16;k++)
 	{ Display_8_8(character,1);
 	}
 Col  = 0;
 }
  Delay(500);

 for(c=0;c<8;c++)
 { Page = c; 
	for(k=0;k<16;k++)
 	{ Display_8_8(character,2);
 	}
 Col  = 0;
 }

}



void Display_Chinese()
{unsigned char a=0;
// Page = 0;   
// Col  = 0;
// for(a=0;a<8;a++)
// {
// Display_16_16(font,a);
// }
	LcmPutBMP(BMP0); 
 Page = 2;   
 Col  = 0;
 for(a=0;a<8;a++)
 {
 Display_16_16(font,a);
 }

 Page = 4;   
 Col  = 0;
 for(a=8;a<16;a++)
 {
 Display_16_16(font,a);
 }

// Page = 6;   
// Col  = 0;
// for(a=24;a<32;a++)
// {
// Display_16_16(font,a);
// }

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
 while(loge);
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
	loge=!loge;
    while(!int0);
  return;

}

unsigned char key(void)
{
	if(KEY==0)
	{
	 Delay(1);
	 while(KEY==0);
	 return 1;		
	}
	return 0;
	
}

void DisplayPatterns() 
{
    static int step = 0; 

    switch (step) 
    {
        case 0: LcmPutDots(0x55, 0xaa); Delay(100); break;
        case 1: LcmPutDots(0xaa, 0x55); Delay(100); break;
        case 2: LcmPutDots(0x55, 0x55); Delay(100); break;
        case 3: LcmPutDots(0xaa, 0xaa); Delay(100); break;
        case 4: LcmPutDots(0xff, 0x00); Delay(100); break;
        case 5: LcmPutDots(0x00, 0xff); Delay(100); break;
        case 6: LcmPutDots(0xff, 0xff); Delay(100); break;
        case 7: Display_Chinese();Delay(100); break;
    }
    step++;
    if (step > 7) step = 0;
}

void HandleKey() 
{
	unsigned char mode;
    mode++;           
    mode%=6;    
    LcmClear();       
    switch (mode) 
    {	
        case 0: LcmPutDots(0xff, 0x00); Delay(1000); break;
        case 1: LcmPutDots(0x55, 0xaa); Delay(1000); break;
        case 2: LcmPutDots(0x55, 0x55); Delay(1000); break;
        case 3: LcmPutDots(0x00, 0xff); Delay(1000); break;
        case 4: LcmPutDots(0xaa, 0xaa); Delay(1000); break;
        case 5: Display_Chinese(); Delay(1000); break;


    }
}

