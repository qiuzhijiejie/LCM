#include "vk2c24a.h"
#include <intrins.h>
unsigned char seg,com,i,j;
unsigned char vk2c24a_segi,vk2c24a_comi;
unsigned char vk2c24a_maxcom;//驱动的com数VK2C24A可以是4com或者8com
extern unsigned char Vk2c24a_segtab[Vk2c24a_SEGNUM];

void main(void)
{	
	Vk2c24a_Init();
	Vk2c24a_DisAll(0x00);

	while(1)
	{
//		Vk2c24a_DisSegComOn(15,8);
		
		Vk2c24a_DisAll(0xff);			//LCD全显
//		delay_nms(1000);					//延时1S
		
		


//		for(vk2c24a_segi=0;vk2c24a_segi<Vk2c24a_SEGNUM;vk2c24a_segi++)//seg
//		{
//			for(vk2c24a_comi=0;vk2c24a_comi<vk2c24a_maxcom;vk2c24a_comi++)//com
//			{
//				if(vk2c24a_maxcom==8)
//					Vk2c24a_DisSegComOn(Vk2c24a_segtab[vk2c24a_segi]-4,vk2c24a_comi);	//LCD单点点亮
//				else
//					Vk2c24a_DisSegComOn(Vk2c24a_segtab[vk2c24a_segi],vk2c24a_comi);	//LCD单点点亮
//				delay_nms(300);				//延时300mS
//				Vk2c24a_DisAll(0x00);	//LCD全关
//			}
//		}

//		for(vk2c24a_segi=0;vk2c24a_segi<Vk2c24a_SEGNUM;vk2c24a_segi++)//seg
//		{
//			for(vk2c24a_comi=0;vk2c24a_comi<vk2c24a_maxcom;vk2c24a_comi++)//com
//			{
//				if(vk2c24a_maxcom==8)
//					Vk2c24a_DisSegComOff(Vk2c24a_segtab[vk2c24a_segi]-4,vk2c24a_comi);	//LCD单点关闭
//				else
//				Vk2c24a_DisSegComOff(Vk2c24a_segtab[vk2c24a_segi],vk2c24a_comi);	//LCD单点关闭
//					delay_nms(300);				//延时300mS
//				Vk2c24a_DisAll(0xff);	//LCD全显
//			}
//		}

	}
}