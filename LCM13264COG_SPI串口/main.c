#include <reg52.h>
#include <intrins.h>
#include <ST24C02A.c>					//读写存储器程序
#include "pic.h"						//图片存储数据
#include <stdio.h>
#include "lcm13264cog.h"


void main(void)
{
	init_cpu();
	Delay_Xms(50);
   	InitST7567();	//一开始初始化控制器
//	hz_display16(0,0,0,jin); 
	DispFram( 0x00,0x00 );	
	while(1)  
	{

  Start:	//跳转标号
      			   	    		
//	  hz_display16(0,0,0,jin); 
//	  Delay_Xms(1000);
		 

		
	  DispFram( 0xff,0xff );				 //全显
	  Delay_Xms(2800);
//	  if( Change(70)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上
//	  push();
//	  Pause();


	
	  DispDot( 0xf0,0x0f );			 //大棋盘
	  Delay_Xms(2800);


	  DispDots( 0x33,0xcc );	    //小棋盘
	  Delay_Xms(3000);

	
	  DispFram( 0x55,0xaa );		 //花点		
	  Delay_Xms(3000);

	
	  DispFram( 0x55,0x55 );		//	 横线
	  Delay_Xms(3000);

		
	  DispFram( 0xff,0x00 );		 //竖线 
	  Delay_Xms(3000);
 

	  DispPic( PIC_5 );
	  Delay_Xms(3000);
	  if( Change(50)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上	
	
	  DispPic( PIC_6 );
      Delay_Xms(3000);
	  if( Change(50)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上	
	
/*	

    ST7567_WriteComd(0x81);	//The Electronic Volume (Double Byte Command)数字电位器调节VO输出电压(也就是对比度)两字节				 0x81原来的数值
	ST7567_WriteComd(55);	 //vop 设置
	DispPic( PIC_9 );	//中值
	Delay_Xms(600);
	if( Change(50)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上	
    push();

    ST7567_WriteComd(0x81);	//The Electronic Volume (Double Byte Command)数字电位器调节VO输出电压(也就是对比度)两字节				 0x81原来的数值
	ST7567_WriteComd(57);	 //vop 设置	
 	DispPic( PIC_10 );	   //上限
	Delay_Xms(600);
	if( Change(50)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上	
	push();

    ST7567_WriteComd(0x81);	//The Electronic Volume (Double Byte Command)数字电位器调节VO输出电压(也就是对比度)两字节				 0x81原来的数值
	ST7567_WriteComd(52);	 //vop 设置
    DispPic( PIC_11 );		  //下限
	Delay_Xms(600);
	if( Change(50)==1 ) goto Start;	//暂停键有动作跳转到程序的最头上	
    push();
*/

	}
}
