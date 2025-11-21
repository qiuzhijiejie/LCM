#include "lcm12864cog.h"
#include "DATA.txt"

//图片存储数据
void main(void)
{

	while(1)  
	{
	init_cpu();
	Delay_Xms(50);
   	InitST7567();	//一开始初始化控制器

  Start:	//跳转标号
      			   	    
//-------------------------------------------------------------------------		
//		hz_display16(3,3,0,jin); 
//		Delay_Xms(100000);


//		hz_display16(0,0,18,se); 
//		Delay_Xms(1000);
		
//		DispPic( pic12864_KH8 );
//		Delay_Xms(60000);


		DispFram( 0xff,0xff );				 //全显
		Delay_Xms(5000);
		if( Change(70)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上
		push();
		Pause();


		DispFram( 0x00,0x00 );			   //无显
		Delay_Xms(5000);
		if( Change(70)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上
		Pause();


//		------------------------------------------------------------------	
		DispDot( 0xf0,0x0f );			 //大棋盘
		Delay_Xms(5000);
		if( Change(70)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上
		push();
		Pause();
//		------------------------------------------------------------------


		DispDots( 0x33,0xcc );			  //小棋盘
		Delay_Xms(5000);
		if( Change(70)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上
		push();
		Pause();
//		------------------------------------------------------------------	
		DispFram( 0x55,0xaa );				  //花点		
		Delay_Xms(5000);
		if( Change(70)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上
		push();
		Pause();

		DispFram( 0x55,0x55 );					 //	 横线
		Delay_Xms(5000);
		if( Change(70)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上
		push();
		Pause();

		DispFram( 0xaa,0xaa );				     //	横线
		Delay_Xms(1000);
		if( Change(300)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上
		push();
		Pause();
//		-------------------------------------------------------------------------		
		DispFram( 0xff,0x00 );					 //竖线 
		Delay_Xms(5000);
		if( Change(70)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上
		push();
		Pause();

		DispFram( 0x00,0xff );
		Delay_Xms(1000);
		if( Change(300)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上
		push();
		Pause();
		
		
		
//----------------------------------------------------------------------------	 

//DispPic( PIC_1 );
//Delay_Xms(1000);
//if( Change(300)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上
//push();
//Pause();	

//ST7567_WriteComd(0xa7);
//Delay_Xms(1000);
//if( Change(300)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上
//Pause();

//ST7567_WriteComd(0xa6);
//Delay_Xms(1000);
//if( Change(300)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上
//Pause();

//DispPic( pic12864_KH8 );
//Delay_Xms(500);
//if( Change(300)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上
//Pause();

//DispPic1( PIC_CS );
//Delay_Xms(500);
//if( Change(50)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上	  

//Pause();




//if( Change(70)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上
//Pause();


//DispPic1( PIC_4 );
//Delay_Xms(600);
//if( Change(50)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上	  


//DispPic( PIC_5 );
//Delay_Xms(600);
//if( Change(50)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上	

//DispPic( PIC_6 );
//Delay_Xms(600);
//if( Change(50)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上	

//DispPic( PIC_7 );
//Delay_Xms(600);
//if( Change(50)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上	

//DispPic( PIC_8 );
//Delay_Xms(600);
//if( Change(50)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上	
///*	

//ST7567_WriteComd(0x81);	//The Electronic Volume (Double Byte Command)数字电位器调节VO输出电压(也就是对比度)两字节				 0x81原来的数值
//ST7567_WriteComd(55);	 //vop 设置
//DispPic( PIC_9 );	//中值
//Delay_Xms(600);
//if( Change(50)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上	
//push();

//ST7567_WriteComd(0x81);	//The Electronic Volume (Double Byte Command)数字电位器调节VO输出电压(也就是对比度)两字节				 0x81原来的数值
//ST7567_WriteComd(57);	 //vop 设置	
//DispPic( PIC_10 );	   //上限
//Delay_Xms(600);
//if( Change(50)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上	
//push();

//ST7567_WriteComd(0x81);	//The Electronic Volume (Double Byte Command)数字电位器调节VO输出电压(也就是对比度)两字节				 0x81原来的数值
//ST7567_WriteComd(52);	 //vop 设置
//DispPic( PIC_11 );		  //下限
//Delay_Xms(600);
//if( Change(50)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上	
//push();
//*/


//DispPic( PIC_12 );
//Delay_Xms(600);
//if( Change(50)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上	

//DispPic( PIC_13 );
//Delay_Xms(600);
//if( Change(50)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上	


//DispPic( PIC_14 );
//Delay_Xms(600);
//if( Change(50)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上	



//DispPic( PIC_15 );
//Delay_Xms(600);
//if( Change(50)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上	


//DispPic( PIC_16 );
//Delay_Xms(600);
//if( Change(50)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上	


//DispPic( PIC_17 );
//Delay_Xms(600);
//if( Change(50)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上	


//DispPic( PIC_18 );
//Delay_Xms(600);
//if( Change(50)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上	


//DispPic( PIC_19 );
//Delay_Xms(600);
//if( Change(50)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上	


//DispPic( PIC_20 );
//Delay_Xms(600);
//if( Change(50)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上	

//DispPic( PIC_21 );
//Delay_Xms(600);
//if( Change(50)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上	

//DispPic( PIC_22 );
//Delay_Xms(600);
//if( Change(50)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上	

//DispPic( PIC_23 );
//Delay_Xms(600);
//if( Change(50)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上	

//DispPic( PIC_24 );
//Delay_Xms(600);
//if( Change(50)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上	

//DispPic( PIC_25 );
//Delay_Xms(600);
//if( Change(50)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上	

//DispPic( PIC_26 );
//Delay_Xms(600);
//if( Change(50)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上	

//DispPic( PIC_27 );
//Delay_Xms(600);
//if( Change(50)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上	

//DispPic( PIC_28);
//Delay_Xms(600);
//if( Change(50)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上	

	}
}