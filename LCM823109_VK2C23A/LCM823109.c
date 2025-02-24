#include "LCM823109.h"
#include <intrins.h> 

#define VK2C23A_CLK 1 //SCL�ź���Ƶ��,��delay_nusʵ�� 50->10kHz 10->50kHz 5->100kHz

//VK2C23A 8com 
unsigned char VK2C23A_segtab[VK2C23A_SEGNUM]={
	5,6,7,8,9,				        //SEG5-SEG9
	10,11,12,13,14,15,16,17,18,19,	//SEG10-SEG19
	20,21,22,23,24,25,26,27,28,29,	//SEG20-SEG29
	30,31,32,33,34,35,36,37,38,39,  //SEG30-SEG39
	40,41,42,43,44,45,46,47,48,49,  //SEG40-SEG49
	50,51,52,53,54					//SEG50-SEG54
};

//8com
unsigned char VK2C23A_dispram[VK2C23A_SEGNUM];//8COMʱÿ���ֽ����ݶ�Ӧ1��SEG
//��ʾRAM bufferΪ8λ��Ӧ1��SEG,bit7->com7,bit6->com6,...,bit0->com0

unsigned char VK2C23A_segi,VK2C23A_comi;
unsigned char VK2C23A_maxcom;//������com��VK2C23A������4com����8com
unsigned char VK2C23A_readbuf[52];
void VK2C23A_InitSequence(void);

/*******************************************************************************
* Function Name  : delay_nus
* Description    : ��ʱ1uS����
* Input          : x->��ʱʱ��xus
* Output         : None
* Return         : None
*******************************************************************************/
void delay_nus(unsigned int x)	   
{
	unsigned char i;
	while(x--)
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
* Input          : x->��ʱʱ��xms
* Output         : None
* Return         : None
*******************************************************************************/
void delay_nms(unsigned long int x)
{
	while(x--)
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
void VK2C23A_I2CStart( void )
{
  VK2C23A_SCL_H();
  VK2C23A_SDA_H();
  delay_nus(VK2C23A_CLK);
  VK2C23A_SDA_L();
  delay_nus(VK2C23A_CLK);
}
/*******************************************************************************
* Function Name  : I2CStop
* Description    : ʱ���߸�ʱ���������ɵ͵��ߵ����䣬��ʾI2Cֹͣ�ź�
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void VK2C23A_I2CStop( void )
{
  VK2C23A_SCL_H();
  VK2C23A_SDA_L();
  delay_nus(VK2C23A_CLK);
  VK2C23A_SDA_H();
  delay_nus(VK2C23A_CLK);
}
/*******************************************************************************
* Function Name  : I2CSlaveAck
* Description    : I2C�ӻ��豸Ӧ���ѯ
* Input          : None
* Output         : None
* Return         : 0-ok 1-fail
*******************************************************************************/
unsigned char VK2C23A_I2CSlaveAck( void )
{
  unsigned int TimeOut;
  unsigned char RetValue;
	
  VK2C23A_SCL_L();
  delay_nus(VK2C23A_CLK);
  VK2C23A_SCL_H();//��9��sclk������
  TimeOut = 10000;
  while( TimeOut-- > 0 )
  {
    if( VK2C23A_GET_SDA()!=0 )//��ȡack
    {
      RetValue = 1;
    }
    else
    {
      RetValue = 0;
      break;
    }
  } 
	VK2C23A_SCL_L();
  
  return RetValue;
}
/*******************************************************************************
* Function Name  : I2CSendAck
* Description    : I2C��������ACK
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void VK2C23A_I2CSendAck( void )
{
  VK2C23A_SCL_L();
  VK2C23A_SDA_L();
  delay_nus(VK2C23A_CLK*2);
  VK2C23A_SCL_H();
  delay_nus(VK2C23A_CLK*2);
  VK2C23A_SCL_L();
  VK2C23A_SDA_H();
}
/*******************************************************************************
* Function Name  : I2CSendNAck
* Description    : I2C��������NACK
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void VK2C23A_I2CSendNAck( void )
{
  VK2C23A_SCL_L();
  VK2C23A_SDA_H();
  delay_nus(VK2C23A_CLK);
  VK2C23A_SCL_H();
  delay_nus(VK2C23A_CLK);
}
/*******************************************************************************
* Function Name  : I2CWriteByte
* Description    : I2Cдһ�ֽ�,���͸�λ
* Input          : byte-Ҫд�������
* Output         : None
* Return         : None
*******************************************************************************/
void VK2C23A_I2CWriteByte( unsigned char byte )
{
	unsigned char i=8;
	while (i--)
	{ 
		VK2C23A_SCL_L();
		if(byte&0x80)
			VK2C23A_SDA_H();
		else
			VK2C23A_SDA_L();
		byte<<=1; 
		delay_nus(VK2C23A_CLK);
		VK2C23A_SCL_H();     
		delay_nus(VK2C23A_CLK);
	}
}

/*******************************************************************************
* Function Name  : WriteCmd
* Description    : д1�ֽ����������
* Input          : cmd-Ҫд�������
*                : data-Ҫд��Ĳ���
* Output         : None
* Return         : 0-ok 1-fail
*******************************************************************************/
unsigned char WriteCmdVK2C23A(unsigned char cmd, unsigned char Data )
{
	VK2C23A_I2CStart();

	VK2C23A_I2CWriteByte( VK2C23A_ADDR );
	if( VK2C23A_I2CSlaveAck() == 1 )
	{
		VK2C23A_I2CStop(); 
		return 0;   
	}
	VK2C23A_I2CWriteByte( cmd );
	if(VK2C23A_I2CSlaveAck() ==1  )
	{
		VK2C23A_I2CStop(); 
		return 0;   
	}
	VK2C23A_I2CWriteByte( Data );
	if(VK2C23A_I2CSlaveAck() == 1 )
	{
		VK2C23A_I2CStop(); 
		return 0;   
	}
  VK2C23A_I2CStop();
 
  return 0;    //���ز����ɰܱ�־
}
/*******************************************************************************
* Function Name  : I2CRDDatByte
* Description    : I2C��һ�ֽ�����,�������͵�λ
* Input          : None
* Output         : None
* Return         : ������1�ֽ�����
*******************************************************************************/
unsigned char VK2C23A_I2CRDDat( void )
{
 unsigned char i,RetValue,Bit;
	
//RetValue=0;	
  for( i=0; i<8; i++ )
  {
	VK2C23A_SCL_H(); 
    delay_nus(VK2C23A_CLK);
    if(VK2C23A_GET_SDA() == 1 )
      Bit = 0X01;
    else
      Bit = 0x00;
      
    RetValue = (RetValue<<1)|Bit;
    VK2C23A_SCL_L();
    delay_nus(VK2C23A_CLK);
	}
  
  return RetValue;
}


/*******************************************************************************
* Function Name  : Write1Data
* Description    : д1�ֽ����ݵ���ʾRAM
* Input          : Addr-д��ram�ĵ�ַ
*                : Dat->д��ram������
* Output         : None
* Return         : 0-ok 1-fail
*******************************************************************************/
unsigned char Write1DataVK2C23A(unsigned char Addr,unsigned char Dat)
{
	//START �ź�
	VK2C23A_I2CStart(); 
	//SLAVE��ַ
	VK2C23A_I2CWriteByte(VK2C23A_ADDR);  
	if( VK2C23A_I2CSlaveAck() == 1 )
	{		
		VK2C23A_I2CStop();
		return 1; 
	}	
	//д��ʾRAM����
	VK2C23A_I2CWriteByte(VK2C23A_RWRAM); 						
	if(VK2C23A_I2CSlaveAck() == 1 )
	{
		VK2C23A_I2CStop();													
		return 0;
	}	
	//��ʾRAM��ַ
	VK2C23A_I2CWriteByte(Addr); 
	if(VK2C23A_I2CSlaveAck() == 1 )
	{
		VK2C23A_I2CStop();
		return 1;
	}
	//��ʾ����,1�ֽ����ݰ���2��SEG
	VK2C23A_I2CWriteByte(Dat);
	if( VK2C23A_I2CSlaveAck()==1 )
	{
		VK2C23A_I2CStop();
		return 1;
	}
	
 //STOP�ź�
 VK2C23A_I2CStop();
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
unsigned char  WritenDataVK2C23A(unsigned char Addr,unsigned char *Databuf,unsigned char Cnt)
{
	unsigned char i;
	
	//START�ź�	
	VK2C23A_I2CStart(); 									
	//SLAVE��ַ
	VK2C23A_I2CWriteByte(VK2C23A_ADDR); 	
	if(VK2C23A_I2CSlaveAck() == 1 )
	{
		VK2C23A_I2CStop();													
		return 0;										
	}
	//д��ʾRAM����
	VK2C23A_I2CWriteByte(VK2C23A_RWRAM); 						
	if(VK2C23A_I2CSlaveAck() == 1 )
	{
		VK2C23A_I2CStop();													
		return 0;
	}
	//��ʾRAM��ʼ��ַ
	VK2C23A_I2CWriteByte(Addr); 						
	if(VK2C23A_I2CSlaveAck() == 1 )
	{
		VK2C23A_I2CStop();													
		return 0;
	}
	//����Cnt�����ݵ���ʾRAM
	for(i=0;i<Cnt;i++)
	{ 
		VK2C23A_I2CWriteByte(*Databuf++);
		if( VK2C23A_I2CSlaveAck()==1 )
		{
			VK2C23A_I2CStop();													
			return 0;
		}
	}
	//STOP�ź�
	 VK2C23A_I2CStop();											
	 return 0;    
}
/*******************************************************************************
* Function Name  : ReadnData
* Description    : ����ʾRAM���������
* Input          : Addr-��ram����ʼ��ַ
*                : Databuf->��ram������bufferָ��
*                : Cnt->��ram�����ݸ���
* Output         : None
* Return         : 0-ok 1-fail
*******************************************************************************/
unsigned char  ReadnDataVK2C23A(unsigned char Addr,unsigned char *Databuf,unsigned char Cnt)
{
	unsigned char n;
	
	//START�ź�	
	VK2C23A_I2CStart(); 									
	//SLAVE��ַд
	VK2C23A_I2CWriteByte(VK2C23A_ADDR); 	
	if( 1 == VK2C23A_I2CSlaveAck() )
	{
		VK2C23A_I2CStop();													
		return 0;										
	}       
	//д��ʾRAM����
	VK2C23A_I2CWriteByte(VK2C23A_RWRAM); 	
	if( 1 == VK2C23A_I2CSlaveAck() )
	{
		VK2C23A_I2CStop();													
		return 0;										
	}   
	//��ʾRAM��ʼ��ַ
	VK2C23A_I2CWriteByte(Addr); 						
	if( 1 == VK2C23A_I2CSlaveAck() )
	{
		VK2C23A_I2CStop();													
		return 0;
	}
	
	//STOP�ź�
	VK2C23A_I2CStop();	
	
	//START�ź�	
	VK2C23A_I2CStart(); 									
	//SLAVE��ַ��
	VK2C23A_I2CWriteByte(VK2C23A_ADDRRD); 	
	if( 1 == VK2C23A_I2CSlaveAck() )
	{
		VK2C23A_I2CStop();													
		return 0;										
	}  
	//��Cnt�����ݵ���ʾRAM
	for(n=0;n<Cnt-1;n++)
	{ 
		*Databuf++=VK2C23A_I2CRDDat();
		VK2C23A_I2CSendAck();
	}
	*Databuf++=VK2C23A_I2CRDDat();
	VK2C23A_I2CSendNAck();
	//STOP�ź�
	VK2C23A_I2CStop();											
	 return 0;    
}
/*******************************************************************************
* Function Name  : VK2C23A_DisAll
* Description    : ����SEG��ʾͬһ�����ݣ�bit7/bit3-COM3 bit6/bit2-COM2 bit5/bit1-COM1 bit4/bit0-COM0
* 				 : ���磺0xffȫ�� 0x00ȫ�� 0x55�������� 0xaa�������� 0x33�������� 
* Input          ��dat->д��ram������(1���ֽ����ݶ�Ӧ2��SEG)  
* Output         : None
* Return         : None
*******************************************************************************/
void VK2C23A_DisAll(unsigned char dat)
{
	unsigned char segi;
	
	if(VK2C23A_maxcom==4)
	{
		for(segi=0;segi<28;segi++)
		{
			VK2C23A_dispram[segi]=dat;
		}
		WritenDataVK2C23A(0,VK2C23A_dispram,28);//������8bit���ݶ�Ӧ2��SEG��ÿ8bit���ݵ�ַ��1��ÿ8λ����1��ACK
	}
	else
	{
		for(segi=0;segi<52;segi++)
		{
			VK2C23A_dispram[segi]=dat;
		}
		
		WritenDataVK2C23A(0,VK2C23A_dispram,52);//������8bit���ݶ�Ӧ1��SEG��ÿ8bit���ݵ�ַ��1��ÿ8λ����1��ACK
	}
}
/*******************************************************************************
* Function Name  : DisSegComOn
* Description    : ����1����(1��seg��1��com�����Ӧ����ʾ��)
* Input          ��seg->���Ӧ��seg��  
* 		         ��com->���Ӧcom��  
* Output         : None
* Return         : None
*******************************************************************************/
void VK2C23A_DisDotOn(unsigned char seg,unsigned char com)
{
	unsigned char addrbit,tempdat;
	if(VK2C23A_maxcom==4)
	{
		if(seg%2==0)
			addrbit=(1<<com);//������8λ���ݵ�4bit��Ч��ÿ8bit���ݵ�ַ��1��ÿ8λ����1��ACK
		else
			addrbit=(1<<(4+com));//������8λ���ݸ�4bit��Ч��ÿ8bit���ݵ�ַ��1��ÿ8λ����1��ACK
		tempdat=VK2C23A_dispram[seg/2]|addrbit;
	  VK2C23A_dispram[seg/2]=tempdat;
		WritenDataVK2C23A(seg/2,&tempdat,1);
	}
	else
	{
		addrbit=(1<<com);
		tempdat=VK2C23A_dispram[seg]|addrbit;
		VK2C23A_dispram[seg]=tempdat;
		WritenDataVK2C23A(seg,&tempdat,1);//������8λ���ݵ�4bit��Ч��ÿ8bit���ݵ�ַ��1��ÿ8λ����1��ACK
	}
}
/*******************************************************************************
* Function Name  : DisSegComOff
* Description    : �ر�1����(1��seg��1��com�����Ӧ����ʾ��)
* Input          ��seg->���Ӧ��seg��  
* 		         ��com->���Ӧcom��  
* Output         : None
* Return         : None
*******************************************************************************/
void VK2C23A_DisDotOff(unsigned char seg,unsigned char com)
{
	unsigned char addrbit,tempdat;
	if(VK2C23A_maxcom==4)
	{
		if(seg%2==0)
			addrbit=(1<<com);//������8λ���ݵ�4bit��Ч��ÿ8bit���ݵ�ַ��1��ÿ8λ����1��ACK
		else
			addrbit=(1<<(4+com));//������8λ���ݸ�4bit��Ч��ÿ8bit���ݵ�ַ��1��ÿ8λ����1��ACK
		tempdat=VK2C23A_dispram[seg/2]&~addrbit;
	  VK2C23A_dispram[seg/2]=tempdat;
		WritenDataVK2C23A(seg/2,&tempdat,1);
	}
	else
	{
		addrbit=(1<<com);
		tempdat=VK2C23A_dispram[seg]&~addrbit;
		VK2C23A_dispram[seg]=tempdat;
		WritenDataVK2C23A(seg,&tempdat,1);
	}
}
/*******************************************************************************
* Function Name  : Enter_Standby
* Description    : �������͹���ģʽ,��������ʾ
* Input          ��None 
* Output         : None
* Return         : 0-ok 1-fail
*******************************************************************************/
void VK2C23A_Enter_PowerOff(void)
{		
	WriteCmdVK2C23A(VK2C23A_SYSSET,SYSOFF_LCDOFF);
}
/*******************************************************************************
* Function Name  : Exit_Standby
* Description    : �˳�����͹���ģʽ
* Input          ��None 
* Output         : None
* Return         : None
*******************************************************************************/
void VK2C23A_Exit_PowerOff(void)
{	
	WriteCmdVK2C23A(VK2C23A_SYSSET,SYSON_LCDON);
}

/*******************************************************************************
* Function Name  : Lowlevel_Init
* Description    : ����ͨ����GPIO
* Input          ��None 
* Output         : None
* Return         : None
*******************************************************************************/
void VK2C23A_Lowlevel_Init(void)
{
	//ͨ���ߵ�ƽ��ͬ������ӵ�ƽת����·
	//�˺������ݿͻ���Ƭ������Ӧ���޸�	
//	GPIO_SetMode(VK2C23A_SCL_PORT, VK2C23A_SCL_PIN, GPIO_MODE_OUTPUT);
//	GPIO_SetMode(VK2C23A_SDA_PORT, VK2C23A_SDA_PIN, GPIO_MODE_QUASI);
		      
	VK2C23A_SCL_H();  
	VK2C23A_SDA_H(); 	
}
/*******************************************************************************
* Function Name  : Init
* Description    : ��ʼ������
* Input          ��None 
* Output         : None
* Return         : None
*******************************************************************************/
void VK2C23A_InitSequence(void)
{	
	//�ܽ����ø��ݿͻ���Ƭ������Ӧ���޸�
	VK2C23A_Lowlevel_Init();
//	WriteCmdVK2C23A(VK2C23A_MODESET,CCOM_1_3__4); 	//ģʽ����  1/3 Bais 1/4 Duty
//	WriteCmdVK2C23A(VK2C23A_MODESET,CCOM_1_4__4); //ģʽ����  1/4 Bais 1/4 Duty
//	VK2C23A_maxcom=4;
//	WriteCmdVK2C23A(VK2C23A_MODESET,CCOM_1_3__8); //ģʽ����  1/3 Bais 1/8 Duty
	WriteCmdVK2C23A(VK2C23A_MODESET,CCOM_1_4__8); //ģʽ����  1/4 Bais 1/8 Duty
	VK2C23A_maxcom=8;
	WriteCmdVK2C23A(VK2C23A_SYSSET,SYSON_LCDON); 		//�ڲ�ϵͳ��������lcd����ʾ
	WriteCmdVK2C23A(VK2C23A_FRAMESET,FRAME_80HZ); 	//֡Ƶ��80Hz
//	WriteCmdVK2C23A(VK2C23A_FRAMESET,FRAME_160HZ); //֡Ƶ��160Hz
	WriteCmdVK2C23A(VK2C23A_BLINKSET,BLINK_OFF); 		//��˸�ر�	
//	WriteCmdVK2C23A(VK2C23A_BLINKSET,BLINK_2HZ); 		//��˸2HZ
//	WriteCmdVK2C23A(VK2C23A_BLINKSET,BLINK_1HZ); 		//��˸1HZ
//	WriteCmdVK2C23A(VK2C23A_BLINKSET,BLINK_0D5HZ); 	//��˸0.5HZ
	//SEG/VLCD���ý���ΪVLCD���ڲ���ѹ�������ܹر�,VLCD��VDD�̽�VR=0ƫ�õ�ѹ=VDD
//	WriteCmdVK2C23A(VK2C23A_IVASET,VLCDSEL_IVAOFF_R0); 
	//SEG/VLCD���ý���ΪVLCD���ڲ���ѹ�������ܹر�,VLCD��VDD���ӵ���VR>0ƫ�õ�ѹ=VLCD
//	WriteCmdVK2C23A(VK2C23A_IVASET,VLCDSEL_IVAOFF_R1); 
	//SEG/VLCD���ý���ΪSEG���ڲ�ƫ�õ�ѹ������1/3bias=0.652VDD 1/4bias=0.714VDD
	WriteCmdVK2C23A(VK2C23A_IVASET,SEGSEL_IVA00H);	
}
/*******************************************************************************
* Function Name  : Init
* Description    : ��ʼ������
* Input          ��None 
* Output         : None
* Return         : None
*******************************************************************************/
void VK2C23A_Init(void)
{	
	//�ܽ����ø��ݿͻ���Ƭ������Ӧ���޸�
	VK2C23A_Lowlevel_Init();
	//��ʼ��ʱ��
	VK2C23A_InitSequence();
}

unsigned char data shuzi_zimo[15]= //���ֺ��ַ���ģ
{             
// 0    1    2    3    4    5    6    7    8    9      
  0xEE,0x48,0xD6,0xDA,0x78,0XBA,0XBE,0XC8,0XFE,0XF8};
void DisplayDigitOrChar(unsigned char pos, unsigned char index) 
{
	unsigned char  Data;
    // ��������Ƿ������鷶Χ��
    if (index >= sizeof(shuzi_zimo)) 
		return;

    // ��ȡ��ģ����
    Data = shuzi_zimo[index];

    // ʹ�� Write1DataVk2c22 ������д����ʾ RAM ��ָ��λ��
    Write1DataVK2C23A(pos, Data);
}


unsigned char data shuzi_zimo1[15]= //���ֺ��ַ���ģ
{             
// 0    1    2    3    4    5    6    7    8    9      
  0x77,0x12,0x6B,0x5B,0x1E,0X5D,0X7D,0X13,0X7F,0X1F};
void DisplayDigitOrChar1(unsigned char pos, unsigned char index) 
{
	unsigned char  Data;
    // ��������Ƿ������鷶Χ��
    if (index >= sizeof(shuzi_zimo1)) 
		return;

    // ��ȡ��ģ����
    Data = shuzi_zimo1[index];

    // ʹ�� Write1DataVk2c22 ������д����ʾ RAM ��ָ��λ��
    Write1DataVK2C23A(pos, Data);
}