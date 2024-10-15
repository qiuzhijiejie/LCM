#include <INTRINS.H>        
#include <MATH.H>    
#include <ABSACC.H> 
#include <REG52.H> 
#include <STDIO.H> 
#include "LCD12864.H"
#include "HANZI.H"
#include "intrins.h"

#define uint unsigned int
#define uchar unsigned char


sbit KEY = P2^0;  // 按键引脚
uchar mode = 0;  // 模式变量



unsigned char code BMP3[]; 
unsigned char code BMP4[]; 
unsigned char code font[];
unsigned char code Character[];

void  main(void)
{
	 Delay(20); 
//	 IE=0x81;     //配置中断使能寄存器 IE 的值为 0x81。
	 IP=0x01;     //配置中断优先级寄存器 IP 的值为 0x01
	 TCON=0x01;   //配置定时器/计数器控制寄存器 TCON 的值为 0x01
//	 int0=1;      //将INT0引脚设置为高电平，以避免立即触发中断
	 Lcd12864_Init();
	 Clear();
	 
 
while(1)
{
	
	//按键控制
    if (ReadKey()) {
    mode++;
    mode %= 9; // 确保 mode 的值在 0 到 8 之间循环
	Clear(); 
    // 直接根据 mode 的值显示图案，不再等待图案完成
    switch (mode) {
        case 0:
        LcmPutDots1(0x98);
        break;
    case 1:
		LcmPutBMP(BMP6); 
        break;
    case 2:
        LcmPutDots1(0xaa);//偶数行 
        break;
    case 3:
        LcmPutDotsOddColumns();//奇数列  
        break;
    case 4:
        LcmPutBMP(BMP9); 
        break;
    case 5:
        LcmPutDotsStars(0x42);//星罗棋布 
        break;
    case 6:
        LcmPutDotsStars(0xaa);//互偶
        break;
    case 7:
        LcmPutDotsStars(0xa7);  
        break;
	case 8:
        LcmPutBMP(BMP7);  
        break;

    }
}


//LcmPutDotsOddColumns();//奇数列

//LcmPutDotsStars(0x42);//星罗棋布

//LcmPutDotsStars(0xaa);//互偶

//LcmPutDotsStars(0x55);//互偶

//LcmPutDots1(0xaa);//偶数行

//LcmPutDots1(0x55);//奇数行



//Showpicture_VerticalWave(0xAA, 0x55);  // 中间往两边波浪般（＄）
//Delay(500);  // 停留500ms
//Clear();

//Showpicture_Advanced(0x80, 0x40);  // 单条线中间往来两边
//Delay(500);  // 停留500ms
//Clear();

//Showpicture_Advanced(0x18, 0x3C);  // 四条线中间往来两边
//Delay(500);  // 停留500ms
//Clear();

//Showpicture_Advanced(0xAA, 0x55);  // 奇数行中间往来两边
//Delay(500);  // 停留500ms
//Clear();

//Showpicture_Advanced(0x18, 0x3C);  // 4行一单位
//Delay(500);  // 停留500ms
//Clear();

//Showpicture_Advanced1(0x18, 0x3C);  // 逐渐加速和减速
//Delay(500);  // 停留500ms
//Clear();

//Showpicture_Advanced2(0x80, 0x40);  // 偶数行两边往中间来
//Delay(500);  // 停留500ms
//Clear();

//Showpicture_CrossCursorEffect(0x80, 0x40);  // 溜溜球从上往下来（不完整）
//Delay(500);  // 停留500ms
//Clear();

//LcmPutBMP(BMP6);  // 
//Delay(500);  // 停留500ms
//Clear();

//LcmPutBMP(BMP4);  // 
//Delay(500);  // 停留500ms
//Clear();

//LcmPutBMP(BMP5);  // 梅奔
//Delay(500);  // 停留500ms
//Clear();


//LcmPutBMP(BMP6);
//Delay(500);
//LcmPutBMP(BMP7);//ST公司
//Delay(500);
//LcmPutBMP(BMP9);



}
}