#ifndef __lcm160160_H__
#define __lcm160160_H__

#include <REGX52.H>

#define  uchar unsigned char
#define  uint  unsigned int
#define  xchar unsigned char code
	
#define LCD_DLY 	10
#define DataPort 	P1

//MCU Interface Defination
sbit SDA_PORT		=	P1^4;
sbit SCK_PORT		=	P1^2;
sbit RST_PORT		=	P1^5;
sbit A0_PORT		=	P3^0;
sbit CS_PORT		=	P1^1;
sbit CS1_PORT		=	P1^1;

sbit BLE_PORT		=	P0^6;
sbit BEEP_PORT		=	P0^1;
sbit CURDECT_PORT	=	P0^0;


sbit Key1 =P2^5;//HOLD
sbit Key2 =P2^6;//UP
sbit Key3 =P2^4;//DN
sbit PAUSE=P2^7;

sbit      Vop_Inc = P0^0;   // if Vop_Inc = 0, then increase Vop;
sbit      Vop_Dec = P0^1;   // if Vop_Dec = 0, then decrease Vop;

sbit        OTP   = P3^2;   // if OTP = 0, then run OTP function;
sbit       Enb_DC = P3^7;   // when run OTP function, Vpp connect to 6.5V;
sbit       EXTB   = P3^3;   // when run OTP function, EXTB Set to "L"



extern uchar uc_IsPause ;

static uint ui_VopSet =296;
static uint ui_VopMsr =296;

static uint ui_VopAdj1 =0;
static uint ui_VopAdj2 =0;

extern uchar uc_ProdExist1,uc_ProdExist2;

extern uchar Errflag1[5],Errflag2[5];


void delayms(uint uc_ms);
void delay100ms();
void BeepN(uchar uc_times);
void Func_int0();
void SendByte(uchar uc_dat);
void wr_cmd(uchar uc_cmd);
void wr_dat(uchar uc_dat);
void wr_cmd1(uchar uc_cmd);
void wr_dat1(uchar uc_dat);
void wr_cmd2(uchar uc_cmd);
void wr_dat2(uchar uc_dat);
void wr_cmdXcs(uchar uc_cmd);
uchar rd_reg1();
uchar rd_reg2();
uchar rd_ProId1();
uchar rd_ProId2();
void LCD_RST();
void LCD_Initial();
void VOP_Fine_TuneH();
void VOP_Fine_TuneL();
void OTP_Write();
void Chk_ProdExist();
void disp_ASC1(uchar uc_RowNo,uchar uc_ColNo,uchar uc_CharCode);
void disp_ASCStr1(uchar uc_RowNum,uchar uc_ColNum,uchar *StrAddr,uchar uc_StrLen);
void disp_ASC2(uchar uc_RowNo,uchar uc_ColNo,uchar uc_CharCode);
void disp_ASCStr2(uchar uc_RowNum,uchar uc_ColNum,uchar *StrAddr,uchar uc_StrLen);
void disp_GB1(uchar uc_RowNo,uchar uc_ColNo,uchar uc_CharCode);
void disp_GB2(uchar uc_RowNo,uchar uc_ColNo,uchar uc_CharCode);
void disp_GB(uchar uc_RowNo,uchar uc_ColNo,uchar uc_CharCode);
void disp_GBStr(uchar uc_RowNum,uchar uc_ColNum,uchar *StrAddr,uchar uc_StrLen);
void disp_Numeric1(uchar uc_PageAddr,uchar uc_ColAddr,uint ui_NumVal);
void disp_Numeric2(uchar uc_PageAddr,uchar uc_ColAddr,uint ui_NumVal);
void disp_Vopinf();
void disp_dots(uchar dat1,uchar dat2);
void LCD_Clear();
void disp_bmp(xchar *bmpinf);
void Contrast_Adj( void );
void Chk_BLCathode();
void Chk_CSignal();
void Chk_AllSignal();
void Yehui_UI();
void WhatsApp();
unsigned int HOLD();
unsigned int UP();
unsigned int DN() ;


#endif
