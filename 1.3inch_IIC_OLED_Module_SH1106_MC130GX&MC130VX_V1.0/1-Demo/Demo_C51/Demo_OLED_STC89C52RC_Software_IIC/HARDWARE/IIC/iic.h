//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//����Ӳ������Ƭ��STC89C52RC,����12M  ��Ƭ��������ѹ3.3V��5V
//QDtech-OLEDҺ������ for C51
//xiao��@ShenZhen QDtech co.,LTD
//��˾��վ:www.qdtft.com
//�Ա���վ��http://qdtech.taobao.com
//wiki������վ��http://www.lcdwiki.com
//��˾�ṩ����֧�֣��κμ������⻶ӭ��ʱ����ѧϰ
//�̻�(����) :+86 0755-23594567 
//�ֻ�:15989313508���빤�� 
//����:lcdwiki01@gmail.com    support@lcdwiki.com    goodtft@163.com
//����֧��QQ:3002773612  3002778157
//��������QQȺ:324828016
//��������:2018/9/14
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������ȫ�����Ӽ������޹�˾ 2018-2028
//All rights reserved
/****************************************************************************************************
//=========================================��Դ����================================================//
// OLEDģ��                C51��Ƭ��
//   VCC         ��       DC 5V/3.3V      //OLED����Դ��
//   GND         ��          GND          //OLED����Դ��
//=======================================Һ���������߽���==========================================//
//��ģ��Ĭ��������������ΪIIC
// OLEDģ��                C51��Ƭ��
//   SDA        ��           P15          //OLED��SPIд�ź�
//=======================================Һ���������߽���==========================================//
// OLEDģ��                C51��Ƭ��
//   SCL        ��           P17          //OLED��SPIʱ���ź�
//=========================================����������=========================================//
//��ģ�鲻���������ܣ����Բ���Ҫ����������
****************************************************************************************************/	
/***************************************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, QD electronic SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
****************************************************************************************************/
#ifndef _IIC_H_
#define _IIC_H_
#include "sys.h"

//�����Գ���ʹ�õ������ģ��IIC�ӿ�����
//IIC���������Ŷ����ʱ�����Ŷ��嶼���������޸�


//����IIC���豸��ַ
#define IIC_SLAVE_ADDR 0x78


//--------------IIC�������Ŷ���-----------------------
sbit OLED_SDA = P1^5;      //OLED��IIC�����ź� P15
sbit OLED_SCL = P1^7;      //OLED��IICʱ���ź� P17

//--------------IIC�˿ڲ�������---------------------
#define	OLED_SDA_SET()  	OLED_SDA = 1
#define	OLED_SDA_CLR()  	OLED_SDA = 0
  
#define	OLED_SCL_SET()  	OLED_SCL = 1   
#define	OLED_SCL_CLR()  	OLED_SCL = 0   

//IIC��������
void IIC_Start(void);
void IIC_Stop(void);
void IIC_Wait_Ack(void);
void Write_IIC_Byte(u8 IIC_Byte);
void Write_IIC_Command(u8 IIC_Command);
void Write_IIC_Data(u8 IIC_Data);

#endif
