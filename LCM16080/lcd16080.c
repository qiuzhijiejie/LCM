#include <REGX52.H>
#include <INTRINS.H>
#include "lcd16080.h"
#include "picture.h"

// LCD 控制引脚定义
sbit LCM_CD     = P3^0;   // 命令/数据选择引脚 (1: 数据, 0: 命令)
sbit LCM_FS     = P3^5;   // 字体选择 (1: 6x8, 0: 8x8)
sbit LCM_RESET  = P3^4;   // 复位引脚
sbit LCM_CE     = P3^3;   // 芯片使能 (低电平有效)
sbit LCM_WR     = P3^1;   // 写入引脚 (低电平写数据)
sbit LCM_RD     = P3^2;   // 读取引脚 (低电平读数据)
sbit KEY1       = P2^0;
bit CharImageReverse; 
#define LCM_PORT P1
unsigned char data Y; 
unsigned char data X; 
#define XDOTS   160                     //图形空间X方向大小
#define YDOTS   80                      //图形空间X方向大小
#define ENGLISHCHARNUMBER  8       //精简版英文字符库中的英文字符的个数
#define CHINESECHARNUMBER  11      //精简版中文字符库中的中文字符的个数
#define ENGLISHCHARSIZE    8       //英文字符X方向显示点的个数
#define CHINESECHARSIZE    16      //中文字符X方向及中英文字符Y方向显示点的个数
#define ENGLISHCHARDOTSIZE 16      //单个英文字符点阵的字节数
#define CHINESECHARDOTSIZE 32      //单个中文字符点阵的字节数

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

unsigned int ReadKey1() // 按键检测函数
{
    if (KEY1 == 1)           
	{  // 按键被按下
        Delay(1);  // 简单的去抖动处理
        if (KEY1 == 1) 
		{  // 检查按键是否仍然按下
            while (KEY1 == 1);  // 等待按键释放
            return 1;  // 返回按键被按下
        }
    }
    return 0;  // 按键没有被按下
}



/**
 * @brief  等待LCD控制器空闲
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
		if((status&0x3)==0x3)  //STAO,STA1地两位为1. Table6-4
		break;	
	}
	LCM_CE=1;
}

/**
 * @brief  发送数据到LCD
 * @param  data: 要发送的数据字节
 * @retval None
 */
void lcdWriteData(unsigned char Data)  //< Table 5-1 >
{   
    lcdwaitBusy();          // 检测LCD控制器是否空闲
    LCM_PORT = Data;        // 将数据写入P1端口
    LCM_CD = 0;             // CD=0 表示发送数据
    LCM_CE = 0;             // 激活LCD
    LCM_WR = 0;             // 开始写入
    _nop_();                // 延迟
    LCM_WR = 1;             // 结束写入
    LCM_CE = 1;             // 取消LCD选择
}


/**
 * @brief  从LCD读取数据
 * @param  None
 * @retval 读取的数据字节
 */
unsigned char lcdReadData(void)//< Table 5-1 >         
{   
    unsigned char Data;

    lcdwaitBusy();         // 检测LCD控制器是否空闲

    LCM_PORT = 0xFF;       // 设置P1为输入模式

    LCM_CD = 0;            // 设置为数据读取模式
    LCM_CE = 0;            // 激活LCD
    LCM_RD = 0;            // 开始读取数据
    _nop_();               // 延迟

    Data = LCM_PORT;       // 从P1读取数据

    LCM_RD = 1;            // 结束读取
    LCM_CE = 1;            // 取消LCD选择
    return Data;           // 返回读取的数据
}

/**
 * @brief  发送2字节指令到LCD
 * @param  command: 要发送的指令码
 * @param  param1: 指令的第1个参数
 * @param  param2: 指令的第2个参数
 * @retval None
 */
void lcdWriteCommand2(unsigned char command, unsigned char param1, unsigned char param2) // 发送3字节指令
{   
    lcdWriteData(param1);          // 发送第1个指令参数
    lcdWriteData(param2);          // 发送第2个指令参数
    lcdWriteCommand(command);      // 发送指令码
}

/**
 * @brief  发送1字节指令到LCD
 * @param  command: 要发送的指令码
 * @param  param1: 指令的第1个参数
 * @retval None
 */
void lcdWriteCommand1(unsigned char command, unsigned char param) // 发送2字节指令
{   
    lcdWriteData(param);        // 发送第1个指令参数
    lcdWriteCommand(command);   // 发送指令码
}



/**
 * @brief  发送指令到LCD
 * @param  command: 要发送的指令字节
 * @retval None
 */
void lcdWriteCommand(unsigned char command)
{   
    lcdwaitBusy();         // 检测LCD控制器是否空闲	 
    LCM_PORT = command;    // 将指令字节写入数据端口
    LCM_CD = 1;            // 设置为命令模式 (CD=0)
    LCM_CE = 0;            // 激活LCD (CE=0)
    LCM_WR = 0;            // 开始写入 (WR=0)
    _nop_();               // 短暂延迟
    LCM_WR = 1;            // 结束写入 (WR=1)
    LCM_CE = 1;            // 取消LCD选择 (CE=1)
}


/**
 * @brief  从LCD读取图形数据
 * @retval 读取到的图形数据
 */
unsigned char lcdReadGraphicData(void)
{
    lcdWriteCommand(0xC1);   //  (0xC1) Table6-19
	//Data Read and Increment ADP
    return lcdReadData();    // 读取数据并返回
}

/**
 * @brief  发送图形数据到LCD
 * @param  data: 要发送的图形数据
 * @retval None
 */
void lcdWriteGraphicData(unsigned char Data)
{
    lcdWriteCommand1(0xC0, Data);//  Table6-19
}   //Data Write and Increment ADP



/**
 * @brief  初始化液晶显示控制器
 * @retval None
 */
void lcdInitialize(void)
{
    lcdWriteCommand(0x80);      // 设置为 OR 模式 6-8
//	lcdWriteCommand(0x81);      // 设置为 EXOR 模式
//	lcdWriteCommand(0x83);      // 设置为 AND 模式
//	lcdWriteCommand(0x84);      // 设置为内部字符生成器模式模式
//	lcdWriteCommand(0x90);      // 设置为外部字符生成器模式模式

	
    lcdWriteCommand(0x98);       // 开启图形模式 6-9
//	lcdWriteCommand(0x90);     	 //Display off 
//	lcdWriteCommand(0x92);		 //Cursor on, Blink off 
//	lcdWriteCommand(0x93);		 //Cursor on, Blink on 
//	lcdWriteCommand(0x94);		 //Text on, Graphic off
//	lcdWriteCommand(0x9c);		 //Text on, Graphic on

	
//	lcdWriteCommand2(0x40, 0, 0); // 设置文本显示区首地址为 0  6-7 
//  lcdWriteCommand2(0x41, 20, 0); // 设置文本显示区宽度为 20 字节，共 160 点
//  lcdWriteCommand2(0x41, 26, 0); // 设置文本显示区宽度为 26 字节，共 160 点
	
    lcdWriteCommand2(0x42, 0, 0); // 设置图形显示区首地址为 0  6-7 
    lcdWriteCommand2(0x43, 30, 0); // 设置图形显示区宽度为 30 字节，共 160 点
}



/**
 * @brief  设置内部 RAM 地址的坐标点 (XPOS, YPOS)
 * @retval None
 */
void lcdSetPosition(void)
{
    unsigned int currentAddress;
    
    // 计算当前地址: LCDRAM 地址 = 30 * YPOS + (XPOS / 8)
    currentAddress = Y * 30;           // 每行 30 字节
    currentAddress += (X / 8);         // 添加 X 坐标的偏移量
 
    // 发送坐标地址                     获取低8位              获取高8位
    lcdWriteCommand2(0x24, currentAddress & 0xFF, currentAddress / 256); // 发送低字节和高字节
}           //0x24-Set Address drawPointer





/**
 * @brief 填充整个 LCD 显示屏为指定的字节数据
 * @param data 要显示的字节数据，范围为 0x00 到 0xFF。
 */
void lcdFill(unsigned char Data)         // 整屏显示指定字节数据的子程序
{
    unsigned char row, column;           // 行和列的循环变量

    // 设置内部 RAM 地址为 0
    lcdWriteCommand2(0x24, 0, 0);                  

    // 循环遍历每一行
    for(row = 0; row < 80; row++)        // 80 行
    
        // 循环遍历每一列
        for(column = 0; column < 30; column++) // 30 个字节数据
        
            lcdWriteGraphicData(Data);                   // 送图形数据
        
    
}


/**
 * @brief 绘制一个矩形
 * 
 * 此函数在 LCD 屏幕上绘制一个矩形，矩形的左上角和右下角坐标
 * 由参数 (x0, y0) 和 (x1, y1) 指定。矩形的边框由四条线段组成，
 * 线条的状态由参数 b 控制。
 * 
 * @param x0 矩形左上角的 X 坐标
 * @param y0 矩形左上角的 Y 坐标
 * @param x1 矩形右下角的 X 坐标
 * @param y1 矩形右下角的 Y 坐标
 * @param b 线条的状态，通常为 0 或 1，表示不同的显示状态
 */
void rect(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, bit b)
{
    line(x0, y0, x1, y0, b); // 绘制上边界
    line(x1, y0, x1, y1, b); // 绘制右边界
    line(x1, y1, x0, y1, b); // 绘制下边界
    line(x0, y1, x0, y0, b); // 绘制左边界
}



/**
 * @brief 绘制从当前坐标到指定坐标的直线
 * 
 * 此函数在 LCD 屏幕上绘制一条直线，从当前坐标 (XPOS, YPOS)
 * 到目标坐标 (x1, y1)。线条的状态由参数 b 控制。
 * 
 * @param x1 目标坐标的 X 坐标
 * @param y1 目标坐标的 Y 坐标
 * @param b 线条的状态，通常为 0 或 1，表示不同的显示状态
 */
void lineto(unsigned char x1, unsigned char y1, bit b) // 绘制 (XPOS, YPOS) 到 (x1, y1) 的直线
{
    line(X, Y, x1, y1, b); // 调用 line 函数绘制直线
}



/**
 * @brief 绘制一条直线
 * 
 * 此函数使用 Bresenham 算法在 LCD 屏幕上绘制一条从起点 (x0, y0)
 * 到终点 (x1, y1) 的直线。线条的状态由参数 b 控制。
 * 
 * @param x0 起点的 X 坐标
 * @param y0 起点的 Y 坐标
 * @param x1 终点的 X 坐标
 * @param y1 终点的 Y 坐标
 * @param b 线条的状态，通常为 0 或 1，表示不同的显示状态
 */
void line(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, bit b)
{
    unsigned char dx, dy;          // 存储 X 和 Y 方向的增量
    unsigned int dk;               // 用于计算斜率

    X = x0;                     // 设置起始坐标
    Y = y0;
    drawdrawPoint(b);                      // 在起始点绘制点

    dx = (x1 >= x0) ? x1 - x0 : x0 - x1; // 计算 X 方向的增量
    dy = (y1 >= y0) ? y1 - y0 : y0 - y1; // 计算 Y 方向的增量

    if (dx == dy)                  // 斜率为 1 的情况
    {
        while (X != x1)         // 循环直到到达终点
        {
            if (x1 > x0) X++;   // 更新 X 坐标
            else X--;

            if (y1 > y0) Y++;   // 更新 Y 坐标
            else Y--;

            drawdrawPoint(b);              // 绘制当前点
        }
    }
    else if (dx > dy)              // X 方向增量大于 Y 方向增量
    {
        dk = dy;
        dy = 0;

        while (X != x1)         // 循环直到到达终点
        {
            if (x1 > x0) X++;   // 更新 X 坐标
            else X--;

            dy++;
            // 根据斜率计算新的 Y 坐标
            if (y1 > y0)
                Y = y0 + (dk * dy + dx / 2) / dx;
            else
                Y = y0 - (dk * dy + dx / 2) / dx;

            drawdrawPoint(b);              // 绘制当前点
        }
    }
    else                            // Y 方向增量大于 X 方向增量
    {
        dk = dx;
        dx = 0;

        while (Y != y1)         // 循环直到到达终点
        {
            if (y1 > y0) Y++;   // 更新 Y 坐标
            else Y--;

            dx++;
            // 根据斜率计算新的 X 坐标
            if (x1 > x0)
                X = x0 + (dk * dx + dy / 2) / dy;
            else
                X = x0 - (dk * dx + dy / 2) / dy;

            drawdrawPoint(b);              // 绘制当前点
        }
    }
}

/**
 * @brief 在当前坐标点绘制一个像素。
 * 
 * @param isEnabled 像素状态：1 表示点亮，0 表示熄灭。
 */
void drawdrawPoint(bit isEnabled)
{
    unsigned char bitPosition, lcdMask, currentData;

    // 设置初始掩码为最高位
    lcdMask = 0x80;//OR Mode

    // 计算当前像素在字节中的位置
    bitPosition = X & 0x07; // XPOS的低三位表示位置 (0-7)
    
    // 将掩码右移到相应位置
    while (bitPosition > 0)
    {
        lcdMask >>= 1; // 右移掩码
        bitPosition--;
    }

    // 设置当前坐标点
    lcdSetPosition(); 
    currentData = lcdReadGraphicData(); // 读取当前字节数据

    // 设置当前坐标点（可能会根据不同的硬件需要再设置一次）
    lcdSetPosition(); 

    // 根据状态更新当前字节
    if (isEnabled) {
        lcdWriteGraphicData(currentData | lcdMask); // 点亮像素
    } else {
        lcdWriteGraphicData(currentData & (~lcdMask)); // 熄灭像素
    }
}



/**
 * @brief 显示图形子程序
 * @param s 指向图像数据的指针，图像的第一个字节表示宽度，第二个字节表示高度，后面是像素数据
 */
void displayImage(unsigned char code *imageData)
{
    unsigned char imageWidth, imageHeight;
    
    // 提取图像的宽度
    imageWidth = *imageData;
    imageData++;  // 移动到下一个字节
    
    // 提取图像的高度
    imageHeight = *imageData;
    imageData++;  // 移动到图像像素数据的起始位置
    
    // 调用函数显示图像，传递宽度、高度和图像像素数据
    displaySizedImage(imageWidth, imageHeight, imageData);
}



/**
 * @brief 在屏幕上显示指定大小的图像数据
 * @param imageWidth 图像的宽度（以字节为单位）
 * @param imageHeight 图像的高度
 * @param imageData 指向图像像素数据的指针
 */
void displaySizedImage(unsigned char imageWidth, unsigned char imageHeight, unsigned char code *imageData)
{
    unsigned char xOffsetBits, currentX, currentY, byte1, byte2, startX, lcdMask;

    // 计算起始位置的位偏移
    xOffsetBits = X & 0x07;
    imageWidth += xOffsetBits;
    startX = X;
    imageData--; // 移动数据指针以匹配后续使用

    // 遍历每行
    for (currentY = 0; currentY < imageHeight; currentY++, Y++)
    {
        for (X = startX, currentX = xOffsetBits; currentX < imageWidth;)
        {
            unsigned char bitPos;

            // 读取两个字节的数据
            byte1 = *imageData;
            imageData++;
            byte2 = *imageData;

            // 如果启用了反向显示，取反数据
            if (CharImageReverse)
            {
                byte1 = ~byte1;
                byte2 = ~byte2;
            }

            // 对齐像素数据
            for (bitPos = 0; bitPos < xOffsetBits; bitPos++)
            {
                byte2 >>= 1;
                if (byte1 & 0x01)
                    byte2 |= 0x80;
                byte1 >>= 1;
            }

            // 如果位移为0，且剩余宽度小于8位，则直接写入
            if ((xOffsetBits == 0) && (imageWidth < currentX + 8))
            {
                lcdSetPosition();
                currentX += 8;
                X += 8;
            }
            else
            {
                // 否则，读取当前LCD内容并更新像素
                lcdSetPosition();
                byte1 = lcdReadGraphicData();
                lcdSetPosition();
                currentX += 8;
                X += 8;

                lcdMask = 0xff;
                bitPos = imageWidth & 0x07;

                // 处理宽度不足8位的情况
                while (bitPos > 0)
                {
                    lcdMask >>= 1;
                    X--;
                    bitPos--;
                }

                // 将像素数据与LCD内容混合
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

            // 写入图形数据
            lcdWriteGraphicData(byte2);
        }

        // 如果需要补齐数据的情况
        if ((xOffsetBits != 0) && ((imageWidth & 0x07) != 0) && (xOffsetBits >= (imageWidth & 0x07)))
            imageData--;
    }

    // 更新X和Y坐标
    if (XDOTS - startX < imageWidth - xOffsetBits)
        X = 0;
    else
        Y -= imageHeight;
}



/**
 * @brief 获取给定英文字母字符在点阵中的位置
 * 
 * 该函数使用二分查找法在 EnglishCode 数组中查找目标字符的位置，
 * 并返回对应点阵数据的起始地址指针。
 * 
 * @param targetChar 要查找的英文字母字符
 * @return unsigned char* 返回指向该字符点阵数据的指针
 */
unsigned char code *getEnglishCodePosition(unsigned char targetChar)
{
    unsigned char minIndex = 0, maxIndex = ENGLISHCHARNUMBER - 1;
    unsigned char midIndex, midChar;

    // 使用二分查找法定位字符
    while (1)
    {
        // 如果查找范围小于2，直接检查min和max位置
        if (maxIndex - minIndex < 2)
        {
            if (EnglishCode[minIndex] == targetChar)
                midIndex = minIndex;
            else if (EnglishCode[maxIndex] == targetChar)
                midIndex = maxIndex;
            else
                midIndex = 0;  // 如果未找到字符，返回默认值0
            break;
        }

        // 计算中间位置
        midIndex = (maxIndex + minIndex) / 2;
        midChar = EnglishCode[midIndex];

        // 如果找到了目标字符
        if (midChar == targetChar)
            break;

        // 如果中间字符比目标字符大，则在左半部分查找
        else if (midChar > targetChar)
            maxIndex = midIndex - 1;

        // 否则在右半部分查找
        else
            minIndex = midIndex + 1;
    }

    // 返回字符点阵数据的指针
    return EnglishCharDot + midIndex * ENGLISHCHARDOTSIZE;
}

/**
 * @brief 获取给定中文字符在点阵中的位置
 * 
 * 该函数使用二分查找法在 ChineseCode 数组中查找目标字符的编码位置，
 * 并返回对应点阵数据的起始地址指针。
 * 
 * @param targetChar 要查找的中文字符编码（2字节）。
 * @return unsigned char* 返回指向该字符点阵数据的指针
 */
unsigned char code *getChineseCodePosition(unsigned int targetChar)
{
    unsigned int minIndex = 0, maxIndex = CHINESECHARNUMBER - 1;
    unsigned int midIndex, midChar;

    // 使用二分查找法定位字符
    while (1)
    {
        // 如果查找范围小于2，直接检查min和max位置
        if (maxIndex - minIndex < 2)
        {
            if (ChineseCode[minIndex] == targetChar)
                midIndex = minIndex;
            else if (ChineseCode[maxIndex] == targetChar)
                midIndex = maxIndex;
            else
                midIndex = 0;  // 如果未找到字符，返回默认值0
            break;
        }

        // 计算中间位置
        midIndex = (maxIndex + minIndex) / 2;
        midChar = ChineseCode[midIndex];

        // 如果找到了目标字符
        if (midChar == targetChar)
            break;

        // 如果中间字符比目标字符大，则在左半部分查找
        else if (midChar > targetChar)
            maxIndex = midIndex - 1;

        // 否则在右半部分查找
        else
            minIndex = midIndex + 1;
    }

    // 返回字符点阵数据的指针
    return ChineseCharDot + midIndex * CHINESECHARDOTSIZE;
}


/**
 * @brief 显示字符串，遇到字符码为 0 时退出。
 * @param s 指向字符串的指针，字符码为 0 表示结束。
 */
void putstr(unsigned char code *s)
{
    unsigned int charCode;
    while (1)
    {
        charCode = *s; // 获取当前字符
        s++; // 移动到下一个字符
        
        if (charCode == 0) break; // 遇到空字符则退出

        // 处理ASCII字符 (0-127)
        if (charCode < 128)
        {
            putchar(charCode); // 显示单字节字符
        }
        // 处理双字节字符 (汉字等)
        else
        {
            charCode = charCode * 256 + *s; // 组合两个字节成完整的字符编码
            s++; // 跳过第二个字节
            putchar(charCode); // 显示双字节字符
        }
    }
}


/**
 * @brief 在屏幕上显示一个字符（支持ASCII字符和汉字）。
 * @param c 要显示的字符的编码（单字节ASCII或双字节汉字）。
 */
void putchar(unsigned int c)
{
    // 判断是汉字还是ASCII字符
    if (c > 128)
    {
        // 汉字字符：按汉字的尺寸显示
        displaySizedImage(CHINESECHARSIZE, CHINESECHARSIZE, getChineseCodePosition(c));
    }
    else
    {
        // ASCII字符：按英文字母的尺寸显示
        displaySizedImage(ENGLISHCHARSIZE, CHINESECHARSIZE, getChineseCodePosition(c));
    }
}

void FillHorizontalStripePattern1(void)
{
    unsigned char x, y;

    for (y = 0; y < 80; y++)  // 遍历Y轴
    {
        Y = y;             // 将当前行赋值给 YPOS
        lcdSetPosition();             // 设置当前行的起始位置

        for (x = 0; x < 30; x++)  // 遍历X轴
        {
            if (y % 2 == 0)  // 偶数行显示交替亮
            {
                lcdWriteGraphicData(0xff);  // 发送 0xAA, 即 10101010
            }
            else  // 奇数行显示全暗
            {
                lcdWriteGraphicData(0x00);  // 发送 0x00，全暗
            }
        }
    }
}


void FillHorizontalStripePattern2(void)
{
    unsigned char x, y;

    for (y = 0; y < 80; y++)  // 遍历Y轴
    {
        Y = y;             // 将当前行赋值给 YPOS
        lcdSetPosition();             // 设置当前行的起始位置

        for (x = 0; x < 30; x++)  // 遍历X轴
        {
            if (y % 2 == 0)  // 偶数行显示交替亮
            {
                lcdWriteGraphicData(0x55);  // 发送 0xAA, 即 10101010
            }
            else  // 奇数行显示全暗
            {
                lcdWriteGraphicData(0xaa);  // 发送 0x00，全暗
            }
        }
    }
}
void draw_more_nested_rectangles()
{
    // 最外层矩形，覆盖整个 160x80 的屏幕
    rect(0, 0, 159, 79, 1);   // 最外层矩形
    // 逐步缩进，保持每次缩小 5 像素的间距
    rect(5, 5, 154, 74, 1);   // 外层矩形
    rect(10, 10, 149, 69, 1); // 第二个矩形
    rect(15, 15, 144, 64, 1); // 第三个矩形
    rect(20, 20, 139, 59, 1); // 第四个矩形
    rect(25, 25, 134, 54, 1); // 第五个矩形
    rect(30, 30, 129, 49, 1); // 第六个矩形
    rect(35, 35, 124, 44, 1); // 第七个矩形
    rect(40, 40, 119, 38, 1); // 第八个矩形
			
}



void circle(unsigned char x_center, unsigned char y_center, unsigned char radius, bit b) {
    int x = 0;          // 当前点的 x 坐标
    int y = radius;     // 当前点的 y 坐标
    int d = 3 - 2 * radius; // 决策参数

    // 绘制圆的八个对称点
    while (x <= y) {
        // 绘制圆的八个象限的点
        line(x_center + x, y_center + y, x_center + x, y_center + y, b); // 第1象限
        line(x_center + y, y_center + x, x_center + y, y_center + x, b); // 第2象限
        line(x_center - x, y_center + y, x_center - x, y_center + y, b); // 第3象限
        line(x_center - y, y_center + x, x_center - y, y_center + x, b); // 第4象限
        line(x_center - x, y_center - y, x_center - x, y_center - y, b); // 第5象限
        line(x_center - y, y_center - x, x_center - y, y_center - x, b); // 第6象限
        line(x_center + x, y_center - y, x_center + x, y_center - y, b); // 第7象限
        line(x_center + y, y_center - x, x_center + y, y_center - x, b); // 第8象限

        // 更新决策参数
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
    // 确保半径不为负数
    if (max_radius > 0) {
        for (r = max_radius; r > 0; r -= 5) { // 每次减少半径5
            circle(x_center, y_center, r, b); // 绘制圆形
        }
    }
}



