#include "LCM823109.h"
unsigned char seg,i,j;
void main(void)
{	
	VK2C23A_Init();
	VK2C23A_DisAll(0x00);
	
	
		for(i=0;i<8;i++)//com
		{
			for ( seg = 0; seg < VK2C23A_SEGNUM; seg++){VK2C23A_DisDotOn(seg,i);}
		}
		 delay_nms(10); 
		
		 VK2C23A_DisAll(0x00);
		
		for ( i = 10; i >= 0; i-- ) //9-0
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
	while(1)
	{
	      unsigned char mode;
		  if (ReadKey()) 
		  {  
			VK2C23A_Init();
			VK2C23A_DisAll(0x00);
			mode++;
			mode %= 3;			
			switch (mode) 
		  {
			case 0:            
				VK2C23A_DisAll(0xFF);delay_nms(20); VK2C23A_DisAll(0x00);break;
				
			case 1:
				VK2C23A_DisAll(0xFF);delay_nms(20); VK2C23A_DisAll(0x00);break; 
				
			case 2:
			for(i=0;i<8;i++)
			{
				for ( seg = 0; seg < VK2C23A_SEGNUM; seg++){VK2C23A_DisDotOn(seg,i);}
			}break;   
			
			case 3:
			  while(1)
			  {
				for(i=0;i<8;i++)//com
				{
					for ( seg = 0; seg < VK2C23A_SEGNUM; seg++){VK2C23A_DisDotOn(seg,i);}
				}
				
				for ( i = 10; i >= 0; i-- ) //9-0
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
			  }
				  
			}
	}
	}
}
