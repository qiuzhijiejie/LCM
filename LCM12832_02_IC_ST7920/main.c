#include <reg52.h>
#include "lcd12832.h"
#include "picture.h"

//unsigned char mode;

//void main(void)
//{   
//     lcdreset();              
//     charlcdfill(' ');              
//	
//	 glcdfill_with_params(0xff,0x00) ;
//	 exdelay(); 
//	 glcdfill_with_params(0x00,0xff) ;
//	 exdelay();
//	 glcdfill_with_params(0xaa,0x55) ;
//	 exdelay(); 
//	 glcdfill_with_params(0x55,0xaa) ;
//	 exdelay();
//	 glcdfill_with_params(0xaa,0xaa) ;
//	 exdelay(); 
//	 glcdfill_with_params(0x55,0x55) ;
//	 exdelay();
//	 glcdfill_with_params(0xaa,0x00) ;
//	 exdelay(); 
//	 glcdfill_with_params(0x00,0x55) ;
//	 exdelay(); 
//	 lcdreset(); 
//	 charlcdfill(' ');
//     putstrxy(0,0,"烨辉科技有限公司");   
//	 putstrxy(2,1,"LCM12832_02 ");                             
//	 exdelay();	
//	while(1)
//    {  
//		
//		if(key())
//		{
//			lcdreset(); 
//			charlcdfill(' ');
//			mode++;
//			mode%=8;
//			switch(mode)
//			{
//				case 0:glcdfill_with_params(0xff,0x00) ;
//				break;
//				case 1:glcdfill_with_params(0x00,0xff) ;
//				break;
//				case 2:glcdfill_with_params(0x55,0x55) ;
//				break;
//				case 3:glcdfill_with_params(0xaa,0xaa) ;
//				break;
//				case 4:glcdfill_with_params(0xaa,0x55) ;
//				break;
//				case 5:glcdfill_with_params(0x55,0xaa) ;
//				break;
//				case 6:glcdfill_with_params(0xff,0xff) ;
//				break;
//				case 7:putstrxy(0,0,"烨辉科技有限公司"); 
//					   putstrxy(2,1,"LCM12832_02 ");   
//				break;				
//			}
//		}
//    }
//}

void main(void)   
{

     lcdreset();              
     charlcdfill(' ');  

    while (1)  
    {
        if (key()) 
        {
            HandleKey(); 
        } 
        else 
        {
          
            DisplayPatterns(); 
        }
    }   
}
