 #include "lcd24388.h"
#include <intrins.h> 
sbit   KEY1  = P2^0;	

//segtab[]�����Ӧʵ�ʵ�оƬ��LCD���ߣ����߼�-VK2c22�ο���·
//4com 
//Vk2c22a 
//unsigned char vk2c22_segtab[Vk2c22_SEGNUM]={
//	0,1,2,3,4,5,6,7,8,9,			//SEG0-SEG9
//	10,11,12,13,14,15,16,17,18,19,	//SEG10-SEG19
//	20,21,22,23,24,25,26,27,28,29,	//SEG20-SEG29
//	30,31,32,33,34,35,36,37,38,39,  //SEG30-SEG39
//	40,41,42,43,					//SEG40-SEG43
//};
//Vk2c22b 
//unsigned char vk2c22_segtab[Vk2c22_SEGNUM]={
//	0,1,2,3,4,5,6,7,8,9,			//SEG0-SEG9
//	10,11,12,13,14,15,16,17,18,19,	//SEG10-SEG19
//	20,21,22,23,24,25,26,27,28,29,	//SEG20-SEG29
//	30,31,32,33,34,35,36,37,38,39,  //SEG30-SEG39
//	40,41,
//};
//����LCDʵ��ֻ����SEG10��SEG21ʹ����������
//.h�ļ�����VK2c22_SEGNUM=12
//const unsigned char vk2c22_segtab[VK2c22_SEGNUM]={	
//	10,11,12,13,14,15,16,17,18,19,	//SEG10-SEG19
//	20,21,													//SEG20-SEG21
//};
//vk2c22_dispram��ӦоƬ����ʾRAM
//4com
unsigned char vk2c22_dispram[Vk2c22_SEGNUM/2];//4COMʱÿ���ֽ����ݶ�Ӧ2��SEG
//��Ӧ����vk2c22_segtab[VK2c22_SEGNUM]
//��ʾ RAM bufferΪ8λ��Ӧ2��SEG��bit7/bit3->com3,bit6/bit2->com2,bit5/bit1->com1,bit4/bit0->com0
//LCD ʵ��  3��8��
//1�ǰ�λ��2��ʮλ��3�Ǹ�λ
/*��ʾbuffer��lcdͼ��ӳ���ϵ����
/4COM
vk2c22_dispram[VK2c22_SEGNUM/2]=
{//com3   com2   com1     com0 //��ӦоƬSEG��  ��Ӧ��ʾRAM����
	1D,     1E,    1F,      1A,     //seg4   			vk2c22_dispram[2]-bit3-bit0
	  ,     1C,    1G,      1B,     //seg5				vk2c22_dispram[2]-bit7-bit4
	2D,     2E,    2F,      2A,     //seg6				vk2c22_dispram[3]-bit3-bit0
	  ,     2C,    2G,      2B,     //seg7				vk2c22_dispram[3]-bit7-bit4
	3D,     3E,    3F,      3A,		//seg8				vk2c22_dispram[4]-bit3-bit0
	  ,     3C,    3G,      3B,		//seg9				vk2c22_dispram[4]-bit7-bit4
}
*/



unsigned char Vk2c22_InitSequence(void);
/*******************************************************************************
* Function Name  : delay_nus
* Description    : ��ʱ1uS����
* Input          : n->��ʱʱ��nuS
* Output         : None
* Return         : None
*******************************************************************************/
void delay_nus(unsigned int n)	   
{
	unsigned char i;
	while(n--)
	{
		i=10;
		while(i--)
		{//nopָ����ݵ�Ƭ������Ӧ���޸�
			_nop_();
		}
	}
}
/*******************************************************************************
* Function Name  : delay_nms
* Description    : ��ʱ1mS����
* Input          : n->��ʱʱ��nmS
* Output         : None
* Return         : None
*******************************************************************************/
void delay_nms(unsigned long int n)
{
	while(n--)
	{
		delay_nus(1000);
	}
}
/*******************************************************************************
* Function Name  : I2CStart
* Description    : ʱ���߸�ʱ���������ɸߵ��͵����䣬��ʾI2C��ʼ�ź�
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Vk2c22_I2CStart( void )
{
  Vk2c22_SCL_H();
  Vk2c22_SDA_H();
  delay_nus(VK2c22_CLK);
  Vk2c22_SDA_L();
  delay_nus(VK2c22_CLK);
}
/*******************************************************************************
* Function Name  : I2CStop
* Description    : ʱ���߸�ʱ���������ɵ͵��ߵ����䣬��ʾI2Cֹͣ�ź�
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Vk2c22_I2CStop( void )
{
  Vk2c22_SCL_H();
  Vk2c22_SDA_L();
  delay_nus(VK2c22_CLK);
  Vk2c22_SDA_H();
  delay_nus(VK2c22_CLK);
}
/*******************************************************************************
* Function Name  : I2CSlaveAck
* Description    : I2C�ӻ��豸Ӧ���ѯ
* Input          : None
* Output         : None
* Return         : 0-ok 1-fail
*******************************************************************************/
unsigned char Vk2c22_I2CSlaveAck( void )
{
  unsigned int TimeOut;
  unsigned char RetValue;
	
  Vk2c22_SCL_L();
	//��Ƭ��SDA��Ϊ����IOҪ��Ϊ�����
//	Vk2c22_SET_SDA_IN();
  delay_nus(VK2c22_CLK);
  Vk2c22_SCL_H();//��9��sclk������

  TimeOut = 10000;
  while( TimeOut-- > 0 )
  {
    if( Vk2c22_GET_SDA()!=0 )//��ȡack
    {
      RetValue = 1;
    }
    else
    {
      RetValue = 0;
      break;
    }
  } 
	Vk2c22_SCL_L();
	//��Ƭ��SDA��Ϊ����IOҪ��Ϊ�����
//	Vk2c22_SET_SDA_OUT();  
  
  return RetValue;
}
/*******************************************************************************
* Function Name  : I2CWriteByte
* Description    : I2Cдһ�ֽ�����,�������͸�λ
* Input          : byte-Ҫд�������
* Output         : None
* Return         : None
*******************************************************************************/
void Vk2c22_I2CWriteByte( unsigned char byte )
{
	unsigned char i=8;
	while (i--)
	{ 
		Vk2c22_SCL_L();
		if(byte&0x80)
			Vk2c22_SDA_H();
		else
			Vk2c22_SDA_L();
		byte<<=1; 
		delay_nus(VK2c22_CLK);
		Vk2c22_SCL_H();     
		delay_nus(VK2c22_CLK);
	}
}

/*******************************************************************************
* Function Name  : WriteCmd
* Description    : д1�ֽ�����
* Input          : cmd-Ҫд�������
* Output         : None
* Return         : 0-ok 1-fail
*******************************************************************************/
unsigned char WriteCmdVk2c22(unsigned char cmd)
{
	Vk2c22_I2CStart();

	Vk2c22_I2CWriteByte( Vk2c22_ADDR|0x00 );
	if( 1 == Vk2c22_I2CSlaveAck() )
	{
		Vk2c22_I2CStop(); 
		return 0;   
	}
	Vk2c22_I2CWriteByte( cmd );
	if( 1 == Vk2c22_I2CSlaveAck() )
	{
		Vk2c22_I2CStop(); 
		return 0;   
	}
  Vk2c22_I2CStop();
 
  return 0;    //���ز����ɰܱ�־
}
/*******************************************************************************
* Function Name  : Write1Data
* Description    : д1�ֽ����ݵ���ʾRAM
* Input          : Addr-д��ram�ĵ�ַ
*                : Dat->д��ram������
* Output         : None
* Return         : 0-ok 1-fail
*******************************************************************************/
unsigned char Write1DataVk2c22(unsigned char Addr,unsigned char Dat)
{
	//START �ź�
	Vk2c22_I2CStart(); 
	//SLAVE��ַ
	Vk2c22_I2CWriteByte(Vk2c22_ADDR); 
	if( 1 == Vk2c22_I2CSlaveAck() )
	{		
		Vk2c22_I2CStop();
		return 1; 
	}
	//��ʾRAM��ַ
	Vk2c22_I2CWriteByte(Addr); 
	if( 1 == Vk2c22_I2CSlaveAck() )
	{
		Vk2c22_I2CStop();
		return 1;
	}
	//��ʾ���ݣ�1�ֽ����ݰ���2��SEG
	Vk2c22_I2CWriteByte(Dat);
	if( Vk2c22_I2CSlaveAck()==1 )
	{
		Vk2c22_I2CStop();
		return 1;
	}
	//STOP�ź�
 Vk2c22_I2CStop();
 return 0;   
}
/*******************************************************************************
* Function Name  : WritenData
* Description    : д������ݵ���ʾRAM
* Input          : Addr-д��ram����ʼ��ַ
*                : Databuf->д��ram������bufferָ��
*                : Cnt->д��ram�����ݸ���
* Output         : None
* Return         : 0-ok 1-fail
*******************************************************************************/
unsigned char  WritenDataVk2c22(unsigned char Addr,unsigned char *Databuf,unsigned char Cnt)
{
	unsigned char n;
	
	//START�ź�	
	Vk2c22_I2CStart(); 									
	//SLAVE��ַ
	Vk2c22_I2CWriteByte(Vk2c22_ADDR); 	
	if( 1 == Vk2c22_I2CSlaveAck() )
	{
		Vk2c22_I2CStop();													
		return 0;										
	}
	//��ʾRAM��ʼ��ַ
	Vk2c22_I2CWriteByte(Addr); 						
	if( 1 == Vk2c22_I2CSlaveAck() )
	{
		Vk2c22_I2CStop();													
		return 0;
	}
	//����Cnt�����ݵ���ʾRAM
	for(n=0;n<Cnt;n++)
	{ 
		Vk2c22_I2CWriteByte(*Databuf++);
		if( Vk2c22_I2CSlaveAck()==1 )
		{
			Vk2c22_I2CStop();													
			return 0;
		}
	}
	//STOP�ź�
	 Vk2c22_I2CStop();											
	 return 0;    
}
/*******************************************************************************
* Function Name  : Vk2c22_DisAll
* Description    : ����SEG��ʾͬһ�����ݣ�bit7/bit3-COM3 bit6/bit2-COM2 bit5/bit1-COM1 bit4/bit0-COM0
* 					     : ���磺0xffȫ�� 0x00ȫ�� 0x55�������� 0xaa�������� 0x33�������� 
* Input          ��dat->д��ram������(1���ֽ����ݶ�Ӧ2��SEG)  
* Output         : None
* Return         : None
*******************************************************************************/
void Vk2c22_DisAll(unsigned char dat)
{
	unsigned char segi;
	unsigned char dispram[22];
	
	for(segi=0;segi<28;segi++)
	{
		dispram[segi]=dat;
	}
	WritenDataVk2c22(0,dispram,22);//������8bit���ݶ�Ӧ2��SEG��ÿ8bit���ݵ�ַ��1��ÿ8λ����1��ACK	
}
/*******************************************************************************
* Function Name  : DisSegComOn
* Description    : ����1����(1��seg��1��com�����Ӧ����ʾ��)
* Input          ��seg->���Ӧ��seg��  
* 		         ��com->���Ӧcom��  
* Output         : None
* Return         : None
*******************************************************************************/

void Vk2c22_DisSegComOn(unsigned char seg,unsigned char com)
{
	unsigned char addrbit, tempdat;
//	if(seg%2==0)
//		Write1DataVk2c22(seg/2,(1<<(com)));//������8λ���ݵ�4bit��Ч��ÿ8bit���ݵ�ַ��1��ÿ8λ����1��ACK)
//	else
//		Write1DataVk2c22(seg/2,(1<<(4+com)));//������8λ���ݸ�4bit��Ч��ÿ8bit���ݵ�ַ��1��ÿ8λ����1��ACK	
	
    if (seg % 2 == 0)//�ж��ǲ���ż��
        addrbit = (1 << com); // ��Ӧ�� 4 λ
    else
        addrbit = (1 << (4 + com)); // ��Ӧ�� 4 λ
    // ����ʾ״̬�ۼӣ������Ǹ���
    tempdat = vk2c22_dispram[seg / 2] | addrbit;
    vk2c22_dispram[seg / 2] = tempdat;
    WritenDataVk2c22(seg / 2, &tempdat, 1);	
}
/*******************************************************************************
* Function Name  : DisSegComOff
* Description    : �ر�1����(1��seg��1��com�����Ӧ����ʾ��)
* Input          ��seg->���Ӧ��seg��  
* 		           ��com->���Ӧcom��  
* Output         : None
* Return         : None
*******************************************************************************/

void Vk2c22_DisSegComOff(unsigned char seg, unsigned char com)
{
    unsigned char addrbit, tempdat;

    // �ж��ǵ�4λ���Ǹ�4λ
    if (seg % 2 == 0)
        addrbit = ~(1 << com); // �����4λ�е�Ŀ��λ
    else
        addrbit = ~(1 << (4 + com)); // �����4λ�е�Ŀ��λ

    // ��ȡ��ǰ�Ķ���״̬
    tempdat = vk2c22_dispram[seg / 2];
    tempdat &= addrbit; // ���Ŀ��λ
    vk2c22_dispram[seg / 2] = tempdat; // �����ڴ�

    // ���µ�״̬д����ʾRAM
    WritenDataVk2c22(seg / 2, &tempdat, 1);
}

/*******************************************************************************
* Function Name  : Enter_Standby
* Description    : �������͹���ģʽ,��������ʾ
* Input          ��None 
* Output         : None
* Return         : 0-ok 1-fail
*******************************************************************************/
void Vk2c22_Enter_Standby(void)
{		
//	WriteCmdVk2c22(Vk2c22_MODESET|BIAS_1_2|F_80HZ|RCOFF_LCDOFF);  //ģʽ����  1/2 Bais 1/4 Duty֡Ƶ��80Hz,�ڲ�ϵͳ��������lcd����ʾ
	WriteCmdVk2c22(Vk2c22_MODESET|BIAS_1_3|F_80HZ|RCOFF_LCDOFF);  //ģʽ����  1/3 Bais 1/4 Duty֡Ƶ��80Hz,�ڲ�ϵͳ��������lcd����ʾ
//	WriteCmdVk2c22(Vk2c22_MODESET|BIAS_1_2|F_160HZ|RCOFF_LCDOFF); //ģʽ����  1/2 Bais 1/4 Duty֡Ƶ��160Hz,�ڲ�ϵͳ��������lcd����ʾ
//	WriteCmdVk2c22(Vk2c22_MODESET|BIAS_1_3|F_160HZ|RCOFF_LCDOFF); //ģʽ����  1/3 Bais 1/4 Duty֡Ƶ��60Hz,�ڲ�ϵͳ��������lcd����ʾ
}
/*******************************************************************************
* Function Name  : Exit_Standby
* Description    : �˳�����͹���ģʽ
* Input          ��None 
* Output         : None
* Return         : None
*******************************************************************************/
void Vk2c22_Exit_Standby(void)
{	
//	WriteCmdVk2c22(Vk2c22_MODESET|BIAS_1_2|F_80HZ|RCON_LCDON);  //ģʽ����  1/2 Bais 1/4 Duty֡Ƶ��80Hz,�ڲ�ϵͳ��������lcd����ʾ
	WriteCmdVk2c22(Vk2c22_MODESET|BIAS_1_3|F_80HZ|RCON_LCDON);  //ģʽ����  1/3 Bais 1/4 Duty֡Ƶ��80Hz,�ڲ�ϵͳ��������lcd����ʾ
//	WriteCmdVk2c22(Vk2c22_MODESET|BIAS_1_2|F_160HZ|RCON_LCDON); //ģʽ����  1/2 Bais 1/4 Duty֡Ƶ��160Hz,�ڲ�ϵͳ��������lcd����ʾ
//	WriteCmdVk2c22(Vk2c22_MODESET|BIAS_1_3|F_160HZ|RCON_LCDON); //ģʽ����  1/3 Bais 1/4 Duty֡Ƶ?60Hz,�ڲ�ϵͳ��������lcd����ʾ
}

/*******************************************************************************
* Function Name  : Lowlevel_Init
* Description    : ����ͨ����GPIO
* Input          ��None 
* Output         : None
* Return         : None
*******************************************************************************/
void Vk2c22_Lowlevel_Init(void)
{
	//ͨ���ߵ�ƽ��ͬ������ӵ�ƽת����·
	//�˺������ݿͻ���Ƭ������Ӧ���޸�	
//	Vk2c22_SET_SDA_DIR();
//	GPIO_SetMode(Vk2c22_SCL_PORT, Vk2c22_SCL_PIN, GPIO_MODE_OUTPUT);
//	GPIO_SetMode(Vk2c22_SDA_PORT, Vk2c22_SDA_PIN, GPIO_MODE_QUASI);
		     
	Vk2c22_SCL_H();  
	Vk2c22_SDA_H(); 	
}
/*******************************************************************************
* Function Name  : Init
* Description    : ��ʼ������
* Input          ��None 
* Output         : None
* Return         : None
*******************************************************************************/
void Vk2c22_Init(void)
{	
	//�ܽ����ø��ݿͻ���Ƭ������Ӧ���޸�
	Vk2c22_Lowlevel_Init();
	//1/4 Duty
//	WriteCmdVk2c22(Vk2c22_MODESET|BIAS_1_2|F_80HZ|RCON_LCDON);  //ģʽ����  1/2 Bais 1/4 Duty֡Ƶ��80Hz,�ڲ�ϵͳ��������lcd����ʾ
	WriteCmdVk2c22(Vk2c22_MODESET|BIAS_1_3|F_80HZ|RCON_LCDON);  //ģʽ����  1/3 Bais 1/4 Duty֡Ƶ��80Hz,�ڲ�ϵͳ��������lcd����ʾ
//	WriteCmdVk2c22(Vk2c22_MODESET|BIAS_1_2|F_160HZ|RCON_LCDON); //ģʽ����  1/2 Bais 1/4 Duty֡Ƶ��160Hz,�ڲ�ϵͳ��������lcd����ʾ
//	WriteCmdVk2c22(Vk2c22_MODESET|BIAS_1_3|F_160HZ|RCON_LCDON); //ģʽ����  1/3 Bais 1/4 Duty֡Ƶ?60Hz,�ڲ�ϵͳ��������lcd����ʾ
	vk2c22_maxcom=4;
	WriteCmdVk2c22(Vk2c22_BLINKSET|BLINK_OFF); 		//��˸�ر�	
//	WriteCmdVk2c22(Vk2c22_BLINKSET|BLINK_2HZ); 		//��˸2HZ
//	WriteCmdVk2c22(Vk2c22_BLINKSET|BLINK_1HZ); 		//��˸1HZ
//	WriteCmdVk2c22(Vk2c22_BLINKSET|BLINK_0D5HZ); 	//��˸0.5HZ
	//SEG/VLCD���ý���ΪVLCD���ڲ���ѹ�������ܹر�,VLCD��VDD�̽�VR=0ƫ�õ�ѹ=VDD
//	WriteCmdVk2c22(Vk2c22_IVASET|VLCDSEL_IVAOFF_R0); 
	//SEG/VLCD���ý���ΪVLCD���ڲ���ѹ�������ܹر�,VLCD��VDD���ӵ���VR>0ƫ�õ�ѹ=VLCD
//	WriteCmdVk2c22(Vk2c22_IVASET|VLCDSEL_IVAOFF_R1); 
	//SEG/VLCD���ý���ΪSEG���ڲ�ƫ�õ�ѹ������1/3bias=0.652VDD 1/4bias=0.714VDD
	WriteCmdVk2c22(Vk2c22_IVASET|SEGSEL_IVA02H);	 
}
/*******************************************************************************
* Function Name  : disp_3num
* Description    : ��ʾ3λ����
* Input          ��dat->3λ���� ʮ���� 
* Output         : None
* Return         : None
*******************************************************************************/
void disp_3num(unsigned int dat)
{		
	unsigned dat8;
		
	dat8=dat/100%10;	//��ʾ��λ
	vk2c22_dispram[2]&=0xf0;
	vk2c22_dispram[2]|=shuzi_zimo[dat8]&0x0f;
	vk2c22_dispram[2]&=0x8f;
	vk2c22_dispram[2]|=shuzi_zimo[dat8]&0xf0;
	
	dat8=dat/10%10; 	//��ʾʮλ
	vk2c22_dispram[3]&=0xf0;
	vk2c22_dispram[3]|=shuzi_zimo[dat8]&0x0f;
	vk2c22_dispram[3]&=0x8f;
	vk2c22_dispram[3]|=shuzi_zimo[dat8]&0xf0;
	
	dat8=dat%10;			//��ʾ��λ
	vk2c22_dispram[4]&=0xf0;
	vk2c22_dispram[4]|=shuzi_zimo[dat8]&0x0f;
	vk2c22_dispram[4]&=0x8f;
	vk2c22_dispram[4]|=shuzi_zimo[dat8]&0xf0;
		
	if(dat<100)				//����С��100����λ����ʾ
	{
	vk2c22_dispram[2]&=0xf0;
	vk2c22_dispram[2]&=0x8f;
	}
	if(dat<10) 	//����С��10��ʮλ����ʾ
	{
	vk2c22_dispram[3]&=0xf0;
	vk2c22_dispram[3]&=0x8f;
	}
	//SEG������1��1��������
//		Write1DataVk2c22(2,vk2c22_dispram[2]);
//		Write1DataVk2c22(3,vk2c22_dispram[3]);
//		Write1DataVk2c22(4,vk2c22_dispram[4]);
	//SEG�����Ͷ������
	WritenDataVk2c22(0,&vk2c22_dispram[0],5);	
}	




unsigned char Key() 
{
    if (KEY1 == 0) 
	{  
        delay_nms(1);  
        if (KEY1 == 0) 
		{  
            while (KEY1 == 0);  
            return 1;  
        }
    }
    return 0;  
}


unsigned char data shuzi_zimo[15]= //���ֺ��ַ���ģ
{             
// 0    1    2    3    4    5    6    7    8    9    -    L    0    H    
  0XAF,0xA0,0xCB,0xE9,0xE4,0X6D,0X6F,0XA8,0XEF,0XEC,0X40,0X07,0XAF,0XE6,0xFF,
};

unsigned char data shuzi_zimo2[15]= //���ֺ��ַ���ģ
{             
//   0    1    2    3    4    5    6    7    8    9    -    L    0    H 	
	0XFA,0X0A,0xBC,0X9E,0X4E,0XD6,0XF6,0X8A,0XFE,0XCE,0X40,0X70,0XFA,0X6E,0xFF,
};

void DisplayDigitOrChar(unsigned char pos, unsigned char index) 
{
	unsigned char  Data;
    // ��������Ƿ������鷶Χ��
    if (index >= sizeof(shuzi_zimo)) 
		return;

    // ��ȡ��ģ����
    Data = shuzi_zimo[index];

    // ʹ�� Write1DataVk2c22 ������д����ʾ RAM ��ָ��λ��
    Write1DataVk2c22(pos, Data);
}


void DisplayDigitOrChar2(unsigned char pos, unsigned char index) 
{
	unsigned char  Data;
    // ��������Ƿ������鷶Χ��
    if (index >= sizeof(shuzi_zimo2)) 
		return;

    // ��ȡ��ģ����
    Data = shuzi_zimo2[index];

    // ʹ�� Write1DataVk2c22 ������д����ʾ RAM ��ָ��λ��
    Write1DataVk2c22(pos, Data);
}