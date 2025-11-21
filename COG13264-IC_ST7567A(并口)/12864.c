#include <REGX52.H>
#include "12864.h"
#include <INTRINS.H> 
#include "int/ascii0816x.c"
#include "int/ascii0508x.c"
#include "int/hanzi1616.c"

// Delay microsecond less than 255us
void delayUs(uint Tus)
{
	// If Tus >= 18, the delay is precise, otherwise not!
	if (Tus > 17)
	{
		Tus = Tus/2 - 9;
		if (Tus != 0)
			while(--Tus != 0);
	}
}

// Delay Tms ms
void delayMs(uint Tms)
{
	uint i;
	while(Tms-- != 0)
	{
		i = 4;
		while(i-- != 0)
		delayUs(250);
	}
}
 
   
/****************************reset IC for start****************************/   
void LCD_Reset()   
{   
	 IE  =  0x00;   
	 rst =  0;   
	 delayMs(50);   
	 rst =  1;   
	 delayMs(50);   
}  

//-----------------写指令或者数据--------------------      
void write_cmd_dat(uchar cmda,uchar com)   
{    
	 cs=0;   
	 di=cmda; 
	 rd=1;
	 wr=0; 
	 data=com;  
	 delayUs(2); 
	 wr=1;  
	 delayUs(2); 
} 


// Set Column Address 4
void SetCA(uchar Caddr)
{ 
   write_cmd_dat(0, Caddr & 0x0F);          // Lower 4-bit
   write_cmd_dat(0,(Caddr >> 4) | 0x10);    // Higher 4-bit
}

// Set Page Address 7
void SetPA(uchar Paddr)
{
   write_cmd_dat(0,0xb0+Paddr);
}

//--------------向某页某列写数据----------------------------
void write_data(uint com,uchar page,uchar clum)
{
  SetPA(page);
  SetCA(clum);
  write_cmd_dat(1,com);
}

// Display data RAM page and column address
void GotoXY(uchar Paddr, uchar Caddr)
{
   SetPA(Paddr);
   SetCA(Caddr);
}

//----------------------组装测试-------------------------
void display_test(uchar fst,uchar snd) 
{ 
  uchar i,j;
 for(j=0;j<8;j++)// 遍历所有页(Page),ST7567有8个页,每页8行
 {	
  GotoXY(j,0);// 设置当前页 j,列地址为 0
   for(i=0;i<132/2;i++) // 每页写 64 次,每次写两个字节
  {
   write_cmd_dat(1,fst);
   write_cmd_dat(1,snd);
  }
 }
}

// Clear LCD screen
void Clear(void)
{
	display_test(0x00,0x00);
	delayUs(4);
}  
// Wait some time and clear the screen
void wait_and_clear(void)
{
	delayMs(200);
	Clear();
}
  
void lcd_test()
{
  uchar i;       //行 列
  uchar code tdata[][2]=
  {
  	{0xff,0xff},	// All
	{0x00,0xFF},	// Virtical 
	{0xff,0x00},	// Virtical
	{0xAA,0xAA},	// Horizontal 
	{0x55,0x55},	// Horizontal 
	{0xAA,0x55},	// Stars 
	{0x55,0xaa},	// Stars 
  };
  for(i=0;i<7;i++)
  {
   display_test(tdata[i][0],tdata[i][1]);
   delayMs(800);
   wait_and_clear();
  }
}  

 //------------------显示方框-----------------------
void lcd_kuang(void)
{
 uchar i,j;
  for(i=0;i<8;i++)
   {	 
    for(j=0;j<132;j++)
	 { 
	  if((j==0)||(j==131)) { GotoXY(i,j); write_cmd_dat(1,0xff);}
	  if(i==0)
	   { 
	    if((j>0)&&(j<131)) { GotoXY(i,j); write_cmd_dat(1,0x01);}}
	  if(i==7)
	   { 
	    if((j>0)&&(j<131)) { GotoXY(i,j); write_cmd_dat(1,0x80);}}
	 }
   }
}


void lcd_nested_frames(void)
{
    uchar top = 0;
    uchar bottom = 7;
    uchar left = 0;
    uchar right = 127;
    uchar i, j;

    while (top <= bottom && left <= right)
    {
        // 上边
        for(j = left; j <= right; j++)
        {
            GotoXY(top, j);
            write_cmd_dat(1, 0xFF);
        }

        // 下边
        for(j = left; j <= right; j++)
        {
            GotoXY(bottom, j);
            write_cmd_dat(1, 0xFF);
        }

        // 左边
        for(i = top; i <= bottom; i++)
        {
            GotoXY(i, left);
            write_cmd_dat(1, 0xFF);
        }

        // 右边
        for(i = top; i <= bottom; i++)
        {
            GotoXY(i, right);
            write_cmd_dat(1, 0xFF);
        }

        // 缩小一圈
        top++;
        bottom--;
        left += 2;
        right -= 2;
    }
}



//------------------显示8X16点阵字符串-------------------------		
 void disp_ascii0816(uchar *ptrAscii, uchar startP, uchar startC)
	{

		uchar i,xclum;
		uint index;
		GotoXY(startP,startC);
		while((*ptrAscii) != '\0')
		{
		  	index = ((*ptrAscii)-0x20)*16; 
		  
			for(i=0;i<16;i++)
			{
			 if(i%8==0)
			  {
			  xclum=startC;
			  }
			 write_data(AsciiDot0816[index++],startP+i/8,xclum++);			 
			}
			startC+=8;
	 	   ptrAscii++;				  
		//	if((128-xclum)<8)
	//	{
		// startP+=2;
		//	 startC=0;
			 //if(startP<8)
		//	 {Gotoxy(startP,startC);}
		//	}
	}
}



//------------------显示5X8点阵字库串-------------------------
void disp_ascii0508(uchar *ptrAscii, uchar startP, uchar startC)
{

		uchar i;
		uint index;
		GotoXY(startP,startC);
		while((*ptrAscii) != '\0')
		{
			index = ((*ptrAscii) - 0x20)*5;
			for (i=0;i<5;i++)	
			{
			 write_data(AsciiDot0508[index++],startP,startC++);		
			}
	 	   ptrAscii++;				  
			if((128-startC)<6)
			{
			 startP++;
			 startC=0;
			 if(startP<8)
			 {GotoXY(startP,startC);}
			}
	}
}
//------------------显示一个汉字-------------------------
void display_hanzi(uchar *p,uchar startP, uchar startC)
{

   uchar i,j;
	 for(i=0;i<2;i++)
	   {
         GotoXY(startP+i,startC);
			  for(j=0;j<16;j++)
			    {write_cmd_dat(1,*p++);}//发送数据
     }
}

//------------------显示一个反白汉字-------------------------
void display_hanzi_fan(uchar *p,uchar startP, uchar startC)
{

   uchar i,j;
	 for(i=0;i<2;i++)
	   {
         GotoXY(startP+i,startC);
			  for(j=0;j<16;j++)
			    {write_cmd_dat(1,~*p++);}//取反发送数据
     }
}

//-------------显示一副128X64点阵的图片------------------------------
void display_photo(uchar *p,uchar startP,uchar startC)	 
{
   uchar i,j;
   GotoXY(startP,startC);
   for(i=0;i<8;i++)
    {
	  for(j=0;j<128;j++)
	  {
	   write_data(*p++, startP+i, startC+j);
	  }
	}
}

//-------------取反显示一副128X64点阵的图片------------------------------
void trans_display_photo(uchar *p,uchar startP,uchar startC)	 
{
   uchar i,j;
   GotoXY(startP,startC);
   for(i=0;i<8;i++)
    {
	  for(j=0;j<132;j++)
	  {
	   write_data(~*p++, startP+i, startC+j);
	  }
	}
}
void Init_IC()   
{   
	
	 cs  =  0;   
	 P1  =  0xFF;   
	 LCD_Reset(); 
	 write_cmd_dat(0,0xE2); //initialize interal function	
	 write_cmd_dat(0,0xA2); //LCD Bias selection(1/65 Duty,1/9Bias) 	
	 write_cmd_dat(0,0xA0); //ADC selection(SEG0->SEG128) //改变SEG扫描方向 
	 write_cmd_dat(0,0xC8); //SHL selection(COM64->COM0) //改变COM扫描方向 
	 write_cmd_dat(0,0x2F); //power control(VB,VR,VF=1,1,1)
	 write_cmd_dat(0,0x27); //Regulator resistor select 110 内部电位器调节
	 write_cmd_dat(0,0x81); //set reference voltage mode
	 write_cmd_dat(0,0x17); //set reference voltage //改变此值可以改变效果  
	 write_cmd_dat(0,0x40); //Initial Display Line
	 write_cmd_dat(0,0xAF); //Display on 
	 Clear();

}   


//按键函数
unsigned int HOLD()
{
    if (Key1 == 0) 
	{  
        delayUs(5);  
        if (Key1 == 0) 
		{  
            while (Key1 == 0); 
            return 1; 
        }
    }
    return 0; 
}

//按键函数
unsigned int UP() 
{
    if (Key2 == 0) 
	{  
        delayUs(5);  
        if (Key2 == 0) 
		{ 
            while (Key2 == 0);  
            return 1;  
        }
    }
    return 0; 
}

//按键函数
unsigned int DN() 
{
    if (Key3 == 0) 
	{  
        delayUs(5);  
        if (Key3 == 0) 
		{ 
            while (Key3 == 0);  
            return 1;  
        }
    }
    return 0; 
}


void Yehui_UI()
{
	uchar i; 
	lcd_kuang();
//	const unsigned char* hanzi_array[] = {guang, don, ye, hui, ke};
//	const unsigned char* hanzi_array2[] = {ji, you, xian, gong, si};
//	
//	for ( i = 0; i < 5; i++) {
//		display_hanzi_fan(hanzi_array[i], 0, 25 + i * 16);
//	}

//	for ( i = 0; i < 5; i++) {
//		display_hanzi_fan(hanzi_array2[i], 2, 25 + i * 16);
//	}
	 
    disp_ascii0816("Made in China",1,15);	
	disp_ascii0816("132*64 Dots",3,20);
	disp_ascii0816("IC:ST7565A",  5,25);
	
}

void whatsapp()
{
	disp_ascii0816("WhatsApp:",       2,0);
	disp_ascii0816("+86136 2249 6694",6,0);
	
}



void lcd_describe(void)
{
	lcd_kuang();	 
    disp_ascii0508("Support 8-Bit parallel", 2, 10);
    disp_ascii0508("Bus (8080 or 6800 Mode)", 4, 10);
    disp_ascii0508(" Serial Bus Interface",   6, 6);
}
