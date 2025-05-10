#include <REGX52.H>
#include <INTRINS.H>
#include "lcm160160.h"
#include "pic.h"
unsigned char UC_ISPAUSE ;

void delayms(uint uc_ms)
{
	uchar uc_dlay1;

	while (uc_ms--)
		for (uc_dlay1=0; uc_dlay1<125; uc_dlay1++);
}

void delay100ms()
{
	uchar uc_dlay;
	uc_dlay = LCD_DLY;
	while (uc_dlay--)
		delayms(100);
}

//蜂鸣器
void BeepN(uchar uc_times)
{
	uchar i;
	for (i=0;i<uc_times;i++)
	{
		BEEP_PORT =1;
		delayms(1000);
		BEEP_PORT =0;
		delayms(1000);

	};
}

//Interrupt 0 外部中断0（INT0） 
void Func_int0() interrupt 0 using 1
{
	if (uc_IsPause == 0)
 		uc_IsPause = 1;
	else
		uc_IsPause = 0;


	if ( uc_IsPause == 1 )
		while (1)
		{
			if (PAUSE == 0)
				return;
		}
}

void SendByte(uchar uc_dat)
{
	uchar uc_bitcnt;
	for(uc_bitcnt=0; uc_bitcnt<8; uc_bitcnt++)
	{
		SCK_PORT = 0; 
		SDA_PORT = 0;

		if (uc_dat & 0x80) 
			SDA_PORT = 1; /*发送数据由高位到低位传送*/

		SCK_PORT = 1; /*钳住总线准备下一个数据位*/
//		uc_dat= uc_dat <<1; /*发送数据左移一位*/
		uc_dat <<=1; /*发送数据左移一位*/
	}
}

//Write Instruction Code
void wr_cmd(uchar uc_cmd)
{
	A0_PORT = 0;

	CS_PORT = 0;
	CS1_PORT = 0;
	
	SendByte(uc_cmd);

	CS_PORT = 1;
	CS1_PORT = 1;
}

//Write Display RAM Data
void wr_dat(uchar uc_dat)
{
	A0_PORT = 1;

	CS_PORT = 0;
	CS1_PORT = 0;

	SendByte(uc_dat);

	CS_PORT = 1;
	CS1_PORT = 1;
}


//Write Instruction Code
void wr_cmd1(uchar uc_cmd)
{
	A0_PORT = 0;

	CS_PORT = 0;
	
	SendByte(uc_cmd);

	CS_PORT = 1;
}

//Write Display RAM Data
void wr_dat1(uchar uc_dat)
{
	A0_PORT = 1;

	CS_PORT = 0;

	SendByte(uc_dat);

	CS_PORT = 1;
}

//Write Instruction Code
void wr_cmd2(uchar uc_cmd)
{
	A0_PORT = 0;

	CS1_PORT = 0;
	
	SendByte(uc_cmd);

	CS1_PORT = 1;
}

//Write Display RAM Data
void wr_dat2(uchar uc_dat)
{
	A0_PORT = 1;

	CS1_PORT = 0;

	SendByte(uc_dat);

	CS1_PORT = 1;
}

void wr_cmdXcs(uchar uc_cmd)
{
	A0_PORT = 0;

	CS_PORT = 1;
	CS1_PORT = 1;
	
	SendByte(uc_cmd);
}

uchar rd_reg1()
{
	uchar uc_RetVal,uc_BitCnt;
	bit b_sda;

	A0_PORT  = 0;
	CS_PORT  = 0;

	SendByte(0xFE);

	SDA_PORT = 1;

	uc_RetVal =0;
	for(uc_BitCnt=0; uc_BitCnt<8; uc_BitCnt++)
	{
		uc_RetVal= uc_RetVal <<1;
	
		SCK_PORT = 0; 
		
		b_sda =SDA_PORT;
		if ( b_sda==1)
			uc_RetVal =uc_RetVal |0x01;

		SCK_PORT = 1; 
	};

	CS_PORT = 1;

	return uc_RetVal;
}

uchar rd_reg2()
{
	uchar uc_RetVal,uc_BitCnt;
	bit b_sda;

	A0_PORT  = 0;
	CS1_PORT = 0;

	SendByte(0xFE);

	SDA_PORT = 1;

	uc_RetVal =0;
	for(uc_BitCnt=0; uc_BitCnt<8; uc_BitCnt++)
	{
		uc_RetVal= uc_RetVal <<1;
	
		SCK_PORT = 0; 
		
		b_sda =SDA_PORT;
		if ( b_sda==1)
			uc_RetVal =uc_RetVal |0x01;

		SCK_PORT = 1; 
	};

	CS1_PORT = 1;

	return uc_RetVal;
}

uchar rd_ProId1()
{	uchar uc_ProID;
	wr_cmd1(0x38); //Extension Command 3
	wr_cmd1(0x7F); //Enable Read ID

	wr_cmd1(0x30);//Extension command 1
	uc_ProID =rd_reg1();

	wr_cmd1(0x38); //Extension Command 3
	wr_cmd1(0x7E); //Disable Read ID
	wr_cmd1(0x30);//Extension command 1
	
	return uc_ProID;
}

uchar rd_ProId2()
{	uchar uc_ProID;
	wr_cmd2(0x38); //Extension Command 3
	wr_cmd2(0x7F); //Enable Read ID

	wr_cmd2(0x30);//Extension command 1
	uc_ProID =rd_reg2();

	wr_cmd2(0x38); //Extension Command 3
	wr_cmd2(0x7E); //Disable Read ID
	wr_cmd2(0x30);//Extension command 1

	return uc_ProID;
}

void LCD_RST()
{
	RST_PORT=0;
	delayms(5);

	RST_PORT=1;
	delayms(150);
}

//ST75161 Initialization
void LCD_Initial()
{
	
	wr_cmd(0x31); // Extension Command 2
	wr_cmd(0xD7); // Disable Auto Read
	wr_dat(0x9F);

	wr_cmd(0xE0); // Enable OTP Read
	wr_dat(0x00);
	delayms(10);

	wr_cmd(0xE3); // OTP Up-Load
	delayms(20);

	wr_cmd(0xE1); // OTP Control Out

	wr_cmd(0x30);//Extension command 1
	wr_cmd(0x94);//Sleep out mode
	wr_cmd(0xD1);//Turns on the internal oscillation circuit.
	
	wr_cmd(0xAE);//Display OFF
	delayms(50);
	wr_cmd(0xA6);//INV=0: Normal display

	wr_cmd(0xCA);//Display Control
	wr_dat(0x00);//CLD=0: not divide
	wr_dat(0x9F);//1/160 Duty
	wr_dat(0x25);//0 Line cycles in a frame  0x00

	wr_cmd(0xBC);//Data Scan Direction
	wr_dat(0x00);//MV =0: In the column direction;MX =0: Normal

	wr_cmd(0xAB);//Start line address
	wr_dat(0x00);

	wr_cmd(0x20);//Power Control
	wr_dat(0x0B);//VB\VR\VF On

	wr_cmd(0x81);//Power Control
	wr_dat(ui_VopSet&0x3F);//Vop5~Vop0,0~3F  0x20
	wr_dat(ui_VopSet>>6);//Vop8~Vop6,0~7   0x04

	
	wr_cmd(0x0C);//Data Format Select,DO=1: LSB is on top; D0~D7  0C
	
	wr_cmd(0xF0);//Display Mode
	wr_dat(0x10);//DM=0 :Mono(Default)
	
	wr_cmd(0x76);//ICON=0 ; Disable ICON RAM.

	wr_cmd(0x31);//Extension command 2
	wr_cmd(0x32);//Analog circuit set
	wr_dat(0x00);//
	wr_dat(0x01);//Booster Efficiency =6KHz
	wr_dat(0x03);//1/11 Bias

	wr_cmd(0x51);//Booster Level
	wr_dat(0xFB);//10 Times

	wr_cmd(0xF0);    // Frame Rate 
	wr_dat(0x01);    // FA=10Hz
	wr_dat(0x03);    // FB=34.5Hz
	wr_dat(0x06);    // FC=46Hz
	wr_dat(0x0D);    // FD=73Hz

	wr_cmd(0xF2);     // Temperature Range
	wr_dat(0x03);    // TA=-37
	wr_dat(0x23);    // TB=-5
	wr_dat(0x40);    // TC=24
	
	wr_cmd(0xF4);    // Temperature Gradient Compensation
	wr_dat(0x43);    // MT1=4,MT0=3
	wr_dat(0x11);    // MT3=1,MT2=1
	wr_dat(0x11);    // MT5=1,MT4=1
	wr_dat(0x11);    // MT7=1,MT6=1
	wr_dat(0x00);    // MT9=0,MT8=0
	wr_dat(0x06);    // MTB=0,MTA=6 
	wr_dat(0xBC);    // MTD=11,MTV=12
	wr_dat(0xFF);    // MTF=15,MTE=15
	
	wr_cmd(0x30);//Extension command 1
	wr_cmd(0xAF);//Display On



//	RST_PORT=0; 
//	delayms(100); 
//	RST_PORT=1; 
//	delayms(500); 
//	wr_cmd(0x30);  //EXT=0
//	wr_cmd(0x94);  //Sleep out 
//	wr_cmd(0x31);  //EXT=1 
//	wr_cmd(0xD7);  //Autoread disable 
//	wr_dat(0X9F);     
//	wr_cmd(0x32);  //Analog SET 
//	wr_dat(0x00);  //OSC Frequency adjustment   
//	wr_dat(0x01);  //Frequency on booster capacitors->6KHz   
//	wr_dat(0x00);  //Bias=1/14   
//	wr_cmd(0x20);  // Gray Level 
//	wr_dat(0x01);  
//	wr_dat(0x03); 
//	wr_dat(0x05); 
//	wr_dat(0x07);  
//	wr_dat(0x09); 
//	wr_dat(0x0b); 
//	wr_dat(0x0d); 
//	wr_dat(0x10); 
//	wr_dat(0x11); 
//	wr_dat(0x13); 
//	wr_dat(0x15); 
//	wr_dat(0x17); 
//	wr_dat(0x19); 
//	wr_dat(0x1b); 
//	wr_dat(0x1d); 
//	wr_dat(0x1f);
//	wr_cmd(0x31);   //EXT=1 
//	wr_cmd(0xf0);   //此指令比较重要,不加此指令升压会慢0.5s 
//	wr_dat(0x0f); 	
//	wr_dat(0x0f); 
//	wr_dat(0x0f); 
//	wr_dat(0x0f); 
//	wr_cmd(0x30);   //EXT=0 
//	wr_cmd(0x75);   //Page Address setting 
//	wr_dat(0X00);   // XS=0   
//	wr_dat(0X28);   // XE=159 0x28  
//	wr_cmd(0x15);   //Clumn Address setting 
//	wr_dat(0X00);   // XS=0  
//	wr_dat(0Xff);   // XE=256  
//	wr_cmd(0xBC);   //Data scan direction
//	wr_dat(0x00);   //MX.MY=Normal  
//	wr_dat(0xA6);  
//	wr_cmd(0xCA);   //Display Control 
//	wr_dat(0X00);      
//	wr_dat(0X9F);    //Duty=160  
//	wr_dat(0X20);    //Nline=off  
//	wr_cmd(0xF0);    //Display Mode  
//	wr_dat(0X10);    //10=Monochrome Mode,11=4Gray  
//	wr_cmd(0x81);    //EV control 
//	wr_dat(0x27);    //微调对比度的值，可设置范围0x00～0x3f
//	wr_dat(0x05);    //粗调对比度，可设置范围0x00～0x07
//	wr_cmd(0x20);    //Power control   
//	wr_dat(0x0B);    //D0=regulator ; D1=follower ; D3=booste,  on:1 off:0   
//	delayms(20);            
//	wr_cmd(0xAF);    //Display on	

} 



//========================================================
void VOP_Fine_TuneH()
{
	wr_cmd(0xD6);
}
//========================================================
void VOP_Fine_TuneL()
{
	wr_cmd(0xD7);
}

//========================================================
void OTP_Write()     
{       
  wr_cmd(0x30);//Extension Command 1 
  wr_cmd(0xAE);//Display OFF 
  delayms(50);   // Delay 50ms 

  wr_cmd(0x39);//Enable OTP
  wr_cmd(0xD6);    
  wr_dat(0x10);
   
  wr_cmd(0x31);//Extension Command 2   
  wr_cmd(0xF0);//Frame Rate = 73Hz 
  wr_dat(0x0D);     
  wr_dat(0x0D);     
  wr_dat(0x0D);     
  wr_dat(0x0D);     
       
  wr_cmd(0xE4);//OTP Selection Control 
  wr_dat(0x99);   
  
  wr_cmd(0xE5);//OTP Programming Setting 
  wr_dat(0x0F); 
    
  wr_cmd(0xE0);//OTP WR/RD Control 
  wr_dat(0x20);     
  delayms(100);  //Delay 50ms 

  wr_cmd(0xE2);//OTP Write 
  delayms(100);  //Delay 100ms 

  wr_cmd(0xE1);//OTP Control Out 

  wr_cmd(0x30);//Extension Command 1 
} 

void Chk_ProdExist()
{
	uchar uc_rdStus;

	uc_rdStus =rd_ProId1();
	if ((uc_rdStus &0x03)==0x01)
	{
		uc_ProdExist1 =1;
	}
	else
	{
		uc_ProdExist1 =0;
	};


	uc_rdStus =rd_ProId2();
	if ((uc_rdStus &0x03)==0x01)
	{
		uc_ProdExist2 =1;
	}
	else
	{
		uc_ProdExist2 =0;
	};
}

void disp_ASC1(uchar uc_RowNo,uchar uc_ColNo,uchar uc_CharCode)
{
	uchar uc_ColCnt,uc_PageCnt;

	wr_cmd1(0x30);//Extension command 1

	wr_cmd1(0x75);//Set Page Address
	wr_dat1(uc_RowNo);//Start Page
	wr_dat1(uc_RowNo +1);//End Page

	wr_cmd1(0x15);//Set Column Address
	wr_dat1(uc_ColNo);//Start Column
	wr_dat1(uc_ColNo +7);//End Column
	
	wr_cmd1(0x5C);
	
	for (uc_PageCnt=0;uc_PageCnt < 2;uc_PageCnt++)	
	{
		for (uc_ColCnt=0;uc_ColCnt <8;uc_ColCnt++)
			wr_dat1(ASCIILIB[(uc_CharCode -0x20) *16 +uc_PageCnt *8 +uc_ColCnt]);
	}
}

void disp_ASCStr1(uchar uc_RowNum,uchar uc_ColNum,uchar *StrAddr,uchar uc_StrLen)
{
	uchar uc_CharCnt,uc_CharCode;

	for (uc_CharCnt=0;uc_CharCnt < uc_StrLen;uc_CharCnt++)	
	{
		uc_CharCode = StrAddr[uc_CharCnt];
		disp_ASC1(uc_RowNum,uc_ColNum +uc_CharCnt*8,uc_CharCode);
	}
}

void disp_ASC2(uchar uc_RowNo,uchar uc_ColNo,uchar uc_CharCode)
{
	uchar uc_ColCnt,uc_PageCnt;

	wr_cmd2(0x30);//Extension command 1

	wr_cmd2(0x75);//Set Page Address
	wr_dat2(uc_RowNo);//Start Page
	wr_dat2(uc_RowNo +1);//End Page

	wr_cmd2(0x15);//Set Column Address
	wr_dat2(uc_ColNo);//Start Column
	wr_dat2(uc_ColNo +7);//End Column
	
	wr_cmd2(0x5C);
	
	for (uc_PageCnt=0;uc_PageCnt < 2;uc_PageCnt++)	
	{
		for (uc_ColCnt=0;uc_ColCnt <8;uc_ColCnt++)
			wr_dat2(ASCIILIB[(uc_CharCode -0x20) *16 +uc_PageCnt *8 +uc_ColCnt]);
	}
}

void disp_ASCStr2(uchar uc_RowNum,uchar uc_ColNum,uchar *StrAddr,uchar uc_StrLen)
{
	uchar uc_CharCnt,uc_CharCode;

	for (uc_CharCnt=0;uc_CharCnt < uc_StrLen;uc_CharCnt++)	
	{
		uc_CharCode = StrAddr[uc_CharCnt];
		disp_ASC2(uc_RowNum,uc_ColNum +uc_CharCnt*8,uc_CharCode);
	}
}


void disp_GB1(uchar uc_RowNo,uchar uc_ColNo,uchar uc_CharCode)
{
	uchar uc_ColCnt,uc_PageCnt;

	wr_cmd1(0x30);//Extension command 1

	wr_cmd1(0x75);//Set Page Address
	wr_dat1(uc_RowNo);//Start Page
	wr_dat1(uc_RowNo +1);//End Page

	wr_cmd1(0x15);//Set Column Address
	wr_dat1(uc_ColNo);//Start Column
	wr_dat1(uc_ColNo +15);//End Column
	
	wr_cmd1(0x5C);
	
	for (uc_PageCnt=0;uc_PageCnt < 2;uc_PageCnt++)	
	{
		for (uc_ColCnt=0;uc_ColCnt <16;uc_ColCnt++)
			wr_dat1(GBCSLIB[(uc_CharCode) *32 +uc_PageCnt *16 +uc_ColCnt]);
	}
}

void disp_GB2(uchar uc_RowNo,uchar uc_ColNo,uchar uc_CharCode)
{
	uchar uc_ColCnt,uc_PageCnt;

	wr_cmd2(0x30);//Extension command 1

	wr_cmd2(0x75);//Set Page Address
	wr_dat2(uc_RowNo);//Start Page
	wr_dat2(uc_RowNo +1);//End Page

	wr_cmd2(0x15);//Set Column Address
	wr_dat2(uc_ColNo);//Start Column
	wr_dat2(uc_ColNo +15);//End Column
	
	wr_cmd2(0x5C);
	
	for (uc_PageCnt=0;uc_PageCnt < 2;uc_PageCnt++)	
	{
		for (uc_ColCnt=0;uc_ColCnt <16;uc_ColCnt++)
			wr_dat2(GBCSLIB[(uc_CharCode) *32 +uc_PageCnt *16 +uc_ColCnt]);
	}
}

void disp_GB(uchar uc_RowNo,uchar uc_ColNo,uchar uc_CharCode)
{
	uchar uc_ColCnt,uc_PageCnt;

	wr_cmd(0x30);//Extension command 1

	wr_cmd(0x75);//Set Page Address
	wr_dat(uc_RowNo);//Start Page
	wr_dat(uc_RowNo +1);//End Page

	wr_cmd(0x15);//Set Column Address
	wr_dat(uc_ColNo);//Start Column
	wr_dat(uc_ColNo +15);//End Column
	
	wr_cmd(0x5C);
	
	for (uc_PageCnt=0;uc_PageCnt < 2;uc_PageCnt++)	
	{
		for (uc_ColCnt=0;uc_ColCnt <16;uc_ColCnt++)
			wr_dat(GBCSLIB[(uc_CharCode) *32 +uc_PageCnt *16 +uc_ColCnt]);
	}
}

void disp_GBStr(uchar uc_RowNum,uchar uc_ColNum,uchar *StrAddr,uchar uc_StrLen)
{
	uchar uc_CharCnt,uc_CharCode;

	for (uc_CharCnt=0;uc_CharCnt < uc_StrLen;uc_CharCnt++)	
	{
		uc_CharCode = StrAddr[uc_CharCnt];
		disp_GB(uc_RowNum,uc_ColNum +uc_CharCnt*16,uc_CharCode);
	}
}

void disp_Numeric1(uchar uc_PageAddr,uchar uc_ColAddr,uint ui_NumVal)
{
	uchar uc_Num2ASC[3];
	
	uc_Num2ASC[2] = '0' +ui_NumVal%10;
	uc_Num2ASC[1] = '0' +ui_NumVal/10%10;
	uc_Num2ASC[0] = '0' +ui_NumVal/100%10;
	disp_ASCStr1(uc_PageAddr,uc_ColAddr,uc_Num2ASC,3);
}

void disp_Numeric2(uchar uc_PageAddr,uchar uc_ColAddr,uint ui_NumVal)
{
	uchar uc_Num2ASC[3];
	
	uc_Num2ASC[2] = '0' +ui_NumVal%10;
	uc_Num2ASC[1] = '0' +ui_NumVal/10%10;
	uc_Num2ASC[0] = '0' +ui_NumVal/100%10;
	disp_ASCStr2(uc_PageAddr,uc_ColAddr,uc_Num2ASC,3);
}

void disp_Vopinf()
{
	uint uc_VopH,uc_VopL,ui_VopMsr;

	//显示第1个模块的OTP信息
	if (uc_ProdExist1 ==1)
	{
		wr_cmd1(0x30);//EXT=0
		wr_cmd1(0x7C);//read vop[5:0]
		uc_VopL=rd_reg1();
		wr_cmd1(0x25);
		
		wr_cmd1(0x30);
		wr_cmd1(0x7D);//read vop[8:6]
		uc_VopH=rd_reg1();
		wr_cmd1(0x25);
	
		ui_VopMsr =uc_VopH;
		ui_VopMsr =(ui_VopMsr <<6)|uc_VopL;
		ui_VopAdj1 =ui_VopMsr;

		disp_ASCStr1(16,32,"SV=",3);
		disp_Numeric1(16,56,ui_VopSet);

		disp_ASCStr1(16,88,"PV=",3);
		disp_Numeric1(16,112,ui_VopMsr);
	};


	//显示第2个模块的OTP信息
	if (uc_ProdExist2 ==1)
	{
		wr_cmd2(0x30);//EXT=0
		wr_cmd2(0x7C);//read vop[5:0]
		uc_VopL=rd_reg2();
		wr_cmd2(0x25);
		
		wr_cmd2(0x30);
		wr_cmd2(0x7D);//read vop[8:6]
		uc_VopH=rd_reg2();
		wr_cmd2(0x25);
	
		ui_VopMsr =uc_VopH;
		ui_VopMsr =(ui_VopMsr <<6)|uc_VopL;
		ui_VopAdj2 =ui_VopMsr;
	
		disp_ASCStr2(16,32,"SV=",3);
		disp_Numeric2(16,56,ui_VopSet);

		disp_ASCStr2(16,88,"PV=",3);
		disp_Numeric2(16,112,ui_VopMsr);
	};
}

//Write Data To DDRAM
void disp_dots(uchar dat1,uchar dat2)
{
	uchar uc_RowCnt,uc_ColCnt;	 //LINE=COL COL=LINE

	wr_cmd(0x30);//Extension command 1

	wr_cmd(0x75);//Set Page Address
	wr_dat(0x00);//Start Page
	wr_dat(0x13);//End Page

	wr_cmd(0x15);//Set Column Address
	wr_dat(0x00);//Start Column
	wr_dat(0x9F);//End Column

	wr_cmd(0x5C);//
		
	for (uc_RowCnt=0;uc_RowCnt<20;uc_RowCnt++)
	{
		for (uc_ColCnt=0;uc_ColCnt<80;uc_ColCnt++)
		{
				wr_dat(dat1);
				wr_dat(dat2);
		}
	}

	delay100ms();
}

void LCD_Clear()
{
	uchar uc_RowCnt,uc_ColCnt;	 //LINE=COL COL=LINE

	wr_cmd(0x30);//Extension command 1

	wr_cmd(0x75);//Set Page Address
	wr_dat(0x00);//Start Page
	wr_dat(0x13);//End Page

	wr_cmd(0x15);//Set Column Address
	wr_dat(0x00);//Start Column
	wr_dat(0x9F);//End Column

	wr_cmd(0x5C);//
		
	for (uc_RowCnt=0;uc_RowCnt<20;uc_RowCnt++)
		for (uc_ColCnt=0;uc_ColCnt<160;uc_ColCnt++)
			wr_dat(0x00);
}

void disp_bmp(xchar *bmpinf)
{
	uchar uc_PageCnt,uc_ColCnt;

	wr_cmd(0x30);//Extension command 1

	wr_cmd(0x75);//Set Page Address
	wr_dat(0x00);//Start Page
	wr_dat(0x13);//End Page

	wr_cmd(0x15);//Set Column Address
	wr_dat(0x00);//Start Column
	wr_dat(0x9F);//End Column

	wr_cmd(0x5C);//

	for (uc_PageCnt=0;uc_PageCnt<20;uc_PageCnt++)
		for (uc_ColCnt=0;uc_ColCnt<160;uc_ColCnt++)
			wr_dat(bmpinf[uc_PageCnt*160 + uc_ColCnt]);
}

void Contrast_Adj( void )
{
	Vop_Inc=1;
	Vop_Dec=1;
	
	if(!Vop_Inc)
	{
		delayms(30);
		if(!Vop_Inc)
		{
		    while(!Vop_Inc);
			VOP_Fine_TuneH();

			ui_VopAdj1 ++;
			ui_VopAdj2 ++;
			disp_Numeric1(16,112,ui_VopAdj1);
			disp_Numeric2(16,112,ui_VopAdj2);
		}
	}
	else if(!Vop_Dec)
	{
		delayms(30);
		if(!Vop_Dec)
		{
			while(!Vop_Dec);
			VOP_Fine_TuneL();

			ui_VopAdj1 --;
			ui_VopAdj2 --;
			disp_Numeric1(16,112,ui_VopAdj1);
			disp_Numeric2(16,112,ui_VopAdj2);
		}
	}
	else if(!OTP)
      {
		 delayms(30); 	 
         if(!OTP)
         {
            while(!OTP);

			EXTB=0; 
		    Enb_DC = 1;
			delayms(50);
           
            OTP_Write();
           	delayms(10);

			Enb_DC = 0; //Remove VPP
			EXTB=1; //Remove vss   
			delayms(50);
         
            LCD_Initial();
            disp_bmp(CustImg);
			disp_Vopinf();
         }
	  }
}


void Chk_BLCathode()
{
	BLE_PORT =0;
	delay100ms();
	BLE_PORT =1;
}

void Chk_CSignal()
{
	wr_cmdXcs(0xAE);//测试/CS信号焊接情况,如果漏焊或者与旁边的VSS信号短路,则LCD不显示
	delayms(50);
}
void Chk_AllSignal()
{
	uchar uc_rdStus,uc_Errcode1,uc_Errcode2;

	uchar uc_testOK =44;
	uchar uc_testNG =45;
	uchar uc_testNO =46;

	uc_Errcode1=0;
	uc_Errcode2=0;

	//检测片选信号缺陷
	Errflag1[0] =0;
	Errflag2[0] =0;

	LCD_Initial();
	LCD_Clear();
	delayms(220);

	Chk_CSignal();
	if (uc_ProdExist1 ==1)
	{
		uc_rdStus =rd_reg1();
		if ((uc_rdStus &0x08)==0x00)
		{
			Errflag1[0] =1;
		}
	};

	if (uc_ProdExist2 ==1)
	{
		uc_rdStus =rd_reg2();
		if ((uc_rdStus &0x08) ==0x00)
		{
			Errflag2[0] =1;
		}
	};

	//检测复位信号缺陷
	Errflag1[1] =0;
	Errflag2[1] =0;

	LCD_RST();
	if (uc_ProdExist1 ==1)
	{
		uc_rdStus =rd_reg1();
		if ((uc_rdStus &0x08)==0x08)
		{
			Errflag1[1] =1;
		};
	};

	if (uc_ProdExist2 ==2)
	{
		uc_rdStus =rd_reg2();
		if ((uc_rdStus &0x08) ==0x08)
		{
			Errflag2[1] =1;
		}
	};

	//检测读写信号缺陷
	Errflag1[2] =0;
	Errflag2[2] =0;

	//检测背光源与VSS短路
	Errflag1[3] =0;
	Errflag2[3] =0;
	

	//显示检测结果
	LCD_Initial();
	LCD_Clear();

	disp_GBStr(0,16*2,Msg01,7);
	disp_GBStr(2,0,Msg02a,5);
	disp_GBStr(4,16,Msg02b,9);
	disp_GBStr(6,0,Msg03a,5);
	disp_GBStr(8,16,Msg03b,9);
	disp_GBStr(10,0,Msg04a,5);
	disp_GBStr(12,16,Msg04b,7);
	disp_GBStr(14,0,Msg05a,9);
	disp_GBStr(16,16,Msg05b,8);

	if (uc_ProdExist1 ==1)
	{
		//片选信号缺陷
		if (Errflag1[0] ==1)
		{
			disp_GB1(2,16*9,uc_testNG);
			uc_Errcode1 ++;	
		}
		else
		{
			disp_GB1(2,16*9,uc_testOK);
		};

		//复位信号缺陷
		if (Errflag1[1]==1)
		{
			disp_GB1(6,16*9,uc_testNG);
			uc_Errcode1 ++;
		}
		else
		{
			disp_GB1(6,16*9,uc_testOK);
		};

		//读写信号缺陷
		disp_GB1(10,16*9,uc_testNO);
	
		//背光源负极和VSS电路
		disp_GB1(14,16*9,uc_testNO);
	};

	if (uc_ProdExist2 ==1)
	{
		//片选信号缺陷
		if (Errflag2[0]==1)
		{
			disp_GB2(2,16*9,uc_testNG);
			uc_Errcode2 ++;	
		}
		else
		{
			disp_GB2(2,16*9,uc_testOK);
		};

		
		//复位信号缺陷
		if (Errflag2[1]==1)
		{
			disp_GB2(6,16*9,uc_testNG);
			uc_Errcode2 ++;
		}
		else
		{
			disp_GB2(6,16*9,uc_testOK);
		};

		//读写信号缺陷
		disp_GB2(10,16*9,uc_testNO);

		//背光源负极和VSS电路
		disp_GB2(14,16*9,uc_testNO);
	};

	//检测背光源
	Chk_BLCathode();
	
	//结论和告警
	if ((uc_Errcode1 >0) ||(uc_Errcode2 >0))
	{
		disp_GBStr(18,0,Msg09a,9);
		BeepN(3);
		while(1);
	}
	else
	{
		disp_GBStr(18,0,Msg09b,9);	
		delay100ms();
	}
}

char str[] = "LCM160160";
unsigned char i ;
void Yehui_UI()
{
	disp_bmp(Box);
	disp_GBStr(6,40,Msg10a,5);	//广东烨辉科
	disp_GBStr(8,40,Msg10b,5);	//技有限公司
	
     //disp_ASC1(8, 10, 'L');
	for ( i = 0; str[i] != '\0'; i++) 
	{
		unsigned char Row = 12;
		unsigned char Col = 42;		
		disp_ASC1(Row, Col + i * 8, str[i]);
	}
	WhatsApp();
}

char str1[] = " Whats App:";
char str2[] = " +86 136 2249 6694";
char str3[] = "Guangdong Yehui ";
char str4[] = "Technology Co.,Ltd.";
void WhatsApp()
{

     //disp_ASC1(8, 10, 'L');
//	for ( i = 0; str1[i] != '\0'; i++) 
//	{
//		unsigned char Row = 15;
//		unsigned char Col = 2;		
//		disp_ASC1(Row, Col + i * 8, str1[i]);
//	}
//	
//	for ( i = 0; str2[i] != '\0'; i++) 
//	{
//		unsigned char Row = 17;
//		unsigned char Col = 2;		
//		disp_ASC1(Row, Col + i * 8, str2[i]);
//	}
	
	
	for ( i = 0; str3[i] != '\0'; i++) 
	{
		unsigned char Row = 1;
		unsigned char Col = 16;		
		disp_ASC1(Row, Col + i * 8, str3[i]);
	}
	
	for ( i = 0; str4[i] != '\0'; i++) 
	{
		unsigned char Row = 3;
		unsigned char Col = 3;		
		disp_ASC1(Row, Col + i * 8, str4[i]);
	}
}

//按键函数
unsigned int HOLD()
{
    if (Key1 == 0) 
	{  
        delayms(5);  
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
        delayms(5);  
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
        delayms(5);  
        if (Key3 == 0) 
		{ 
            while (Key3 == 0);  
            return 1;  
        }
    }
    return 0; 
}