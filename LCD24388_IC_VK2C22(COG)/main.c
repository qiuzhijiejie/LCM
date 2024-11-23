#include <REGX52.H>
#include "lcd24388.h"
unsigned char seg,com,i,j;
unsigned char mode=0;
unsigned char vk2c22_segi,vk2c22_comi;
unsigned char vk2c22_maxcom;



void main(void)
{	
	Vk2c22_Init();
	Vk2c22_DisAll(0x00);	 	

for ( i = 0; i <= 15; i++) 
{
    for ( j = 0; j <= 6; j++) 
	{  
        DisplayDigitOrChar(j, i);
    }
	    for ( j = 7; j <= 20; j++) 
	{  
        DisplayDigitOrChar2(j, i);
    }
}

	while(1)
	{

		if (Key()) 
		{
		 mode++;
		 mode%=2;		 
			switch (mode) 
		 {
			 
			case 0:	
			for ( seg = 0; seg < 42; seg++)
			{
				Vk2c22_DisSegComOn(seg, 1); 
				Vk2c22_DisSegComOn(seg, 3);
			}
			
			break;
			case 1:	
			for ( seg = 0; seg < 42; seg++)
			{   
				
				Vk2c22_DisSegComOn(seg, 0);
				Vk2c22_DisSegComOn(seg, 2); 
			}
			break;
//			case 2:
//			for ( seg = 0; seg < 42; seg++)
//			{
//				Vk2c22_DisSegComOn(seg, 2); 

//			}
//			break;
//			case 3:
//			for ( seg = 0; seg < 42; seg++)
//			{
//				Vk2c22_DisSegComOn(seg, 9); 

//			}
//			break;
		 }
		}		
		
	}
}
	
