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
//		VK2C23A_DisAll(0xff);			//LCDȫ��
//		delay_nms(3000);					//��ʱ1S
//	
//		VK2C23A_DisAll(0x00);			//LCDȫ��
//		delay_nms(3000);					//��ʱ1S
//		
//		//LCD����
//		VK2C23A_DisAll(0x55);			
//		ReadnDataVK2C23A(0,VK2C23A_readbuf,52);	//����RAM����
//		delay_nms(1500);					//��ʱ
//		VK2C23A_DisAll(0xAA);
//		delay_nms(500);		
//		ReadnDataVK2C23A(0,VK2C23A_readbuf,52);	//����RAM����
//		delay_nms(500);					//��ʱ
//		
//		//����
//		VK2C23A_DisAll(0x00);			//LCDȫ��		
//		VK2C23A_Enter_PowerOff();	//�������ģʽ
//		delay_nms(5000);					//��ʱ5S		
//		VK2C23A_Exit_PowerOff();	//�˳�����ģʽ
//		
//		VK2C23A_DisAll(0x00);			//LCDȫ��
//		delay_nms(500);					  //��ʱ
//		for(VK2C23A_segi=0;VK2C23A_segi<VK2C23A_SEGNUM;VK2C23A_segi++)//seg
//		{
//			for(VK2C23A_comi=0;VK2C23A_comi<8;VK2C23A_comi++)//com
//			{
//					VK2C23A_DisDotOn(VK2C23A_segtab[VK2C23A_segi]-4,VK2C23A_comi);	//LCD�������
//				delay_nms(300);				//��ʱ300mS
//				VK2C23A_DisDotOff(VK2C23A_segtab[VK2C23A_segi]-4,VK2C23A_comi);	  //������ϵ����������ʵ�ֵ�����������δ���˳�����
//			}
//		}
//		
//		VK2C23A_DisAll(0xff);			//LCDȫ��
//		delay_nms(500);					  //��ʱ
//		for(VK2C23A_segi=0;VK2C23A_segi<VK2C23A_SEGNUM;VK2C23A_segi++)//seg
//		{
//			for(VK2C23A_comi=0;VK2C23A_comi<8;VK2C23A_comi++)//com
//			{
//					VK2C23A_DisDotOff(VK2C23A_segtab[VK2C23A_segi]-4,VK2C23A_comi);	//LCD����ر�
//				delay_nms(300);				//��ʱ300mS
//				VK2C23A_DisDotOn(VK2C23A_segtab[VK2C23A_segi]-4,VK2C23A_comi);  //������ϵ���رպ���ʵ�ֵ���رգ����δ���˳��ر�
//			}
//		}
//		delay_nms(1000);					  //��ʱ
	}
}
/************************END OF FILE****/
