#ifndef __W25Q128_H__
#define __W25Q128_H__
/**********************
*       INCLUDE        
**********************/
#include "stm32f1xx_hal.h"
#include "./SYSTEM/spi/spi.h"
/**********************
*       DEFINE
**********************/
/*
24bit address 0x000000~0xFFFFFF
最高两位指示当前块block数
第三位指定扇区数
第四位指定页数
剩下两位指定字节数
0x65   4     3   21
  块   扇区  页  字节
规律是16M字节分为256个块，每个块分为16个扇区(4KB)，每个扇区16页，每一页256字节
*/
/**********************
*   EXTERN VARIABLE
**********************/

/**********************
*  FUNCTIONS DECLARE
**********************/
uint16_t BSP_w25q128_read_device_ID(void);
void BSP_w25q128_read_data(uint8_t *buf,uint32_t addr,size_t readlength);
void BSP_w25q128_write_data(uint8_t *buf,uint32_t addr,size_t length);
#endif