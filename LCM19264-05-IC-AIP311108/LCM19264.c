#include "lcm19264.h"
#include "pic.h"
bit  LOG =0; 
unsigned char Page;     
unsigned char Col; 



unsigned int  ReadKey() // 按键检测函数
{
    if (LCD_KEY == 1)           
	{  
        Delay(1);  
        if (LCD_KEY == 1) 
		{  
            while (LCD_KEY == 1);  
            return 1;  
        }
    }
    return 0;  // 按键没有被按下
}

void Busy(unsigned char cs1, unsigned char cs2, unsigned char cs3)
{
    unsigned char flag = 0xFF;

    CS1 = cs1;
    CS2 = cs2;
    CS3 = cs3;

    RS = 0;        // 读状态
    RW = 1;        // 读操作
    P1 = 0xFF;     // 设置为输入模式

    while ((flag & 0x80) == 0x80)  // 忙等待
    {
        E = 1;
        _nop_();
        flag = P1;
        E = 0;
    }
}
   
  
void Locatexy(void)   
{   
	 unsigned char x,y;   
	 switch (Col&0xc0) /*  col.and.0xC0 */   
	 {    
	  case 0:    {Busy(0, 1, 1);;break;}  
	  case 0x40: {Busy(1, 0, 1);;break;}  
	  case 0x80: {Busy(1, 1, 0);;break;} 
	 }   
	 x = Col&0x3F |0x40; /* col.and.0x3f.or.Set X Address*/   
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
 

void WriteCommand(unsigned char cs1, unsigned char cs2, unsigned char cs3, unsigned char CommandByte)
{
    Busy(cs1, cs2, cs3);     // 先等忙
    P1 = CommandByte;        // 写入命令
    RS = 0;                  // 命令模式
    RW = 0;                  // 写操作
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
	 CS1=1;
	 CS2=1;
	 CS3=1;
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
	 CS1=1;
	 CS2=1;
	 CS3=1; 
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
	 WriteCommand(0,1,1,0x3f); //DISPALY ON
	 WriteCommand(1,0,1,0x3f);   
	 WriteCommand(1,1,0,0x3f);   
	   
	 WriteCommand(0,1,1,0xc0); //SET START ADDRESS  
	 WriteCommand(1,0,1,0xc0);   
	 WriteCommand(1,1,0,0xc0);   
	   
	 WriteCommand(0,1,1,0x3f); //DISPLAY ON  
	 WriteCommand(1,0,1,0x3f);   
	 WriteCommand(1,1,0,0x3f);   
		 
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
	for(k=0;k<24;k++)
 	{ Display_8_8(character,0);
 	}
 Col  = 0;
 }
  Delay(1500);

 for(c=0;c<8;c++)
 { Page = c; 
	for(k=0;k<24;k++)
 	{ Display_8_8(character,1);
 	}
 Col  = 0;
 }
  Delay(1500);

 for(c=0;c<8;c++)
 { Page = c; 
	for(k=0;k<24;k++)
 	{ Display_8_8(character,2);
 	}
 Col  = 0;
 }
  Delay(1500);

}


void Display_Chinese()
{
	 unsigned char a=0;
	
//	 Page = 0;   
//	 Col  = 0;
//	 for(a=0;a<12;a++)
//	 {
//	 Display_16_16(font,a);
//	 }
	 
	 Page = 2;   
	 Col  = 18;
	 for(a=0;a<10;a++)
	 {
	 Display_16_16(font,a);
	 }

	 Page = 4;   
	 Col  = 10;
	 for(a=10;a<21;a++)
	 {
	 Display_16_16(font,a);
	 }

//	 Page = 6;   
//	 Col  = 0;
//	 for(a=36;a<48;a++)
//	 {
//	 Display_16_16(font,a);
//	 }

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
 while(LOG);
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
	LOG=!LOG;
    while(!int0);
  return;

}