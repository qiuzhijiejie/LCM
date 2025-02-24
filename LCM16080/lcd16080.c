#include <REGX52.H>
#include <INTRINS.H>
#include "lcd16080.h"
#include "picture.h"

// LCD �������Ŷ���
sbit LCM_CD     = P3^0;   // ����/����ѡ������ (1: ����, 0: ����)
sbit LCM_FS     = P3^5;   // ����ѡ�� (1: 6x8, 0: 8x8)
sbit LCM_RESET  = P3^4;   // ��λ����
sbit LCM_CE     = P3^3;   // оƬʹ�� (�͵�ƽ��Ч)
sbit LCM_WR     = P3^1;   // д������ (�͵�ƽд����)
sbit LCM_RD     = P3^2;   // ��ȡ���� (�͵�ƽ������)
sbit KEY1       = P2^0;
bit CharImageReverse; 
#define LCM_PORT P1
unsigned char data Y; 
unsigned char data X; 
#define XDOTS   160                     //ͼ�οռ�X�����С
#define YDOTS   80                      //ͼ�οռ�X�����С
#define ENGLISHCHARNUMBER  8       //�����Ӣ���ַ����е�Ӣ���ַ��ĸ���
#define CHINESECHARNUMBER  11      //����������ַ����е������ַ��ĸ���
#define ENGLISHCHARSIZE    8       //Ӣ���ַ�X������ʾ��ĸ���
#define CHINESECHARSIZE    16      //�����ַ�X������Ӣ���ַ�Y������ʾ��ĸ���
#define ENGLISHCHARDOTSIZE 16      //����Ӣ���ַ�������ֽ���
#define CHINESECHARDOTSIZE 32      //���������ַ�������ֽ���

void Delay(unsigned char xms)	//@12.000MHz
{
	while(xms--){
	unsigned char data i, j;

	i = 12;
	j = 169;
	do
	{
		while (--j);
	} while (--i);
}
}

unsigned int ReadKey1() // ������⺯��
{
    if (KEY1 == 1)           
	{  // ����������
        Delay(1);  // �򵥵�ȥ��������
        if (KEY1 == 1) 
		{  // ��鰴���Ƿ���Ȼ����
            while (KEY1 == 1);  // �ȴ������ͷ�
            return 1;  // ���ذ���������
        }
    }
    return 0;  // ����û�б�����
}



/**
 * @brief  �ȴ�LCD����������
 * @param  None
 * @retval None
 */
void lcdwaitBusy(void)// 6-5-1
{
	unsigned char i ,status;
	LCM_PORT=0xff;
	
	LCM_CD=1;
	LCM_CE=0;
	for(i=0;i<20;i++)
	{
		LCM_RD=0;
		_nop_();
		status=LCM_PORT;
		LCM_RD=1;
		if((status&0x3)==0x3)  //STAO,STA1����λΪ1. Table6-4
		break;	
	}
	LCM_CE=1;
}

/**
 * @brief  �������ݵ�LCD
 * @param  data: Ҫ���͵������ֽ�
 * @retval None
 */
void lcdWriteData(unsigned char Data)  //< Table 5-1 >
{   
    lcdwaitBusy();          // ���LCD�������Ƿ����
    LCM_PORT = Data;        // ������д��P1�˿�
    LCM_CD = 0;             // CD=0 ��ʾ��������
    LCM_CE = 0;             // ����LCD
    LCM_WR = 0;             // ��ʼд��
    _nop_();                // �ӳ�
    LCM_WR = 1;             // ����д��
    LCM_CE = 1;             // ȡ��LCDѡ��
}


/**
 * @brief  ��LCD��ȡ����
 * @param  None
 * @retval ��ȡ�������ֽ�
 */
unsigned char lcdReadData(void)//< Table 5-1 >         
{   
    unsigned char Data;

    lcdwaitBusy();         // ���LCD�������Ƿ����

    LCM_PORT = 0xFF;       // ����P1Ϊ����ģʽ

    LCM_CD = 0;            // ����Ϊ���ݶ�ȡģʽ
    LCM_CE = 0;            // ����LCD
    LCM_RD = 0;            // ��ʼ��ȡ����
    _nop_();               // �ӳ�

    Data = LCM_PORT;       // ��P1��ȡ����

    LCM_RD = 1;            // ������ȡ
    LCM_CE = 1;            // ȡ��LCDѡ��
    return Data;           // ���ض�ȡ������
}

/**
 * @brief  ����2�ֽ�ָ�LCD
 * @param  command: Ҫ���͵�ָ����
 * @param  param1: ָ��ĵ�1������
 * @param  param2: ָ��ĵ�2������
 * @retval None
 */
void lcdWriteCommand2(unsigned char command, unsigned char param1, unsigned char param2) // ����3�ֽ�ָ��
{   
    lcdWriteData(param1);          // ���͵�1��ָ�����
    lcdWriteData(param2);          // ���͵�2��ָ�����
    lcdWriteCommand(command);      // ����ָ����
}

/**
 * @brief  ����1�ֽ�ָ�LCD
 * @param  command: Ҫ���͵�ָ����
 * @param  param1: ָ��ĵ�1������
 * @retval None
 */
void lcdWriteCommand1(unsigned char command, unsigned char param) // ����2�ֽ�ָ��
{   
    lcdWriteData(param);        // ���͵�1��ָ�����
    lcdWriteCommand(command);   // ����ָ����
}



/**
 * @brief  ����ָ�LCD
 * @param  command: Ҫ���͵�ָ���ֽ�
 * @retval None
 */
void lcdWriteCommand(unsigned char command)
{   
    lcdwaitBusy();         // ���LCD�������Ƿ����	 
    LCM_PORT = command;    // ��ָ���ֽ�д�����ݶ˿�
    LCM_CD = 1;            // ����Ϊ����ģʽ (CD=0)
    LCM_CE = 0;            // ����LCD (CE=0)
    LCM_WR = 0;            // ��ʼд�� (WR=0)
    _nop_();               // �����ӳ�
    LCM_WR = 1;            // ����д�� (WR=1)
    LCM_CE = 1;            // ȡ��LCDѡ�� (CE=1)
}


/**
 * @brief  ��LCD��ȡͼ������
 * @retval ��ȡ����ͼ������
 */
unsigned char lcdReadGraphicData(void)
{
    lcdWriteCommand(0xC1);   //  (0xC1) Table6-19
	//Data Read and Increment ADP
    return lcdReadData();    // ��ȡ���ݲ�����
}

/**
 * @brief  ����ͼ�����ݵ�LCD
 * @param  data: Ҫ���͵�ͼ������
 * @retval None
 */
void lcdWriteGraphicData(unsigned char Data)
{
    lcdWriteCommand1(0xC0, Data);//  Table6-19
}   //Data Write and Increment ADP



/**
 * @brief  ��ʼ��Һ����ʾ������
 * @retval None
 */
void lcdInitialize(void)
{
    lcdWriteCommand(0x80);      // ����Ϊ OR ģʽ 6-8
//	lcdWriteCommand(0x81);      // ����Ϊ EXOR ģʽ
//	lcdWriteCommand(0x83);      // ����Ϊ AND ģʽ
//	lcdWriteCommand(0x84);      // ����Ϊ�ڲ��ַ�������ģʽģʽ
//	lcdWriteCommand(0x90);      // ����Ϊ�ⲿ�ַ�������ģʽģʽ

	
    lcdWriteCommand(0x98);       // ����ͼ��ģʽ 6-9
//	lcdWriteCommand(0x90);     	 //Display off 
//	lcdWriteCommand(0x92);		 //Cursor on, Blink off 
//	lcdWriteCommand(0x93);		 //Cursor on, Blink on 
//	lcdWriteCommand(0x94);		 //Text on, Graphic off
//	lcdWriteCommand(0x9c);		 //Text on, Graphic on

	
//	lcdWriteCommand2(0x40, 0, 0); // �����ı���ʾ���׵�ַΪ 0  6-7 
//  lcdWriteCommand2(0x41, 20, 0); // �����ı���ʾ�����Ϊ 20 �ֽڣ��� 160 ��
//  lcdWriteCommand2(0x41, 26, 0); // �����ı���ʾ�����Ϊ 26 �ֽڣ��� 160 ��
	
    lcdWriteCommand2(0x42, 0, 0); // ����ͼ����ʾ���׵�ַΪ 0  6-7 
    lcdWriteCommand2(0x43, 30, 0); // ����ͼ����ʾ�����Ϊ 30 �ֽڣ��� 160 ��
}



/**
 * @brief  �����ڲ� RAM ��ַ������� (XPOS, YPOS)
 * @retval None
 */
void lcdSetPosition(void)
{
    unsigned int currentAddress;
    
    // ���㵱ǰ��ַ: LCDRAM ��ַ = 30 * YPOS + (XPOS / 8)
    currentAddress = Y * 30;           // ÿ�� 30 �ֽ�
    currentAddress += (X / 8);         // ��� X �����ƫ����
 
    // ���������ַ                     ��ȡ��8λ              ��ȡ��8λ
    lcdWriteCommand2(0x24, currentAddress & 0xFF, currentAddress / 256); // ���͵��ֽں͸��ֽ�
}           //0x24-Set Address drawPointer





/**
 * @brief ������� LCD ��ʾ��Ϊָ�����ֽ�����
 * @param data Ҫ��ʾ���ֽ����ݣ���ΧΪ 0x00 �� 0xFF��
 */
void lcdFill(unsigned char Data)         // ������ʾָ���ֽ����ݵ��ӳ���
{
    unsigned char row, column;           // �к��е�ѭ������

    // �����ڲ� RAM ��ַΪ 0
    lcdWriteCommand2(0x24, 0, 0);                  

    // ѭ������ÿһ��
    for(row = 0; row < 80; row++)        // 80 ��
    
        // ѭ������ÿһ��
        for(column = 0; column < 30; column++) // 30 ���ֽ�����
        
            lcdWriteGraphicData(Data);                   // ��ͼ������
        
    
}


/**
 * @brief ����һ������
 * 
 * �˺����� LCD ��Ļ�ϻ���һ�����Σ����ε����ϽǺ����½�����
 * �ɲ��� (x0, y0) �� (x1, y1) ָ�������εı߿��������߶���ɣ�
 * ������״̬�ɲ��� b ���ơ�
 * 
 * @param x0 �������Ͻǵ� X ����
 * @param y0 �������Ͻǵ� Y ����
 * @param x1 �������½ǵ� X ����
 * @param y1 �������½ǵ� Y ����
 * @param b ������״̬��ͨ��Ϊ 0 �� 1����ʾ��ͬ����ʾ״̬
 */
void rect(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, bit b)
{
    line(x0, y0, x1, y0, b); // �����ϱ߽�
    line(x1, y0, x1, y1, b); // �����ұ߽�
    line(x1, y1, x0, y1, b); // �����±߽�
    line(x0, y1, x0, y0, b); // ������߽�
}



/**
 * @brief ���ƴӵ�ǰ���굽ָ�������ֱ��
 * 
 * �˺����� LCD ��Ļ�ϻ���һ��ֱ�ߣ��ӵ�ǰ���� (XPOS, YPOS)
 * ��Ŀ������ (x1, y1)��������״̬�ɲ��� b ���ơ�
 * 
 * @param x1 Ŀ������� X ����
 * @param y1 Ŀ������� Y ����
 * @param b ������״̬��ͨ��Ϊ 0 �� 1����ʾ��ͬ����ʾ״̬
 */
void lineto(unsigned char x1, unsigned char y1, bit b) // ���� (XPOS, YPOS) �� (x1, y1) ��ֱ��
{
    line(X, Y, x1, y1, b); // ���� line ��������ֱ��
}



/**
 * @brief ����һ��ֱ��
 * 
 * �˺���ʹ�� Bresenham �㷨�� LCD ��Ļ�ϻ���һ������� (x0, y0)
 * ���յ� (x1, y1) ��ֱ�ߡ�������״̬�ɲ��� b ���ơ�
 * 
 * @param x0 ���� X ����
 * @param y0 ���� Y ����
 * @param x1 �յ�� X ����
 * @param y1 �յ�� Y ����
 * @param b ������״̬��ͨ��Ϊ 0 �� 1����ʾ��ͬ����ʾ״̬
 */
void line(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, bit b)
{
    unsigned char dx, dy;          // �洢 X �� Y ���������
    unsigned int dk;               // ���ڼ���б��

    X = x0;                     // ������ʼ����
    Y = y0;
    drawdrawPoint(b);                      // ����ʼ����Ƶ�

    dx = (x1 >= x0) ? x1 - x0 : x0 - x1; // ���� X ���������
    dy = (y1 >= y0) ? y1 - y0 : y0 - y1; // ���� Y ���������

    if (dx == dy)                  // б��Ϊ 1 �����
    {
        while (X != x1)         // ѭ��ֱ�������յ�
        {
            if (x1 > x0) X++;   // ���� X ����
            else X--;

            if (y1 > y0) Y++;   // ���� Y ����
            else Y--;

            drawdrawPoint(b);              // ���Ƶ�ǰ��
        }
    }
    else if (dx > dy)              // X ������������ Y ��������
    {
        dk = dy;
        dy = 0;

        while (X != x1)         // ѭ��ֱ�������յ�
        {
            if (x1 > x0) X++;   // ���� X ����
            else X--;

            dy++;
            // ����б�ʼ����µ� Y ����
            if (y1 > y0)
                Y = y0 + (dk * dy + dx / 2) / dx;
            else
                Y = y0 - (dk * dy + dx / 2) / dx;

            drawdrawPoint(b);              // ���Ƶ�ǰ��
        }
    }
    else                            // Y ������������ X ��������
    {
        dk = dx;
        dx = 0;

        while (Y != y1)         // ѭ��ֱ�������յ�
        {
            if (y1 > y0) Y++;   // ���� Y ����
            else Y--;

            dx++;
            // ����б�ʼ����µ� X ����
            if (x1 > x0)
                X = x0 + (dk * dx + dy / 2) / dy;
            else
                X = x0 - (dk * dx + dy / 2) / dy;

            drawdrawPoint(b);              // ���Ƶ�ǰ��
        }
    }
}

/**
 * @brief �ڵ�ǰ��������һ�����ء�
 * 
 * @param isEnabled ����״̬��1 ��ʾ������0 ��ʾϨ��
 */
void drawdrawPoint(bit isEnabled)
{
    unsigned char bitPosition, lcdMask, currentData;

    // ���ó�ʼ����Ϊ���λ
    lcdMask = 0x80;//OR Mode

    // ���㵱ǰ�������ֽ��е�λ��
    bitPosition = X & 0x07; // XPOS�ĵ���λ��ʾλ�� (0-7)
    
    // ���������Ƶ���Ӧλ��
    while (bitPosition > 0)
    {
        lcdMask >>= 1; // ��������
        bitPosition--;
    }

    // ���õ�ǰ�����
    lcdSetPosition(); 
    currentData = lcdReadGraphicData(); // ��ȡ��ǰ�ֽ�����

    // ���õ�ǰ����㣨���ܻ���ݲ�ͬ��Ӳ����Ҫ������һ�Σ�
    lcdSetPosition(); 

    // ����״̬���µ�ǰ�ֽ�
    if (isEnabled) {
        lcdWriteGraphicData(currentData | lcdMask); // ��������
    } else {
        lcdWriteGraphicData(currentData & (~lcdMask)); // Ϩ������
    }
}



/**
 * @brief ��ʾͼ���ӳ���
 * @param s ָ��ͼ�����ݵ�ָ�룬ͼ��ĵ�һ���ֽڱ�ʾ��ȣ��ڶ����ֽڱ�ʾ�߶ȣ���������������
 */
void displayImage(unsigned char code *imageData)
{
    unsigned char imageWidth, imageHeight;
    
    // ��ȡͼ��Ŀ��
    imageWidth = *imageData;
    imageData++;  // �ƶ�����һ���ֽ�
    
    // ��ȡͼ��ĸ߶�
    imageHeight = *imageData;
    imageData++;  // �ƶ���ͼ���������ݵ���ʼλ��
    
    // ���ú�����ʾͼ�񣬴��ݿ�ȡ��߶Ⱥ�ͼ����������
    displaySizedImage(imageWidth, imageHeight, imageData);
}



/**
 * @brief ����Ļ����ʾָ����С��ͼ������
 * @param imageWidth ͼ��Ŀ�ȣ����ֽ�Ϊ��λ��
 * @param imageHeight ͼ��ĸ߶�
 * @param imageData ָ��ͼ���������ݵ�ָ��
 */
void displaySizedImage(unsigned char imageWidth, unsigned char imageHeight, unsigned char code *imageData)
{
    unsigned char xOffsetBits, currentX, currentY, byte1, byte2, startX, lcdMask;

    // ������ʼλ�õ�λƫ��
    xOffsetBits = X & 0x07;
    imageWidth += xOffsetBits;
    startX = X;
    imageData--; // �ƶ�����ָ����ƥ�����ʹ��

    // ����ÿ��
    for (currentY = 0; currentY < imageHeight; currentY++, Y++)
    {
        for (X = startX, currentX = xOffsetBits; currentX < imageWidth;)
        {
            unsigned char bitPos;

            // ��ȡ�����ֽڵ�����
            byte1 = *imageData;
            imageData++;
            byte2 = *imageData;

            // ��������˷�����ʾ��ȡ������
            if (CharImageReverse)
            {
                byte1 = ~byte1;
                byte2 = ~byte2;
            }

            // ������������
            for (bitPos = 0; bitPos < xOffsetBits; bitPos++)
            {
                byte2 >>= 1;
                if (byte1 & 0x01)
                    byte2 |= 0x80;
                byte1 >>= 1;
            }

            // ���λ��Ϊ0����ʣ����С��8λ����ֱ��д��
            if ((xOffsetBits == 0) && (imageWidth < currentX + 8))
            {
                lcdSetPosition();
                currentX += 8;
                X += 8;
            }
            else
            {
                // ���򣬶�ȡ��ǰLCD���ݲ���������
                lcdSetPosition();
                byte1 = lcdReadGraphicData();
                lcdSetPosition();
                currentX += 8;
                X += 8;

                lcdMask = 0xff;
                bitPos = imageWidth & 0x07;

                // �����Ȳ���8λ�����
                while (bitPos > 0)
                {
                    lcdMask >>= 1;
                    X--;
                    bitPos--;
                }

                // ������������LCD���ݻ��
                bitPos = 0xff;
                while (imageWidth < currentX)
                {
                    currentX--;
                    X--;
                    bitPos <<= 1;
                }

                lcdMask &= bitPos;
                byte2 &= lcdMask;
                byte2 |= byte1 & (~lcdMask);
            }

            // д��ͼ������
            lcdWriteGraphicData(byte2);
        }

        // �����Ҫ�������ݵ����
        if ((xOffsetBits != 0) && ((imageWidth & 0x07) != 0) && (xOffsetBits >= (imageWidth & 0x07)))
            imageData--;
    }

    // ����X��Y����
    if (XDOTS - startX < imageWidth - xOffsetBits)
        X = 0;
    else
        Y -= imageHeight;
}



/**
 * @brief ��ȡ����Ӣ����ĸ�ַ��ڵ����е�λ��
 * 
 * �ú���ʹ�ö��ֲ��ҷ��� EnglishCode �����в���Ŀ���ַ���λ�ã�
 * �����ض�Ӧ�������ݵ���ʼ��ַָ�롣
 * 
 * @param targetChar Ҫ���ҵ�Ӣ����ĸ�ַ�
 * @return unsigned char* ����ָ����ַ��������ݵ�ָ��
 */
unsigned char code *getEnglishCodePosition(unsigned char targetChar)
{
    unsigned char minIndex = 0, maxIndex = ENGLISHCHARNUMBER - 1;
    unsigned char midIndex, midChar;

    // ʹ�ö��ֲ��ҷ���λ�ַ�
    while (1)
    {
        // ������ҷ�ΧС��2��ֱ�Ӽ��min��maxλ��
        if (maxIndex - minIndex < 2)
        {
            if (EnglishCode[minIndex] == targetChar)
                midIndex = minIndex;
            else if (EnglishCode[maxIndex] == targetChar)
                midIndex = maxIndex;
            else
                midIndex = 0;  // ���δ�ҵ��ַ�������Ĭ��ֵ0
            break;
        }

        // �����м�λ��
        midIndex = (maxIndex + minIndex) / 2;
        midChar = EnglishCode[midIndex];

        // ����ҵ���Ŀ���ַ�
        if (midChar == targetChar)
            break;

        // ����м��ַ���Ŀ���ַ���������벿�ֲ���
        else if (midChar > targetChar)
            maxIndex = midIndex - 1;

        // �������Ұ벿�ֲ���
        else
            minIndex = midIndex + 1;
    }

    // �����ַ��������ݵ�ָ��
    return EnglishCharDot + midIndex * ENGLISHCHARDOTSIZE;
}

/**
 * @brief ��ȡ���������ַ��ڵ����е�λ��
 * 
 * �ú���ʹ�ö��ֲ��ҷ��� ChineseCode �����в���Ŀ���ַ��ı���λ�ã�
 * �����ض�Ӧ�������ݵ���ʼ��ַָ�롣
 * 
 * @param targetChar Ҫ���ҵ������ַ����루2�ֽڣ���
 * @return unsigned char* ����ָ����ַ��������ݵ�ָ��
 */
unsigned char code *getChineseCodePosition(unsigned int targetChar)
{
    unsigned int minIndex = 0, maxIndex = CHINESECHARNUMBER - 1;
    unsigned int midIndex, midChar;

    // ʹ�ö��ֲ��ҷ���λ�ַ�
    while (1)
    {
        // ������ҷ�ΧС��2��ֱ�Ӽ��min��maxλ��
        if (maxIndex - minIndex < 2)
        {
            if (ChineseCode[minIndex] == targetChar)
                midIndex = minIndex;
            else if (ChineseCode[maxIndex] == targetChar)
                midIndex = maxIndex;
            else
                midIndex = 0;  // ���δ�ҵ��ַ�������Ĭ��ֵ0
            break;
        }

        // �����м�λ��
        midIndex = (maxIndex + minIndex) / 2;
        midChar = ChineseCode[midIndex];

        // ����ҵ���Ŀ���ַ�
        if (midChar == targetChar)
            break;

        // ����м��ַ���Ŀ���ַ���������벿�ֲ���
        else if (midChar > targetChar)
            maxIndex = midIndex - 1;

        // �������Ұ벿�ֲ���
        else
            minIndex = midIndex + 1;
    }

    // �����ַ��������ݵ�ָ��
    return ChineseCharDot + midIndex * CHINESECHARDOTSIZE;
}


/**
 * @brief ��ʾ�ַ����������ַ���Ϊ 0 ʱ�˳���
 * @param s ָ���ַ�����ָ�룬�ַ���Ϊ 0 ��ʾ������
 */
void putstr(unsigned char code *s)
{
    unsigned int charCode;
    while (1)
    {
        charCode = *s; // ��ȡ��ǰ�ַ�
        s++; // �ƶ�����һ���ַ�
        
        if (charCode == 0) break; // �������ַ����˳�

        // ����ASCII�ַ� (0-127)
        if (charCode < 128)
        {
            putchar(charCode); // ��ʾ���ֽ��ַ�
        }
        // ����˫�ֽ��ַ� (���ֵ�)
        else
        {
            charCode = charCode * 256 + *s; // ��������ֽڳ��������ַ�����
            s++; // �����ڶ����ֽ�
            putchar(charCode); // ��ʾ˫�ֽ��ַ�
        }
    }
}


/**
 * @brief ����Ļ����ʾһ���ַ���֧��ASCII�ַ��ͺ��֣���
 * @param c Ҫ��ʾ���ַ��ı��루���ֽ�ASCII��˫�ֽں��֣���
 */
void putchar(unsigned int c)
{
    // �ж��Ǻ��ֻ���ASCII�ַ�
    if (c > 128)
    {
        // �����ַ��������ֵĳߴ���ʾ
        displaySizedImage(CHINESECHARSIZE, CHINESECHARSIZE, getChineseCodePosition(c));
    }
    else
    {
        // ASCII�ַ�����Ӣ����ĸ�ĳߴ���ʾ
        displaySizedImage(ENGLISHCHARSIZE, CHINESECHARSIZE, getChineseCodePosition(c));
    }
}

void FillHorizontalStripePattern1(void)
{
    unsigned char x, y;

    for (y = 0; y < 80; y++)  // ����Y��
    {
        Y = y;             // ����ǰ�и�ֵ�� YPOS
        lcdSetPosition();             // ���õ�ǰ�е���ʼλ��

        for (x = 0; x < 30; x++)  // ����X��
        {
            if (y % 2 == 0)  // ż������ʾ������
            {
                lcdWriteGraphicData(0xff);  // ���� 0xAA, �� 10101010
            }
            else  // ��������ʾȫ��
            {
                lcdWriteGraphicData(0x00);  // ���� 0x00��ȫ��
            }
        }
    }
}


void FillHorizontalStripePattern2(void)
{
    unsigned char x, y;

    for (y = 0; y < 80; y++)  // ����Y��
    {
        Y = y;             // ����ǰ�и�ֵ�� YPOS
        lcdSetPosition();             // ���õ�ǰ�е���ʼλ��

        for (x = 0; x < 30; x++)  // ����X��
        {
            if (y % 2 == 0)  // ż������ʾ������
            {
                lcdWriteGraphicData(0x55);  // ���� 0xAA, �� 10101010
            }
            else  // ��������ʾȫ��
            {
                lcdWriteGraphicData(0xaa);  // ���� 0x00��ȫ��
            }
        }
    }
}
void draw_more_nested_rectangles()
{
    // �������Σ��������� 160x80 ����Ļ
    rect(0, 0, 159, 79, 1);   // ��������
    // ������������ÿ����С 5 ���صļ��
    rect(5, 5, 154, 74, 1);   // ������
    rect(10, 10, 149, 69, 1); // �ڶ�������
    rect(15, 15, 144, 64, 1); // ����������
    rect(20, 20, 139, 59, 1); // ���ĸ�����
    rect(25, 25, 134, 54, 1); // ���������
    rect(30, 30, 129, 49, 1); // ����������
    rect(35, 35, 124, 44, 1); // ���߸�����
    rect(40, 40, 119, 38, 1); // �ڰ˸�����
			
}



void circle(unsigned char x_center, unsigned char y_center, unsigned char radius, bit b) {
    int x = 0;          // ��ǰ��� x ����
    int y = radius;     // ��ǰ��� y ����
    int d = 3 - 2 * radius; // ���߲���

    // ����Բ�İ˸��ԳƵ�
    while (x <= y) {
        // ����Բ�İ˸����޵ĵ�
        line(x_center + x, y_center + y, x_center + x, y_center + y, b); // ��1����
        line(x_center + y, y_center + x, x_center + y, y_center + x, b); // ��2����
        line(x_center - x, y_center + y, x_center - x, y_center + y, b); // ��3����
        line(x_center - y, y_center + x, x_center - y, y_center + x, b); // ��4����
        line(x_center - x, y_center - y, x_center - x, y_center - y, b); // ��5����
        line(x_center - y, y_center - x, x_center - y, y_center - x, b); // ��6����
        line(x_center + x, y_center - y, x_center + x, y_center - y, b); // ��7����
        line(x_center + y, y_center - x, x_center + y, y_center - x, b); // ��8����

        // ���¾��߲���
        if (d < 0) {
            d += 4 * x + 6;
        } else {
            d += 4 * (x - y) + 10;
            y--;
        }
        x++;
    }
}
unsigned char r;
void drawNestedCircles(unsigned char x_center, unsigned char y_center, unsigned char max_radius, bit b) {
    // ȷ���뾶��Ϊ����
    if (max_radius > 0) {
        for (r = max_radius; r > 0; r -= 5) { // ÿ�μ��ٰ뾶5
            circle(x_center, y_center, r, b); // ����Բ��
        }
    }
}



