#ifndef __VK2C23A_H
#define __VK2C23A_H
#include <REGX52.H>

//I2C地址写
#define VK2C23A_ADDR   			    0x7C    // IIC地址
//I2C地址读
#define VK2C23A_ADDRRD   			0x7D    // IIC地址+ bit0=1 读
//基本设置
#define VK2C23A_RWRAM				0x80    // 读写显示RAM
#define VK2C23A_MODESET		        0x82    // BIAS,COM设置
	#define CCOM_1_3__4 			0x00  	// 1/3bias 4com
	#define CCOM_1_4__4 			0x01  	// 1/4bias 4com
	#define CCOM_1_3__8 			0x02  	// 1/3bias 8com
	#define CCOM_1_4__8 			0x03  	// 1/4bias 8com
#define VK2C23A_SYSSET				0x84    // IRC,LCD开关设置
	#define SYSOFF_LCDOFF  		    0x00  	// IRC off,LCD off
	#define SYSON_LCDOFF  		    0x02  	// IRC on,LCD off
	#define SYSON_LCDON  			0x03  	// IRC on,LCD on
#define VK2C23A_FRAMESET			0x86    // 帧频设置
	#define FRAME_80HZ  			0x00  	// 帧频80HZ
	#define FRAME_160HZ  			0x01  	// 帧频160HZ
#define VK2C23A_BLINKSET			0x88    // 闪烁频率设置
	#define BLINK_OFF  				0x00  	// 闪烁关闭
	#define BLINK_2HZ  				0x01  	// 闪烁2HZ
	#define BLINK_1HZ  				0x02  	// 闪烁1HZ
	#define BLINK_0D5HZ  			0x03  	// 闪烁0.5HZ
#define VK2C23A_IVASET				0x8A    // SEG/VLCD共用脚设置和内部电压调整设置
	#define VLCDSEL_IVAOFF_R0 0x00  	// SEG/VLCD共用脚设为VLCD，内部电压调整功能关闭,VLCD和VDD短接VR=0偏置电压=VDD
	#define VLCDSEL_IVAOFF_R1 0x0F  	// SEG/VLCD共用脚设为VLCD，内部电压调整功能关闭,VLCD和VDD串接电阻VR>0偏置电压=VLCD
	
	#define VLCDSEL_IVA00H 		0x10  	// SEG/VLCD共用脚设为VLCD输出，内部偏置电压调整：1/3bias=VDD 			1/4bias=VDD
	#define VLCDSEL_IVA01H 		0x11  	// SEG/VLCD共用脚设为VLCD输出，内部偏置电压调整：1/3bias=0.944VDD 1/4bias=0.957VDD
	#define VLCDSEL_IVA02H 		0x12  	// SEG/VLCD共用脚设为VLCD输出，内部偏置电压调整：1/3bias=0.894VDD 1/4bias=0.918VDD
	#define VLCDSEL_IVA03H 		0x13  	// SEG/VLCD共用脚设为VLCD输出，内部偏置电压调整：1/3bias=0.849VDD 1/4bias=0.882VDD
	#define VLCDSEL_IVA04H 		0x14  	// SEG/VLCD共用脚设为VLCD输出，内部偏置电压调整：1/3bias=0.808VDD 1/4bias=0.849VDD
	#define VLCDSEL_IVA05H 		0x15  	// SEG/VLCD共用脚设为VLCD输出，内部偏置电压调整：1/3bias=0.771VDD 1/4bias=0.818VDD
	#define VLCDSEL_IVA06H 		0x16  	// SEG/VLCD共用脚设为VLCD输出，内部偏置电压调整：1/3bias=0.738VDD 1/4bias=0.789VDD
	#define VLCDSEL_IVA07H 		0x17  	// SEG/VLCD共用脚设为VLCD输出，内部偏置电压调整：1/3bias=0.707VDD 1/4bias=0.763VDD
	#define VLCDSEL_IVA08H 		0x18  	// SEG/VLCD共用脚设为VLCD输出，内部偏置电压调整：1/3bias=0.678VDD 1/4bias=0.738VDD
	#define VLCDSEL_IVA09H 		0x19  	// SEG/VLCD共用脚设为VLCD输出，内部偏置电压调整：1/3bias=0.652VDD 1/4bias=0.714VDD
	#define VLCDSEL_IVA0AH 		0x1A  	// SEG/VLCD共用脚设为VLCD输出，内部偏置电压调整：1/3bias=0.628VDD 1/4bias=0.692VDD
	#define VLCDSEL_IVA0BH 		0x1B  	// SEG/VLCD共用脚设为VLCD输出，内部偏置电压调整：1/3bias=0.605VDD 1/4bias=0.672VDD
	#define VLCDSEL_IVA0CH 		0x1C  	// SEG/VLCD共用脚设为VLCD输出，内部偏置电压调整：1/3bias=0.584VDD 1/4bias=0.652VDD
	#define VLCDSEL_IVA0DH 		0x1D  	// SEG/VLCD共用脚设为VLCD输出，内部偏置电压调整：1/3bias=0.565VDD 1/4bias=0.634VDD
	#define VLCDSEL_IVA0EH 		0x1E  	// SEG/VLCD共用脚设为VLCD输出，内部偏置电压调整：1/3bias=0.547VDD 1/4bias=0.616VDD
	#define VLCDSEL_IVA0FH 		0x1F  	// SEG/VLCD共用脚设为VLCD输出，内部偏置电压调整：1/3bias=0.529VDD 1/4bias=0.600VDD
	
	#define SEGSEL_IVAOFF			0x20  	// SEG/VLCD共用脚设为SEG，内部电压调整功能关闭,VDD提供偏置电压
	#define SEGSEL_IVA00H 		0x30  	// SEG/VLCD共用脚设为SEG，内部偏置电压调整：1/3bias=VDD 			1/4bias=VDD
	#define SEGSEL_IVA01H 		0x31  	// SEG/VLCD共用脚设为SEG，内部偏置电压调整：1/3bias=0.944VDD 1/4bias=0.957VDD
	#define SEGSEL_IVA02H 		0x32  	// SEG/VLCD共用脚设为SEG，内部偏置电压调整：1/3bias=0.894VDD 1/4bias=0.918VDD
	#define SEGSEL_IVA03H 		0x33  	// SEG/VLCD共用脚设为SEG，内部偏置电压调整：1/3bias=0.849VDD 1/4bias=0.882VDD
	#define SEGSEL_IVA04H 		0x34  	// SEG/VLCD共用脚设为SEG，内部偏置电压调整：1/3bias=0.808VDD 1/4bias=0.849VDD
	#define SEGSEL_IVA05H 		0x35  	// SEG/VLCD共用脚设为SEG，内部偏置电压调整：1/3bias=0.771VDD 1/4bias=0.818VDD
	#define SEGSEL_IVA06H 		0x36  	// SEG/VLCD共用脚设为SEG，内部偏置电压调整：1/3bias=0.738VDD 1/4bias=0.789VDD
	#define SEGSEL_IVA07H 		0x37  	// SEG/VLCD共用脚设为SEG，内部偏置电压调整：1/3bias=0.707VDD 1/4bias=0.763VDD
	#define SEGSEL_IVA08H 		0x38  	// SEG/VLCD共用脚设为SEG，内部偏置电压调整：1/3bias=0.678VDD 1/4bias=0.738VDD
	#define SEGSEL_IVA09H 		0x39  	// SEG/VLCD共用脚设为SEG，内部偏置电压调整：1/3bias=0.652VDD 1/4bias=0.714VDD
	#define SEGSEL_IVA0AH 		0x3A  	// SEG/VLCD共用脚设为SEG，内部偏置电压调整：1/3bias=0.628VDD 1/4bias=0.692VDD
	#define SEGSEL_IVA0BH 		0x3B  	// SEG/VLCD共用脚设为SEG，内部偏置电压调整：1/3bias=0.605VDD 1/4bias=0.672VDD
	#define SEGSEL_IVA0CH 		0x3C  	// SEG/VLCD共用脚设为SEG，内部偏置电压调整：1/3bias=0.584VDD 1/4bias=0.652VDD
	#define SEGSEL_IVA0DH 		0x3D  	// SEG/VLCD共用脚设为SEG，内部偏置电压调整：1/3bias=0.565VDD 1/4bias=0.634VDD
	#define SEGSEL_IVA0EH 		0x3E  	// SEG/VLCD共用脚设为SEG，内部偏置电压调整：1/3bias=0.547VDD 1/4bias=0.616VDD
	#define SEGSEL_IVA0FH 		0x3F  	// SEG/VLCD共用脚设为SEG，内部偏置电压调整：1/3bias=0.529VDD 1/4bias=0.600VDD	

//驱动seg数
//8com
#define 	VK2C23A_SEGNUM					51

//以下管脚配置根据客户单片机做相应的修改 
#define VK2C23A_SCL_PIN    2
#define VK2C23A_SCL_PORT   P3
#define VK2C23A_SCL_IO     P3_2

#define VK2C23A_SDA_PIN    1
#define VK2C23A_SDA_PORT   P3
#define VK2C23A_SDA_IO     P3_1

//以下管脚输出定义根据客户单片机做相应的修改  
#define VK2C23A_SCL_H() 				VK2C23A_SCL_IO = 1 
#define VK2C23A_SCL_L() 				VK2C23A_SCL_IO = 0 

#define VK2C23A_SDA_H() 				VK2C23A_SDA_IO = 1 
#define VK2C23A_SDA_L() 				VK2C23A_SDA_IO = 0 

#define VK2C23A_GET_SDA() 			    VK2C23A_SDA_IO
/* Exported functions ------------------------------------------------------- */
void VK2C23A_Init(void);
void VK2C23A_DisAll(unsigned char dat);
void VK2C23A_DisSegComOn(unsigned char seg,unsigned char com);
void VK2C23A_DisSegComOff(unsigned char seg,unsigned char com);
void VK2C23A_DisDotOn(unsigned char seg,unsigned char com);
void VK2C23A_DisDotOff(unsigned char seg,unsigned char com);
void DisplayDigitOrChar(unsigned char pos, unsigned char index) ;
void delay_nms(unsigned long int n);
void delay_nus(unsigned int n)	;
void DisplayDigitOrChar1(unsigned char pos, unsigned char index) ;

#endif  /*__VK2C23A_H*/






//#include <avr/io.h>
//#include <util/delay.h>

//// 定义引脚
//#define DATA_PORT PORTB
//#define DATA_DDR  DDRB
//#define RS_PIN    PORTC0
//#define RW_PIN    PORTC1
//#define EN_PIN    PORTC2

//// 函数声明
//void LCD_Command(unsigned char command);
//void LCD_Data(unsigned char data);
//void LCD_Init();
//void LCD_String(char *str);

//int main(void) {
//    // 初始化LCD
//    LCD_Init();
//    
//    // 显示字符串
//    LCD_String("Hello, World!");
//    
//    while (1) {
//        // 主循环
//    }
//}

//// 发送命令到LCD
//void LCD_Command(unsigned char command) {
//    DATA_PORT = command;
//    PORTC &= ~(1 << RS_PIN); // RS = 0 (命令模式)
//    PORTC &= ~(1 << RW_PIN); // RW = 0 (写模式)
//    PORTC |= (1 << EN_PIN);  // EN = 1
//    _delay_ms(1);
//    PORTC &= ~(1 << EN_PIN); // EN = 0
//    _delay_ms(1);
//}

//// 发送数据到LCD
//void LCD_Data(unsigned char data) {
//    DATA_PORT = data;
//    PORTC |= (1 << RS_PIN);  // RS = 1 (数据模式)
//    PORTC &= ~(1 << RW_PIN); // RW = 0 (写模式)
//    PORTC |= (1 << EN_PIN);  // EN = 1
//    _delay_ms(1);
//    PORTC &= ~(1 << EN_PIN); // EN = 0
//    _delay_ms(1);
//}

//// 初始化LCD
//void LCD_Init() {
//    DATA_DDR = 0xFF; // 设置数据端口为输出
//    DDRC |= (1 << RS_PIN) | (1 << RW_PIN) | (1 << EN_PIN); // 设置控制引脚为输出
//    
//    _delay_ms(20); // 等待LCD上电稳定
//    
//    LCD_Command(0x38); // 8位数据接口，2行显示，5x7点阵
//    LCD_Command(0x0C); // 显示开，光标关，光标不闪烁
//    LCD_Command(0x06); // 地址自动递增，不移屏
//    LCD_Command(0x01); // 清屏
//    _delay_ms(2);
//}

//// 显示字符串
//void LCD_String(char *str) {
//    while (*str) {
//        LCD_Data(*str++);
//    }
//}