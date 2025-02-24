#ifndef __VK2C23A_H
#define __VK2C23A_H
#include <REGX52.H>

//I2C��ַд
#define VK2C23A_ADDR   			    0x7C    // IIC��ַ
//I2C��ַ��
#define VK2C23A_ADDRRD   			0x7D    // IIC��ַ+ bit0=1 ��
//��������
#define VK2C23A_RWRAM				0x80    // ��д��ʾRAM
#define VK2C23A_MODESET		        0x82    // BIAS,COM����
	#define CCOM_1_3__4 			0x00  	// 1/3bias 4com
	#define CCOM_1_4__4 			0x01  	// 1/4bias 4com
	#define CCOM_1_3__8 			0x02  	// 1/3bias 8com
	#define CCOM_1_4__8 			0x03  	// 1/4bias 8com
#define VK2C23A_SYSSET				0x84    // IRC,LCD��������
	#define SYSOFF_LCDOFF  		    0x00  	// IRC off,LCD off
	#define SYSON_LCDOFF  		    0x02  	// IRC on,LCD off
	#define SYSON_LCDON  			0x03  	// IRC on,LCD on
#define VK2C23A_FRAMESET			0x86    // ֡Ƶ����
	#define FRAME_80HZ  			0x00  	// ֡Ƶ80HZ
	#define FRAME_160HZ  			0x01  	// ֡Ƶ160HZ
#define VK2C23A_BLINKSET			0x88    // ��˸Ƶ������
	#define BLINK_OFF  				0x00  	// ��˸�ر�
	#define BLINK_2HZ  				0x01  	// ��˸2HZ
	#define BLINK_1HZ  				0x02  	// ��˸1HZ
	#define BLINK_0D5HZ  			0x03  	// ��˸0.5HZ
#define VK2C23A_IVASET				0x8A    // SEG/VLCD���ý����ú��ڲ���ѹ��������
	#define VLCDSEL_IVAOFF_R0 0x00  	// SEG/VLCD���ý���ΪVLCD���ڲ���ѹ�������ܹر�,VLCD��VDD�̽�VR=0ƫ�õ�ѹ=VDD
	#define VLCDSEL_IVAOFF_R1 0x0F  	// SEG/VLCD���ý���ΪVLCD���ڲ���ѹ�������ܹر�,VLCD��VDD���ӵ���VR>0ƫ�õ�ѹ=VLCD
	
	#define VLCDSEL_IVA00H 		0x10  	// SEG/VLCD���ý���ΪVLCD������ڲ�ƫ�õ�ѹ������1/3bias=VDD 			1/4bias=VDD
	#define VLCDSEL_IVA01H 		0x11  	// SEG/VLCD���ý���ΪVLCD������ڲ�ƫ�õ�ѹ������1/3bias=0.944VDD 1/4bias=0.957VDD
	#define VLCDSEL_IVA02H 		0x12  	// SEG/VLCD���ý���ΪVLCD������ڲ�ƫ�õ�ѹ������1/3bias=0.894VDD 1/4bias=0.918VDD
	#define VLCDSEL_IVA03H 		0x13  	// SEG/VLCD���ý���ΪVLCD������ڲ�ƫ�õ�ѹ������1/3bias=0.849VDD 1/4bias=0.882VDD
	#define VLCDSEL_IVA04H 		0x14  	// SEG/VLCD���ý���ΪVLCD������ڲ�ƫ�õ�ѹ������1/3bias=0.808VDD 1/4bias=0.849VDD
	#define VLCDSEL_IVA05H 		0x15  	// SEG/VLCD���ý���ΪVLCD������ڲ�ƫ�õ�ѹ������1/3bias=0.771VDD 1/4bias=0.818VDD
	#define VLCDSEL_IVA06H 		0x16  	// SEG/VLCD���ý���ΪVLCD������ڲ�ƫ�õ�ѹ������1/3bias=0.738VDD 1/4bias=0.789VDD
	#define VLCDSEL_IVA07H 		0x17  	// SEG/VLCD���ý���ΪVLCD������ڲ�ƫ�õ�ѹ������1/3bias=0.707VDD 1/4bias=0.763VDD
	#define VLCDSEL_IVA08H 		0x18  	// SEG/VLCD���ý���ΪVLCD������ڲ�ƫ�õ�ѹ������1/3bias=0.678VDD 1/4bias=0.738VDD
	#define VLCDSEL_IVA09H 		0x19  	// SEG/VLCD���ý���ΪVLCD������ڲ�ƫ�õ�ѹ������1/3bias=0.652VDD 1/4bias=0.714VDD
	#define VLCDSEL_IVA0AH 		0x1A  	// SEG/VLCD���ý���ΪVLCD������ڲ�ƫ�õ�ѹ������1/3bias=0.628VDD 1/4bias=0.692VDD
	#define VLCDSEL_IVA0BH 		0x1B  	// SEG/VLCD���ý���ΪVLCD������ڲ�ƫ�õ�ѹ������1/3bias=0.605VDD 1/4bias=0.672VDD
	#define VLCDSEL_IVA0CH 		0x1C  	// SEG/VLCD���ý���ΪVLCD������ڲ�ƫ�õ�ѹ������1/3bias=0.584VDD 1/4bias=0.652VDD
	#define VLCDSEL_IVA0DH 		0x1D  	// SEG/VLCD���ý���ΪVLCD������ڲ�ƫ�õ�ѹ������1/3bias=0.565VDD 1/4bias=0.634VDD
	#define VLCDSEL_IVA0EH 		0x1E  	// SEG/VLCD���ý���ΪVLCD������ڲ�ƫ�õ�ѹ������1/3bias=0.547VDD 1/4bias=0.616VDD
	#define VLCDSEL_IVA0FH 		0x1F  	// SEG/VLCD���ý���ΪVLCD������ڲ�ƫ�õ�ѹ������1/3bias=0.529VDD 1/4bias=0.600VDD
	
	#define SEGSEL_IVAOFF			0x20  	// SEG/VLCD���ý���ΪSEG���ڲ���ѹ�������ܹر�,VDD�ṩƫ�õ�ѹ
	#define SEGSEL_IVA00H 		0x30  	// SEG/VLCD���ý���ΪSEG���ڲ�ƫ�õ�ѹ������1/3bias=VDD 			1/4bias=VDD
	#define SEGSEL_IVA01H 		0x31  	// SEG/VLCD���ý���ΪSEG���ڲ�ƫ�õ�ѹ������1/3bias=0.944VDD 1/4bias=0.957VDD
	#define SEGSEL_IVA02H 		0x32  	// SEG/VLCD���ý���ΪSEG���ڲ�ƫ�õ�ѹ������1/3bias=0.894VDD 1/4bias=0.918VDD
	#define SEGSEL_IVA03H 		0x33  	// SEG/VLCD���ý���ΪSEG���ڲ�ƫ�õ�ѹ������1/3bias=0.849VDD 1/4bias=0.882VDD
	#define SEGSEL_IVA04H 		0x34  	// SEG/VLCD���ý���ΪSEG���ڲ�ƫ�õ�ѹ������1/3bias=0.808VDD 1/4bias=0.849VDD
	#define SEGSEL_IVA05H 		0x35  	// SEG/VLCD���ý���ΪSEG���ڲ�ƫ�õ�ѹ������1/3bias=0.771VDD 1/4bias=0.818VDD
	#define SEGSEL_IVA06H 		0x36  	// SEG/VLCD���ý���ΪSEG���ڲ�ƫ�õ�ѹ������1/3bias=0.738VDD 1/4bias=0.789VDD
	#define SEGSEL_IVA07H 		0x37  	// SEG/VLCD���ý���ΪSEG���ڲ�ƫ�õ�ѹ������1/3bias=0.707VDD 1/4bias=0.763VDD
	#define SEGSEL_IVA08H 		0x38  	// SEG/VLCD���ý���ΪSEG���ڲ�ƫ�õ�ѹ������1/3bias=0.678VDD 1/4bias=0.738VDD
	#define SEGSEL_IVA09H 		0x39  	// SEG/VLCD���ý���ΪSEG���ڲ�ƫ�õ�ѹ������1/3bias=0.652VDD 1/4bias=0.714VDD
	#define SEGSEL_IVA0AH 		0x3A  	// SEG/VLCD���ý���ΪSEG���ڲ�ƫ�õ�ѹ������1/3bias=0.628VDD 1/4bias=0.692VDD
	#define SEGSEL_IVA0BH 		0x3B  	// SEG/VLCD���ý���ΪSEG���ڲ�ƫ�õ�ѹ������1/3bias=0.605VDD 1/4bias=0.672VDD
	#define SEGSEL_IVA0CH 		0x3C  	// SEG/VLCD���ý���ΪSEG���ڲ�ƫ�õ�ѹ������1/3bias=0.584VDD 1/4bias=0.652VDD
	#define SEGSEL_IVA0DH 		0x3D  	// SEG/VLCD���ý���ΪSEG���ڲ�ƫ�õ�ѹ������1/3bias=0.565VDD 1/4bias=0.634VDD
	#define SEGSEL_IVA0EH 		0x3E  	// SEG/VLCD���ý���ΪSEG���ڲ�ƫ�õ�ѹ������1/3bias=0.547VDD 1/4bias=0.616VDD
	#define SEGSEL_IVA0FH 		0x3F  	// SEG/VLCD���ý���ΪSEG���ڲ�ƫ�õ�ѹ������1/3bias=0.529VDD 1/4bias=0.600VDD	

//����seg��
//8com
#define 	VK2C23A_SEGNUM					51

//���¹ܽ����ø��ݿͻ���Ƭ������Ӧ���޸� 
#define VK2C23A_SCL_PIN    2
#define VK2C23A_SCL_PORT   P3
#define VK2C23A_SCL_IO     P3_2

#define VK2C23A_SDA_PIN    1
#define VK2C23A_SDA_PORT   P3
#define VK2C23A_SDA_IO     P3_1

//���¹ܽ����������ݿͻ���Ƭ������Ӧ���޸�  
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

//// ��������
//#define DATA_PORT PORTB
//#define DATA_DDR  DDRB
//#define RS_PIN    PORTC0
//#define RW_PIN    PORTC1
//#define EN_PIN    PORTC2

//// ��������
//void LCD_Command(unsigned char command);
//void LCD_Data(unsigned char data);
//void LCD_Init();
//void LCD_String(char *str);

//int main(void) {
//    // ��ʼ��LCD
//    LCD_Init();
//    
//    // ��ʾ�ַ���
//    LCD_String("Hello, World!");
//    
//    while (1) {
//        // ��ѭ��
//    }
//}

//// �������LCD
//void LCD_Command(unsigned char command) {
//    DATA_PORT = command;
//    PORTC &= ~(1 << RS_PIN); // RS = 0 (����ģʽ)
//    PORTC &= ~(1 << RW_PIN); // RW = 0 (дģʽ)
//    PORTC |= (1 << EN_PIN);  // EN = 1
//    _delay_ms(1);
//    PORTC &= ~(1 << EN_PIN); // EN = 0
//    _delay_ms(1);
//}

//// �������ݵ�LCD
//void LCD_Data(unsigned char data) {
//    DATA_PORT = data;
//    PORTC |= (1 << RS_PIN);  // RS = 1 (����ģʽ)
//    PORTC &= ~(1 << RW_PIN); // RW = 0 (дģʽ)
//    PORTC |= (1 << EN_PIN);  // EN = 1
//    _delay_ms(1);
//    PORTC &= ~(1 << EN_PIN); // EN = 0
//    _delay_ms(1);
//}

//// ��ʼ��LCD
//void LCD_Init() {
//    DATA_DDR = 0xFF; // �������ݶ˿�Ϊ���
//    DDRC |= (1 << RS_PIN) | (1 << RW_PIN) | (1 << EN_PIN); // ���ÿ�������Ϊ���
//    
//    _delay_ms(20); // �ȴ�LCD�ϵ��ȶ�
//    
//    LCD_Command(0x38); // 8λ���ݽӿڣ�2����ʾ��5x7����
//    LCD_Command(0x0C); // ��ʾ�������أ���겻��˸
//    LCD_Command(0x06); // ��ַ�Զ�������������
//    LCD_Command(0x01); // ����
//    _delay_ms(2);
//}

//// ��ʾ�ַ���
//void LCD_String(char *str) {
//    while (*str) {
//        LCD_Data(*str++);
//    }
//}