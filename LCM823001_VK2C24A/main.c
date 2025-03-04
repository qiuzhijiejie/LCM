#include "vk2c24a.h"
extern unsigned char vk2c24a_segi,vk2c24a_comi;
extern unsigned char Vk2c24a_segtab[Vk2c24a_SEGNUM];
void main(void)
{	
	unsigned char vk2c24a_maxcom=4;
	unsigned char seg;
	Vk2c24a_Init();
	Vk2c24a_DisAll(0x00);
	
	while(1)
	{
			for ( seg = 0; seg < 71; seg++)
			{		
				Vk2c24a_DisSegComOn(seg, 0);

			}
			
			for ( seg = 0; seg < 71; seg++)
			{
			
				Vk2c24a_DisSegComOn(seg, 1); 

			}
			
			for ( seg = 0; seg < 71; seg++)
			{
			
				Vk2c24a_DisSegComOn(seg, 2); 

			}
			
			for ( seg = 0; seg < 71; seg++)
			{
			
				Vk2c24a_DisSegComOn(seg, 3); 

			}

	}
}
