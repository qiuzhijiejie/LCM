#ifndef __SPI_H
#define __SPI_H			  	 
#include "sys.h"
#include "stm32f10x_spi.h"




void SPI1_Init(void);
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler);
u8 SPI1_ReadWriteByte(u8 TxData);

#endif




