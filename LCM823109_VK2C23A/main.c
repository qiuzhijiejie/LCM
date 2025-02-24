#include "LCM823109.h"
unsigned char seg,i,j;
void main(void)
{	
	VK2C23A_Init();
	VK2C23A_DisAll(0x00);
	while(1)
	{
		
//	for(i=0;i<8;i++)
//	{
//		for ( seg = 0; seg < VK2C23A_SEGNUM; seg++){VK2C23A_DisDotOn(seg,i);}
//	}
//	VK2C23A_DisAll(0x00);
	
	for ( i = 10; i > 0; i-- ) 
	{
		for ( j = 0; j < 24; j++) 
		{  
			DisplayDigitOrChar(j, i);
		}
		
		for ( j = 24; j < 51; j++) 
		{  
			DisplayDigitOrChar1(j, i);
		}
	}
	
	

//	VK2C23A_DisAll(0x00);
		
//	   for ( seg = 0; seg < VK2C23A_SEGNUM; seg++){VK2C23A_DisDotOn(seg,7);}
//		VK2C23A_DisAll(0xff);			//LCD全显
//		delay_nms(3000);					//延时1S
//	
//		VK2C23A_DisAll(0x00);			//LCD全关
//		delay_nms(3000);					//延时1S
//		
//		//LCD半显
//		VK2C23A_DisAll(0x55);			
//		ReadnDataVK2C23A(0,VK2C23A_readbuf,52);	//读回RAM数据
//		delay_nms(1500);					//延时
//		VK2C23A_DisAll(0xAA);
//		delay_nms(500);		
//		ReadnDataVK2C23A(0,VK2C23A_readbuf,52);	//读回RAM数据
//		delay_nms(500);					//延时
//		
//		//掉电
//		VK2C23A_DisAll(0x00);			//LCD全关		
//		VK2C23A_Enter_PowerOff();	//进入掉电模式
//		delay_nms(5000);					//延时5S		
//		VK2C23A_Exit_PowerOff();	//退出掉电模式
//		
//		VK2C23A_DisAll(0x00);			//LCD全关
//		delay_nms(500);					  //延时
//		for(VK2C23A_segi=0;VK2C23A_segi<VK2C23A_SEGNUM;VK2C23A_segi++)//seg
//		{
//			for(VK2C23A_comi=0;VK2C23A_comi<8;VK2C23A_comi++)//com
//			{
//					VK2C23A_DisDotOn(VK2C23A_segtab[VK2C23A_segi]-4,VK2C23A_comi);	//LCD单点点亮
//				delay_nms(300);				//延时300mS
//				VK2C23A_DisDotOff(VK2C23A_segtab[VK2C23A_segi]-4,VK2C23A_comi);	  //此行配合单点点亮函数实现单点点亮，屏蔽此行顺序点亮
//			}
//		}
//		
//		VK2C23A_DisAll(0xff);			//LCD全显
//		delay_nms(500);					  //延时
//		for(VK2C23A_segi=0;VK2C23A_segi<VK2C23A_SEGNUM;VK2C23A_segi++)//seg
//		{
//			for(VK2C23A_comi=0;VK2C23A_comi<8;VK2C23A_comi++)//com
//			{
//					VK2C23A_DisDotOff(VK2C23A_segtab[VK2C23A_segi]-4,VK2C23A_comi);	//LCD单点关闭
//				delay_nms(300);				//延时300mS
//				VK2C23A_DisDotOn(VK2C23A_segtab[VK2C23A_segi]-4,VK2C23A_comi);  //此行配合单点关闭函数实现单点关闭，屏蔽此行顺序关闭
//			}
//		}
//		delay_nms(1000);					  //延时
	}
}
/************************END OF FILE****/
