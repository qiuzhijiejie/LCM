#include "LCM13264(cog).h"
#include "pic.h"

void main(void)
{
//	unsigned char mode;
	
	init_cpu();
	Delay_Xms(50);
	
   	InitST7567();	//一开始初始化控制器
	DispFram(0x00,0x00);
	
	while(1)  
	{
//     Start:	//跳转标号	
//if (HOLD()) {  
//    mode++;           // 先增加 mode
//    mode %= 8;        // 让 mode 保持在 0~7 之间

//    switch (mode) {
//        case 0:  
//            DispPic( PIC_5 );
//            break;
//        case 1:            
//            yehui_UI();		
//            break;
//        case 2:
//            DispDots(0x33, 0xcc);  // 小棋盘	
//            break;
//        case 3:
//            DispFram(0x55, 0xaa);	
//            break;
//        case 4:
//            DispDot(0xf0, 0x0f);  // 大棋盘
//            break;
//        case 5:
//            DispFram(0x55, 0x55);		
//            break;
//        case 6:
//            DispFram(0xff, 0x00);	 
//            break;
//        case 7:
//            DispFram(0xff, 0xff);	
//            break;
//        default:
//            mode = 0;  // 防止异常情况
//            break;
//    }
//}

//	 if (UP()) 
//	  {  
//		  while(1)
//		 {
//			DispDot(0xf0,0x0f);
//			Delay_Xms(600);			 
//          DispDots( 0x33,0xcc);
//			Delay_Xms(600);				 
//          DispFram(0x55,0xaa);
//			Delay_Xms(600);				 
//          yehui_UI();
//			Delay_Xms(600);				 
//          DispFram(0x55,0x55);	
//			Delay_Xms(600);				 
//          DispFram(0xff,0x00);	 
//			Delay_Xms(600);				 
//			DispFram(0x00,0xff);	
//			Delay_Xms(600);
//			WhatsApp();
//			Delay_Xms(100);	
//		  }	
//	}
      			
//	 if (DN()) 
//	  {  
//		RST = 0;    // 先拉低复位引脚
//		Delay_Xms(10);  // 等待 10ms，确保复位信号有效
//		RST = 1;    // 再拉高，完成复位	
//	  }

            DispPic( PIC_5 );
			Delay_Xms(5000);
            yehui_UI();	//     
			Delay_Xms(5000);
			DispDot(0xf0,0x0f);
			Delay_Xms(5000);			 
            DispDots( 0x33,0xcc);
			Delay_Xms(5000);				 
            DispFram(0x55,0xaa);
			Delay_Xms(5000);				 
            yehui_UI();
			Delay_Xms(5000);				 
            DispFram(0x55,0x55);	
			Delay_Xms(5000);				 
            DispFram(0xff,0x00);	 
			Delay_Xms(5000);				 
			DispFram(0x00,0xff);	
			Delay_Xms(5000);

	}

}