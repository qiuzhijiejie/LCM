#include "stm32c0xx_hal.h"



// 定义引脚
#define LCD_RS_PIN GPIO_PIN_15  // RS 引脚连接到 GPIOC 的第 15 引脚
#define LCD_RS_PORT GPIOC       // RS 引脚所在的 GPIO 端口是 GPIOC

#define LCD_RW_PIN GPIO_PIN_0   // RW 引脚连接到 GPIOC 的第 0 引脚
#define LCD_RW_PORT GPIOC       // RW 引脚所在的 GPIO 端口是 GPIOC

#define LCD_E_PIN  GPIO_PIN_14  // E 引脚连接到 GPIOC 的第 14 引脚
#define LCD_E_PORT GPIOC        // E 引脚所在的 GPIO 端口是 GPIOC

#define LCD_CS1_PIN GPIO_PIN_6  // CS1 引脚连接到 GPIOC 的第 6 引脚
#define LCD_CS1_PORT GPIOC      // CS1 引脚所在的 GPIO 端口是 GPIOC

#define LCD_CS2_PIN GPIO_PIN_2  // CS2 引脚连接到 GPIOA 的第 2 引脚
#define LCD_CS2_PORT GPIOB      // CS2 引脚所在的 GPIO 端口是 GPIOB



// 初始化 GPIO
void GPIO_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // 启用 GPIOC 和 GPIOA 的时钟
    __HAL_RCC_GPIOC_CLK_ENABLE(); 
    __HAL_RCC_GPIOA_CLK_ENABLE();

    // 配置 RS 引脚 (PC15)
    GPIO_InitStruct.Pin = LCD_RS_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LCD_RS_PORT, &GPIO_InitStruct);

    // 配置 RW 引脚 (PA0)
    GPIO_InitStruct.Pin = LCD_RW_PIN;
    HAL_GPIO_Init(LCD_RW_PORT, &GPIO_InitStruct);

    // 配置 E 引脚 (PA14)
    GPIO_InitStruct.Pin = LCD_E_PIN;
    HAL_GPIO_Init(LCD_E_PORT, &GPIO_InitStruct);

    // 配置 CS1 引脚 (PA3)
    GPIO_InitStruct.Pin = LCD_CS1_PIN;
    HAL_GPIO_Init(LCD_CS1_PORT, &GPIO_InitStruct);

    // 配置 CS2 引脚 (PA4)
    GPIO_InitStruct.Pin = LCD_CS2_PIN;
    HAL_GPIO_Init(LCD_CS2_PORT, &GPIO_InitStruct);
}



void Lcd12864_BusyL(void) 
{
    uint8_t temp = 0xFF; // 初始化变量 temp 为 0xFF
	
    HAL_GPIO_WritePin(GPIOC, LCD_CS1_PIN, GPIO_PIN_SET); 	// 使能选择信号 CS1
    HAL_GPIO_WritePin(GPIOB, LCD_CS2_PIN, GPIO_PIN_RESET); 	// 禁用选择信号 CS2
    HAL_GPIO_WritePin(GPIOC, LCD_RS_PIN,  GPIO_PIN_RESET); 	// 设置 RS 端口为 0，选择命令寄存器
    HAL_GPIO_WritePin(GPIOC, LCD_RW_PIN,  GPIO_PIN_SET); 	// 设置 RW 端口为 1，切换为读取模式
    
    // 读取数据引脚
    GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin  = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | 
						   GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;

    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    while ((temp & 0x80) == 0x80) { // 循环直到忙碌标志位被清除
        HAL_GPIO_WritePin(GPIOC, LCD_E_PIN, GPIO_PIN_SET); // 使能 LCD12864（将 E 端口设为高电平）
        HAL_Delay(1); // 等待，确保信号稳定
        temp = GPIOA->IDR; // 读取数据引脚的状态到 temp
        HAL_GPIO_WritePin(GPIOC, LCD_E_PIN, GPIO_PIN_RESET); // 禁用 LCD12864（将 E 端口设为低电平）
    }
}


void Lcd12864_BusyR(void) {
    uint8_t temp = 0xFF; // 初始化变量 temp 为 0xFF
    HAL_GPIO_WritePin(GPIOA, LCD_CS1_PIN, GPIO_PIN_RESET); // 禁用选择信号 CS1
    HAL_GPIO_WritePin(GPIOA, LCD_CS2_PIN, GPIO_PIN_SET); // 使能选择信号 CS2
    HAL_GPIO_WritePin(GPIOA, LCD_RS_PIN, GPIO_PIN_RESET); // 设置 RS 端口为 0，选择命令寄存器
    HAL_GPIO_WritePin(GPIOA, LCD_RW_PIN, GPIO_PIN_SET); // 设置 WR 端口为 1，切换为读取模式
    
    // 读取数据引脚
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin  = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | 
						   GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    while ((temp & 0x80) == 0x80) { // 循环直到忙碌标志位被清除
        HAL_GPIO_WritePin(GPIOA, LCD_E_PIN, GPIO_PIN_SET); // 使能 LCD12864（将 E 端口设为高电平）
        HAL_Delay(1); // 等待，确保信号稳定
        temp = GPIOA->IDR; // 读取数据引脚的状态到 temp
        HAL_GPIO_WritePin(GPIOA, LCD_E_PIN, GPIO_PIN_RESET); // 禁用 LCD12864（将 E 端口设为低电平）
    }
}




extern uint8_t column;
extern uint8_t Page;

void SetPage_Column(void) {
    uint8_t x, y;

    // 根据列地址的高 2 位选择处理函数
    switch (column & 0xC0) {
        case 0:
            Lcd12864_BusyL(); // 处理左半部分 0-63
            break;
        case 0x40:
            Lcd12864_BusyR(); // 处理右半部分 64-127
            break;
    }

    // 计算列地址 (y)
    y = (column & 0x3F) | 0x40; // 保留低 6 位并设置列地址基准值 0x40

    // 计算页地址 (x)
    x = (Page & 0x07) | 0xB8;   // 保留低 3 位并设置页地址基准值 0xB8

    // 发送页地址到 LCD
    HAL_GPIO_WritePin(GPIOA, LCD_RS_PIN, GPIO_PIN_RESET); // RS = 0，选择命令寄存器
    HAL_GPIO_WritePin(GPIOA, LCD_RW_PIN, GPIO_PIN_RESET); // WR = 0，写模式
    GPIOA->ODR = x; // 将页地址写入数据端口
    HAL_GPIO_WritePin(GPIOA, LCD_E_PIN, GPIO_PIN_SET); // 使能 LCD
    HAL_Delay(1); // 确保信号稳定
    HAL_GPIO_WritePin(GPIOA, LCD_E_PIN, GPIO_PIN_RESET); // 禁用 LCD
    HAL_Delay(1); // 确保信号稳定

    // 发送列地址到 LCD
    HAL_GPIO_WritePin(GPIOA, LCD_RS_PIN, GPIO_PIN_RESET); // RS = 0，选择命令寄存器
    HAL_GPIO_WritePin(GPIOA, LCD_RW_PIN, GPIO_PIN_RESET); // WR = 0，写模式
    GPIOA->ODR = y; // 将列地址写入数据端口
    HAL_GPIO_WritePin(GPIOA, LCD_E_PIN, GPIO_PIN_SET); // 使能 LCD
    HAL_Delay(1); // 确保信号稳定
    HAL_GPIO_WritePin(GPIOA, LCD_E_PIN, GPIO_PIN_RESET); // 禁用 LCD
    HAL_Delay(1); // 确保信号稳定
}


void WriteCommand_L(uint8_t Command) {
    // 等待 LCD 准备好
    Lcd12864_BusyL();
    
    // 选择左半屏
    HAL_GPIO_WritePin(GPIOA, LCD_CS1_PIN, GPIO_PIN_SET); // CS1 = 1
    HAL_GPIO_WritePin(GPIOA, LCD_CS2_PIN, GPIO_PIN_RESET); // CS2 = 0
    
    // 将命令写入数据端口
    GPIOA->ODR = Command; // 将命令发送到数据端口

    // 发送命令
    HAL_GPIO_WritePin(GPIOA, LCD_RS_PIN, GPIO_PIN_RESET); // RS = 0，选择命令寄存器
    HAL_GPIO_WritePin(GPIOA, LCD_RW_PIN, GPIO_PIN_RESET); // WR = 0，写模式
    HAL_GPIO_WritePin(GPIOA, LCD_E_PIN, GPIO_PIN_SET); // 使能 LCD
    HAL_Delay(1); // 等待，确保信号稳定
    HAL_GPIO_WritePin(GPIOA, LCD_E_PIN, GPIO_PIN_RESET); // 禁用 LCD
    HAL_Delay(1); // 等待，确保信号稳定
}






void WriteCommand_R(uint8_t Command) {
    // 等待 LCD 准备好
    Lcd12864_BusyR();
    
    // 选择右半屏
    HAL_GPIO_WritePin(GPIOA, LCD_CS1_PIN, GPIO_PIN_RESET); // CS1 = 0
    HAL_GPIO_WritePin(GPIOA, LCD_CS2_PIN, GPIO_PIN_SET);   // CS2 = 1
    
    // 将命令写入数据端口
    GPIOA->ODR = Command; // 将命令发送到数据端口

    // 发送命令
    HAL_GPIO_WritePin(GPIOA, LCD_RS_PIN, GPIO_PIN_RESET); // RS = 0，选择命令寄存器
    HAL_GPIO_WritePin(GPIOA, LCD_RW_PIN, GPIO_PIN_RESET); // WR = 0，写模式
    HAL_GPIO_WritePin(GPIOA, LCD_E_PIN, GPIO_PIN_SET);   // 使能 LCD
    HAL_Delay(1); // 等待，确保信号稳定
    HAL_GPIO_WritePin(GPIOA, LCD_E_PIN, GPIO_PIN_RESET); // 禁用 LCD
    HAL_Delay(1); // 等待，确保信号稳定
}




uint8_t ReadDate(void) {
    uint8_t Date;
    
    // 设置页和列地址
    SetPage_Column();
    
    // 选择数据寄存器
    HAL_GPIO_WritePin(GPIOA, LCD_RS_PIN, GPIO_PIN_SET); // RS = 1，选择数据寄存器
    HAL_GPIO_WritePin(GPIOA, LCD_RW_PIN, GPIO_PIN_SET); // WR = 1，读模式
    
    // 将数据端口设置为高阻态（读取数据前准备）
    GPIOA->ODR = 0xFF; // 设置数据端口为高电平
    
    // 使能 LCD
    HAL_GPIO_WritePin(GPIOA, LCD_E_PIN, GPIO_PIN_SET); // E = 1
    HAL_Delay(1); // 确保信号稳定
    
    // 读取数据
    Date = GPIOA->IDR; // 从数据端口读取数据
    
    // 禁用 LCD
    HAL_GPIO_WritePin(GPIOA, LCD_E_PIN, GPIO_PIN_RESET); // E = 0
    
    // 禁用选择信号
    HAL_GPIO_WritePin(GPIOA, LCD_CS1_PIN, GPIO_PIN_RESET); // CS1 = 0
    HAL_GPIO_WritePin(GPIOA, LCD_CS2_PIN, GPIO_PIN_RESET); // CS2 = 0
    
    return Date;
}



void WriteDate(uint8_t Date) {
    // 设置页和列地址
    SetPage_Column();
    
    // 选择数据寄存器
    HAL_GPIO_WritePin(GPIOA, LCD_RS_PIN, GPIO_PIN_SET); // RS = 1，选择数据寄存器
    HAL_GPIO_WritePin(GPIOA, LCD_RW_PIN, GPIO_PIN_RESET); // WR = 0，写模式
    
    // 将数据写入数据端口
    GPIOA->ODR = Date; // 将数据发送到数据端口
    
    // 使能 LCD
    HAL_GPIO_WritePin(GPIOA, LCD_E_PIN, GPIO_PIN_SET); // E = 1
    HAL_Delay(1); // 确保信号稳定
    
    // 禁用 LCD
    HAL_GPIO_WritePin(GPIOA, LCD_E_PIN, GPIO_PIN_RESET); // E = 0
    
    // 禁用选择信号
    HAL_GPIO_WritePin(GPIOA, LCD_CS1_PIN, GPIO_PIN_RESET); // CS1 = 0
    HAL_GPIO_WritePin(GPIOA, LCD_CS2_PIN, GPIO_PIN_RESET); // CS2 = 0
}


void Lcd12864_Init(void) {
    // 初始化 LCD
    // 选择左半屏
    HAL_GPIO_WritePin(GPIOA, LCD_CS1_PIN, GPIO_PIN_SET); // CS1 = 1
    HAL_GPIO_WritePin(GPIOA, LCD_CS2_PIN, GPIO_PIN_RESET); // CS2 = 0
    
    // 显示开命令
    WriteCommand_L(0x3F);
    WriteCommand_R(0x3F);
    
    // 显示开始行命令
    WriteCommand_L(0xC0);
    WriteCommand_R(0xC0);
    
    // 显示开命令
    WriteCommand_L(0x3F);
    WriteCommand_R(0x3F);
    
    // 初始化 P1 为输入模式，假设按键连接到 P1 口
    // 在 STM32 中设置 GPIO 为输入模式的代码应在 GPIO 初始化时配置
    // 这里假设按键连接在某个 GPIO 端口，例如 GPIOB
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    __HAL_RCC_GPIOB_CLK_ENABLE(); // 使能 GPIOB 时钟
    
    GPIO_InitStruct.Pin  = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | 
						   GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT; // 设置为输入模式
    GPIO_InitStruct.Pull = GPIO_NOPULL; // 不使用上拉或下拉电阻
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}



// 假设 Page 和 column 变量在适当的位置声明
extern uint8_t Page;
extern uint8_t column;

void Clear(void) {
    // 清除屏幕
    Page = 0;   // 初始化页为0
    column = 0; // 初始化列为0
    
    for (Page = 0; Page < 8; Page++) {          // 遍历所有页
        for (column = 0; column < 128; column++) { // 遍历所有列
            WriteDate(0); // 写入0，清屏
        }
    }
}




// 假设 Page 和 column 变量在适当的位置声明
extern uint8_t Page;
extern uint8_t column;

void Showpicture(uint8_t DotByte, uint8_t DotByte1) {
    Page = 0; // 初始化页为0
    column = 0; // 初始化列为0
    
    for (Page = 0; Page < 8; Page++) { // 遍历所有页
        for (column = 0; column < 128; column++) { // 遍历所有列
            WriteDate(DotByte); // 写入第一个数据字节
            column++; // 列号递增
            WriteDate(DotByte1); // 写入第二个数据字节
        }
    }
}




