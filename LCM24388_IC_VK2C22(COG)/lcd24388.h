
#ifndef __lcd24388_H
#define __lcd24388_H
#include <REGX52.H>
#define VK2c22_CLK 2 //SCL�ź���Ƶ��,��delay_nusʵ�� 50->10kHz 10->50kHz 5->100kHz

#define Vk2c22_ADDR   			0x7E    // IIC��ַ
//��������
#define Vk2c22_MODESET		  0x80    // BIAS,֡Ƶ����ʾ��������
	#define BIAS_1_2					0x01  	// 1/2bias 4com
	#define BIAS_1_3					0x00  	// 1/3bias 4com
	#define F_80HZ						0x00  	// ֡Ƶ80hz
	#define F160HZ						0x10  	// ֡Ƶ160hz
	#define RCON_LCDON 				0x0C  	// �ڲ�RC����lcd��
	#define RCOFF_LCDOFF 			0x00  	// �ڲ�RC�أ�lcd��
#define Vk2c22_BLINKSET			0xC0    // ��˸Ƶ������
	#define BLINK_OFF  				0x00  	// ��˸�ر�
	#define BLINK_2HZ  				0x01  	// ��˸2HZ
	#define BLINK_1HZ  				0x02  	// ��˸1HZ
	#define BLINK_0D5HZ  			0x03  	// ��˸0.5HZ
#define Vk2c22_IVASET				0x40    // SEG/VLCD���ý����ú��ڲ���ѹ��������
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
//4com 
#define 	Vk2c22_SEGNUM					42

//���¹ܽ����ø��ݿͻ���Ƭ������Ӧ���޸� 
#define Vk2c22_SCL_PIN    1
#define Vk2c22_SCL_PORT   P3
#define Vk2c22_SCL_IO     P3_1

#define Vk2c22_SDA_PIN    0
#define Vk2c22_SDA_PORT   P3
#define Vk2c22_SDA_IO     P3_0

//���¹ܽ����������ݿͻ���Ƭ������Ӧ���޸�
#define Vk2c22_SCL_H() 				Vk2c22_SCL_IO = 1 
#define Vk2c22_SCL_L() 				Vk2c22_SCL_IO = 0 

#define Vk2c22_SDA_H() 				Vk2c22_SDA_IO = 1 
#define Vk2c22_SDA_L() 				Vk2c22_SDA_IO = 0 

#define Vk2c22_GET_SDA() 			Vk2c22_SDA_IO
//��Ƭ��SDA��Ϊ����IOҪ��Ϊ�����
//���ݿͻ���Ƭ���޸�
#define Vk2c22_DIRDAT_PIN   	BIT0
#define Vk2c22_DIRDAT_PORT  	PB
#define Vk2c22_DIRDAT_IO    	PB0
#define Vk2c22_DIRDAT1_PIN   	BIT12
#define Vk2c22_DIRDAT1_PORT  	PA
#define Vk2c22_DIRDAT1_IO    	PA12

#define Vk2c22_SET_SDA_IN() 	{\
  Vk2c22_DIRDAT_IO = 0;\
  Vk2c22_DIRDAT1_IO = 0;\
}
#define Vk2c22_SET_SDA_OUT() 	{\
 	Vk2c22_DIRDAT_IO = 1;\
  Vk2c22_DIRDAT1_IO = 1;\
}



extern unsigned char vk2c22_segtab[Vk2c22_SEGNUM];
extern unsigned char vk2c22_dispram[Vk2c22_SEGNUM/2];
extern unsigned char data shuzi_zimo[15];
extern unsigned char data shuzi_zimo2[15];
extern unsigned char vk2c22_segi,vk2c22_comi;
extern unsigned char vk2c22_maxcom;

unsigned char Vk2c22_InitSequence(void);
void delay_nus(unsigned int n)	;
void delay_nms(unsigned long int n);
void Vk2c22_I2CStart( void );
void Vk2c22_I2CStop( void );
unsigned char Vk2c22_I2CSlaveAck( void );
void Vk2c22_I2CWriteByte( unsigned char byte );
unsigned char WriteCmdVk2c22(unsigned char cmd);
unsigned char Write1DataVk2c22(unsigned char Addr,unsigned char Dat);
unsigned char  WritenDataVk2c22(unsigned char Addr,unsigned char *Databuf,unsigned char Cnt);
void Vk2c22_DisAll(unsigned char dat);
void Vk2c22_DisSegComOn(unsigned char seg,unsigned char com);
void Vk2c22_DisSegComOff(unsigned char seg,unsigned char com);
void Vk2c22_Enter_Standby(void);
void Vk2c22_Exit_Standby(void);
void Vk2c22_Lowlevel_Init(void);
void Vk2c22_Init(void);
void disp_3num(unsigned int dat);
unsigned char Key() ;
void DisplayDigitOrChar(unsigned char pos, unsigned char index) ;
void DisplayDigitOrChar2(unsigned char pos, unsigned char index) ;

#endif  