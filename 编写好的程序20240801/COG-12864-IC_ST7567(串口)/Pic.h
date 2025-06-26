#ifndef __pic_H__
#define __pic_H__

#define uchar unsigned char
#define uint unsigned int

extern uchar  code guang[32];
extern uchar  code dong[32];
extern uchar  code ye[32];
extern uchar  code hui[32];
extern uchar  code ke[32];
extern uchar  code ji[32];
extern uchar  code you[32];
extern uchar  code xian[32];
extern uchar  code gong[32];
extern uchar  code si[32];




unsigned char code num[][16];
unsigned char code PIC_99[];
unsigned char code PIC_5[];


unsigned char code disp_vop[][16];


extern uchar code aa[16];
extern uchar code a[16];
extern uchar code b[16];
extern uchar code C[16];
extern uchar code D[16];
extern uchar code E[16];
extern uchar code F[16];
extern uchar code G[16];
extern uchar code hh[16];
extern uchar code h[16];
extern uchar code I[16];
extern uchar code J[16];
extern uchar code K[16];
extern uchar code L[16];
extern uchar code M[16];
extern uchar code N[16];
extern uchar code O[16];
extern uchar code pp[16];
extern uchar code p[16];
extern uchar code Q[16];
extern uchar code R[16];
extern uchar code S[16];
extern uchar code tt[16];
extern uchar code t[16];
extern uchar code U[16];
extern uchar code V[16];
extern uchar code W[16];
extern uchar code X[16];
extern uchar code Y[16];
extern uchar code Z[16];
extern uchar code KG[16];
extern uchar code DH[16];
extern uchar code DIAN[16];


extern uchar code one[16];
extern uchar code two[16];
extern uchar code three[16];
extern uchar code four[16];
extern uchar code five[16];
extern uchar code six[16];
extern uchar code seven[16];
extern uchar code eight[16];
extern uchar code nine[16];
extern uchar code xing[16];
extern uchar code JIA[16];


//	while(1)  
//	{
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
//            DispPic( PIC_5 );
//			Delay_Xms(5000);
//            yehui_UI();	//     
//			Delay_Xms(5000);
//			DispDot(0xf0,0x0f);
//			Delay_Xms(5000);			 
//            DispDots( 0x33,0xcc);
//			Delay_Xms(5000);				 
//            DispFram(0x55,0xaa);
//			Delay_Xms(5000);				 
//            yehui_UI();
//			Delay_Xms(5000);				 
//            DispFram(0x55,0x55);	
//			Delay_Xms(5000);				 
//            DispFram(0xff,0x00);	 
//			Delay_Xms(5000);				 
//			DispFram(0x00,0xff);	
//			Delay_Xms(5000);
	
//		-------------------------------------------------------------------------		
//		hz_display16(0,0,0,jin); 
//		Delay_Xms(1000);

//		hz_display16(0,0,18,se); 
//		Delay_Xms(1000);

//		DispFram( 0xff,0xff );				 //全显
//		Delay_Xms(2800);
//		if( Change(70)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上
//		push();
//		Pause();

//		DispFram( 0x00,0x00 );			   //无显
//		Delay_Xms(2800);
//		if( Change(70)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上
//		Pause();

//		------------------------------------------------------------------	
//		DispDot( 0xf0,0x0f );			 //大棋盘
//		Delay_Xms(2800);
//		if( Change(70)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上
//		push();
//		Pause();
//		------------------------------------------------------------------

//		DispDots( 0x33,0xcc );			  //小棋盘
//		Delay_Xms(2800);
//		if( Change(70)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上
//		push();
//		Pause();
//		------------------------------------------------------------------	
//		DispFram( 0x55,0xaa );				  //花点		
//		Delay_Xms(2800);
//		if( Change(70)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上
//		push();
//		Pause();

//		DispFram( 0x55,0x55 );					 //	 横线
//		Delay_Xms(2800);
//		if( Change(70)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上
//		push();
//		Pause();

//		DispFram( 0xaa,0xaa );				     //	横线
//		Delay_Xms(1000);
//		if( Change(300)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上
//		//push();
//		Pause();
//		-------------------------------------------------------------------------		
//		DispFram( 0xff,0x00 );					 //竖线 
//		Delay_Xms(500);
//		if( Change(70)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上
//		// push();
//		Pause();

//		DispFram( 0x00,0xff );
//		Delay_Xms(1000);
//		if( Change(300)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上
//		push();
//		Pause();
//		----------------------------------------------------------------------------	 

//		DispPic( PIC_1 );
//		Delay_Xms(1000);
//		if( Change(300)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上
//		push();
//		Pause();	

//		ST7567_WriteComd(0xa7);
//		Delay_Xms(1000);
//		if( Change(300)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上
//		Pause();

//		ST7567_WriteComd(0xa6);
//		Delay_Xms(1000);
//		if( Change(300)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上
//		Pause();

//		DispPic( pic12864_KH8 );
//		Delay_Xms(500);
//		if( Change(300)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上
//		Pause();

//		DispPic1( PIC_CS );
//		Delay_Xms(500);
//		if( Change(50)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上	  
//		Pause();


//		DispPic( PIC_5 );
//		Delay_Xms(2800);
//		if( Change(70)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上
//		Pause();

//		DispPic1( PIC_4 );
//		Delay_Xms(600);
//		if( Change(50)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上	  


//		DispPic( PIC_5 );
//		Delay_Xms(2800);
//		if( Change(50)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上	

//		DispPic( PIC_6 );
//		Delay_Xms(2500);
//		if( Change(50)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上	

//		DispPic( PIC_7 );
//		Delay_Xms(600);
//		if( Change(50)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上	

//		DispPic( PIC_8 );
//		Delay_Xms(600);
//		if( Change(50)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上	
//		/*	

//		ST7567_WriteComd(0x81);	//The Electronic Volume (Double Byte Command)数字电位器调节VO输出电压(也就是对比度)两字节				 0x81原来的数值
//		ST7567_WriteComd(55);	 //vop 设置
//		DispPic( PIC_9 );	//中值
//		Delay_Xms(600);
//		if( Change(50)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上	
//		push();

//		ST7567_WriteComd(0x81);	//The Electronic Volume (Double Byte Command)数字电位器调节VO输出电压(也就是对比度)两字节				 0x81原来的数值
//		ST7567_WriteComd(57);	 //vop 设置	
//		DispPic( PIC_10 );	   //上限
//		Delay_Xms(600);
//		if( Change(50)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上	
//		push();

//		ST7567_WriteComd(0x81);	//The Electronic Volume (Double Byte Command)数字电位器调节VO输出电压(也就是对比度)两字节				 0x81原来的数值
//		ST7567_WriteComd(52);	 //vop 设置
//		DispPic( PIC_11 );		  //下限
//		Delay_Xms(600);
//		if( Change(50)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上	
//		push();
//		*/


//		DispPic( PIC_12 );
//		Delay_Xms(600);
//		if( Change(50)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上	


//	}


#endif