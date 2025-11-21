#include <REGX52.H>
#include "lcm160160.h" 
#include "pic.h"

unsigned char UC_PRODEXIST1 = 0;
unsigned char UC_PRODEXIST2 = 0;
unsigned char ERRFLAG1 = 0;
unsigned char ERRFLAG2 = 0;

void main()
{
	EA=1; //Interurupt Enabled

	IT0 = 1;//INT0 Falling Edge Trigger

	EX0 = 1;//INT0 Enabled

	CURDECT_PORT =1;//关闭电流检测

	BEEP_PORT =0;//Alarm Off

	BLE_PORT =1;//Backlight ON

	EXTB=1;//Remove VSS 

	Enb_DC = 0;//Remove VPP

	LCD_RST();

//	Chk_ProdExist();

//	Chk_AllSignal();
	LCD_Initial();
	LCD_Clear();
	delayms(20);

	while(1)
	{
		while (DN());
		disp_dots(0xFF,0xFF);
		delayms(5000);
		while (DN());
		disp_dots(0xAA,0x55);
		delayms(5000);
		while (DN());
		disp_dots(0x55,0xAA);
		delayms(5000);
		while (DN());
		disp_dots(0xFF,0x00);
		delayms(5000);
		while (DN());
		disp_dots(0x00,0xFF);
		delayms(5000);
		while (DN());
		disp_dots(0xAA,0xAA);
		delayms(5000);
		while (DN());
		disp_dots(0x55,0x55);
		delayms(5000);
		while (DN());
		disp_bmp(Border);
		delayms(5000);
		while (DN());
//		Yehui_UI();
//		delayms(5000);
//		while (DN());
		disp_bmp(CustImg);
		delayms(5000);


		
//		disp_Vopinf();
//		CURDECT_PORT =0;//启用电流检测

//		EX0 = 0;//INT0 Disabled
		
//		while (1)
//			Contrast_Adj();
	};
 }

